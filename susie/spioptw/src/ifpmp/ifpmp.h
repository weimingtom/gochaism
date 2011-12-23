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
/* この Plugin の情報 */
/*-------------------------------------------------------------------------*/
static const char *pluginfo[] = {
  "00IN",                       /* Plug-in API バージョン */
  "OPTWORKS PMP",               /* Plug-in名,バージョン及び copyright */
  "*.pmp",                      /* 代表的な拡張子 ("*.JPG" "*.JPG;*.JPEG" など) */
  "OPTWORKS PMP Image (*.PMP)", /* ファイル形式名 */
};

#pragma pack(push)
#pragma pack(1) /* 構造体のメンバ境界を1バイトにする */
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

/* ヘッダチェック等に必要なサイズ.2KB以内で. */
#define HEADBUF_SIZE sizeof(PMPFILEHEADER)

BOOL APIENTRY SpiEntryPoint(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
BOOL IsSupportedEx(char *filename, char *data);
int GetPictureInfoEx(char *data, struct PictureInfo *lpInfo);
int GetPictureEx(long datasize, HANDLE *pHBInfo, HANDLE *pHBm,
    SPI_PROGRESS lpPrgressCallback, long lData, char *data);

#endif
