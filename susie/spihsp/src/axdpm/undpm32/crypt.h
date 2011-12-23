/*--------------------------------------------------------------------------
  crypt.h       UNDPM32.DLL CRYPT
                written by gocha <http://gocha.s151.xrea.com>
                all rights NOT reserved, feel free to redistribute ;)

  言語:         日本語 (Japanese)
  最終更新:     2004 年 10 月 12 日
  複製・改変:   自由に行ってかまいません。
--------------------------------------------------------------------------*/

#ifndef __UNDPM32_CRYPT_H__
#define __UNDPM32_CRYPT_H__

#include <windows.h>
#include "undpm32.h"

/*--------------------------------------------------------------------------
  定数宣言
--------------------------------------------------------------------------*/
#define CRYPT_DPMFILE_KEY_NOCRYPT   0
#define CRYPT_DPMFILE_KEY_VER255    0x11223344

/*--------------------------------------------------------------------------
  関数宣言
--------------------------------------------------------------------------*/
LPVOID UnDpmDecrypt(LPVOID lpDest, LPCVOID lpSrc, DWORD dwSize, DWORD dwDpmFileKey);

BOOL UnDpmAdjustCryptKey(HDPM hDpm);

WORD UnDpmMakeDpmKey(DWORD dwDpmFileKey);
WORD UnDpmMakeExeKey(DWORD dwExeFileKey, DWORD dwDataSectionSize);
WORD UnDpmMakeCryptKey(WORD wDpmKey, WORD wExeKey, BOOL bUseExeKey);

DWORD UnDpmMakeDpmFileKey(WORD wCryptKey);

/*--------------------------------------------------------------------------
  ファイル終了
--------------------------------------------------------------------------*/
#endif // !defined(__UNDPM32_CRYPT_H__)