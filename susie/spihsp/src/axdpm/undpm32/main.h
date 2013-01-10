/*--------------------------------------------------------------------------
  main.h        UNDPM32.DLL main
--------------------------------------------------------------------------*/

#ifndef __UNDPM32_MAIN_H__
#define __UNDPM32_MAIN_H__

#include <windows.h>
#include "undpm32.h"

/*--------------------------------------------------------------------------
  �^�錾
--------------------------------------------------------------------------*/

// �A���C�����g�� 1 �o�C�g��
#pragma pack(push, 1)

typedef struct tagDPMHED {
  BYTE sSignature[4];     // �V�O�l�`��
  DWORD dwOffsetToDS;     // �f�[�^�Z�N�V�����ւ̃I�t�Z�b�g
  DWORD nNumberOfFile;    // �t�@�C����
  DWORD dwReserved;       // �\��ς�
} DPMHED, *LPDPMHED;

typedef struct tagDPMFILEDIR {
  BYTE sFileName[16];     // �t�@�C���� (NULL ����������)
  DWORD dwReserved;       // �\��ς�? (��� 0xFFFFFFFF)
  DWORD dwCryptKey;       // �Í���
  DWORD dwOffsetInDS;     // �f�[�^�ւ̃I�t�Z�b�g
  DWORD dwFileSize;       // �f�[�^�T�C�Y
} DPMFILEDIR, *LPDPMFILEDIR;

typedef struct tagHSPHED {
  BYTE szSignature[9];    // �V�O�l�`��
  BYTE sDpmOffset[8];     // DPM �ւ̃I�t�Z�b�g
  BYTE szExeType[2];      // ���s�t�@�C���̃^�C�v
  BYTE cMarkerX;          // ���̃f�[�^���������� (x)
  WORD nWidth;            // �E�B���h�E x �T�C�Y
  BYTE cMarkerY;          // ���̃f�[�^���������� (y)
  WORD nHeight;           // �E�B���h�E y �T�C�Y
  BYTE cMarkerD;          // ���̃f�[�^���������� (d)
  WORD fDisplaySwitch;    // �E�B���h�E���\���ɂ���t���O
  BYTE cMarkerS;          // ���̃f�[�^���������� (s)
  WORD wCheckSum;         // �`�F�b�N�T��
  BYTE cMarkerK;          // ���̃f�[�^���������� (k)
  DWORD dwCryptKey;       // �Í��� (���s�t�@�C��)
  BYTE byReserved;        // �\��ς� (_)
} HSPHED, *LPHSPHED;

// �A���C�����g��߂�
#pragma pack(pop)

typedef struct tagDPMINTERNALINFO {
  DWORD dwFileSize;               // �t�@�C���T�C�Y
  DWORD dwVersion;                // �A�[�J�C�u�̃o�[�W����
  BOOL bExecutable;               // ���s�t�@�C���`����
  DWORD dwExeFileKey;             // ���s�t�@�C���̌�
  DWORD iFindFileId;              // FindFirstFile �Ŏg���t�@�C�� ID
  WCHAR szFindName[_MAX_PATH];    // FindFirstFile �Ŏg��������
  DWORD dwOriginalSizeTotal;      // �𓀌�̃T�C�Y�̍��v
  DWORD dwCompressedSizeTotal;    // ���k�T�C�Y�̍��v
} DPMINTERNALINFO, *LPDPMINTERNALINFO;

/*--------------------------------------------------------------------------
  �֐��錾
--------------------------------------------------------------------------*/
LPDPMINTERNALINFO UnDpmGetInternalInfo(HDPM hDpm);
BOOL UnDpmGetDpmHed(HDPM hDpm, LPDPMHED lpHed);
BOOL UnDpmGetFileDir(HDPM hDpm, DWORD iFileId, LPDPMFILEDIR lpFileDir);
LPVOID UnDpmGetDataSection(HDPM hDpm);
BOOL UnDpmIsFileId(HDPM hDpm, DWORD iFileId);

/*--------------------------------------------------------------------------
  �t�@�C���I��
--------------------------------------------------------------------------*/
#endif // !defined(__UNDPM32_MAIN_H__)