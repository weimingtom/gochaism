/*--------------------------------------------------------------------------
  isform plugin                                           written by gocha
----------------------------------------------------------------------------
□最終更新      2005/01/09
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
#define CPI_NAME			"FORM Check Plug-In"
#define CPI_VERSTR			"1.0.0"

/*--------------------------------------------------------------------------
  グローバル
--------------------------------------------------------------------------*/
char *pSupported[] = { "aif" };

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
  ビッグエンディアン数値読み込み
--------------------------------------------------------------------------*/
int ReadBigEndian(void *pBuf, int size)
{
	int i, data;
	BYTE *bBuf = (BYTE *)pBuf;

	data = 0;
	for(i = 0; i < size; i++){
		data = (data << 8) | *bBuf++;
	}
	return data;
}

/*--------------------------------------------------------------------------
  ファイルデータ照合
--------------------------------------------------------------------------*/
EXPORT BOOL cpiCheckData(CPIFILEINFO *cfi, BYTE *pBuf, int nSize, FARPROC lpfnProgressCallback)
{
	// RIFF ヘッダチェック("FORM")
	if(*(DWORD *)pBuf != 0x4D524F46) return FALSE;

	// ファイル情報構造体の初期化
	ZeroMemory(cfi, sizeof(CPIFILEINFO));

	// チャンクサイズ(ヘッダサイズ加算)
	cfi->nSize = ReadBigEndian(pBuf + 0x04, 4) + 0x08;
	if(cfi->nSize <= 0x14 || cfi->nSize > nSize) return FALSE;

	// ファイル名バッファ確保
	cfi->szFileName = malloc(_MAX_FNAME);

	// 拡張子判定(優先度の高いものを上にするといい)
	switch(*(DWORD *)(pBuf + 0x08)){
	// aif / AIFF サウンド("AIFF")
	case 0x46464941:
		strcpy(cfi->szFileName, ".aiff");
		break;
	// 未知のデータ
	default:
		strcpy(cfi->szFileName, ".form");
	}
	return TRUE;
}
