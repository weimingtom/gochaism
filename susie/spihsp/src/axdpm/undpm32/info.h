/*--------------------------------------------------------------------------
  info.h        UNDPM32.DLL INFORMATION
                written by gocha <http://gocha.s151.xrea.com>
                all rights NOT reserved, feel free to redistribute ;)

  言語:         日本語 (Japanese)
  最終更新:     2004 年 10 月 14 日
  複製・改変:   自由に行ってかまいません。
--------------------------------------------------------------------------*/

#ifndef __UNDPM32_INFO_H__
#define __UNDPM32_INFO_H__

#include <windows.h>
#include "undpm32.h"

/*--------------------------------------------------------------------------
  関数宣言
--------------------------------------------------------------------------*/
DWORD UnDpmGetArcVersion(HDPM hDpm);
DWORD UnDpmGetCryptKey(HDPM hDpm, DWORD iFileId);

/*--------------------------------------------------------------------------
  ファイル終了
--------------------------------------------------------------------------*/
#endif // !defined(__UNDPM32_INFO_H__)