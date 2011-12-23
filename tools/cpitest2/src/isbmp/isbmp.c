/*--------------------------------------------------------------------------
  isbmp plugin                                            written by gocha
----------------------------------------------------------------------------
□最終更新      2004/01/26
□作成環境      Visual C++ 6.0
--------------------------------------------------------------------------*/

#include <windows.h>
#include <stdlib.h>
#include <string.h>

//#include <memory.h>
#include "winmem.h"

#include "cncplug.h"

/*--------------------------------------------------------------------------
  定数宣言
--------------------------------------------------------------------------*/
#define CPI_NAME			"Bitmap Check Plug-In"
#define CPI_VERSTR			"1.0.1"

/*--------------------------------------------------------------------------
  グローバル
--------------------------------------------------------------------------*/
char *pSupported[] = { "bmp" };

/*--------------------------------------------------------------------------
  エントリ ポイント
--------------------------------------------------------------------------*/
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return TRUE;
}

/*--------------------------------------------------------------------------
  プラグインシステム情報取得
--------------------------------------------------------------------------*/
EXPORT void cpiGetPluginSystemInfo(CPISYSTEMINFO *csi)
{
	ZeroMemory(csi, sizeof(CPISYSTEMINFO));
	csi->uSystemVersion = CPI_SYSVER;
	csi->uSystemType = CPI_TYPE_CHECK;
	return;
}

/*--------------------------------------------------------------------------
  プラグインバージョン情報取得
--------------------------------------------------------------------------*/
EXPORT void cpiGetPluginInfo(CPIVERSIONINFO *cvi)
{
	ZeroMemory(cvi, sizeof(CPIVERSIONINFO));
	cvi->szPluginName = malloc(sizeof(CPI_NAME));
	cvi->szPluginVersion = malloc(sizeof(CPI_VERSTR));
	strcpy(cvi->szPluginName, CPI_NAME);
	strcpy(cvi->szPluginVersion, CPI_VERSTR);
	return;
}

/*--------------------------------------------------------------------------
  サポートされているデータの拡張子を取得
--------------------------------------------------------------------------*/
EXPORT LPSTR cpiGetSupportedList(void)
{
	return *pSupported;
}

/*--------------------------------------------------------------------------
  ファイルデータ照合
--------------------------------------------------------------------------*/
EXPORT BOOL cpiCheckData(CPIFILEINFO *cfi, BYTE *pBuf, int nSize, FARPROC lpfnProgressCallback)
{
	DWORD cmptype;

	// 領域チェック
	if(nSize < 0x1E) return FALSE;

	// シグネチャ("BM")
	if(*(WORD *)pBuf != 0x4D42) return FALSE;

	// リザーブチェック
	if(*(WORD *)(pBuf + 0x06)) return FALSE;
	if(*(WORD *)(pBuf + 0x08)) return FALSE;

	// 構造体の初期化
	ZeroMemory(cfi, sizeof(CPIFILEINFO));

	// 先に拡張子を入れておく
	cfi->szFileName = malloc(_MAX_FNAME);
	strcpy(cfi->szFileName, ".bmp");

	// ヘッダサイズ
	switch(*(DWORD *)(pBuf + 0x0E)){
	// BITMAPINFOHEADER(Windows)
	case 0x28:
		// 画像の幅
		if(*(long *)(pBuf + 0x12) <= 0) return FALSE;
		// 画像の高さ
		if(*(long *)(pBuf + 0x16) <= 0) return FALSE;
		// プレーン数
		if(*(WORD *)(pBuf + 0x1A) != 1) return FALSE;

		// ビット深度
		switch(*(WORD *)(pBuf + 0x1C)){
		case 1:
		case 4:
		case 8:
		case 16:
		case 24:
		case 32:
			break;
		default:
			return FALSE;
		}

		// 圧縮形式
		cmptype = *(DWORD *)(pBuf + 0x1E);
		/*
		switch(cmptype){
		// BI_RLE8, RI_RLE4
		case 1:
		case 2:
			strcpy(cfi->szFileName, ".rle");
		}
		*/

		break;
	// BITMAPCOREHEADER(OS/2)
	case 0x0C:
		// 画像の幅
		if(*(long *)(pBuf + 0x12) <= 0) return FALSE;
		// 画像の高さ
		if(*(long *)(pBuf + 0x16) <= 0) return FALSE;
		// プレーン数
		if(*(WORD *)(pBuf + 0x16) != 1) return FALSE;

		// ビット深度
		switch(*(WORD *)(pBuf + 0x18)){
		case 1:
		case 4:
		case 8:
		case 16:
		case 24:
		case 32:
			break;
		default:
			return FALSE;
		}

		break;
	// Unknown Format
	default:
		return FALSE;
	}

	cfi->nSize = *(long *)(pBuf + 0x02);
	if(cfi->nSize <= 0 || cfi->nSize > nSize) return FALSE;
	return TRUE;
}
