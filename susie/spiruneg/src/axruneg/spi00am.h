/**
 * SUSIE32 '00AM' Plug-in Sample by shimitei (modified by gocha)
 * http://www.asahi-net.or.jp/~kh4s-smz/spi/make_spi.html
 */

#ifndef spi00am_h
#define spi00am_h

#include <windows.h>

/*-------------------------------------------------------------------------*/
// �t�@�C�����\����
/*-------------------------------------------------------------------------*/
#pragma pack(push)
#pragma pack(1) //�\���̂̃����o���E��1�o�C�g�ɂ���
typedef struct fileInfo
{
  unsigned char method[8];      /* ���k�@�̎�� */
  unsigned long position;       /* �t�@�C����ł̈ʒu */
  unsigned long compsize;       /* ���k���ꂽ�T�C�Y */
  unsigned long filesize;       /* ���̃t�@�C���T�C�Y */
  long /*time_t*/ timestamp;    /* �t�@�C���̍X�V���� */
  char path[200];               /* ���΃p�X */
  char filename[200];           /* �t�@�C���l�[�� */
  unsigned long crc;            /* CRC */
} fileInfo;
#pragma pack(pop)

/*-------------------------------------------------------------------------*/
// �G���[�R�[�h
/*-------------------------------------------------------------------------*/
#define SPI_NO_FUNCTION         -1  /* ���̋@�\�̓C���v�������g����Ă��Ȃ� */
#define SPI_ALL_RIGHT           0   /* ����I�� */
#define SPI_ABORT               1   /* �R�[���o�b�N�֐�����0��Ԃ����̂œW�J�𒆎~���� */
#define SPI_NOT_SUPPORT         2   /* ���m�̃t�H�[�}�b�g */
#define SPI_OUT_OF_ORDER        3   /* �f�[�^�����Ă��� */
#define SPI_NO_MEMORY           4   /* �������[���m�ۏo���Ȃ� */
#define SPI_MEMORY_ERROR        5   /* �������[�G���[ */
#define SPI_FILE_READ_ERROR     6   /* �t�@�C�����[�h�G���[ */
#define SPI_WINDOW_ERROR        7   /* �����J���Ȃ� (����J�̃G���[�R�[�h) */
#define SPI_OTHER_ERROR         8   /* �����G���[ */
#define SPI_FILE_WRITE_ERROR    9   /* �������݃G���[ (����J�̃G���[�R�[�h) */
#define SPI_END_OF_FILE         10  /* �t�@�C���I�[ (����J�̃G���[�R�[�h) */

/*-------------------------------------------------------------------------*/
// '00AM'�֐��̃v���g�^�C�v�錾
/*-------------------------------------------------------------------------*/
//int PASCAL ProgressCallback(int nNum, int nDenom, long lData);
typedef int (CALLBACK *SPI_PROGRESS)(int, int, long);
extern "C"
{
  int __declspec(dllexport) __stdcall GetPluginInfo
      (int infono, LPSTR buf, int buflen);
  int __declspec(dllexport) __stdcall IsSupported(LPSTR filename, DWORD dw);
  int __declspec(dllexport) __stdcall GetArchiveInfo(
      LPSTR buf, long len, unsigned int flag, HLOCAL *lphInf);
  int __declspec(dllexport) __stdcall GetFileInfo(LPSTR buf,long len,
      LPSTR filename, unsigned int flag, fileInfo *lpInfo);
  int __declspec(dllexport) __stdcall GetFile(LPSTR src,long len,
         LPSTR dest, unsigned int flag,
         SPI_PROGRESS prgressCallback, long lData);
  int __declspec(dllexport) __stdcall GetPreview
      (LPSTR buf,long len, unsigned int flag,
       HANDLE *pHBInfo, HANDLE *pHBm,
       SPI_PROGRESS lpPrgressCallback, long lData);
}

#endif
