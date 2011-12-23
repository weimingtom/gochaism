/**
 * Susie plug-in: RUNE/CAGE g archive extractor
 * written by gocha, feel free to redistribute
 * 
 * based on spi00am_ex.cpp by Shimitei
 * http://www.asahi-net.or.jp/~kh4s-smz/spi/make_spi.html
 * 
 * === g�A�[�J�C�u�T�v ===
 * 
 * �S�̓I�ȍ\���͎����ăV���v���ł��B
 * 
 * 8 ���ʎq "GML_ARC\0"
 * 4 �t�@�C���f�[�^�ʒu
 * 4 �W�J��̃t�@�C�����̃T�C�Y
 * 4 �t�@�C�����̃T�C�Y
 * m �t�@�C����� (���k�ƈÍ������{����Ă���)
 * n �t�@�C���f�[�^ (�Í������{����Ă���)
 * 
 * �t�@�C�����ɂ͈ȉ��̂��̂��܂܂�܂��B
 * 
 * 1. �t�@�C���f�[�^�𕜍����邽�߂̃e�[�u��
 * 2. �e�t�@�C���̃t�@�C�����̒����ƃt�@�C����
 * 3. �e�t�@�C���̃I�t�Z�b�g�ƃT�C�Y
 * 4. �e�t�@�C���̐擪4�o�C�g
 * 
 * �ڍׂ̓\�[�X�R�[�h�����ǂ��Ă��������B
 * ���[�ɍ��������悤�Ƃ������Ƃœ�ǉ�����Ă��܂����A
 * ����ł��t�A�Z���u�����X�g���͓ǂ݂₷���Ǝv���܂��B
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
 * �G���g���|�C���g
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
 * �t�@�C���擪2KB�ȓ�����Ή��t�H�[�}�b�g�����f
 * (���f�Ɏg�p����T�C�Y�̓w�b�_�t�@�C���Œ�`)
 * �t�@�C���������f�ޗ��Ƃ��ēn����Ă���݂���
 */
BOOL IsSupportedEx(char *filename, char *data)
{
  GArcHead* gHdr = (GArcHead*) data;
  const BYTE gHdrSig[8] = { 'G', 'M', 'L', '_', 'A', 'R', 'C', 0 };
  DWORD fatOffset;
  DWORD infoRealSize;
  DWORD infoSize;

  // �V�O�l�`���̃`�F�b�N
  if(memcmp(gHdr->sig, gHdrSig, 8) != 0)
  {
    return FALSE;
  }

  // �Ƃ肠�����l���茳�Ɏ��
  fatOffset = gHdr->fatOffset;
  infoRealSize = gHdr->infoRealSize;
  infoSize = gHdr->infoSize;

  // ������Ƃ����T�C�Y�`�F�b�N
  if (infoRealSize < 0x0100 || fatOffset < sizeof(GArcHead) + infoSize)
  {
    return FALSE;
  }

  return TRUE;
}

/**
 * �A�[�J�C�u���S�t�@�C���̏��擾
 * filename�̈ʒulen��擪�Ƃ݂Ȃ��ăA�[�J�C�u�Ƀt�@�C�������i�[
 */
