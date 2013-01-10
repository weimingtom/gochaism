/**
 * Susie plug-in: HSP DPM archive extractor
 * written by gocha, feel free to redistribute
 * 
 * based on spi00am_ex.h by Shimitei
 * http://www.asahi-net.or.jp/~kh4s-smz/spi/make_spi.html
 * 
 * ���s�t�@�C���T�|�[�g�̗L����undpm32.h�̐錾�Ō��܂�܂�
 */

#ifndef axdpm_h
#define axdpm_h

#include <windows.h>
#include "spi00am.h"
#include "undpm32/undpm32.h"

/*-------------------------------------------------------------------------*/
// ����Plugin�̏��
/*-------------------------------------------------------------------------*/
static const char *pluginfo[] = {
  "00AM",                           /* Plug-in API�o�[�W���� */
  "HSP DPM extractor by gocha",     /* Plug-in���A�o�[�W�����y�� copyright */
#ifdef UNDPM32_ALLOWEXEFILE
  "*.dpm;*.exe;*.scr",              /* ��\�I�Ȋg���q ("*.JPG" "*.RGB;*.Q0" �Ȃ�) */
#else
  "*.dpm",                          /* ��\�I�Ȋg���q ("*.JPG" "*.RGB;*.Q0" �Ȃ�) */
#endif
#ifdef HSP33_SUPPORT
  "Hot Soup Processor DPM (3.3+)",  /* �t�@�C���`���� */
#else
  "Hot Soup Processor DPM (2.6+)",  /* �t�@�C���`���� */
#endif
};

// �w�b�_�`�F�b�N���ɕK�v�ȃT�C�Y.2KB�ȓ���
#define HEADBUF_SIZE  sizeof(0x10)

BOOL APIENTRY SpiEntryPoint(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
BOOL IsSupportedEx(char *filename, char *data);
int GetArchiveInfoEx(LPSTR filename, long len, HLOCAL *lphInf);
int GetFileEx(char *filename, HLOCAL *dest, fileInfo *pinfo, SPI_PROGRESS lpPrgressCallback, long lData);

#endif
