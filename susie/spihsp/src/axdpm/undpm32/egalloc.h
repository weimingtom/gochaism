/*--------------------------------------------------------------------------
  egalloc.h     memory allocation functions for Windows
--------------------------------------------------------------------------*/

#ifndef __EGALLOC_H__
#define __EGALLOC_H__

#include <windows.h>

/*--------------------------------------------------------------------------
  �֐��錾
--------------------------------------------------------------------------*/
LPVOID MemoryAlloc(SIZE_T nBytes);

LPVOID MemoryReAlloc(LPVOID lpMem, SIZE_T nBytes);

DWORD MemorySize(LPCVOID lpMem);

BOOL MemoryFree(LPVOID lpMem);

/*--------------------------------------------------------------------------
  �t�@�C���I��
--------------------------------------------------------------------------*/
#endif // !defined(__EGALLOC_H__)