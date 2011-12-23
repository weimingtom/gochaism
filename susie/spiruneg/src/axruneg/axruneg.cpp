/**
 * Susie plug-in: RUNE/CAGE g archive extractor
 * written by gocha, feel free to redistribute
 * 
 * based on spi00am_ex.cpp by Shimitei
 * http://www.asahi-net.or.jp/~kh4s-smz/spi/make_spi.html
 * 
 * === gアーカイブ概要 ===
 * 
 * 全体的な構造は至ってシンプルです。
 * 
 * 8 識別子 "GML_ARC\0"
 * 4 ファイルデータ位置
 * 4 展開後のファイル情報のサイズ
 * 4 ファイル情報のサイズ
 * m ファイル情報 (圧縮と暗号化が施されている)
 * n ファイルデータ (暗号化が施されている)
 * 
 * ファイル情報には以下のものが含まれます。
 * 
 * 1. ファイルデータを復号するためのテーブル
 * 2. 各ファイルのファイル名の長さとファイル名
 * 3. 各ファイルのオフセットとサイズ
 * 4. 各ファイルの先頭4バイト
 * 
 * 詳細はソースコードから解読してください。
 * 半端に高速化しようとしたことで難読化されていますが、
 * それでも逆アセンブルリストよりは読みやすいと思います。
 */

#include <windows.h>
#include <stdlib.h>
#include <memory.h>
#include "spi00am.h"
#include "axruneg.h"

DWORD gArcDecLz(LPBYTE dst, LPCBYTE src, DWORD srcSize, DWORD dstSize, DWORD offset);
void gArcReadFromDic(LPBYTE dst, LPCBYTE lzDic, DWORD srcOfs, DWORD size);
void gArcWriteToDic(LPBYTE lzDic, LPDWORD dicOfs, LPCBYTE src, DWORD size);
void gArcRefreshSub(LPBYTE lzDic, LPDWORD dicOfs, DWORD srcOfs, DWORD size);

/**
 * エントリポイント
 */
