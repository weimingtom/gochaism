/*--------------------------------------------------------------------------
  egstring.c    string functions
--------------------------------------------------------------------------*/

#include <windows.h>
#include <tchar.h>
#include "egstring.h"

/*--------------------------------------------------------------------------
  �萔�錾
--------------------------------------------------------------------------*/
#define STRMATCH_IGNORE_CASE  0x00000001  // �A���t�@�x�b�g�̑啶������������ʂ��Ȃ�

/*--------------------------------------------------------------------------
  �֐��錾
--------------------------------------------------------------------------*/
static INT CharacterToInteger(UINT cNumChar);

static BOOL _StrMatch(LPCTSTR lpszString, LPCTSTR lpszPattern, DWORD dwCmpFlags);

/*--------------------------------------------------------------------------
  �T�v:         �w�肵���������A���t�@�x�b�g�啶���ɕϊ����܂��B

  �錾:         UINT ToUpperCase(
                    UINT cTestChar  // �ϊ����镶���R�[�h
                );

  �p�����[�^:   cTestChar
                    �A���t�@�x�b�g�啶���ɕϊ������������R�[�h���w�肵�܂��B

  �߂�l:       cTestChar ���A���t�@�x�b�g�啶���ɕϊ����������R�[�h���Ԃ�
                �܂��B

  ���:         �w�肵���������A���t�@�x�b�g�啶���ɕϊ����܂��B
                �A���t�@�x�b�g�������ȊO�̕����ɕω��͂���܂���B
--------------------------------------------------------------------------*/
UINT ToUpperCase(UINT cTestChar)
{
  // �A���t�@�x�b�g������
  if(cTestChar >= 'a' && cTestChar <= 'z')
  {
    // �啶���ɕϊ�����
    return cTestChar + ('A' - 'a');
  }
  else
  {
    // �����ϊ����Ȃ�
    return cTestChar;
  }
}

/*--------------------------------------------------------------------------
  �T�v:         �w�肵���������A���t�@�x�b�g�������ɕϊ����܂��B

  �錾:         UINT ToLowerCase(
                    UINT cTestChar  // �ϊ����镶���R�[�h
                );

  �p�����[�^:   cTestChar
                    �A���t�@�x�b�g�������ɕϊ������������R�[�h���w�肵�܂��B

  �߂�l:       cTestChar ���A���t�@�x�b�g�������ɕϊ����������R�[�h���Ԃ�
                �܂��B

  ���:         �w�肵���������A���t�@�x�b�g�������ɕϊ����܂��B
                �A���t�@�x�b�g�啶���ȊO�̕����ɕω��͂���܂���B
--------------------------------------------------------------------------*/
UINT ToLowerCase(UINT cTestChar)
{
  // �A���t�@�x�b�g�啶��
  if(cTestChar >= 'A' && cTestChar <= 'Z')
  {
    // �������ɕϊ�����
    return cTestChar + ('a' - 'A');
  }
  else
  {
    // �����ϊ����Ȃ�
    return cTestChar;
  }
}

/*--------------------------------------------------------------------------
  �T�v:         ������̎��̕����R�[�h�𓾂܂��B

  �錾:         UINT GetCharCode(
                    LPCTSTR lpszString  // ������ւ̃|�C���^
                );

  �p�����[�^:   lpszString
                    �����R�[�h�𓾂���������̃|�C���^���w�肵�܂��B

  �߂�l:       lpszString �����������̕����R�[�h���Ԃ�܂��B

  ���:         �w�肵��������̎��̕����R�[�h�𓾂܂��B
--------------------------------------------------------------------------*/
UINT GetCharCode(LPCTSTR lpszString)
{
#ifdef _MBCS
  // DBCS �̐�s�o�C�g��
  if(IsDBCSLeadByte(*(LPBYTE)lpszString))
  {
    // �_�u���o�C�g����
    return (UINT)(*(LPWORD)lpszString);
  }
  else
  {
    // �V���O���o�C�g����
    return (UINT)(*(LPBYTE)lpszString);
  }
#else
  // ���C�h����
  return *(LPWSTR)lpszString;
#endif // defined(_MBCS)
}

