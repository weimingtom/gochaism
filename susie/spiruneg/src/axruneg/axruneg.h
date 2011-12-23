/**
 * Susie plug-in: RUNE/CAGE g archive extractor
 * written by gocha, feel free to redistribute
 * 
 * based on spi00am_ex.h by Shimitei
 * http://www.asahi-net.or.jp/~kh4s-smz/spi/make_spi.html
 */

#ifndef axruneg_h
#define axruneg_h

#include <windows.h>
#include "spi00am.h"

/*-------------------------------------------------------------------------*/
// ����Plugin�̏��
/*-------------------------------------------------------------------------*/
static const char *pluginfo[] = {
  "00AM",                           /* Plug-in API�o�[�W���� */
  "RUNE/CAGE g extractor by gocha", /* Plug-in���A�o�[�W�����y�� copyright */
  "*.g",                            /* ��\�I�Ȋg���q ("*.JPG" "*.RGB;*.Q0" �Ȃ�) */
  "RUNE/CAGE g",                    /* �t�@�C���`���� */
};

//���̃v���O�C���p�̍\����
#pragma pack(push)
#pragma pack(1)
typedef struct tagGArcHead
{
  BYTE sig[8];
  DWORD fatOffset;
  DWORD infoRealSize;
  DWORD infoSize;
} GArcHead;
#pragma pack(pop)

// �w�b�_�`�F�b�N���ɕK�v�ȃT�C�Y.2KB�ȓ���
#define HEADBUF_SIZE  sizeof(GArcHead)

BOOL APIENTRY SpiEntryPoint(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
BOOL IsSupportedEx(char *filename, char *data);
int GetArchiveInfoEx(LPSTR filename, long len, HLOCAL *lphInf);
int GetFileEx(char *filename, HLOCAL *dest, fileInfo *pinfo, SPI_PROGRESS lpPrgressCallback, long lData);

#endif
