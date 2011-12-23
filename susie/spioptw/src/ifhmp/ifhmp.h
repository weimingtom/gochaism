/**
 * Susie plug-in: OPTWORKS HMP Image
 * written by gocha, feel free to redistribute
 * 
 * based on spi00in_ex.h by Shimitei
 * http://www.asahi-net.or.jp/~kh4s-smz/spi/make_spi.html
 */

#ifndef ifhmp_h
#define ifhmp_h

#include <windows.h>
#include "spi00in.h"

/*-------------------------------------------------------------------------*/
/* ���� Plugin �̏�� */
/*-------------------------------------------------------------------------*/
static const char *pluginfo[] = {
  "00IN",                       /* Plug-in API �o�[�W���� */
  "OPTWORKS HMP",               /* Plug-in��,�o�[�W�����y�� copyright */
  "*.hmp",                      /* ��\�I�Ȋg���q ("*.JPG" "*.JPG;*.JPEG" �Ȃ�) */
  "OPTWORKS HMP Image (*.HMP)", /* �t�@�C���`���� */
};

#pragma pack(push)
#pragma pack(1) /* �\���̂̃����o���E��1�o�C�g�ɂ��� */
typedef struct TAGHMPFILEHEADER
{
  WORD hfType;
  DWORD hfWidth;
  DWORD hfHeight;
  DWORD hfFlgMapLen;
  DWORD hfValMapLen;
} HMPFILEHEADER;
#pragma pack(pop)

/* �w�b�_�`�F�b�N���ɕK�v�ȃT�C�Y.2KB�ȓ���. */
#define HEADBUF_SIZE sizeof(HMPFILEHEADER)

BOOL APIENTRY SpiEntryPoint(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
BOOL IsSupportedEx(char *filename, char *data);
int GetPictureInfoEx(char *data, struct PictureInfo *lpInfo);
int GetPictureEx(long datasize, HANDLE *pHBInfo, HANDLE *pHBm,
    SPI_PROGRESS lpPrgressCallback, long lData, char *data);

#endif
