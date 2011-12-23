/*--------------------------------------------------------------------------
  egalloc.h     EASY MEMORY ALLOCATION
                written by gocha <http://gocha.s151.xrea.com>
                all rights NOT reserved, feel free to redistribute ;)

  ����:         ���{�� (Japanese)
  �ŏI�X�V:     2004 �N 10 �� 10 ��
  �����E����:   ���R�ɍs���Ă��܂��܂���B
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