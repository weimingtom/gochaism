/*--------------------------------------------------------------------------
  ismid plugin                                            written by gocha
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
#define CPI_NAME			"MIDI File Check Plug-In"
#define CPI_VERSTR			"1.0.1"

/*--------------------------------------------------------------------------
  グローバル
--------------------------------------------------------------------------*/
char *pSupported[] = { "mid" };

/*--------------------------------------------------------------------------
  エントリ ポイント
--------------------------------------------------------------------------*/
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return TRUE;
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

	WORD mformat, mtrkcnt, mtrkmax;
	int mtrksize;

	// 領域チェック
	if(nSize < 0x1E) return FALSE;

	// シグネチャ("MThd")
	if(*(DWORD *)pBuf != 0x6468544D) return FALSE;

	// 関数のパラメータを退避
	pBufBase = pBuf;
	nSizeBase = nSize;

	// 構造体の初期化
	ZeroMemory(cfi, sizeof(CPIFILEINFO));

	// ヘッダサイズ
	if(ReadBigEndian(pBuf + 0x04, 4) != 6) return FALSE;

	// フォーマット
	mformat = ReadBigEndian(pBuf + 0x08, 2);
	switch(mformat){
	case 0:
	case 1:
	case 2:
		break;
	default:
		return FALSE;
	}

	// トラック数
	mtrkmax = ReadBigEndian(pBuf + 0x0A, 2);
	if(!mtrkmax) return FALSE;

	pBuf += 0x0E; nSize -= 0x0E;
	for(mtrkcnt = 0; mtrkcnt < mtrkmax; mtrkcnt++){
		// コールバック関数の呼び出し
		if(lpfnProgressCallback){
			if(!(*lpfnProgressCallback)())
				return FALSE;
		}
		// サイズチェック
		if(nSize < 0x08) return FALSE;
		// シグネチャ
		if(memcmp(pBuf, "MTrk", 4)) return FALSE;
		// トラックサイズ
		mtrksize = ReadBigEndian(pBuf + 0x04, 4);
		if(mtrksize <= 0 || mtrksize > nSize) return FALSE;
		pBuf += (mtrksize + 0x08); nSize -= (mtrksize + 0x08);
		// エンド オブ トラック(メタイベント)
		if(ReadBigEndian(pBuf - 3, 3) != 0xFF2F00) return FALSE;
	}

	cfi->nSize = (int) (pBuf - pBufBase);
	if(cfi->nSize > nSizeBase) return FALSE;

	cfi->szFileName = malloc(_MAX_FNAME);
	strcpy(cfi->szFileName, ".mid");
	return TRUE;
}
