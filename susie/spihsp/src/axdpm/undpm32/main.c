/*--------------------------------------------------------------------------
  main.c        UNDPM32.DLL main
--------------------------------------------------------------------------*/

#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "undpm32.h"
#include "main.h"
#include "info.h"
#include "crypt.h"

#include "egalloc.h"
#include "egcnvstr.h"
#include "egstring.h"

/*--------------------------------------------------------------------------
  �萔�錾
--------------------------------------------------------------------------*/
#define DPM_SIGNATURE       "DPMX"          // DPM �̃V�O�l�`��

/*--------------------------------------------------------------------------
  �}�N����`
--------------------------------------------------------------------------*/
#define DWORDAlignment(x)   ((x) + 3 & ~3)  // DWORD ���E�ɑ�����

/*--------------------------------------------------------------------------
  �֐��錾
--------------------------------------------------------------------------*/
static LPVOID   UnDpmGetDpmByHandle(HDPM hDpm);

static LPDPMFILEDIR UnDpmGetFileDirPtr(HDPM hDpm, DWORD iFileId);

static HDPM     UnDpmOpenDpmMem(LPVOID lpBuffer, DWORD dwSize);
static HDPM     UnDpmOpenExeMem(LPVOID lpBuffer, DWORD dwSize);
static BOOL     UnDpmCheckDpmMem(LPVOID lpBuffer, DWORD dwSize, LPDWORD lpdwArcSize);
static BOOL     UnDpmCheckExeMem(LPVOID lpBuffer, DWORD dwSize, LPHSPHED *lpHspHed, LPDPMHED *lpDpmHed, LPDWORD lpdwArcSize);
static BOOL     UnDpmSearchHspHed(LPVOID lpBuffer, DWORD dwSize, LPHSPHED *lpHspHed, LPDPMHED *lpDpmHed, LPDWORD lpdwArcSize);
static DWORD    UnDpmCheckArcVersion(HDPM hDpm);

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u����܂��B

  �錾:         BOOL UnDpmCloseArchive(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         �A�[�J�C�u����܂��BUnDpmOpenArchive �֐��ȂǂŎ擾����
                �n���h����n���܂��B��������n���h���͎g�p���Ȃ��ł��������B
--------------------------------------------------------------------------*/
UNDPMAPI BOOL UnDpmCloseArchive(HDPM hDpm)
{
  return MemoryFree(hDpm);
}

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u���J���܂��B (ANSI)

  �錾:         HDPM UnDpmOpenArchiveA(
                    LPCSTR lpszFileName // �t�@�C����
                );

  �p�����[�^:   lpszFileName
                    �J�������A�[�J�C�u�̃t�@�C�������w�肵�܂��B
                dwOffset
                    �A�[�J�C�u�̐擪�ʒu���w�肵�܂��B�ʏ��0���w�肵�܂��B

  �߂�l:       �֐�����������ƁA�A�[�J�C�u�̃n���h�����Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �A�[�J�C�u���J���܂��BUnDpmOpenArchive �֐��œn�����n���h
                �����e��֐��ɓn�����Ƃő�����s���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI HDPM UnDpmOpenArchiveA(LPCSTR lpszFileName, DWORD dwOffset)
{
  HDPM hDpm;              // �A�[�J�C�u�n���h��
  LPWSTR lpszFileNameW;   // Unicode �ɕϊ������t�@�C����

  // ������� Unicode �ɕϊ�
  lpszFileNameW = ConvertStringAToW(lpszFileName);

  // Unicode �ł̊֐����Ăяo��
  hDpm = UnDpmOpenArchiveW(lpszFileNameW, dwOffset);

  // Unicode �ɕϊ���������������
  FreeConvertedString(lpszFileNameW);

  // �A�[�J�C�u�n���h����Ԃ�
  return hDpm;
}

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u���J���܂��B (Unicode)

  �錾:         HDPM UnDpmOpenArchiveW(
                    LPCWSTR lpszFileName    // �t�@�C����
                );

  �p�����[�^:   lpszFileName
                    �J�������A�[�J�C�u�̃t�@�C�������w�肵�܂��B
                dwOffset
                    �A�[�J�C�u�̐擪�ʒu���w�肵�܂��B�ʏ��0���w�肵�܂��B

  �߂�l:       �֐�����������ƁA�A�[�J�C�u�̃n���h�����Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �A�[�J�C�u�̑�����s�����߂̃n���h�����擾���܂��B
                ���̃n���h�����g���ăA�[�J�C�u�ɑ�����s���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI HDPM UnDpmOpenArchiveW(LPCWSTR lpszFileName, DWORD dwOffset)
{
  HDPM hDpm = NULL;       // �A�[�J�C�u�n���h��

  LPVOID lpBuffer;        // �t�@�C���o�b�t�@
  HANDLE hFile;           // �t�@�C���n���h��
  DWORD dwFileSize;       // �t�@�C���T�C�Y
  DWORD dwReadSize;       // �t�@�C������ǂݍ��񂾃T�C�Y

  // �t�@�C�����J��
  hFile = CreateFileW(
    lpszFileName, 
    GENERIC_READ, 
    0, 
    NULL, 
    OPEN_EXISTING, 
    FILE_ATTRIBUTE_NORMAL, 
    NULL
  );

  // �t�@�C�����J������
  if(hFile != INVALID_HANDLE_VALUE)
  {
    // �t�@�C���T�C�Y���擾
    dwFileSize = GetFileSize(hFile, NULL);

    // ����ɃT�C�Y���擾�ł�����
    if(dwFileSize != -1)
    {
      // �擪�Ƃ���ʒu���ړ�������
      if(SetFilePointer(hFile, dwOffset, NULL, FILE_BEGIN) == dwOffset)
      {
        // �t�@�C���T�C�Y�𒲐�
        dwFileSize -= dwOffset;

        // �������u���b�N�̊��蓖��
        lpBuffer = MemoryAlloc(dwFileSize);

        // ����Ƀ����������蓖�Ă�ꂽ��
        if(lpBuffer)
        {
          // �t�@�C����ǂݍ���
          ReadFile(hFile, lpBuffer, dwFileSize, &dwReadSize, NULL);

          // ����ɓǂݍ��݂ł�����
          if(dwFileSize == dwReadSize)
          {
            // ����������A�[�J�C�u���J��
            hDpm = UnDpmOpenArchiveMem(lpBuffer, dwFileSize);
          }

          // ���������������
          MemoryFree(lpBuffer);
        }
      }
    }

    // �t�@�C�������
    CloseHandle(hFile);
  }
  return hDpm;
}

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u������������J���܂��B

  �錾:         HDPM UnDpmOpenArchiveMem(
                    LPVOID lpBuffer,    // �o�b�t�@�ւ̃|�C���^
                    DWORD dwSize        // �A�[�J�C�u�̃T�C�Y (�o�C�g�P��)
                );

  �p�����[�^:   lpBuffer
                    �A�[�J�C�u��ێ����Ă���o�b�t�@�ւ̃|�C���^���w�肵��
                    ���B

                dwSize
                    �A�[�J�C�u�̃T�C�Y���o�C�g�P�ʂŎw�肵�܂��B
                    ���ۂ̃T�C�Y���傫���l���w�肵�Ă��\���܂���B

  �߂�l:       �֐�����������ƁA�A�[�J�C�u�̃n���h�����Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �A�[�J�C�u�̑�����s�����߂̃n���h�����擾���܂��B
                ���̃n���h�����g���ăA�[�J�C�u�ɑ�����s���܂��B
--------------------------------------------------------------------------*/
UNDPMAPI HDPM UnDpmOpenArchiveMem(LPVOID lpBuffer, DWORD dwSize)
{
  HDPM hDpm = NULL;   // �A�[�J�C�u�n���h��

  // ���s�t�@�C�����J��
  hDpm = UnDpmOpenExeMem(lpBuffer, dwSize);

  // �G���[����������
  if(!hDpm)
  {
    // DPM �A�[�J�C�u���J��
    hDpm = UnDpmOpenDpmMem(lpBuffer, dwSize);
  }

  // ����ɓǂݍ��݂ł���
  if(hDpm)
  {
    // �Í����𒲐�
    UnDpmAdjustCryptKey(hDpm);
#if 0
    // �A�[�J�C�u�Ǎ����ɑS�̂𕜍�����
    {
      BYTE byDecodeSeed = 0;
      DWORD fileCount = UnDpmGetFileCount(hDpm);
      DWORD iFileId;
      for (iFileId = 1; iFileId <= fileCount; iFileId++)
      {
      	LPDPMFILEDIR lpFileDir; // �t�@�C�����
      	LPBYTE lpFileData;  // �t�@�C���f�[�^��
        DWORD dwFileSize;   // �t�@�C���T�C�Y

        // �t�@�C�������擾
        lpFileDir = UnDpmGetFileDirPtr(hDpm, iFileId);

        // �t�@�C���S�̂𕜍�
        if(lpFileDir->dwCryptKey != CRYPT_DPMFILE_KEY_NOCRYPT)
        {
          dwFileSize = UnDpmGetOriginalSize(hDpm, iFileId);
          if(dwFileSize > 0)
          {
            lpFileData = (LPBYTE)UnDpmGetDataSection(hDpm) + lpFileDir->dwOffsetInDS;
            UnDpmDecrypt(lpFileData, lpFileData, dwFileSize, lpFileDir->dwCryptKey, byDecodeSeed);
          }

          // �Í����𕜍��ς݂Ƃ��ă}�[�N
          lpFileDir->dwCryptKey = CRYPT_DPMFILE_KEY_NOCRYPT;
        }
      }
    }
#endif
  }

  // �A�[�J�C�u�n���h����Ԃ�
  return hDpm;
}

/*--------------------------------------------------------------------------
  �T�v:         �t�@�C�����𓀂��܂��B (ANSI)

  �錾:         BOOL UnDpmExtractA(
                    HDPM hDpm,          // �A�[�J�C�u�n���h��
                    DWORD iFileId,      // �t�@�C�� ID
                    LPSTR lpszDirName   // �𓀐�f�B���N�g��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

                iFileId
                    �t�@�C�� ID ���w�肵�܂��B

                szDirName
                    �𓀐�̃f�B���N�g�����w�肵�܂��B

  �߂�l:       �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         �t�@�C�����𓀂��܂��B
--------------------------------------------------------------------------*/
UNDPMAPI BOOL UnDpmExtractA(HDPM hDpm, DWORD iFileId, LPSTR lpszDirName)
{
  BOOL bRet;              // �𓀌���
  LPWSTR lpszDirNameW;    // �𓀐�f�B���N�g�� (Unicode)

  // ������� Unicode �ɕϊ�
  lpszDirNameW = ConvertStringAToW(lpszDirName);

  // Unicode �ł̊֐����Ăяo��
  bRet = UnDpmExtractW(hDpm, iFileId, lpszDirNameW);

  // Unicode �ɕϊ���������������
  FreeConvertedString(lpszDirNameW);

  // ���ʂ�Ԃ�
  return bRet;
}

/*--------------------------------------------------------------------------
  �T�v:         �t�@�C�����𓀂��܂��B (Unicode)

  �錾:         BOOL UnDpmExtractW(
                    HDPM hDpm,          // �A�[�J�C�u�n���h��
                    DWORD iFileId,      // �t�@�C�� ID
                    LPSTR lpszDirName   // �𓀐�f�B���N�g��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

                iFileId
                    �t�@�C�� ID ���w�肵�܂��B

                szDirName
                    �𓀐�̃f�B���N�g�����w�肵�܂��B

  �߂�l:       �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         �t�@�C�����𓀂��܂��B
--------------------------------------------------------------------------*/
UNDPMAPI BOOL UnDpmExtractW(HDPM hDpm, DWORD iFileId, LPWSTR lpszDirName)
{
  BOOL bRet = FALSE;              // �����̌���
  DWORD dwOriginalSize;           // �𓀌�̃T�C�Y
  DWORD dwWrittenSize;            // �������݂��ꂽ�T�C�Y
  WCHAR szCurrentDir[_MAX_PATH];  // �J�����g�f�B���N�g��
  WCHAR szFileName[_MAX_PATH];    // �������݃t�@�C����
  BOOL bMoveDirectory;            // �f�B���N�g���ړ��t���O
  LPVOID lpBuffer;                // �������݃o�b�t�@
  HANDLE hFile;                   // �t�@�C���n���h��

  // �f�B���N�g���ړ��̕K�v�������邩
  bMoveDirectory = (BOOL)((lpszDirName != NULL) && (StrCmpW(lpszDirName, L"") != 0));

  // ����ȃt�@�C�� ID ��
  if(UnDpmIsFileId(hDpm, iFileId))
  {
    // �𓀌�̃T�C�Y���擾
    dwOriginalSize = UnDpmGetOriginalSize(hDpm, iFileId);

    // �������o�b�t�@�̊��蓖��
    lpBuffer = MemoryAlloc(dwOriginalSize);

    // ����Ƀ����������蓖�Ă�ꂽ��
    if(lpBuffer)
    {
      // �������o�b�t�@�ɉ�
      if(UnDpmExtractMem(hDpm, iFileId, lpBuffer, dwOriginalSize))
      {
        // �t�@�C�������擾
        UnDpmGetFileNameW(hDpm, iFileId, szFileName, countof(szFileName));

        // �f�B���N�g���ړ��̕K�v�����邩
        if(bMoveDirectory)
        {
          // �J�����g�f�B���N�g�����擾
          GetCurrentDirectoryW(countof(szCurrentDir), szCurrentDir);

          // �J�����g�f�B���N�g�����ړ�
          SetCurrentDirectoryW(lpszDirName);
        }

        // �t�@�C�����쐬
        hFile = CreateFileW(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

        // �t�@�C���쐬�ɐ���������
        if(hFile != INVALID_HANDLE_VALUE)
        {
          // �f�[�^����������
          WriteFile(hFile, lpBuffer, dwOriginalSize, &dwWrittenSize, NULL);

          // ����ɏ������݂ł�����
          if(dwWrittenSize == dwOriginalSize)
          {
            bRet = TRUE;
          }

          // �t�@�C�������
          CloseHandle(hFile);
        }

        // �f�B���N�g���ړ��̕K�v�����邩
        if(bMoveDirectory)
        {
          // �J�����g�f�B���N�g����߂�
          SetCurrentDirectoryW(szCurrentDir);
        }
      }

      // �������o�b�t�@�����
      MemoryFree(lpBuffer);
    }
  }

  return bRet;
}

/*--------------------------------------------------------------------------
  �T�v:         �������o�b�t�@�Ƀt�@�C�����𓀂��܂��B

  �錾:         BOOL UnDpmExtractMem(
                    HDPM hDpm,          // �A�[�J�C�u�n���h��
                    DWORD iFileId,      // �t�@�C�� ID
                    LPVOID lpBuffer,    // �f�[�^���i�[����o�b�t�@
                    DWORD dwSize        // �ǂݎ��T�C�Y
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

                iFileId
                    �t�@�C�� ID ���w�肵�܂��B

                lpBuffer
                    �𓀐�̃������o�b�t�@�ւ̃|�C���^���w�肵�܂��B

                dwSize
                    �𓀂��s���T�C�Y���w�肵�܂��B

  �߂�l:       �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         �������o�b�t�@�Ƀt�@�C�����𓀂��܂��B
--------------------------------------------------------------------------*/
UNDPMAPI BOOL UnDpmExtractMem(HDPM hDpm, DWORD iFileId, LPVOID lpBuffer, DWORD dwSize)
{
  DPMFILEDIR FileDir;     // �t�@�C�����

  // �t�@�C�������擾
  if(!UnDpmGetFileDir(hDpm, iFileId, &FileDir))
  {
    return FALSE;
  }

  // �T�C�Y�`�F�b�N
  if(UnDpmGetOriginalSize(hDpm, iFileId) < dwSize)
  {
    return FALSE;
  }

  // �������ɉ�
  UnDpmDecrypt(lpBuffer, (LPBYTE)UnDpmGetDataSection(hDpm) + FileDir.dwOffsetInDS, dwSize, FileDir.dwCryptKey, 0x00);

  // �𓀏I��
  return TRUE;
}

/*--------------------------------------------------------------------------
  �T�v:         DPMINTERNALINFO �\���̂ւ̃|�C���^���擾���܂��B

  �錾:         LPDPMINTERNALINFO UnDpmGetInternalInfo(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       �֐�����������ƁA�w�肳�ꂽ�n���h���Ɋ֘A�t����ꂽ 
                DPMINTERNALINFO �\���̂ւ̃|�C���^���Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �w�肳�ꂽ�n���h���Ɋ֘A�t����ꂽ DPMINTERNALINFO �\���̂�
                �̃|�C���^���擾���܂��B
--------------------------------------------------------------------------*/
LPDPMINTERNALINFO UnDpmGetInternalInfo(HDPM hDpm)
{
  return (LPDPMINTERNALINFO)hDpm;
}

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u�̃w�b�_�����擾���܂��B

  �錾:         BOOL UnDpmGetDpmHed(
                    HDPM hDpm,      // �A�[�J�C�u�n���h��
                    LPDPMHED lpHed  // �w�b�_�����i�[����o�b�t�@
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

                lpHed
                    �w�b�_�����i�[����o�b�t�@�̃|�C���^���w�肵�܂��B

  �߂�l:       �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         �A�[�J�C�u�̃w�b�_�����擾���܂��B
--------------------------------------------------------------------------*/
BOOL UnDpmGetDpmHed(HDPM hDpm, LPDPMHED lpHed)
{
  // �w�b�_�����R�s�[
  memcpy(lpHed, UnDpmGetDpmByHandle(hDpm), sizeof(DPMHED));

  return TRUE;
}

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u���t�@�C���̏����擾���܂��B

  �錾:         BOOL UnDpmGetFileDir(
                    HDPM hDpm,              // �A�[�J�C�u�n���h��
                    DWORD iFileId,          // �t�@�C����\�����l
                    LPDPMFILEDIR lpFileDir  // �t�@�C�������i�[����o�b�t�@
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

                iFileId
                    �t�@�C����\�����l���w�肵�܂��B

                    UnDpmFilenameToId �֐����g���ăt�@�C�������炱�̐��l��
                    �擾���邱�Ƃ��ł��܂��B

                lpFileDir
                    �t�@�C�������i�[����o�b�t�@�̃|�C���^���w�肵�܂��B

  �߂�l:       �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         �A�[�J�C�u���t�@�C���̏����擾���܂��B
--------------------------------------------------------------------------*/
BOOL UnDpmGetFileDir(HDPM hDpm, DWORD iFileId, LPDPMFILEDIR lpFileDir)
{
  LPDPMFILEDIR lpFileDirSrc;  // �t�@�C�����|�C���^

  // �t�@�C�������擾
  lpFileDirSrc = UnDpmGetFileDirPtr(hDpm, iFileId);
  if(lpFileDirSrc == NULL)
  {
    return FALSE;
  }

  // �t�@�C�������R�s�[
  memcpy(lpFileDir, lpFileDirSrc, sizeof(DPMFILEDIR));

  // ����
  return TRUE;
}

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u���t�@�C���̏��ւ̃|�C���^���擾���܂��B

  �錾:         LPDPMFILEDIR UnDpmGetFileDirPtr(
                    HDPM hDpm,              // �A�[�J�C�u�n���h��
                    DWORD iFileId           // �t�@�C����\�����l
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

                iFileId
                    �t�@�C����\�����l���w�肵�܂��B

                    UnDpmFilenameToId �֐����g���ăt�@�C�������炱�̐��l��
                    �擾���邱�Ƃ��ł��܂��B

  �߂�l:       �֐�����������ƁA�t�@�C�����ւ̃|�C���^���Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �A�[�J�C�u���t�@�C���̏��̈�ւ̃|�C���^���擾���܂��B
                ���̊֐��͓��e������������ۂɓ����I�Ɏg�p�������̂ł��B
                ���̎擾�ړI�ɂ� UnDpmGetFileDir ���g�p���Ă��������B
--------------------------------------------------------------------------*/
static LPDPMFILEDIR UnDpmGetFileDirPtr(HDPM hDpm, DWORD iFileId)
{
  LPDPMHED lpDpmHed;          // �w�b�_���
  LPDPMFILEDIR lpFirstFile;   // �ŏ��̃t�@�C�����

  // �w�b�_�����擾
  lpDpmHed = (LPDPMHED)UnDpmGetDpmByHandle(hDpm);

  // �t�@�C�� ID �`�F�b�N
  if(!UnDpmIsFileId(hDpm, iFileId))
  {
    return NULL;
  }

  // �ŏ��̃t�@�C�������擾
  lpFirstFile = (LPDPMFILEDIR)(lpDpmHed + 1);

  // �t�@�C�����ւ̃|�C���^��Ԃ�
  return lpFirstFile + (iFileId - 1);
}

/*--------------------------------------------------------------------------
  �T�v:         �f�[�^�Z�N�V�����ւ̃|�C���^���擾���܂��B

  �錾:         LPVOID UnDpmGetDataSection(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       �֐�����������ƁA�f�[�^�Z�N�V�����ւ̃|�C���^���Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �f�[�^�Z�N�V�����ւ̃|�C���^���擾���܂��B
--------------------------------------------------------------------------*/
LPVOID UnDpmGetDataSection(HDPM hDpm)
{
  LPDPMHED lpDpmHed = (LPDPMHED)UnDpmGetDpmByHandle(hDpm);    // �w�b�_���

  return (LPBYTE)lpDpmHed + lpDpmHed->dwOffsetToDS;
}

/*--------------------------------------------------------------------------
  �T�v:         �t�@�C�� ID �����킩�ǂ������ׂ܂��B

  �錾:         BOOL UnDpmIsFileId(
                    HDPM hDpm,      // �A�[�J�C�u�n���h��
                    DWORD iFileId   // �t�@�C�� ID
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

                iFileId
                    �t�@�C�� ID ���w�肵�܂��B

  �߂�l:       ����ȃt�@�C�� ID �Ȃ�΁A0 �ȊO�̒l���Ԃ�܂��B

                �ُ�ȃt�@�C�� ID �Ȃ�΁A0 ���Ԃ�܂��B

  ���:         �t�@�C�� ID �����킩�ǂ������ׂ܂��B
--------------------------------------------------------------------------*/
BOOL UnDpmIsFileId(HDPM hDpm, DWORD iFileId)
{
  if(iFileId != DPM_INVALID_FILE_ID && iFileId <= UnDpmGetFileCount(hDpm))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*--------------------------------------------------------------------------
  �T�v:         �n���h������A�[�J�C�u�f�[�^�̃|�C���^���擾���܂��B

  �錾:         LPVOID UnDpmGetDpmByHandle(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       �֐�����������ƁA�A�[�J�C�u�f�[�^�̃|�C���^���Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �n���h������A�[�J�C�u�f�[�^�̃|�C���^���擾���܂��B
--------------------------------------------------------------------------*/
static LPVOID UnDpmGetDpmByHandle(HDPM hDpm)
{
  return (LPVOID)((LPDPMINTERNALINFO)hDpm + 1);
}

/*--------------------------------------------------------------------------
  �T�v:         DPM �A�[�J�C�u������������J���܂��B

  �錾:         HDPM UnDpmOpenDpmMem(
                    LPVOID lpBuffer,    // �o�b�t�@�ւ̃|�C���^
                    DWORD dwSize        // �A�[�J�C�u�̃T�C�Y (�o�C�g�P��)
                );

  �p�����[�^:   lpBuffer
                    �A�[�J�C�u��ێ����Ă���o�b�t�@�ւ̃|�C���^���w�肵��
                    ���B

                dwSize
                    �A�[�J�C�u�̃T�C�Y���o�C�g�P�ʂŎw�肵�܂��B

  �߂�l:       �֐�����������ƁA�A�[�J�C�u�̃n���h�����Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �A�[�J�C�u�̑�����s�����߂̃n���h�����擾���܂��B

                ���̊֐��ł͎��s�t�@�C���`���̃A�[�J�C�u���J�����Ƃ͂ł���
                ����B���s�t�@�C���`���� UnDpmOpenExeMem �֐����g���ĊJ����
                �Ƃ��ł��܂��B
--------------------------------------------------------------------------*/
static HDPM UnDpmOpenDpmMem(LPVOID lpBuffer, DWORD dwSize)
{
  HDPM hDpm;                          // �A�[�J�C�u�n���h��
  DWORD dwArcSize;                    // �A�[�J�C�u�T�C�Y
  LPDPMINTERNALINFO lpInternalInfo;   // �������

  // �A�[�J�C�u�`�F�b�N
  if(!UnDpmCheckDpmMem(lpBuffer, dwSize, &dwArcSize))
  {
    return NULL;
  }

  // �������u���b�N�����蓖��
  hDpm = MemoryAlloc(dwArcSize + sizeof(DPMINTERNALINFO));

  // ����Ƀ����������蓖�Ă�ꂽ��
  if(hDpm)
  {
    // �t�@�C���f�[�^���R�s�[
    memcpy(UnDpmGetDpmByHandle(hDpm), lpBuffer, dwArcSize);

    // �A�[�J�C�u�����N���A
    lpInternalInfo = UnDpmGetInternalInfo(hDpm);
    ZeroMemory(lpInternalInfo, sizeof(DPMINTERNALINFO));
    lpInternalInfo->dwFileSize = dwArcSize;
    lpInternalInfo->dwVersion = UnDpmCheckArcVersion(hDpm);
    lpInternalInfo->bExecutable = FALSE;
  }

  return hDpm;
}

/*--------------------------------------------------------------------------
  �T�v:         ���s�t�@�C���`���̃A�[�J�C�u������������J���܂��B

  �錾:         HDPM UnDpmOpenExeMem(
                    LPVOID lpBuffer,    // �o�b�t�@�ւ̃|�C���^
                    DWORD dwSize        // �A�[�J�C�u�̃T�C�Y (�o�C�g�P��)
                );

  �p�����[�^:   lpBuffer
                    �A�[�J�C�u��ێ����Ă���o�b�t�@�ւ̃|�C���^���w�肵��
                    ���B

                dwSize
                    �A�[�J�C�u�̃T�C�Y���o�C�g�P�ʂŎw�肵�܂��B

  �߂�l:       �֐�����������ƁA�A�[�J�C�u�̃n���h�����Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �A�[�J�C�u�̑�����s�����߂̃n���h�����擾���܂��B

                ���̊֐��ł͒P�̃t�@�C���`���̃A�[�J�C�u���J�����Ƃ͂ł���
                ����B�P�̃t�@�C���`���� UnDpmOpenDpmMem �֐����g���ĊJ����
                �Ƃ��ł��܂��B
--------------------------------------------------------------------------*/
static HDPM UnDpmOpenExeMem(LPVOID lpBuffer, DWORD dwSize)
{
  HDPM hDpm = NULL;                   // �A�[�J�C�u�n���h��
#ifdef UNDPM32_ALLOWEXEFILE
  DWORD dwDpmSize;                    // DPM �̃T�C�Y
  LPHSPHED lpHspHed;                  // HSPHED �̈ʒu�������|�C���^
  LPDPMHED lpDpmHed;                  // DPMHED �̈ʒu�������|�C���^
  LPDPMINTERNALINFO lpInternalInfo;   // DPMINTERNALINFO �̈ʒu�������|�C���^

  // �A�[�J�C�u�`�F�b�N
  if(UnDpmCheckExeMem(lpBuffer, dwSize, &lpHspHed, &lpDpmHed, &dwDpmSize))
  {
    // ���������� DPM �ǂݍ���
    hDpm = UnDpmOpenDpmMem(lpDpmHed, dwDpmSize);

    // ����ɓǂݍ��߂���
    if(hDpm)
    {
      // �������
      lpInternalInfo = UnDpmGetInternalInfo(hDpm);

      // ���s�t�@�C���`��
      lpInternalInfo->bExecutable = TRUE;

      // ���s�t�@�C���̌�
      lpInternalInfo->dwExeFileKey = lpHspHed->dwCryptKey;
    }
  }
#endif
  return hDpm;
}

/*--------------------------------------------------------------------------
  �T�v:         DPM �A�[�J�C�u�̐��������������܂��B

  �錾:         BOOL UnDpmCheckDpmMem(
                    LPVOID lpBuffer,    // �o�b�t�@�ւ̃|�C���^
                    DWORD dwSize,       // �A�[�J�C�u�̃T�C�Y (�o�C�g�P��)
                    LPDWORD lpdwArcSize // �A�[�J�C�u�̃T�C�Y���󂯎��ϐ�
                );

  �p�����[�^:   lpBuffer
                    �A�[�J�C�u��ێ����Ă���o�b�t�@�ւ̃|�C���^���w�肵��
                    ���B

                dwSize
                    �A�[�J�C�u�̃T�C�Y���o�C�g�P�ʂŎw�肵�܂��B
                    �A�[�J�C�u�̃T�C�Y�����傫���l�ł��\���܂���B

                lpdwArcSize
                    �A�[�J�C�u�̎��ۂ̃T�C�Y���擾����ϐ��ւ̃|�C���^���w
                    �肵�܂��B

  �߂�l:       �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         �A�[�J�C�u�̐��������������܂��B
--------------------------------------------------------------------------*/
static BOOL UnDpmCheckDpmMem(LPVOID lpBuffer, DWORD dwSize, LPDWORD lpdwArcSize)
{
  DWORD i;    // ����ϐ�

  LPDPMHED lpDpmHed = lpBuffer;   // �w�b�_�ւ̃|�C���^
  LPDPMFILEDIR lpFileDir;         // �t�@�C�����ւ̃|�C���^
  DWORD dwRequiredSize;           // �A�[�J�C�u�ɕK�v�ȃT�C�Y

  // �T�C�Y�`�F�b�N (�w�b�_)
  if(dwSize < sizeof(DPMHED))
  {
    return FALSE;
  }

  // �V�O�l�`���̃`�F�b�N
  if(memcmp(lpDpmHed->sSignature, DPM_SIGNATURE, 4) != 0)
  {
    return FALSE;
  }

  // �T�C�Y�`�F�b�N (�t�@�C�����)
  if(dwSize < sizeof(DPMHED) + sizeof(DPMFILEDIR) * lpDpmHed->nNumberOfFile)
  {
    return FALSE;
  }

  // �t�@�C�����ւ̃|�C���^���Z�b�g
  lpFileDir = (LPDPMFILEDIR)(lpDpmHed + 1);

  // ���ׂẴt�@�C�����`�F�b�N
  *lpdwArcSize = sizeof(DPMHED) + sizeof(DPMFILEDIR) * lpDpmHed->nNumberOfFile;
  for(i = 0; i < lpDpmHed->nNumberOfFile; i++)
  {
    // �K�v�ȃT�C�Y���v�Z
    dwRequiredSize = lpDpmHed->dwOffsetToDS             // �f�[�^�Z�N�V�����ʒu
                     + (lpFileDir + i)->dwOffsetInDS    // �t�@�C���f�[�^�ʒu
                     + (lpFileDir + i)->dwFileSize      // �t�@�C���T�C�Y
    ;

    // �T�C�Y�`�F�b�N
    if(dwSize < dwRequiredSize)
    {
      return FALSE;
    }

    // �A�[�J�C�u�̃T�C�Y���X�V
    if(dwRequiredSize > *lpdwArcSize)
    {
      *lpdwArcSize = dwRequiredSize;
    }
  }

  return TRUE;
}

/*--------------------------------------------------------------------------
  �T�v:         ���s�t�@�C���`���̃A�[�J�C�u�̐��������������܂��B

  �錾:         BOOL UnDpmCheckExeMem(
                    LPVOID lpBuffer,    // �o�b�t�@�ւ̃|�C���^
                    DWORD dwSize,       // �A�[�J�C�u�̃T�C�Y (�o�C�g�P��)
                    LPHSPHED *lpHspHed, // HSPHED �̈ʒu���󂯎��|�C���^
                    LPDPMHED *lpDpmHed  // DPMHED �̈ʒu���󂯎��|�C���^
                );

  �p�����[�^:   lpBuffer
                    �A�[�J�C�u��ێ����Ă���o�b�t�@�ւ̃|�C���^���w�肵��
                    ���B

                dwSize
                    �A�[�J�C�u�̃T�C�Y���o�C�g�P�ʂŎw�肵�܂��B
                    �A�[�J�C�u�̃T�C�Y�����傫���l�ł��\���܂���B

                lpHspHed
                    HSPHED �̈ʒu���󂯎��|�C���^�ւ̃|�C���^���w�肵�܂��B

                lpDpmHed
                    DPMHED �̈ʒu���󂯎��|�C���^�ւ̃|�C���^���w�肵�܂��B

                lpdwArcSize
                    �A�[�J�C�u�̎��ۂ̃T�C�Y���擾����ϐ��ւ̃|�C���^���w
                    �肵�܂��B

  �߂�l:       �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         �A�[�J�C�u�̐��������������܂��B

                HSPHED �͎��s�t�@�C���̓���Ɋւ����񂪊i�[���ꂽ�̈�ł��B
                DPM �̈ʒu��Í����Ȃǂ̏��𓾂邱�Ƃ��ł��܂��B
--------------------------------------------------------------------------*/
static BOOL UnDpmCheckExeMem(LPVOID lpBuffer, DWORD dwSize, LPHSPHED *lpHspHed, LPDPMHED *lpDpmHed, LPDWORD lpdwArcSize)
{
#ifdef UNDPM32_ALLOWEXEFILE
  LPVOID lpPEHeader;

  // �T�C�Y�`�F�b�N
  if(dwSize < 0x40)
  {
    return FALSE;
  }

  // ���s�t�@�C����
  if(memcmp(lpBuffer, "MZ", 2) != 0)
  {
    return FALSE;
  }

  // PE (Portable Executable) Header �̈ʒu���擾
  lpPEHeader = (LPBYTE)lpBuffer + *(LPDWORD)((LPBYTE)lpBuffer + 0x3C);

  // PE ��
  if(memcmp(lpPEHeader, "PE\0\0", 4) != 0)
  {
    return FALSE;
  }

  // HSPHED �̈ʒu���擾
  if(!UnDpmSearchHspHed(lpBuffer, dwSize, lpHspHed, lpDpmHed, lpdwArcSize))
  {
    return FALSE;
  }

  return TRUE;
#else
  return FALSE;
#endif
}

/*--------------------------------------------------------------------------
  �T�v:         HSPHED �̈ʒu���擾���܂��B

  �錾:         BOOL UnDpmSearchHspHed(
                    LPVOID lpBuffer,    // �o�b�t�@�ւ̃|�C���^
                    DWORD dwSize,       // �A�[�J�C�u�̃T�C�Y (�o�C�g�P��)
                    LPHSPHED *lpHspHed, // HSPHED �̈ʒu���󂯎��|�C���^
          LPDPMHED *lpDpmHed, // DPM �̃I�t�Z�b�g���󂯎��ϐ�
                    LPDWORD lpdwArcSize // �A�[�J�C�u�̃T�C�Y���󂯎��ϐ�
                );

  �p�����[�^:   lpBuffer
                    �A�[�J�C�u��ێ����Ă���o�b�t�@�ւ̃|�C���^���w�肵��
                    ���B

                dwSize
                    �A�[�J�C�u�̃T�C�Y���o�C�g�P�ʂŎw�肵�܂��B
                    �A�[�J�C�u�̃T�C�Y�����傫���l���w�肵�Ă��\���܂���B

                lpHspHed
                    HSPHED �̈ʒu���󂯎��|�C���^�ւ̃|�C���^���w�肵�܂��B

                lpDpmHed
                    DPM �̈ʒu���󂯎��|�C���^�ւ̃|�C���^���w�肵�܂��B
                    ���̒l�� lpHspHed ������ HSPHED �\���̂̒l�Ɋ�Â��܂��B

                lpdwArcSize
                    �A�[�J�C�u�̎��ۂ̃T�C�Y���擾����ϐ��ւ̃|�C���^���w
                    �肵�܂��B

  �߂�l:       �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         HSPHED �̈ʒu���擾���܂��B

                HSPHED �͎��s�t�@�C���̓���Ɋւ����񂪊i�[���ꂽ�̈�ł��B
                DPM �̈ʒu��Í����Ȃǂ̏��𓾂邱�Ƃ��ł��܂��B
--------------------------------------------------------------------------*/
static BOOL UnDpmSearchHspHed(LPVOID lpBuffer, DWORD dwSize, LPHSPHED *lpHspHed, LPDPMHED *lpDpmHed, LPDWORD lpdwArcSize)
{
  DWORD i;                // ����ϐ�
  CHAR szDpmOffsetA[64];  // DPM �ւ̃I�t�Z�b�g������ (ANSI)
  UINT nDpmOffset;        // DPM �ւ̃I�t�Z�b�g
  LPHSPHED lpHspHedTemp;  // HSPHED �ւ̃|�C���^
  LPDPMHED lpDpmHedTemp;  // DPMHED �ւ̃|�C���^

  // DPM�Ǝ��s�t�@�C���ŗL�̏�������
  for(i = 0; i <= dwSize - sizeof(HSPHED); i++)
  {
    BYTE c;

    // HSPHED �̌��ʒu���i�[ (�o�[�W�����ԍ��ق̓s���ő�������)
    lpHspHedTemp = (LPHSPHED)((LPBYTE)lpBuffer + i);
    // ���l���ۂ��H
    c = lpHspHedTemp->sDpmOffset[0];
    if((c >= '0' && c <= '9') || c == '+' || c == '-')
    {
      // DPM �̃I�t�Z�b�g���擾
      lstrcpynA(szDpmOffsetA, lpHspHedTemp->sDpmOffset, sizeof(lpHspHedTemp->sDpmOffset) + 1);
      // �����񂩂琔�l�ɕϊ�
      nDpmOffset = StrToIntA(szDpmOffsetA) + 0x10000;
      // DPMHED �̌��ʒu���i�[
      lpDpmHedTemp = (LPDPMHED)((LPBYTE)lpBuffer + nDpmOffset);

      // �I�t�Z�b�g�͈̔̓`�F�b�N
      if(nDpmOffset < dwSize)
      {
        // DPM ���`�F�b�N
        if(UnDpmCheckDpmMem(lpDpmHedTemp, dwSize - nDpmOffset, lpdwArcSize))
        {
          *lpHspHed = lpHspHedTemp;
          *lpDpmHed = lpDpmHedTemp;
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u�̃o�[�W�������擾���܂��B

  �錾:         DWORD UnDpmCheckArcVersion(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       �A�[�J�C�u�̃o�[�W�����������l���Ԃ�܂��B

  ���:         �A�[�J�C�u�̃o�[�W�������擾���܂��B
--------------------------------------------------------------------------*/
static DWORD UnDpmCheckArcVersion(HDPM hDpm)
{
  DPMHED DpmHed;          // �w�b�_���
  DPMFILEDIR FileDir;     // �t�@�C�����
  BOOL bNoCrypt = TRUE;   // �Í�������Ă��邩
  BOOL bOldCrypt = TRUE;  // �Â��Í���������
  DWORD i;                // ����ϐ�

  // �w�b�_�����擾
  if(UnDpmGetDpmHed(hDpm, &DpmHed))
  {
    // ���ׂẴt�@�C�����`�F�b�N
    for(i = 1; i <= DpmHed.nNumberOfFile; i++)
    {
      // �t�@�C�������擾
      if(UnDpmGetFileDir(hDpm, i, &FileDir))
      {
        // �Í�������Ă��邩
        if(FileDir.dwCryptKey != CRYPT_DPMFILE_KEY_NOCRYPT)
        {
          bNoCrypt = FALSE;

          // 2.55 �̌Œ�Í�����
          if(FileDir.dwCryptKey != CRYPT_DPMFILE_KEY_VER255)
          {
            bOldCrypt = FALSE;
          }
        }
      }
    }
  }

  // �Í�������Ă��邩
  if(bNoCrypt)
  {
    return DPM_VERSION_HSP2X;
  }
  else{
    // �Â��Í���������
    if(bOldCrypt)
    {
      return DPM_VERSION_HSP255;
    }
    else
    {
      return DPM_VERSION_HSP26;
    }
  }
}