BOOL APIENTRY SpiEntryPoint(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    break;

  case DLL_THREAD_ATTACH:
    break;

  case DLL_THREAD_DETACH:
    break;

  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

/**
 * ファイル先頭2KB以内から対応フォーマットか判断
 * (判断に使用するサイズはヘッダファイルで定義)
 * ファイル名も判断材料として渡されているみたい
 */
BOOL IsSupportedEx(char *filename, char *data)
{
  GArcHead* gHdr = (GArcHead*) data;
  const BYTE gHdrSig[8] = { 'G', 'M', 'L', '_', 'A', 'R', 'C', 0 };
  DWORD fatOffset;
  DWORD infoRealSize;
  DWORD infoSize;

  // シグネチャのチェック
  if(memcmp(gHdr->sig, gHdrSig, 8) != 0)
  {
    return FALSE;
  }

  // とりあえず値を手元に取る
  fatOffset = gHdr->fatOffset;
  infoRealSize = gHdr->infoRealSize;
  infoSize = gHdr->infoSize;

  // ちょっとしたサイズチェック
  if (infoRealSize < 0x0100 || fatOffset < sizeof(GArcHead) + infoSize)
  {
    return FALSE;
  }

  return TRUE;
}

/**
 * アーカイブ内全ファイルの情報取得
 * filenameの位置lenを先頭とみなしてアーカイブにファイル情報を格納
 */
int GetArchiveInfoEx(LPSTR filename, long len, HLOCAL *lphInf)
{
  int result = SPI_ALL_RIGHT;
  HANDLE file;
  DWORD readBytes;

  // ファイルを開く
  file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if(file != INVALID_HANDLE_VALUE)
  {
    // 読み出し基準位置を調整
    if(SetFilePointer(file, len, NULL, FILE_BEGIN) == (DWORD) len)
    {
      GArcHead gHdr;

      // ヘッダを読み込み
      ReadFile(file, &gHdr, sizeof(GArcHead), &readBytes, NULL);
      if(readBytes == sizeof(GArcHead))
      {
//      DWORD fatOffset = gHdr.fatOffset;
        DWORD infoRealSize = gHdr.infoRealSize;
        DWORD infoSize = gHdr.infoSize;
        LPBYTE info;
        LPBYTE infoSrc;

        // ファイル情報のために空間を用意
        infoSrc = (LPBYTE) malloc(infoSize);
        if(infoSrc)
        {
          info = (LPBYTE) malloc(infoRealSize);
          if(info)
          {
            ReadFile(file, infoSrc, infoSize, &readBytes, NULL);
            if(readBytes == infoSize)
            {
              DWORD infoOfs = 0x0100;
              fileInfo* pInfo;
              DWORD nFiles;

              // ファイル情報のLZデコードを行う
              gArcDecLz(info, infoSrc, infoSize, infoRealSize, 0);

              // ファイル数を得る
              nFiles = *(DWORD*)(&info[infoOfs]);
              infoOfs += 4;

              // 返却するファイル情報の割り当て (ファイル数+1個, 初期化必須)
              pInfo = (fileInfo*) LocalAlloc(LPTR, sizeof(fileInfo) * (nFiles+1));
              if(pInfo)
              {
                DWORD fileId;

                *lphInf = (HLOCAL) pInfo;
                // 各ファイルを連続的に処理
                for(fileId = 0; fileId < nFiles; fileId++)
                {
                  DWORD infFullnameLen;
                  DWORD infPathLen;
                  DWORD infFilenameLen = 0;
                  char* infPath;
                  char* infFilename;
                  DWORD infFileSize;
                  DWORD infSpiFilenameLen;
                  DWORD infSpiPathLen;
                  DWORD fnOfs;

                  // ファイル名全体の長さを得る
                  infFullnameLen = *(DWORD*)(&info[infoOfs]);
                  infoOfs += 4;

                  // ディレクトリとファイル名に分割
                  infPath = (char*) (&info[infoOfs]);       // ファイル名先頭
                  infFilename = &infPath[infFullnameLen];   // ファイル名末尾
                  infoOfs += infFullnameLen;
                  while(infFilename != infPath)
                  {
                    BYTE c;

                    infFilename--;
                    c = *infFilename;
                    if(c == '/')
                    {
                      infFilename++;
                      infPathLen = (DWORD) (infFilename - infPath);
                      infFilenameLen = infFullnameLen - infPathLen;
                      break;
                    }
                  }
                  // ディレクトリが見つからない
                  if(infFilenameLen == 0)
                  {
                    infFilenameLen = infFullnameLen;
                    infPath = NULL;
                    infPathLen = 0;
                  }
                  // ホストに送る長さを決定
                  infSpiFilenameLen = (infFilenameLen < 200) ? infFilenameLen : (200-1);
                  infSpiPathLen = (infPathLen < 200) ? infPathLen : (200-1);
                  // 転送分だけ区切り記号を置換
                  for(fnOfs = 0; fnOfs < infSpiPathLen; fnOfs++)
                  {
                    BYTE c;

                    c = infPath[fnOfs];
                    if(c == '/')
                    {
                      infPath[fnOfs] = '\\';
                    }
                  }

                  // ファイルサイズを得る
                  infFileSize = *(DWORD*)(&info[infoOfs + 4]);

                  // 圧縮法: 7文字以内でテケトーに
                  strcpy((char*) pInfo->method, "RUNE G");
                  // 位置: ファイルを識別し、高速に処理するための鍵にすると良い
                  // ※本プラグインでは仕方なく「ファイル情報の」位置を入れる
                  pInfo->position = infoOfs;
                  // 圧縮されたサイズ: ファイル自体は無圧縮
                  // 本来はヘッダなどのサイズも含めるべきらしいです
                  // ただ、含めるべき箇所がよくわからないので含めません
                  pInfo->compsize = infFileSize;
                  // 元のファイルのサイズ: ファイル自体は無圧縮
                  pInfo->filesize = infFileSize;
                  // ファイルの更新日時: おそらく記録なし(=0)
                  pInfo->timestamp = 0;
                  // 相対パス
                  if(infPath)
                  {
                    memcpy(pInfo->path, infPath, infSpiPathLen);
                    pInfo->path[infSpiPathLen] = '\0';
                  }
                  // ファイル名
                  memcpy(pInfo->filename, infFilename, infSpiFilenameLen);
                  pInfo->filename[infSpiFilenameLen] = '\0';
                  // CRC: 不明
                  // pInfo->crc = 0;

                  infoOfs += 12;
                  pInfo++;
                }
              }
              else
              {
                result = SPI_NO_MEMORY;
              }
            }
            else
            {
              result = SPI_FILE_READ_ERROR;
            }
            free(info);
          }
          else
          {
            result = SPI_NO_MEMORY;
          }
          free(infoSrc);
        }
        else
        {
          result = SPI_NO_MEMORY;
        }
      }
      else
      {
        result = SPI_FILE_READ_ERROR;
      }
    }
    else
    {
      result = SPI_FILE_READ_ERROR;
    }
    CloseHandle(file);
  }
  else
  {
    result = SPI_FILE_READ_ERROR;
  }
  return result;
}

/**
 * filenameにあるfileInfoのファイルを読み込む
 * デコードされたファイルを格納した空間をdestに渡す
 */
int GetFileEx(char *filename, HLOCAL *dest, fileInfo *pinfo,
    SPI_PROGRESS lpPrgressCallback, long lData)
{
  int result = SPI_ALL_RIGHT;
  DWORD readBytes;
  HANDLE file;

  // ファイルを開く
  file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if(file != INVALID_HANDLE_VALUE)
  {
    GArcHead gHdr;

    // ヘッダを読み込み
    ReadFile(file, &gHdr, sizeof(GArcHead), &readBytes, NULL);
    if(readBytes == sizeof(GArcHead))
    {
      DWORD infoOfs = (DWORD) pinfo->position;
      DWORD fatOffset = gHdr.fatOffset;
      DWORD infoRealSize = gHdr.infoRealSize;
      DWORD infoSize = gHdr.infoSize;
      DWORD infoSrcReqSize;
      LPBYTE infoSrc;
      BYTE cTbl[256];
      BYTE info[12];

      // ファイル情報読み込み用いるサイズを節約
      // 前半部分の読み込みを気休め程度に高速化
      infoSrcReqSize = infoOfs + 12;
      infoSrcReqSize = infoSrcReqSize + (infoSrcReqSize / 8) + 1;
      infoSrcReqSize = (infoSrcReqSize < infoSize) ? infoSrcReqSize : infoSize;

      // ファイル情報のために空間を用意
      infoSrc = (LPBYTE) malloc(infoSrcReqSize);
      if(infoSrc)
      {
        ReadFile(file, infoSrc, infoSrcReqSize, &readBytes, NULL);
        if(readBytes == infoSrcReqSize)
        {
          DWORD dataOffset;
          DWORD dataSize;
          DWORD dataHead;
          LPBYTE data;

          // 暗号テーブルのLZデコード
          gArcDecLz(cTbl, infoSrc, infoSrcReqSize, 256, 0);
          // ファイル情報のLZデコード
          gArcDecLz(info, infoSrc, infoSrcReqSize, 12, infoOfs);

          // 各種ファイル情報を取得
          dataOffset = *(DWORD*)(&info[0]) + fatOffset;
          dataSize   = *(DWORD*)(&info[4]);
          dataHead   = *(DWORD*)(&info[8]);

          // ファイルバッファを確保 (面倒なので4バイト余分に確保)
          data = (LPBYTE) LocalAlloc(LMEM_FIXED, 4 + dataSize);
          if(data)
          {
            DWORD decBytes;

            *dest = (HLOCAL) data;

            // ファイルを読み込む
            SetFilePointer(file, dataOffset, NULL, FILE_BEGIN);
            ReadFile(file, data, dataSize, &readBytes, NULL);

            // テーブルに基づく暗号解除
            for(decBytes = 0; decBytes < dataSize; decBytes++)
            {
              data[decBytes] = cTbl[data[decBytes]];
            }

            // 先頭4バイトを書き換える
            *((DWORD*) data) = dataHead;
          }
          else
          {
            result = SPI_NO_MEMORY;
          }
        }
        else
        {
          result = SPI_FILE_READ_ERROR;
        }
        free(infoSrc);
      }
      else
      {
        result = SPI_NO_MEMORY;
      }
    }
    else
    {
      result = SPI_FILE_READ_ERROR;
    }
    CloseHandle(file);
  }
  else
  {
    result = SPI_FILE_READ_ERROR;
  }
  return result;
}

/**
 * GアーカイブのLZデータ(ファイル情報)をデコード
 * 圧縮法はLZSSをへんてこりんに暗号化した感じです
 */
DWORD gArcDecLz(LPBYTE dst, LPCBYTE src, DWORD srcSize, DWORD dstSize, DWORD offset)
{
  DWORD transferedSize = 0;
  DWORD sizeToSkip = offset;
  DWORD dstLeftSize = dstSize;
  DWORD dicOfs = 0x0fee;  // へんてこりんな辞書の初期位置
  LPBYTE lzDic;

  // デコード用の辞書を用意する (値は0で初期化)
  lzDic = (LPBYTE) calloc(0x1000, 1);
  if(lzDic)
  {
    DWORD decOfs = 0;

    // 元データが尽きるまで突き進む
    while(decOfs < srcSize)
    {
      BYTE mask = 1;
      BYTE flg = src[decOfs++];

      // マスクを下位ビットから眺めて処理
      while(mask != 0 && decOfs < srcSize && dstLeftSize > 0)
      {
        BYTE b1, b2;

        b1 = ~src[decOfs++];    // NOTを忘れずに

        if(flg & mask)          // 引用
        {
          DWORD subOfs;
          DWORD subLen;
          DWORD subIdx;
          BYTE subData[18];

          b2 = ~src[decOfs++];  // NOTを忘れずに

          // 単語の位置と長さを得る
          subOfs = ((b2 & 0xf0) << 4) | b1; // 上位バイトの上位4ビットと結合
          subLen = (~b2 & 0x0f) + 3;        // 上位バイトの下位4ビット (NOTは解除)

          // 出現した単語は辞書の最新位置にも書き込み
          // gArcRefreshSub(lzDic, &dicOfs, subOfs, subLen);
          // 辞書から単語を引っ張り出して書き込み
          // gArcReadFromDic(&dst[transferedSize], lzDic, subOfs, subLen);
          // transferedSize += subLen;

          // 単語を取り出す
          gArcReadFromDic(subData, lzDic, subOfs, subLen);
          // 単語をバッファに転送
          for(subIdx = 0; subIdx < subLen; subIdx++)
          {
            if(dstLeftSize == 0)
            {
              break;
            }

            if(sizeToSkip > 0)
            {
              sizeToSkip--;
            }
            else
            {
              dst[transferedSize] = subData[subIdx];
              transferedSize++;
              dstLeftSize--;
            }
          }
          // 単語を最新位置に書き戻す
          gArcWriteToDic(lzDic, &dicOfs, subData, subLen);
        }
        else  // 新規
        {
          // 1バイトを辞書とバッファに展開
          gArcWriteToDic(lzDic, &dicOfs, &b1, 1);
          if(sizeToSkip > 0)
          {
            sizeToSkip--;
          }
          else
          {
            dst[transferedSize] = b1;
            transferedSize++;
            dstLeftSize--;
          }
        }

        // ひとつ上野ビットへ
        mask *= 2;
      }
    }

    free(lzDic);
  }
  return transferedSize;
}

/**
 * 辞書から単語を読み込む
 * 位置の循環がなければmemcpyで足りる代物
 */
void gArcReadFromDic(LPBYTE dst, LPCBYTE lzDic, DWORD srcOfs, DWORD size)
{
  DWORD dstOfs;

  for(dstOfs = 0; dstOfs < size; dstOfs++)
  {
    dst[dstOfs] = lzDic[srcOfs];
    srcOfs++;
    srcOfs &= 0x0fff;
  }
}

/**
 * 辞書に単語を書き込む
 * 位置の循環がなければmemcpyで足りる代物
 */
void gArcWriteToDic(LPBYTE lzDic, LPDWORD dicOfs, LPCBYTE src, DWORD size)
{
  DWORD srcOfs;
  DWORD wDicOfs = *dicOfs;

  for(srcOfs = 0; srcOfs < size; srcOfs++)
  {
    lzDic[wDicOfs] = src[srcOfs];
    wDicOfs++;
    wDicOfs &= 0x0fff;
  }
  *dicOfs = wDicOfs;
}

/**
 * 辞書の単語を最新の位置に更新
 * 位置の循環がなければmemcpyで足りる代物
 */
void gArcRefreshSub(LPBYTE lzDic, LPDWORD dicOfs, DWORD srcOfs, DWORD size)
{
  DWORD targetOfs = (*dicOfs + size) & 0x0fff;
  DWORD wDicOfs = *dicOfs;

  while(wDicOfs != targetOfs)
  {
    lzDic[wDicOfs] = lzDic[srcOfs];
    srcOfs++;
    wDicOfs++;
    srcOfs &= 0x0fff;
    wDicOfs &= 0x0fff;
  }
  *dicOfs = wDicOfs;
}
