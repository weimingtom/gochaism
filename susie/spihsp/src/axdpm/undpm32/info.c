/*--------------------------------------------------------------------------
  info.c        UNDPM32.DLL info functions
--------------------------------------------------------------------------*/

#include <windows.h>
#include <shlwapi.h>
#include <stdlib.h>

#include "undpm32.h"
#include "main.h"
#include "info.h"
#include "crypt.h"

#include "egcnvstr.h"
#include "egstring.h"
#include "egalloc.h"

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u�̃o�[�W�������擾���܂��B

  �錾:         DWORD UnDpmGetArcVersion(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       �A�[�J�C�u�̃o�[�W�����������l���Ԃ�܂��B

  ���:         �A�[�J�C�u�̃o�[�W�������擾���܂��B
--------------------------------------------------------------------------*/
DWORD UnDpmGetArcVersion(HDPM hDpm)
{
  return UnDpmGetInternalInfo(hDpm)->dwVersion;
}

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u�Ɋi�[����Ă���t�@�C�������擾���܂��B

  �錾:         DWORD UnDpmGetFileCount(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       �A�[�J�C�u�Ɋi�[����Ă���t�@�C�������Ԃ�܂��B

  ���:         �A�[�J�C�u�Ɋi�[����Ă���t�@�C�������擾���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmGetFileCount(HDPM hDpm)
{
  DPMHED DpmHed;  // �w�b�_���

  // �w�b�_�����擾
  UnDpmGetDpmHed(hDpm, &DpmHed);

  // �t�@�C������Ԃ�
  return DpmHed.nNumberOfFile;
}

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u�̃T�C�Y���擾���܂��B

  �錾:         DWORD UnDpmGetArcFileSize(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       �A�[�J�C�u�̃T�C�Y���Ԃ�܂��B

  ���:         �A�[�J�C�u�̃T�C�Y���擾���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmGetArcFileSize(HDPM hDpm)
{
  return UnDpmGetInternalInfo(hDpm)->dwFileSize;
}

/*--------------------------------------------------------------------------
  �T�v:         �����Ƀq�b�g�����t�@�C���̉𓀌�̃T�C�Y�̍��v�𓾂܂��B

  �錾:         DWORD UnDpmGetArcOriginalSize(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       �𓀌�̃T�C�Y�̍��v���Ԃ�܂��B

  ���:         UnDpmFindFirstFile ����� UnDpmFindNextFile �ɂ�錟����
                �q�b�g�����t�@�C���̉𓀌�̃T�C�Y�̍��v�𓾂܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmGetArcOriginalSize(HDPM hDpm)
{
  return UnDpmGetInternalInfo(hDpm)->dwOriginalSizeTotal;
}

/*--------------------------------------------------------------------------
  �T�v:         �����Ƀq�b�g�����t�@�C���̈��k�T�C�Y�̍��v�𓾂܂��B

  �錾:         DWORD UnDpmGetArcCompressedSize(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       ���k�T�C�Y�̍��v���Ԃ�܂��B

  ���:         UnDpmFindFirstFile ����� UnDpmFindNextFile �ɂ�錟����
                �q�b�g�����t�@�C���̈��k�T�C�Y�̍��v�𓾂܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmGetArcCompressedSize(HDPM hDpm)
{
  return UnDpmGetInternalInfo(hDpm)->dwCompressedSizeTotal;
}

/*--------------------------------------------------------------------------
  �T�v:         ���s�t�@�C���`�����ǂ������ׂ܂��B

  �錾:         BOOL UnDpmIsExecutable(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       ���s�t�@�C���`���Ȃ�΁A0 �ȊO�̒l���Ԃ�܂��B

                ���s�t�@�C���`���łȂ��Ȃ�΁A0 ���Ԃ�܂��B

  ���:         �A�[�J�C�u�����s�t�@�C���`�����ǂ������ׂ܂��B
--------------------------------------------------------------------------*/
UNDPMAPI BOOL UnDpmIsExecutable(HDPM hDpm)
{
  return UnDpmGetInternalInfo(hDpm)->bExecutable;
}

/*--------------------------------------------------------------------------
  �T�v:         �t�@�C�������� ID ���擾���܂��B (ANSI)

  �錾:         DWORD UnDpmGetFileIdA(
                    HDPM hDpm,          // �A�[�J�C�u�n���h��
                    LPCSTR lpszFileName // �t�@�C����
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�n���h�����w�肵�܂��B

                lpszFileName
                    �t�@�C�������w�肵�܂��B

  �߂�l:       �֐�����������ƁA�t�@�C�� ID ���Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �t�@�C��������s�����߂� ID ���擾���܂��B
                ���� ID ���g���ăt�@�C���ɑ�����s���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmGetFileIdA(HDPM hDpm, LPCSTR lpszFileName)
{
  DWORD iFileId;          // �t�@�C�� ID
  LPWSTR lpszFileNameW;   // Unicode �ɕϊ������t�@�C����

  // ������� Unicode �ɕϊ�
  lpszFileNameW = ConvertStringAToW(lpszFileName);

  // Unicode �ł̊֐����Ăяo��
  iFileId = UnDpmGetFileIdW(hDpm, lpszFileNameW);

  // Unicode �ɕϊ���������������
  FreeConvertedString(lpszFileNameW);

  // �t�@�C�� ID ��Ԃ�
  return iFileId;
}

/*--------------------------------------------------------------------------
  �T�v:         �t�@�C�������� ID ���擾���܂��B (Unicode)

  �錾:         DWORD UnDpmGetFileIdW(
                    HDPM hDpm,              // �A�[�J�C�u�n���h��
                    LPCWSTR lpszFileName    // �t�@�C����
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�n���h�����w�肵�܂��B

                lpszFileName
                    �t�@�C�������w�肵�܂��B

  �߂�l:       �֐�����������ƁA�t�@�C�� ID ���Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �t�@�C��������s�����߂� ID ���擾���܂��B
                ���� ID ���g���ăt�@�C���ɑ�����s���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmGetFileIdW(HDPM hDpm, LPCWSTR lpszFileName)
{
  DWORD iFileId = DPM_INVALID_FILE_ID;    // �t�@�C�� ID

  DWORD i;                        // ����ϐ�
  DPMHED DpmHed;                  // �w�b�_���
  DPMFILEDIR FileDir;             // �t�@�C�����
  WCHAR szCurFileName[_MAX_PATH]; // �t�@�C���� (Unicode)

  // �w�b�_�����擾
  UnDpmGetDpmHed(hDpm, &DpmHed);

  // ���ׂẴt�@�C�����`�F�b�N
  for(i = 1; i <= DpmHed.nNumberOfFile; i++)
  {
    // �t�@�C�������擾
    if(UnDpmGetFileDir(hDpm, i, &FileDir))
    {
      // �t�@�C�������擾
      if(UnDpmGetFileNameW(hDpm, i, szCurFileName, countof(szCurFileName)) != 0)
      {
        // �w�肳�ꂽ�t�@�C����
        if(StrCmpW(lpszFileName, szCurFileName) == 0)
        {
          iFileId = i;
          break;
        }
      }
    }
  }

  // �t�@�C�� ID ��Ԃ�
  return iFileId;
}

/*--------------------------------------------------------------------------
  �T�v:         ID ����t�@�C�������擾���܂��B (ANSI)

  �錾:         DWORD UnDpmGetFileNameA(
                    HDPM hDpm,          // �A�[�J�C�u�n���h��
                    DWORD iFileId,      // �t�@�C�� ID
                    LPSTR lpszBuffer,   // �t�@�C�������󂯎��o�b�t�@
                    DWORD cchBuffer     // �o�b�t�@�T�C�Y
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�n���h�����w�肵�܂��B

                iFileId
                    �t�@�C�� ID ���w�肵�܂��B

                lpszBuffer
                    �t�@�C�������󂯎��o�b�t�@�ւ̃|�C���^���w�肵�܂��B

                cchBuffer
                    �o�b�t�@�T�C�Y�𕶎����Ŏw�肵�܂��B

  �߂�l:       �֐�����������ƁAlpszBuffer �p�����[�^�ɃR�s�[���ꂽ������
                �̒������������ŕԂ�܂��B���̒����ɂ́A�I�[�� NULL ������
                �܂܂�܂���B

                lpszBuffer �p�����[�^�Ɏw�肳�ꂽ�o�b�t�@���������āA�����p
                �X���󂯎�邱�Ƃ��ł��Ȃ��ꍇ�́A�����p�X���i�[���邽�߂�
                �K�v�ȃo�b�t�@�T�C�Y���������ŕԂ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         �t�@�C�� ID ����t�@�C�������擾���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmGetFileNameA(HDPM hDpm, DWORD iFileId, LPSTR lpszBuffer, DWORD cchBuffer)
{
  LPWSTR lpszBufferW;     // Unicode �t�@�C�����擾�o�b�t�@
  DWORD cchFileName = 0;  // �t�@�C�����̒���

  // �t�@�C�����̈�̃��������蓖��
  lpszBufferW = MemoryAlloc((cchBuffer + 1) * sizeof(WCHAR));

  // ����Ƀ����������蓖�Ă�ꂽ��
  if(lpszBufferW)
  {
    // �t�@�C�������擾
    cchFileName = UnDpmGetFileNameW(hDpm, iFileId, lpszBufferW, cchBuffer);

    // ANSI ������ɕϊ�
    WideCharToMultiByte(CP_ACP, 0, lpszBufferW, cchFileName + 1, lpszBuffer, cchFileName + 1, NULL, NULL);

    // �����������
    MemoryFree(lpszBufferW);
  }

  // �R�s�[���ꂽ�T�C�Y��Ԃ�
  return cchFileName;
}

/*--------------------------------------------------------------------------
  �T�v:         ID ����t�@�C�������擾���܂��B (Unicode)

  �錾:         DWORD UnDpmGetFileNameW(
                    HDPM hDpm,          // �A�[�J�C�u�n���h��
                    DWORD iFileId,      // �t�@�C�� ID
                    LPWSTR lpszBuffer,  // �t�@�C�������󂯎��o�b�t�@
                    DWORD cchBuffer     // �o�b�t�@�T�C�Y
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�n���h�����w�肵�܂��B

                iFileId
                    �t�@�C�� ID ���w�肵�܂��B

                lpszBuffer
                    �t�@�C�������󂯎��o�b�t�@�ւ̃|�C���^���w�肵�܂��B

                cchBuffer
                    �o�b�t�@�T�C�Y�𕶎����Ŏw�肵�܂��B

  �߂�l:       �֐�����������ƁAlpszBuffer �p�����[�^�ɃR�s�[���ꂽ������
                �̒������������ŕԂ�܂��B���̒����ɂ́A�I�[�� NULL ������
                �܂܂�܂���B

                lpszBuffer �p�����[�^�Ɏw�肳�ꂽ�o�b�t�@���������āA�����p
                �X���󂯎�邱�Ƃ��ł��Ȃ��ꍇ�́A�����p�X���i�[���邽�߂�
                �K�v�ȃo�b�t�@�T�C�Y���������ŕԂ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         �t�@�C�� ID ����t�@�C�������擾���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmGetFileNameW(HDPM hDpm, DWORD iFileId, LPWSTR lpszBuffer, DWORD cchBuffer)
{
  DPMFILEDIR FileDir;             // �t�@�C�����
  DWORD cchFileName = 0;          // �t�@�C�����̒���
  DWORD cchCopySize;              // �R�s�[���钷��
  CHAR szFileNameA[_MAX_PATH];    // �t�@�C���� (ANSI)

  // �ُ�ȃo�b�t�@�T�C�Y�łȂ���΃t�@�C�������擾
  if(cchBuffer != 0 && UnDpmGetFileDir(hDpm, iFileId, &FileDir))
  {
    // �t�@�C�������R�s�[
    lstrcpynA(szFileNameA, FileDir.sFileName, sizeof(FileDir.sFileName) + 1);

    // �t�@�C�����̒����𒲂ׂ�
    cchFileName = lstrlenA(szFileNameA);

    // �t�@�C�������o�b�t�@�Ɏ��܂肫�邩
    if(cchBuffer >= (cchFileName + 1))
    {
      // ���S�Ɏ��܂�ꍇ
      cchCopySize = cchFileName;
    }
    else
    {
      // ���S�ɂ͎��܂肫��Ȃ��ꍇ
      cchCopySize = cchBuffer - 1;
      szFileNameA[cchCopySize] = '\0';
    }

    // Unicode �ɕϊ�
    MultiByteToWideChar(CP_ACP, 0, szFileNameA, cchCopySize + 1, lpszBuffer, cchCopySize + 1);
  }
  return cchFileName;
}

/*--------------------------------------------------------------------------
  �T�v:         �i�[�t�@�C���̉𓀌�̃T�C�Y�𓾂܂��B

  �錾:         DWORD UnDpmGetOriginalSize(
                    HDPM hDpm,      // �A�[�J�C�u�n���h��
                    DWORD iFileId   // �t�@�C�� ID
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

                iFileId
                    �t�@�C�� ID ���w�肵�܂��B

  �߂�l:       �i�[�t�@�C���̉𓀌�̃T�C�Y���Ԃ�܂��B

  ���:         �i�[�t�@�C���̉𓀌�̃T�C�Y�𓾂܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmGetOriginalSize(HDPM hDpm, DWORD iFileId)
{
  DPMFILEDIR FileDir; // �t�@�C�����

  // �t�@�C�������擾
  if(UnDpmGetFileDir(hDpm, iFileId, &FileDir))
  {
    return FileDir.dwFileSize;
  }
  return 0;
}

/*--------------------------------------------------------------------------
  �T�v:         �i�[�t�@�C���̈��k�T�C�Y�𓾂܂��B

  �錾:         DWORD UnDpmGetCompressedSize(
                    HDPM hDpm,      // �A�[�J�C�u�n���h��
                    DWORD iFileId   // �t�@�C�� ID
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

                iFileId
                    �t�@�C�� ID ���w�肵�܂��B

  �߂�l:       �i�[�t�@�C���̈��k�T�C�Y���Ԃ�܂��B

  ���:         �i�[�t�@�C���̈��k�T�C�Y�𓾂܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmGetCompressedSize(HDPM hDpm, DWORD iFileId)
{
  DPMFILEDIR FileDir; // �t�@�C�����

  // �t�@�C�������擾
  if(UnDpmGetFileDir(hDpm, iFileId, &FileDir))
  {
    return FileDir.dwFileSize;
  }
  return 0;
}

/*--------------------------------------------------------------------------
  �T�v:         �i�[�t�@�C�����Í�������Ă��邩���ׂ܂��B

  �錾:         BOOL UnDpmIsCryptedFile(
                    HDPM hDpm,      // �A�[�J�C�u�n���h��
          DWORD iFileId   // �t�@�C�� ID
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

                iFileId
                    �t�@�C�� ID ���w�肵�܂��B

  �߂�l:       �Í�������Ă���΁A0 �ȊO�̒l���Ԃ�܂��B

                �Í�������Ă��Ȃ���΁A0 ���Ԃ�܂��B

  ���:         �i�[�t�@�C�����Í�������Ă��邩���ׂ܂��B
                �Í�������Ă��Ă� UnDpmExtract �Ȃǂœǂݏo�����\�ł��B
--------------------------------------------------------------------------*/
UNDPMAPI BOOL UnDpmIsCryptedFile(HDPM hDpm, DWORD iFileId)
{
  return (BOOL)(UnDpmGetCryptKey(hDpm, iFileId) != CRYPT_DPMFILE_KEY_NOCRYPT);
}

/*--------------------------------------------------------------------------
  �T�v:         �i�[�t�@�C���̌������J�n���܂��B (ANSI)

  �錾:         DWORD UnDpmFindFirstFileA(
                    HDPM hDpm,          // �A�[�J�C�u�n���h��
                    LPCSTR lpszWildName // �����t�@�C����
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�n���h�����w�肵�܂��B

                lpszWildName
                    ��������t�@�C�������w�肵�܂��B
                    ���C���h�J�[�h���g�����w�肪�\�ł��B

  �߂�l:       �֐�����������ƁA�t�@�C�� ID ���Ԃ�܂��B

                �֐������s����ƁADPM_INVALID_FILE_ID ���Ԃ�܂��B

  ���:         �i�[�t�@�C���̌������J�n���܂��B
                ���������� UnDpmFindNextFile �֐����g���čs���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmFindFirstFileA(HDPM hDpm, LPCSTR lpszWildName)
{
  DWORD iFileId;          // �t�@�C�� ID
  LPWSTR lpszWildNameW;   // Unicode �ɕϊ������t�@�C����

  // ������� Unicode �ɕϊ�
  lpszWildNameW = ConvertStringAToW(lpszWildName);

  // Unicode �ł̊֐����Ăяo��
  iFileId = UnDpmFindFirstFileW(hDpm, lpszWildNameW);

  // Unicode �ɕϊ���������������
  FreeConvertedString(lpszWildNameW);

  // �A�[�J�C�u�n���h����Ԃ�
  return iFileId;
}

/*--------------------------------------------------------------------------
  �T�v:         �i�[�t�@�C���̌������J�n���܂��B (Unicode)

  �錾:         DWORD UnDpmFindFirstFileW(
                    HDPM hDpm,              // �A�[�J�C�u�n���h��
                    LPCWSTR lpszWildName    // �����t�@�C����
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�n���h�����w�肵�܂��B

                lpszWildName
                    ��������t�@�C�������w�肵�܂��B
                    ���C���h�J�[�h���g�����w�肪�\�ł��B

  �߂�l:       �֐�����������ƁA�t�@�C�� ID ���Ԃ�܂��B

                �֐������s����ƁADPM_INVALID_FILE_ID ���Ԃ�܂��B

  ���:         �i�[�t�@�C���̌������J�n���܂��B
                ���������� UnDpmFindNextFile �֐����g���čs���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmFindFirstFileW(HDPM hDpm, LPCWSTR lpszWildName)
{
  LPDPMINTERNALINFO lpInternalInfo = UnDpmGetInternalInfo(hDpm);  // �������

  // ���������R�s�[
  lstrcpyW(lpInternalInfo->szFindName, lpszWildName);

  // �ŏ��̃t�@�C����ݒ�
  lpInternalInfo->iFindFileId = 1;

  // �T�C�Y����������
  lpInternalInfo->dwOriginalSizeTotal = 0;
  lpInternalInfo->dwCompressedSizeTotal = 0;

  return UnDpmFindNextFile(hDpm);
}

/*--------------------------------------------------------------------------
  �T�v:         �i�[�t�@�C���̌��������܂��B

  �錾:         DWORD UnDpmFindNextFile(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�n���h�����w�肵�܂��B

  �߂�l:       �w�肵���t�@�C����������ƁA�t�@�C�� ID ���Ԃ�܂��B

                �w�肵���t�@�C����������Ȃ������ꍇ
                DPM_INVALID_FILE_ID ���Ԃ�܂��B

  ���:         �i�[�t�@�C���̌��������܂��B
                �����J�n�� UnDpmFindFirstFile �֐����g���čs���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI DWORD UnDpmFindNextFile(HDPM hDpm)
{
  LPDPMINTERNALINFO lpInternalInfo;   // �������
  DPMHED DpmHed;                      // �w�b�_���
  WCHAR szFileName[_MAX_PATH];        // �t�@�C����

  // ���������擾
  lpInternalInfo = UnDpmGetInternalInfo(hDpm);

  // �w�b�_�����擾
  UnDpmGetDpmHed(hDpm, &DpmHed);

  // �t�@�C��������
  for(; lpInternalInfo->iFindFileId <= DpmHed.nNumberOfFile; lpInternalInfo->iFindFileId++)
  {
    // �t�@�C�������擾
    UnDpmGetFileNameW(hDpm, lpInternalInfo->iFindFileId, szFileName, countof(szFileName));

    // �����ɂ������t�@�C������
    if(PathMatchSpecW(szFileName, lpInternalInfo->szFindName))
    {
      // �T�C�Y���擾����
      lpInternalInfo->dwOriginalSizeTotal += UnDpmGetOriginalSize(hDpm, lpInternalInfo->iFindFileId);
      lpInternalInfo->dwCompressedSizeTotal += UnDpmGetCompressedSize(hDpm, lpInternalInfo->iFindFileId);

      // �t�@�C�� ID ��Ԃ�
      return lpInternalInfo->iFindFileId++;
    }
  }

  return DPM_INVALID_FILE_ID;
}

/*--------------------------------------------------------------------------
  �T�v:         �i�[�t�@�C���̈Í������擾���܂��B

  �錾:         DWORD UnDpmGetCryptKey(
                    HDPM hDpm,      // �A�[�J�C�u�n���h��
                    DWORD iFileId   // �t�@�C�� ID
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

                iFileId
                    �t�@�C�� ID ���w�肵�܂��B

  �߂�l:       �Í������Ԃ�܂��B
                �Í�������Ă��Ȃ��ꍇ�ACRYPT_DPMFILE_KEY_NOCRYPT ���Ԃ�܂��B

  ���:         �i�[�t�@�C���̈Í������擾���܂��B
--------------------------------------------------------------------------*/
DWORD UnDpmGetCryptKey(HDPM hDpm, DWORD iFileId)
{
  DPMFILEDIR FileDir; // �t�@�C�����

  // �t�@�C�������擾
  if(UnDpmGetFileDir(hDpm, iFileId, &FileDir))
  {
    return FileDir.dwCryptKey;
  }
  else
  {
    // �t�@�C�����擾�G���[
    return CRYPT_DPMFILE_KEY_NOCRYPT;
  }
}
