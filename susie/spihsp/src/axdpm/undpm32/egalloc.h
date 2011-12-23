/*--------------------------------------------------------------------------
  egalloc.h     EASY MEMORY ALLOCATION
                written by gocha <http://gocha.s151.xrea.com>
                all rights NOT reserved, feel free to redistribute ;)

  言語:         日本語 (Japanese)
  最終更新:     2004 年 10 月 10 日
  複製・改変:   自由に行ってかまいません。
--------------------------------------------------------------------------*/

#ifndef __EGALLOC_H__
#define __EGALLOC_H__

#include <windows.h>

/*--------------------------------------------------------------------------
  関数宣言
--------------------------------------------------------------------------*/
LPVOID MemoryAlloc(SIZE_T nBytes);

LPVOID MemoryReAlloc(LPVOID lpMem, SIZE_T nBytes);

DWORD MemorySize(LPCVOID lpMem);

BOOL MemoryFree(LPVOID lpMem);

/*--------------------------------------------------------------------------
  ファイル終了
--------------------------------------------------------------------------*/
#endif // !defined(__EGALLOC_H__)