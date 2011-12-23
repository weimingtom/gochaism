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
// このPluginの情報
/*-------------------------------------------------------------------------*/
static const char *pluginfo[] = {
  "00AM",                           /* Plug-in APIバージョン */
  "RUNE/CAGE g extractor by gocha", /* Plug-in名、バージョン及び copyright */
  "*.g",                            /* 代表的な拡張子 ("*.JPG" "*.RGB;*.Q0" など) */
  "RUNE/CAGE g",                    /* ファイル形式名 */
};

//このプラグイン用の構造体
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

// ヘッダチェック等に必要なサイズ.2KB以内で
#define HEADBUF_SIZE  sizeof(GArcHead)

BOOL APIENTRY SpiEntryPoint(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
BOOL IsSupportedEx(char *filename, char *data);
int GetArchiveInfoEx(LPSTR filename, long len, HLOCAL *lphInf);
int GetFileEx(char *filename, HLOCAL *dest, fileInfo *pinfo, SPI_PROGRESS lpPrgressCallback, long lData);

#endif
