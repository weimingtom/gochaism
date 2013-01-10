/*--------------------------------------------------------------------------
  crypt.c       UNDPM32.DLL crypt main
--------------------------------------------------------------------------*/

#include <windows.h>
#include <stdlib.h>

#include "undpm32.h"
#include "main.h"
#include "info.h"
#include "crypt.h"

/*--------------------------------------------------------------------------
  �萔�錾
--------------------------------------------------------------------------*/
#ifdef HSP33_SUPPORT
#define CRYPT_DPM_PRM1          (BYTE)0x5a
#define CRYPT_DPM_PRM2          (BYTE)0xa5
#else
#define CRYPT_DPM_PRM1          (BYTE)0x55
#define CRYPT_DPM_PRM2          (BYTE)0xaa
#endif

#define CRYPT_EXE_PRM1          3
#define CRYPT_EXE_PRM2          5
#define CRYPT_EXE_PRM3          (BYTE)0xaa

#define CRYPT_EXE_KEY_DEFAULT   MAKEWORD(0xaa, 0x55)

#define CRYPT_CRYPT_KEY_VER255  MAKEWORD(0x55, 0x01)

/*--------------------------------------------------------------------------
  �T�v:         �Í������ꂽ�o�b�t�@�𕜍������܂��B

  �錾:         LPVOID UnDpmDecrypt(
                    LPVOID lpDest,      // �R�s�[��o�b�t�@
                    LPCVOID lpSrc,      // �R�s�[���o�b�t�@
                    DWORD dwSize,       // �R�s�[����T�C�Y
                    DWORD dwDpmFileKey, // �Í���
                    BYTE byDecodeSeed   // �����o�C�g�����l
                );

  �p�����[�^:   lpDest
                    �R�s�[��o�b�t�@�ւ̃|�C���^���w�肵�܂��B

                lpSrc
                    �R�s�[���o�b�t�@�ւ̃|�C���^���w�肵�܂��B

                dwSize
                    �R�s�[����T�C�Y���w�肵�܂��B

                dwDpmFileKey
                    �Í������w�肵�܂��B

                byDecodeSeed
                    �����o�C�g�̏����l���w�肵�܂��B
                    �ߋ��̃o�[�W�����ł͕K��0�ł������A
                    HSP3.3�ȍ~�ł́A�O�̊i�[�t�@�C���Ɉˑ����܂��B

  �߂�l:       lpDest ���Ԃ�܂��B

  ���:         �Í������ꂽ�o�b�t�@�𕜍������܂��B
                �O�� DPM �t�@�C���ȊO�̂��̂͂��̂܂܂ł͕������ł��܂���B
                �\�� dwDpmFileKey ���Z�o���Ă������Ƃŕ������ł��܂��B
--------------------------------------------------------------------------*/
LPVOID UnDpmDecrypt(LPVOID lpDest, LPCVOID lpSrc, DWORD dwSize, DWORD dwDpmFileKey, BYTE byDecodeSeed)
{
  const WORD wCryptKey = UnDpmMakeCryptKey(UnDpmMakeDpmKey(dwDpmFileKey), 0, FALSE);
  const BYTE byCryptKey1 = LOBYTE(wCryptKey);
  const BYTE byCryptKey2 = HIBYTE(wCryptKey);

  DWORD i;                      // ����ϐ�
  BYTE byData = byDecodeSeed;   // �������݃f�[�^

  // �Í�������Ă��邩
  if(dwDpmFileKey == CRYPT_DPMFILE_KEY_NOCRYPT)
  {
    // �R�s�[
    if (lpDest != lpSrc)
    {
      memcpy(lpDest, lpSrc, dwSize);
    }
  }
  else
  {
    // ������
    for(i = 0; i < dwSize; i++)
    {
#ifdef HSP33_SUPPORT
      byData += (BYTE)((*((LPBYTE)lpSrc + i) ^ byCryptKey1) - byCryptKey2);
#else
      byData += (BYTE)((*((LPBYTE)lpSrc + i) - byCryptKey2) ^ byCryptKey1);
#endif
      *((LPBYTE)lpDest + i) = byData;
    }
  }

  return lpDest;
}

