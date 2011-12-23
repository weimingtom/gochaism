/*--------------------------------------------------------------------------
  egcnvstr.h    EASY CONVERT STRING A/W
                written by gocha <http://gocha.s151.xrea.com>
                all rights NOT reserved, feel free to redistribute ;)

  ����:         ���{�� (Japanese)
  �ŏI�X�V:     2004 �N 10 �� 10 ��
  �����E����:   ���R�ɍs���Ă��܂��܂���B
--------------------------------------------------------------------------*/

#ifndef __CNVSTR_H__
#define __CNVSTR_H__

#include <windows.h>

/*--------------------------------------------------------------------------
  �֐��錾
--------------------------------------------------------------------------*/
LPWSTR ConvertStringAToW(LPCSTR lpString);

LPSTR ConvertStringWToA(LPCWSTR lpString);

BOOL FreeConvertedString(LPVOID lpString);

/*--------------------------------------------------------------------------
  �t�@�C���I��
--------------------------------------------------------------------------*/
#endif // !defined(__CNVSTR_H__)