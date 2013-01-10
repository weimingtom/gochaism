/*--------------------------------------------------------------------------
  egcnvstr.c    string converter between multibyte string and wide string
--------------------------------------------------------------------------*/

#include <windows.h>
#include "egalloc.h"
#include "egcnvstr.h"

/*--------------------------------------------------------------------------
  �T�v:         �}���`�o�C�g����������C�h������ɕϊ����܂��B

  �錾:         LPWSTR ConvertStringAToW(
                    LPCSTR lpszString   // �}���`�o�C�g������ւ̃|�C���^
                );

  �p�����[�^:   lpszString
                    ���C�h������ɕϊ�����}���`�o�C�g������ւ̃|�C���^��
                    �w�肵�܂��B

  �߂�l:       �֐�����������ƁA���C�h������ւ̃|�C���^���Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         �}���`�o�C�g����������C�h������ɕϊ����܂��B
                �ϊ���̕������ FreeConvertedString �֐��ŉ�����܂��B
--------------------------------------------------------------------------*/
LPWSTR ConvertStringAToW(LPCSTR lpszString)
{
  LPWSTR lpszStringW; // Unicode ������ւ̃|�C���^

  const UINT cchStr = lstrlenA(lpszString) + 1;   // ������ (NULL �����܂�)
  const UINT nSize = cchStr * sizeof(WCHAR);      // �T�C�Y (�o�C�g�P��)

  // �����������蓖�Ă�
  lpszStringW = MemoryAlloc(nSize);

  if(lpszStringW)
  {
    // Unicode ������ɕϊ�
    MultiByteToWideChar(CP_ACP, 0, lpszString, cchStr, lpszStringW, cchStr);
  }

  // Unicode ������ւ̃|�C���^��Ԃ�
  return lpszStringW;
}

/*--------------------------------------------------------------------------
  �T�v:         ���C�h��������}���`�o�C�g������ɕϊ����܂��B

  �錾:         LPSTR ConvertStringWToA(
                    LPCWSTR lpszString  // ���C�h������ւ̃|�C���^
                );

  �p�����[�^:   lpszString
                    �}���`�o�C�g������ɕϊ����郏�C�h������ւ̃|�C���^��
                    �w�肵�܂��B

  �߂�l:       �֐�����������ƁA�}���`�o�C�g������ւ̃|�C���^���Ԃ�܂��B

                �֐������s����ƁANULL ���Ԃ�܂��B

  ���:         ���C�h��������}���`�o�C�g������ɕϊ����܂��B
                �ϊ���̕������ FreeConvertedString �֐��ŉ�����܂��B
--------------------------------------------------------------------------*/
LPSTR ConvertStringWToA(LPCWSTR lpszString)
{
  LPSTR lpszStringA;  // ANSI ������ւ̃|�C���^

  const UINT cchStr = lstrlenW(lpszString) + 1;   // ������ (NULL �����܂�)
  const UINT nSize = cchStr * sizeof(CHAR);       // �T�C�Y (�o�C�g�P��)

  // �����������蓖�Ă�
  lpszStringA = MemoryAlloc(nSize);

  if(lpszStringA)
  {
    // ANSI ������ɕϊ�
    WideCharToMultiByte(CP_ACP, 0, lpszString, cchStr, lpszStringA, cchStr, NULL, NULL);
  }

  // ANSI ������ւ̃|�C���^��Ԃ�
  return lpszStringA;
}

/*--------------------------------------------------------------------------
  �T�v:         ������o�b�t�@��������܂��B

  �錾:         BOOL FreeConvertedString(
                    LPVOID lpszString   // �������ւ̃|�C���^
                );

  �p�����[�^:   lpszString
                    ���������������o�b�t�@�ւ̃|�C���^���w�肵�܂��B
                    ConvertStringAToW �֐��܂��� ConvertStringWToA �֐�����
                    ���|�C���^�ł��B 

  �߂�l:       �֐�����������ƁA0 �ȊO�̒l���Ԃ�܂��B

                �֐������s����ƁA0 ���Ԃ�܂��B

  ���:         ConvertStringAToW �֐��܂��� ConvertStringWToA �֐������蓖
                �Ă�������o�b�t�@��������܂��B
--------------------------------------------------------------------------*/
BOOL FreeConvertedString(LPVOID lpszString)
{
  // ���������������
  return MemoryFree(lpszString);
}
