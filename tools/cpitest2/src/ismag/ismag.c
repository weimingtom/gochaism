/*--------------------------------------------------------------------------
  ismag plugin                                            written by gocha
----------------------------------------------------------------------------
□最終更新      2004/01/23
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
#define CPI_NAME			"MAG Check Plug-In"
#define CPI_VERSTR			"1.0"

/*--------------------------------------------------------------------------
  グローバル
--------------------------------------------------------------------------*/
char *pSupported[] = { "mag" };

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
	int nSizeBase;
	BYTE *pBufBase;

	// 領域チェック
	if(nSize < 0x20) return FALSE;

	// シグネチャ("MAKI02  ")
	if(*(DWORD *)pBuf != 0x494B414D) return FALSE;
	if(*(DWORD *)(pBuf + 0x04) != 0x20203230) return FALSE;

	// 関数のパラメータを退避
	pBufBase = pBuf;
	nSizeBase = nSize;

	// 構造体の初期化
	ZeroMemory(cfi, sizeof(CPIFILEINFO));

	// コメントのシーク
	pBuf += 0x1F; nSize -= 0x1F;
	do{
		// コールバック関数の呼び出し
		if(lpfnProgressCallback){
			if(!(*lpfnProgressCallback)())
				return FALSE;
		}
		// サイズ算出領域を考慮
		if(nSize-- < 0x20) return FALSE;
	} while(*(BYTE *)pBuf++ != 0x1A);

	// ファイルサイズの算出
	cfi->nSize = (int) (pBuf - pBufBase);
	cfi->nSize += *(long *)(pBuf + 0x18);
	cfi->nSize += *(long *)(pBuf + 0x1C);
	if(cfi->nSize <= 0 || cfi->nSize > nSizeBase) return FALSE;

	cfi->szFileName = malloc(_MAX_FNAME);
	strcpy(cfi->szFileName, ".mag");

	return TRUE;
}