/*--------------------------------------------------------------------------
  �T�v:         �A�[�J�C�u�̈Í����𒲐����܂��B

  �錾:         BOOL UnDpmAdjustCryptKey(
                    HDPM hDpm   // �A�[�J�C�u�n���h��
                );

  �p�����[�^:   hDpm
                    �A�[�J�C�u�̃n���h�����w�肵�܂��B

  �߂�l:       �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         �A�[�J�C�u�̈Í����𒲐����܂��B

                �Â��o�[�W�����̃A�[�J�C�u����s�t�@�C���`���̃A�[�J�C�u��
                �J�����Ƃ��ł���悤�ɂȂ�܂��B
--------------------------------------------------------------------------*/
BOOL UnDpmAdjustCryptKey(HDPM hDpm)
{
  LPDPMINTERNALINFO lpInternalInfo = UnDpmGetInternalInfo(hDpm);
  LPDPMHED lpDpmHed = (LPDPMHED)(lpInternalInfo + 1);
  LPDPMFILEDIR lpFirstFile = (LPDPMFILEDIR)(lpDpmHed + 1);

  DWORD i;    // ����ϐ�

  // �Í����̂Ȃ��o�[�W�����͏������Ȃ�
  if(lpInternalInfo->dwVersion > DPM_VERSION_HSP2X)
  {
    // ���ׂẴt�@�C��������
    for(i = 0; i < lpDpmHed->nNumberOfFile; i++)
    {
      // �Í�������Ă��Ȃ�
      if((lpFirstFile + i)->dwCryptKey == CRYPT_DPMFILE_KEY_NOCRYPT)
      {
        continue;
      }

      // �o�[�W�����ɂ���ĕ���
      switch(lpInternalInfo->dwVersion)
      {
      case DPM_VERSION_HSP255:
        (lpFirstFile + i)->dwCryptKey = UnDpmMakeDpmFileKey(CRYPT_CRYPT_KEY_VER255);
        break;

      case DPM_VERSION_HSP26:
        // ���s�t�@�C���`��
        if(lpInternalInfo->bExecutable)
        {
          (lpFirstFile + i)->dwCryptKey = UnDpmMakeDpmFileKey(
            UnDpmMakeCryptKey(
              UnDpmMakeDpmKey((lpFirstFile + i)->dwCryptKey), 
              UnDpmMakeExeKey(lpInternalInfo->dwExeFileKey, lpInternalInfo->dwFileSize - lpDpmHed->dwOffsetToDS), 
              lpInternalInfo->bExecutable
            )
          );
        }
        break;
      }
    }
  }
  return TRUE;
}

/*--------------------------------------------------------------------------
  �T�v:         DPM �t�@�C���̌����� DPM ���𐶐����܂��B

  �錾:         WORD UnDpmMakeDpmKey(
                    DWORD dwDpmFileKey  // DPM �t�@�C���̌�
                );

  �p�����[�^:   dwDpmFileKey
                    DPM �t�@�C���̌����w�肵�܂��B

  �߂�l:       �������ꂽ����Ԃ��܂��B

  ���:         DPM �t�@�C���̌����� DPM ���𐶐����܂��B
--------------------------------------------------------------------------*/
WORD UnDpmMakeDpmKey(DWORD dwDpmFileKey)
{
  const BYTE byDpmKey1 = (BYTE)((LOBYTE(LOWORD(dwDpmFileKey)) + CRYPT_DPM_PRM1) ^ LOBYTE(HIWORD(dwDpmFileKey)));
  const BYTE byDpmKey2 = (BYTE)((HIBYTE(LOWORD(dwDpmFileKey)) + CRYPT_DPM_PRM2) ^ HIBYTE(HIWORD(dwDpmFileKey)));
  return MAKEWORD(byDpmKey1, byDpmKey2);
}

