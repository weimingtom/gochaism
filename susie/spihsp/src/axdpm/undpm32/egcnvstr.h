/*--------------------------------------------------------------------------
  egcnvstr.h    string converter between multibyte string and wide string
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