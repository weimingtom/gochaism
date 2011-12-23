/*--------------------------------------------------------------------------
  isflac plugin                                           written by gocha
----------------------------------------------------------------------------
□最終更新      2005/08/24
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
#define CPI_NAME			"FLAC Check Plug-In"
#define CPI_VERSTR			"1.0.0"

#define FLAC__METADATA_TYPE_STREAMINFO      0
#define FLAC__METADATA_TYPE_PADDING         1
#define FLAC__METADATA_TYPE_APPLICATION     2
#define FLAC__METADATA_TYPE_SEEKTABLE       3
#define FLAC__METADATA_TYPE_VORBIS_COMMENT  4
#define FLAC__METADATA_TYPE_CUESHEET        5
#define FLAC__METADATA_TYPE_UNDEFINED       6

/*--------------------------------------------------------------------------
  グローバル
--------------------------------------------------------------------------*/
char *pSupported[] = { "flac" };

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
	DWORD DataSize = 4;

	// ヘッダ+STREAMINFOを保障
	// ヘッダチェック("fLaC")
	if(*(DWORD *)(pBuf) != 0x43614C66) return FALSE;

	// ファイル情報構造体の初期化
	ZeroMemory(cfi, sizeof(CPIFILEINFO));
	// ファイル名バッファ確保
	cfi->szFileName = malloc(_MAX_FNAME);
	// FLAC (*.flac)
	strcpy(cfi->szFileName, ".flac");
	// ファイルサイズ
	cfi->nSize = DataSize;
	// データを返す
	return TRUE;
}
