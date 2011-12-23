/**
 * Susie plug-in: OPTWORKS PMP Image
 * written by gocha, feel free to redistribute
 * 
 * based on spi00in_ex.h by Shimitei
 * http://www.asahi-net.or.jp/~kh4s-smz/spi/make_spi.html
 */

#ifndef ifpmp_h
#define ifpmp_h

#include <windows.h>
#include "spi00in.h"

/*-------------------------------------------------------------------------*/
/* ���� Plugin �̏�� */
/*-------------------------------------------------------------------------*/
static const char *pluginfo[] = {
  "00IN",                       /* Plug-in API �o�[�W���� */
  "OPTWORKS PMP",               /* Plug-in��,�o�[�W�����y�� copyright */
  "*.pmp",                      /* ��\�I�Ȋg���q ("*.JPG" "*.JPG;*.JPEG" �Ȃ�) */
  "OPTWORKS PMP Image (*.PMP)", /* �t�@�C���`���� */
};

#pragma pack(push)
#pragma pack(1) /* �\���̂̃����o���E��1�o�C�g�ɂ��� */
typedef struct TAGPMPFILEHEADER
{
  WORD pfType;
  DWORD pfWidth;
  DWORD pfHeight;
  DWORD pfFlgMapBLen;
  DWORD pfValMapBLen;
  DWORD pfFlgMapGLen;
  DWORD pfValMapGLen;
  DWORD pfFlgMapRLen;
  DWORD pfValMapRLen;
} PMPFILEHEADER;
#pragma pack(pop)

/* �w�b�_�`�F�b�N���ɕK�v�ȃT�C�Y.2KB�ȓ���. */
#define HEADBUF_SIZE sizeof(PMPFILEHEADER)

BOOL APIENTRY SpiEntryPoint(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
BOOL IsSupportedEx(char *filename, char *data);
int GetPictureInfoEx(char *data, struct PictureInfo *lpInfo);
int GetPictureEx(long datasize, HANDLE *pHBInfo, HANDLE *pHBm,
    SPI_PROGRESS lpPrgressCallback, long lData, char *data);

#endif
