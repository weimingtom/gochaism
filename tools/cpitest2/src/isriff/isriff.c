/*--------------------------------------------------------------------------
  isriff plugin                                           written by gocha
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
#define CPI_NAME			"RIFF Check Plug-In"
#define CPI_VERSTR			"1.0.2"

/*--------------------------------------------------------------------------
  グローバル
--------------------------------------------------------------------------*/
char *pSupported[] = { "ani,avi,cda,rmi,rmp,wav" };

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
	// RIFF ヘッダチェック("RIFF")
	if(*(DWORD *)pBuf != 0x46464952) return FALSE;

	// ファイル情報構造体の初期化
	ZeroMemory(cfi, sizeof(CPIFILEINFO));

	// チャンクサイズ(ヘッダサイズ加算)
	cfi->nSize = *(long *)(pBuf + 0x04) + 0x08;
	if(cfi->nSize <= 0x14 || cfi->nSize > nSize) return FALSE;

	// ファイル名バッファ確保
	cfi->szFileName = malloc(_MAX_FNAME);

	// 拡張子判定(優先度の高いものを上にするといい)
	switch(*(DWORD *)(pBuf + 0x08)){
	// wav / Wave サウンド("WAVE")
	case 0x45564157:
		strcpy(cfi->szFileName, ".wav");
		break;
	// avi / ビデオ クリップ("AVI ")
	case 0x20495641:
		strcpy(cfi->szFileName, ".avi");
		break;
	// ani / アニメーションカーソル("ACON")
	case 0x4E4F4341:
		strcpy(cfi->szFileName, ".ani");
		break;
	// rmi / RIFF MIDI("RMID")
	case 0x44494D52:
		strcpy(cfi->szFileName, ".rmi");
		break;
	// rmp / RIFF MP3("RMP3")
	case 0x33504D52:
		strcpy(cfi->szFileName, ".rmp");
		break;
	// cda / CD オーディオ("CDDA")
	case 0x41444443:
		strcpy(cfi->szFileName, ".cda");
		break;
	// 未知のデータ
	default:
		strcpy(cfi->szFileName, ".riff");
	}
	return TRUE;
}
