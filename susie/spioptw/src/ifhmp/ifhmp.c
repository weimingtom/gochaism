/**
 * Susie plug-in: OPTWORKS HMP Image
 * written by gocha, feel free to redistribute
 * 
 * based on spi00in_ex.c by Shimitei
 * http://www.asahi-net.or.jp/~kh4s-smz/spi/make_spi.html
 */

#include <windows.h>
#include <stdlib.h>
#include "ifhmp.h"

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
  HMPFILEHEADER* hmpHead = (HMPFILEHEADER*) data;

  /* お約束のシグネチャチェック、なの */
  if(hmpHead->hfType != 0x4d48) /* "HM" */
  {
    return FALSE;
  }

  /* もう終わり、なの */
  return TRUE;
}

/*
 * ファイルイメージから画像の情報を取得
 * (バッファサイズはヘッダファイルで定義)
 */
int GetPictureInfoEx(char *data, struct PictureInfo *lpInfo)
{
  HMPFILEHEADER* hmpHead = (HMPFILEHEADER*) data;

  lpInfo->left          = 0;
  lpInfo->top           = 0;
  lpInfo->width         = hmpHead->hfWidth;
  lpInfo->height        = hmpHead->hfHeight;
  lpInfo->x_density     = 0;
  lpInfo->y_density     = 0;
  lpInfo->colorDepth    = 16;
  lpInfo->hInfo         = NULL;

  return SPI_ALL_RIGHT;
}

/*
 * 画像ファイルをDIBに変換
 * dataはファイルイメージで,サイズはdatasizeバイト.
 */
int GetPictureEx(long datasize, HANDLE *pHBInfo, HANDLE *pHBm,
			 SPI_PROGRESS lpPrgressCallback, long lData, char *data)
{
  int result = SPI_ALL_RIGHT;

  *pHBInfo = LocalAlloc(LMEM_MOVEABLE, sizeof(BITMAPINFOHEADER));
  if(*pHBInfo)
  {
    HMPFILEHEADER* hmpHead = (HMPFILEHEADER*) data;
    DWORD width = hmpHead->hfWidth;
    DWORD height = hmpHead->hfHeight;
    DWORD imageSize = width * height * 2;

    *pHBm = LocalAlloc(LMEM_MOVEABLE, imageSize);
    if(*pHBm)
    {
      BITMAPINFO* bmpinfo = (BITMAPINFO*) LocalLock(*pHBInfo);
      BYTE* bmp = (BYTE*) LocalLock(*pHBm);

      if(bmpinfo && bmp)
      {
        BITMAPINFOHEADER* bmih = (BITMAPINFOHEADER*) bmpinfo;
        BYTE* hmp = (BYTE*) data;
        DWORD pxFlgOfs = sizeof(HMPFILEHEADER);
        DWORD valOfs = pxFlgOfs + hmpHead->hfFlgMapLen;
        DWORD destOfs = 0;
        BYTE flg;
        BYTE bitMask = 0x80;
        WORD pxVal1 = 0xe003;
        WORD pxVal2 = 0xe003;
        WORD pxVal3 = 0xe003;

        /* BITMAPINFOに情報を格納 */
        bmih->biSize            = sizeof(BITMAPINFOHEADER);
        bmih->biWidth           = width;
        bmih->biHeight          = height;
        bmih->biPlanes          = 1;
        bmih->biBitCount        = 16;
        bmih->biCompression     = BI_RGB;
        bmih->biSizeImage       = imageSize;
        bmih->biXPelsPerMeter   = 0;
        bmih->biYPelsPerMeter   = 0;
        bmih->biClrUsed         = 0;
        bmih->biClrImportant    = 0;

        /* 画像を展開 */
        flg = hmp[pxFlgOfs++];
        while(destOfs < imageSize)
        {
          if((flg & bitMask) == 0)
          {
            pxVal3 = pxVal2;
            pxVal2 = pxVal1;
            pxVal1 = *((WORD*) (&hmp[valOfs]));
            valOfs += 2;
            *((WORD*) (&bmp[destOfs])) = pxVal1;
            destOfs += 2;
          }
          else
          {
            WORD writeVal1;
            WORD writeVal2;
            BYTE lenByte = hmp[valOfs++];
            UINT lenCount;
            UINT len;

            if(lenByte & 0x40)
            {
              writeVal1 = pxVal2;
              writeVal2 = pxVal1;
            }
            else
            {
              writeVal1 = pxVal3;
              writeVal2 = pxVal2;
            }

            len = lenByte & 0x3f;
            if(len == 0)
            {
              len = hmp[valOfs++] + 0x40;
            }

            if(lenByte & 0x80)
            {
              DWORD pxPair = (writeVal1 << 16) | writeVal2;

              for(lenCount = 0; lenCount < len; lenCount++)
              {
                *((DWORD*) (&bmp[destOfs])) = pxPair;
                destOfs += 4;
              }
            }
            else
            {
              for(lenCount = 0; lenCount < len; lenCount++)
              {
                *((WORD*) (&bmp[destOfs])) = writeVal2;
                destOfs += 2;
              }
            }
          }

          bitMask = (bitMask >> 1);
          if(!bitMask)
          {
            flg = hmp[pxFlgOfs++];
            bitMask = 0x80;
          }
        }
      }
      else
      {
        LocalFree(*pHBInfo);
        *pHBInfo = NULL;
        LocalFree(*pHBm);
        *pHBm = NULL;
        result = SPI_MEMORY_ERROR;
      }

      if(*pHBm) LocalUnlock(*pHBm);
    }
    else
    {
      LocalFree(*pHBInfo);
      *pHBInfo = NULL;
      result = SPI_NO_MEMORY;
    }
    if(*pHBInfo) LocalUnlock(*pHBInfo);
  }
  else
  {
    result = SPI_NO_MEMORY;
  }
  return result;
}

/*-------------------------------------------------------------------------*/