int GetArchiveInfoEx(LPSTR filename, long len, HLOCAL *lphInf)
{
  int result = SPI_ALL_RIGHT;
  HANDLE file;
  DWORD readBytes;

  // �t�@�C�����J��
  file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if(file != INVALID_HANDLE_VALUE)
  {
    // �ǂݏo����ʒu�𒲐�
    if(SetFilePointer(file, len, NULL, FILE_BEGIN) == (DWORD) len)
    {
      GArcHead gHdr;

      // �w�b�_��ǂݍ���
      ReadFile(file, &gHdr, sizeof(GArcHead), &readBytes, NULL);
      if(readBytes == sizeof(GArcHead))
      {
//      DWORD fatOffset = gHdr.fatOffset;
        DWORD infoRealSize = gHdr.infoRealSize;
        DWORD infoSize = gHdr.infoSize;
        LPBYTE info;
        LPBYTE infoSrc;

        // �t�@�C�����̂��߂ɋ�Ԃ�p��
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

              // �t�@�C������LZ�f�R�[�h���s��
              gArcDecLz(info, infoSrc, infoSize, infoRealSize, 0);

              // �t�@�C�����𓾂�
              nFiles = *(DWORD*)(&info[infoOfs]);
              infoOfs += 4;

              // �ԋp����t�@�C�����̊��蓖�� (�t�@�C����+1��, �������K�{)
              pInfo = (fileInfo*) LocalAlloc(LPTR, sizeof(fileInfo) * (nFiles+1));
              if(pInfo)
              {
                DWORD fileId;

                *lphInf = (HLOCAL) pInfo;
                // �e�t�@�C����A���I�ɏ���
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

                  // �t�@�C�����S�̂̒����𓾂�
                  infFullnameLen = *(DWORD*)(&info[infoOfs]);
                  infoOfs += 4;

                  // �f�B���N�g���ƃt�@�C�����ɕ���
                  infPath = (char*) (&info[infoOfs]);       // �t�@�C�����擪
                  infFilename = &infPath[infFullnameLen];   // �t�@�C��������
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
                  // �f�B���N�g����������Ȃ�
                  if(infFilenameLen == 0)
                  {
                    infFilenameLen = infFullnameLen;
                    infPath = NULL;
                    infPathLen = 0;
                  }
                  // �z�X�g�ɑ��钷��������
                  infSpiFilenameLen = (infFilenameLen < 200) ? infFilenameLen : (200-1);
                  infSpiPathLen = (infPathLen < 200) ? infPathLen : (200-1);
                  // �]����������؂�L����u��
                  for(fnOfs = 0; fnOfs < infSpiPathLen; fnOfs++)
                  {
                    BYTE c;

                    c = infPath[fnOfs];
                    if(c == '/')
                    {
                      infPath[fnOfs] = '\\';
                    }
                  }

                  // �t�@�C���T�C�Y�𓾂�
                  infFileSize = *(DWORD*)(&info[infoOfs + 4]);

                  // ���k�@: 7�����ȓ��Ńe�P�g�[��
                  strcpy((char*) pInfo->method, "RUNE G");
                  // �ʒu: �t�@�C�������ʂ��A�����ɏ������邽�߂̌��ɂ���Ɨǂ�
                  // ���{�v���O�C���ł͎d���Ȃ��u�t�@�C�����́v�ʒu������
                  pInfo->position = infoOfs;
                  // ���k���ꂽ�T�C�Y: �t�@�C�����͖̂����k
                  // �{���̓w�b�_�Ȃǂ̃T�C�Y���܂߂�ׂ��炵���ł�
                  // �����A�܂߂�ׂ��ӏ����悭�킩��Ȃ��̂Ŋ܂߂܂���
                  pInfo->compsize = infFileSize;
                  // ���̃t�@�C���̃T�C�Y: �t�@�C�����͖̂����k
                  pInfo->filesize = infFileSize;
                  // �t�@�C���̍X�V����: �����炭�L�^�Ȃ�(=0)
                  pInfo->timestamp = 0;
                  // ���΃p�X
                  if(infPath)
                  {
                    memcpy(pInfo->path, infPath, infSpiPathLen);
                    pInfo->path[infSpiPathLen] = '\0';
                  }
                  // �t�@�C����
                  memcpy(pInfo->filename, infFilename, infSpiFilenameLen);
                  pInfo->filename[infSpiFilenameLen] = '\0';
                  // CRC: �s��
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
 * filename�ɂ���fileInfo�̃t�@�C����ǂݍ���
 * �f�R�[�h���ꂽ�t�@�C�����i�[������Ԃ�dest�ɓn��
 */
int GetFileEx(char *filename, HLOCAL *dest, fileInfo *pinfo,
    SPI_PROGRESS lpPrgressCallback, long lData)
{
  int result = SPI_ALL_RIGHT;
  DWORD readBytes;
  HANDLE file;

  // �t�@�C�����J��
  file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if(file != INVALID_HANDLE_VALUE)
  {
    GArcHead gHdr;

    // �w�b�_��ǂݍ���
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

      // �t�@�C�����ǂݍ��ݗp����T�C�Y��ߖ�
      // �O�������̓ǂݍ��݂��C�x�ߒ��x�ɍ�����
      infoSrcReqSize = infoOfs + 12;
      infoSrcReqSize = infoSrcReqSize + (infoSrcReqSize / 8) + 1;
      infoSrcReqSize = (infoSrcReqSize < infoSize) ? infoSrcReqSize : infoSize;

      // �t�@�C�����̂��߂ɋ�Ԃ�p��
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

          // �Í��e�[�u����LZ�f�R�[�h
          gArcDecLz(cTbl, infoSrc, infoSrcReqSize, 256, 0);
          // �t�@�C������LZ�f�R�[�h
          gArcDecLz(info, infoSrc, infoSrcReqSize, 12, infoOfs);

          // �e��t�@�C�������擾
          dataOffset = *(DWORD*)(&info[0]) + fatOffset;
          dataSize   = *(DWORD*)(&info[4]);
          dataHead   = *(DWORD*)(&info[8]);

          // �t�@�C���o�b�t�@���m�� (�ʓ|�Ȃ̂�4�o�C�g�]���Ɋm��)
          data = (LPBYTE) LocalAlloc(LMEM_FIXED, 4 + dataSize);
          if(data)
          {
            DWORD decBytes;

            *dest = (HLOCAL) data;

            // �t�@�C����ǂݍ���
            SetFilePointer(file, dataOffset, NULL, FILE_BEGIN);
            ReadFile(file, data, dataSize, &readBytes, NULL);

            // �e�[�u���Ɋ�Â��Í�����
            for(decBytes = 0; decBytes < dataSize; decBytes++)
            {
              data[decBytes] = cTbl[data[decBytes]];
            }

            // �擪4�o�C�g������������
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
 * G�A�[�J�C�u��LZ�f�[�^(�t�@�C�����)���f�R�[�h
 * ���k�@��LZSS���ւ�Ă����ɈÍ������������ł�
 */
DWORD gArcDecLz(LPBYTE dst, LPCBYTE src, DWORD srcSize, DWORD dstSize, DWORD offset)
{
  DWORD transferedSize = 0;
  DWORD sizeToSkip = offset;
  DWORD dstLeftSize = dstSize;
  DWORD dicOfs = 0x0fee;  // �ւ�Ă����Ȏ����̏����ʒu
  LPBYTE lzDic;

  // �f�R�[�h�p�̎�����p�ӂ��� (�l��0�ŏ�����)
  lzDic = (LPBYTE) calloc(0x1000, 1);
  if(lzDic)
  {
    DWORD decOfs = 0;

    // ���f�[�^���s����܂œ˂��i��
    while(decOfs < srcSize)
    {
      BYTE mask = 1;
      BYTE flg = src[decOfs++];

      // �}�X�N�����ʃr�b�g���璭�߂ď���
      while(mask != 0 && decOfs < srcSize && dstLeftSize > 0)
      {
        BYTE b1, b2;

        b1 = ~src[decOfs++];    // NOT��Y�ꂸ��

        if(flg & mask)          // ���p
        {
          DWORD subOfs;
          DWORD subLen;
          DWORD subIdx;
          BYTE subData[18];

          b2 = ~src[decOfs++];  // NOT��Y�ꂸ��

          // �P��̈ʒu�ƒ����𓾂�
          subOfs = ((b2 & 0xf0) << 4) | b1; // ��ʃo�C�g�̏��4�r�b�g�ƌ���
          subLen = (~b2 & 0x0f) + 3;        // ��ʃo�C�g�̉���4�r�b�g (NOT�͉���)

          // �o�������P��͎����̍ŐV�ʒu�ɂ���������
          // gArcRefreshSub(lzDic, &dicOfs, subOfs, subLen);
          // ��������P�����������o���ď�������
          // gArcReadFromDic(&dst[transferedSize], lzDic, subOfs, subLen);
          // transferedSize += subLen;

          // �P������o��
          gArcReadFromDic(subData, lzDic, subOfs, subLen);
          // �P����o�b�t�@�ɓ]��
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
          // �P����ŐV�ʒu�ɏ����߂�
          gArcWriteToDic(lzDic, &dicOfs, subData, subLen);
        }
        else  // �V�K
        {
          // 1�o�C�g�������ƃo�b�t�@�ɓW�J
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

        // �ЂƂ��r�b�g��
        mask *= 2;
      }
    }

    free(lzDic);
  }
  return transferedSize;
}

/**
 * ��������P���ǂݍ���
 * �ʒu�̏z���Ȃ����memcpy�ő����㕨
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
 * �����ɒP�����������
 * �ʒu�̏z���Ȃ����memcpy�ő����㕨
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
 * �����̒P����ŐV�̈ʒu�ɍX�V
 * �ʒu�̏z���Ȃ����memcpy�ő����㕨
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