/*--------------------------------------------------------------------------
  �T�v:         ������̎��̕����̈ʒu���擾���܂��B

  �錾:         LPTSTR GetNextCharPointer(
                    LPCTSTR lpszString  // ������ւ̃|�C���^
                );

  �p�����[�^:   lpszString
                    ���̕����ʒu�𓾂���������̃|�C���^���w�肵�܂��B

  �߂�l:       lpszString �����������̎��̕����ւ̃|�C���^���Ԃ�܂��B

  ���:         �w�肵��������̎��̕����ւ̃|�C���^�𓾂܂��B
                �}���`�o�C�g���������������̂ɕ֗��ł��B
--------------------------------------------------------------------------*/
LPTSTR GetNextCharPointer(LPCTSTR lpszString)
{
#ifdef _MBCS
  // DBCS �̐�s�o�C�g��
  if(IsDBCSLeadByte(*(LPBYTE)lpszString))
  {
    // �_�u���o�C�g����
    return (LPTSTR)((LPWORD)lpszString + 1);
  }
  else
  {
    // �V���O���o�C�g����
    return (LPTSTR)((LPBYTE)lpszString + 1);
  }
#else
  // ���C�h����
  return (LPTSTR)((LPWSTR)lpszString + 1);
#endif // defined(_MBCS)
}

/*--------------------------------------------------------------------------
  �T�v:         ������̐�������̈ʒu���擾���܂��B

  �錾:         LPTSTR GetNextCharPointerN(
                    LPCTSTR lpszString  // ������ւ̃|�C���^
                    UINT cchMove        // �ړ��� (�������P��)
                );

  �p�����[�^:   lpszString
                    ������̃|�C���^���w�肵�܂��B

                cchMove
                    �i�݂������������w�肵�܂��B

  �߂�l:       lpszString �� cchMove ������ւ̃|�C���^���Ԃ�܂��B

  ���:         �w�肵��������̐�������ւ̃|�C���^�𓾂܂��B
--------------------------------------------------------------------------*/
LPTSTR GetNextCharPointerN(LPCTSTR lpszString, UINT cchMove)
{
  return cchMove == 0 ? (LPTSTR)lpszString : GetNextCharPointerN(GetNextCharPointer(lpszString), cchMove - 1);
}

/*--------------------------------------------------------------------------
  �T�v:         ������̋󔒕������΂��܂��B

  �錾:         LPTSTR SkipWhiteSpace(
                    LPCTSTR lpszString  // ������ւ̃|�C���^
                );

  �p�����[�^:   lpszString
                    �󔒕������΂�����������̃|�C���^���w�肵�܂��B

  �߂�l:       �󔒈ȊO�̕��������ꂽ�ʒu�ւ̃|�C���^��Ԃ��܂��B

  ���:         ������̋󔒕������΂��܂��B
--------------------------------------------------------------------------*/
LPTSTR SkipWhiteSpace(LPCTSTR lpszString)
{
  LPTSTR lpszNewPtr = (LPTSTR)lpszString; // �����Ɏg�p����|�C���^

  UINT cCurrent;  // ���݈ʒu�̕����R�[�h

  // 1 �����擾
  cCurrent = GetCharCode(lpszNewPtr);

  // �󔒕����Ȃ�Ώ���
  while(cCurrent == ' ' || cCurrent == '\t')
  {
    // �󔒕������΂�
    lpszNewPtr = GetNextCharPointer(lpszNewPtr);

    // 1 �����擾
    cCurrent = GetCharCode(lpszNewPtr);
  }
  return lpszNewPtr;
}

/*--------------------------------------------------------------------------
  �T�v:         �����𐔒l�ɕϊ����܂��B

  �錾:         INT CharacterToInteger(
                    UINT cNumChar   // ���l�ɕϊ����镶��
                );

  �p�����[�^:   cNumChar
                    ���l�ɕϊ����镶�����w�肵�܂��B

  �߂�l:       �����𐔒l�ɕϊ������l���Ԃ�܂��B
                �s���ȕ����̏ꍇ�� -1 ���Ԃ�܂��B

  ���:         �����𐔒l�ɕϊ����܂��B
--------------------------------------------------------------------------*/
static INT CharacterToInteger(UINT cNumChar)
{
  cNumChar = ToLowerCase(cNumChar);

  if(cNumChar >= '0' && cNumChar <= '9')
  {
    // ����
    return cNumChar - '0';
  }
  else if(cNumChar >= 'a' && cNumChar <= 'z')
  {
    // �A���t�@�x�b�g
    return cNumChar - 'a' + 10;
  }
  else
  {
    // ���̑�
    return -1;
  }
}