/*--------------------------------------------------------------------------
  �T�v:         ���s�t�@�C���t�@�C���̌�������s�t�@�C�����𐶐����܂��B

  �錾:         WORD UnDpmMakeExeKey(
                    DWORD dwExeFileKey,     // ���s�t�@�C���̌�
                    DWORD dwDataSectionSize // �f�[�^�Z�N�V�����̃T�C�Y
                );

  �p�����[�^:   dwExeFileKey
                    ���s�t�@�C���̌����w�肵�܂��B

                dwDataSectionSize
                    DPM �t�@�C���̃f�[�^�Z�N�V�����T�C�Y���w�肵�܂��B

  �߂�l:       �������ꂽ����Ԃ��܂��B

  ���:         ���s�t�@�C���t�@�C���̌�������s�t�@�C�����𐶐����܂��B
--------------------------------------------------------------------------*/
WORD UnDpmMakeExeKey(DWORD dwExeFileKey, DWORD dwDataSectionSize)
{
  const BYTE byExeKey1 = (BYTE)(((UINT)LOBYTE(LOWORD(dwExeFileKey)) * (UINT)LOBYTE(HIWORD(dwExeFileKey)) / CRYPT_EXE_PRM1) ^ dwDataSectionSize);
  const BYTE byExeKey2 = (BYTE)(((UINT)HIBYTE(LOWORD(dwExeFileKey)) * (UINT)HIBYTE(HIWORD(dwExeFileKey)) / CRYPT_EXE_PRM2) ^ dwDataSectionSize ^ CRYPT_EXE_PRM3);
  return MAKEWORD(byExeKey1, byExeKey2);
}

/*--------------------------------------------------------------------------
  �T�v:         �Í����𐶐����܂��B

  �錾:         WORD UnDpmMakeCryptKey(
                    WORD wDpmKey,   // DPM �t�@�C�����琶������錮
                    WORD wExeKey,   // ���s�t�@�C�����琶������錮
                    BOOL bUseExeKey // ���s�t�@�C���̌����g����
                );

  �p�����[�^:   wDpmKey
                    DPM �t�@�C�����琶������錮

                wCryptKey
                    ���s�t�@�C�����琶������錮���w�肵�܂��B

                bUseExeKey
                    ���s�t�@�C�����琶������錮���g�����ǂ������w�肵�܂��B

  �߂�l:       �������ꂽ�Í�����Ԃ��܂��B

  ���:         �Í����𐶐����܂��B
--------------------------------------------------------------------------*/
WORD UnDpmMakeCryptKey(WORD wDpmKey, WORD wExeKey, BOOL bUseExeKey)
{
  BYTE byKey1;    // �Í��� (����)
  BYTE byKey2;    // �Í��� (���)

  // ���s�t�@�C���̌����g����
  if(!bUseExeKey)
  {
    // �f�t�H���g�̒l���g��
    wExeKey = CRYPT_EXE_KEY_DEFAULT;
  }

  // ���𐶐�����
  byKey1 = (BYTE)(LOBYTE(wDpmKey) + LOBYTE(wExeKey));
  byKey2 = (BYTE)(HIBYTE(wDpmKey) + HIBYTE(wExeKey));

  // ������������Ԃ�
  return MAKEWORD(byKey1, byKey2);
}

/*--------------------------------------------------------------------------
  �T�v:         DPM �t�@�C���̌��𐶐����܂��B

  �錾:         DWORD UnDpmMakeDpmFileKey(
                    WORD wCryptKey  // �Í���
                );

  �p�����[�^:   wCryptKey
                    �Í������w�肵�܂��B

  �߂�l:       �������ꂽ DPM �t�@�C���̌���Ԃ��܂��B

  ���:         DPM �t�@�C���̌��𐶐����܂��B
--------------------------------------------------------------------------*/
DWORD UnDpmMakeDpmFileKey(WORD wCryptKey)
{
  const BYTE byDpmFileKey1 = (BYTE)0; // �D���Ȓl�ō\���܂���
  const BYTE byDpmFileKey2 = (BYTE)0; // ���������Ɏc��𐶐����܂�
  const BYTE byDpmFileKey3 = (BYTE)((LOBYTE(wCryptKey) - LOBYTE(CRYPT_EXE_KEY_DEFAULT)) ^ (byDpmFileKey1 + CRYPT_DPM_PRM1));
  const BYTE byDpmFileKey4 = (BYTE)((HIBYTE(wCryptKey) - HIBYTE(CRYPT_EXE_KEY_DEFAULT)) ^ (byDpmFileKey2 + CRYPT_DPM_PRM2));
  return MAKELONG(MAKEWORD(byDpmFileKey1, byDpmFileKey2), MAKEWORD(byDpmFileKey3, byDpmFileKey4));
}
