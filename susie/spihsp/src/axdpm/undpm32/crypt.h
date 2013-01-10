/*--------------------------------------------------------------------------
  crypt.h       UNDPM32.DLL crypt main
--------------------------------------------------------------------------*/

#ifndef __UNDPM32_CRYPT_H__
#define __UNDPM32_CRYPT_H__

#include <windows.h>
#include "undpm32.h"

/*--------------------------------------------------------------------------
  �萔�錾
--------------------------------------------------------------------------*/
#define CRYPT_DPMFILE_KEY_NOCRYPT   0
#define CRYPT_DPMFILE_KEY_VER255    0x11223344

/*--------------------------------------------------------------------------
  �֐��錾
--------------------------------------------------------------------------*/
LPVOID UnDpmDecrypt(LPVOID lpDest, LPCVOID lpSrc, DWORD dwSize, DWORD dwDpmFileKey, BYTE byDecodeSeed);

BOOL UnDpmAdjustCryptKey(HDPM hDpm);

WORD UnDpmMakeDpmKey(DWORD dwDpmFileKey);
WORD UnDpmMakeExeKey(DWORD dwExeFileKey, DWORD dwDataSectionSize);
WORD UnDpmMakeCryptKey(WORD wDpmKey, WORD wExeKey, BOOL bUseExeKey);

DWORD UnDpmMakeDpmFileKey(WORD wCryptKey);

/*--------------------------------------------------------------------------
  �t�@�C���I��
--------------------------------------------------------------------------*/
#endif // !defined(__UNDPM32_CRYPT_H__)