/*--------------------------------------------------------------------------
  �T�v:         ������𐔒l�ɕϊ����܂��B

  �錾:         INT StringToInteger(
                    LPCTSTR lpszString, // ���l�ɕϊ����镶����
                    UINT nBase          // �
                );

  �p�����[�^:   lpszString
                    ���l�ɕϊ����镶����ւ̃|�C���^���w�肵�܂��B

                nBase
                    ���l�̊���w�肵�܂��B

                    nBase �� 0 ���w�肵���ꍇ�A�����I�Ɋ���I������܂��B
                    �ڂ����͉�������Q�Ƃ��������B

  �߂�l:       ������𐔒l�ɕϊ������l���Ԃ�܂��B

  ���:         ������𐔒l�ɕϊ����܂��B����w�肷�邱�Ƃ��ł��܂��B

                nBase �� 0 ���w�肵���ꍇ�A�����I�Ɋ���I������܂��B
                0x �Ŏn�܂镶����� 16 �i���A����ȊO�� 0 �Ŏn�܂镶�����
                8 �i���A1 ���� 9 �Ŏn�܂镶����� 10 �i���Aa ���� z �Ŏn��
                �镶����� 11 ���� 36 �܂ł̒l�Ɋ��蓖�Ă܂��B
--------------------------------------------------------------------------*/
INT StringToInteger(LPCTSTR lpszString, UINT nBase)
{
  LPTSTR lpszCurrent;     // �����ʒu
  INT iRet = 0;           // �ԋp�����l
  BOOL bUnsigned = TRUE;  // ���� (��: TRUE / ��: FALSE)
  UINT cCurrent;          // �����ʒu�̕���
  UINT nCharacter;        // �������\�����l

  // �󔒕�������΂�
  lpszCurrent = SkipWhiteSpace(lpszString);

  // �����𒲂ׂ�
  cCurrent = GetCharCode(lpszCurrent);
  switch(cCurrent)
  {
  case '-':
    bUnsigned = FALSE;
  case '+':
    lpszCurrent = GetNextCharPointer(lpszCurrent);
  }

  // ����w�肳��Ă��邩
  if(nBase == 0)
  {
    // 1 �����擾����
    nCharacter = CharacterToInteger(GetCharCode(lpszCurrent));

    if(nCharacter == 0)
    {
      // ���̕��������Ĕ��f
      if(ToLowerCase(GetCharCode(GetNextCharPointer(lpszCurrent))) == 'x')
      {
        nBase = 16;
        lpszCurrent = GetNextCharPointerN(lpszCurrent, 2);
      }
      else{
        nBase = 8;
        lpszCurrent = GetNextCharPointer(lpszCurrent);
      }
    }
    if(nCharacter >= 1 && nCharacter <= 9)
    {
      // ���� (0 �ȊO)
      nBase = 10;
    }
    else if(nCharacter >= 10 && nCharacter <= 36)
    {
      // �A���t�@�x�b�g
      nBase = nCharacter;
    }
  }

  // 1 �����擾����
  nCharacter = CharacterToInteger(GetCharCode(lpszCurrent));

  // �L���ȕ�����
  while(nCharacter != -1)
  {
    // ���オ��
    iRet = iRet * nBase + nCharacter;

    // 1 �����i�߂�
    lpszCurrent = GetNextCharPointer(lpszCurrent);

    // 1 �����擾����
    nCharacter = CharacterToInteger(GetCharCode(lpszCurrent));
  }

  // �������]
  if(!bUnsigned)
  {
    iRet = -iRet;
  }

  // �ϊ������l��Ԃ�
  return iRet;
}

/*--------------------------------------------------------------------------
  �T�v:         ���C���h�J�[�h���g�p�����������r���s���܂��B

  �錾:         BOOL _StrMatch(
                    LPCTSTR lpszString,     // ����������ւ̃|�C���^
                    LPCTSTR lpszPattern,    // �p�^�[��������ւ̃|�C���^
                    DWORD dwCmpFlags        // ��r���@�̃I�v�V����
                );

  �p�����[�^:   lpszString
                    ��r�Ɏg����ANULL �ŏI��镶����ւ̃|�C���^���w�肵
                    �܂��B

                lpszPattern
                    ��r�Ɏg����ANULL �ŏI��镶����ւ̃|�C���^���w�肵
                    �܂��B
                    lpszPattern �ɂ̓��C���h�J�[�h���g�p���邱�Ƃ��ł��܂��B
                    '*' �͔C�ӂ̕�����A'?' �͔C�ӂ� 1 ������\���܂��B

                dwCmpFlags
                    2 �̕�������r������@��\���I�v�V�������w�肵�܂��B

  �߂�l:       2 �̕����񂪈�v�����ꍇ�A0 �ȊO�̒l���Ԃ�܂��B

                2 �̕����񂪈�v���Ȃ������ꍇ�A0 ���Ԃ�܂��B

  ���:         ���C���h�J�[�h���g�p����������̔�r���s���܂��B
--------------------------------------------------------------------------*/
static BOOL _StrMatch(LPCTSTR lpszString, LPCTSTR lpszPattern, DWORD dwCmpFlags)
{
  UINT cString = GetCharCode(lpszString);     // ����������̎w�� 1 ����
  UINT cPattern = GetCharCode(lpszPattern);   // �p�^�[��������̎w�� 1 ����

  // �A���t�@�x�b�g�̑啶������������ʂ��Ȃ�
  if(dwCmpFlags & STRMATCH_IGNORE_CASE)
  {
    // �������ɕϊ�����
    cString = ToLowerCase(cString);
    cPattern = ToLowerCase(cPattern);
  }

  // 1 ���������ׂ�
  switch(cPattern)
  {
  // NULL ����
  case '\0':
    return cString == '\0';

  // �C�ӂ̕�����
  case '*':
    return _StrMatch(lpszString, GetNextCharPointer(lpszPattern), dwCmpFlags) || ((cString != '\0') && _StrMatch(GetNextCharPointer(lpszString), lpszPattern, dwCmpFlags));

  // �C�ӂ� 1 ����
  case '?':
    return (cString != '\0') && _StrMatch(GetNextCharPointer(lpszString), GetNextCharPointer(lpszPattern), dwCmpFlags);

  // ����ȊO�̕���
  default:
    return (cPattern == cString) && _StrMatch(GetNextCharPointer(lpszString), GetNextCharPointer(lpszPattern), dwCmpFlags);
  }
}

