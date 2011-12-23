/*--------------------------------------------------------------------------
  info.h        UNDPM32.DLL INFORMATION
                written by gocha <http://gocha.s151.xrea.com>
                all rights NOT reserved, feel free to redistribute ;)

  ����:         ���{�� (Japanese)
  �ŏI�X�V:     2004 �N 10 �� 14 ��
  �����E����:   ���R�ɍs���Ă��܂��܂���B
--------------------------------------------------------------------------*/

#ifndef __UNDPM32_INFO_H__
#define __UNDPM32_INFO_H__

#include <windows.h>
#include "undpm32.h"

/*--------------------------------------------------------------------------
  �֐��錾
--------------------------------------------------------------------------*/
DWORD UnDpmGetArcVersion(HDPM hDpm);
DWORD UnDpmGetCryptKey(HDPM hDpm, DWORD iFileId);

/*--------------------------------------------------------------------------
  �t�@�C���I��
--------------------------------------------------------------------------*/
#endif // !defined(__UNDPM32_INFO_H__)