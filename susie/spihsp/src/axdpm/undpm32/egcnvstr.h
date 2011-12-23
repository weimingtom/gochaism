/*--------------------------------------------------------------------------
  egcnvstr.h    EASY CONVERT STRING A/W
                written by gocha <http://gocha.s151.xrea.com>
                all rights NOT reserved, feel free to redistribute ;)

  言語:         日本語 (Japanese)
  最終更新:     2004 年 10 月 10 日
  複製・改変:   自由に行ってかまいません。
--------------------------------------------------------------------------*/

#ifndef __CNVSTR_H__
#define __CNVSTR_H__

#include <windows.h>

/*--------------------------------------------------------------------------
  関数宣言
--------------------------------------------------------------------------*/
LPWSTR ConvertStringAToW(LPCSTR lpString);

LPSTR ConvertStringWToA(LPCWSTR lpString);

BOOL FreeConvertedString(LPVOID lpString);

/*--------------------------------------------------------------------------
  ファイル終了
--------------------------------------------------------------------------*/
#endif // !defined(__CNVSTR_H__)