/*--------------------------------------------------------------------------
  �T�v:         ���C���h�J�[�h���g�p�����������r���s���܂��B

  �錾:         BOOL StrMatch(
                    LPCTSTR lpszString, // ������ւ̃|�C���^
                    LPCTSTR lpszPattern // �p�^�[��������ւ̃|�C���^
                );

  �p�����[�^:   lpszString
                    ��r�Ɏg����ANULL �ŏI��镶����ւ̃|�C���^���w�肵
                    �܂��B

                lpszPattern
                    ��r�Ɏg����ANULL �ŏI��镶����ւ̃|�C���^���w�肵
                    �܂��B
                    lpszPattern �ɂ̓��C���h�J�[�h���g�p���邱�Ƃ��ł��܂��B
                    '*' �͔C�ӂ̕�����A'?' �͔C�ӂ� 1 ������\���܂��B

  �߂�l:       2 �̕����񂪈�v�����ꍇ�A0 �ȊO�̒l���Ԃ�܂��B

                2 �̕����񂪈�v���Ȃ������ꍇ�A0 ���Ԃ�܂��B

  ���:         ���C���h�J�[�h���g�p����������̔�r���s���܂��B

                StrMatch �֐��̓A���t�@�x�b�g�̑啶������������ʂ��܂��B
                �A���t�@�x�b�g�̑啶������������ʂ��Ȃ��ꍇ�� StrMatchI ��
                �����g�p���Ă��������B
--------------------------------------------------------------------------*/
BOOL StrMatch(LPCTSTR lpszString, LPCTSTR lpszPattern)
{
  return _StrMatch(lpszString, lpszPattern, 0);
}

/*--------------------------------------------------------------------------
  �T�v:         ���C���h�J�[�h���g�p�����������r���s���܂��B

  �錾:         BOOL StrMatchI(
                    LPCTSTR lpszString, // ������ւ̃|�C���^
                    LPCTSTR lpszPattern // �p�^�[��������ւ̃|�C���^
                );

  �p�����[�^:   lpszString
                    ��r�Ɏg����ANULL �ŏI��镶����ւ̃|�C���^���w�肵
                    �܂��B

                lpszPattern
                    ��r�Ɏg����ANULL �ŏI��镶����ւ̃|�C���^���w�肵
                    �܂��B
                    lpszPattern �ɂ̓��C���h�J�[�h���g�p���邱�Ƃ��ł��܂��B
                    '*' �͔C�ӂ̕�����A'?' �͔C�ӂ� 1 ������\���܂��B

  �߂�l:       2 �̕����񂪈�v�����ꍇ�A0 �ȊO�̒l���Ԃ�܂��B

                2 �̕����񂪈�v���Ȃ������ꍇ�A0 ���Ԃ�܂��B

  ���:         ���C���h�J�[�h���g�p����������̔�r���s���܂��B

                StrMatchI �֐��̓A���t�@�x�b�g�̑啶������������ʂ��܂���B
                �A���t�@�x�b�g�̑啶������������ʂ���ꍇ�� StrMatch ��
                �����g�p���Ă��������B
--------------------------------------------------------------------------*/
BOOL StrMatchI(LPCTSTR lpszString, LPCTSTR lpszPattern)
{
  return _StrMatch(lpszString, lpszPattern, STRMATCH_IGNORE_CASE);
}
