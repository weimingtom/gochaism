/*--------------------------------------------------------------------------
  ispng plugin                                            written by gocha
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
#define CPI_NAME			"PNG Check Plug-In"
#define CPI_VERSTR			"1.1"

// データの種類
#define PNG_FILE_UNKNOWN	0x00
#define PNG_FILE_PNG		0x01
#define PNG_FILE_MNG		0x02

/*--------------------------------------------------------------------------
  グローバル
--------------------------------------------------------------------------*/
char *pSupported[] = { "png,mng" };

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
	BYTE *pBufBase;
	int nSizeBase;

	int i;
	int cnkcnt;
	int cnksize;
	BYTE testchr;
	DWORD cnksign;
	DWORD lastcnk;
	UINT png_file;

	// 領域チェック
	if(nSize < 0x08) return FALSE;

	// シグネチャ
	if(*(DWORD *)(pBuf + 0x04) != 0x0A1A0A0D) return FALSE;
	// ファイルタイプ判別
	if(*(DWORD *)pBuf == 0x474E5089){
		// PNG (Portable Network Graphics)
		png_file = PNG_FILE_PNG;
		// IEND で終了
		lastcnk = 0x444E4549;
	}
	else if(*(DWORD *)pBuf == 0x474E4D8A){
		// MNG (Multiple-image Network Graphics)
		png_file = PNG_FILE_MNG;
		// MEND で終了
		lastcnk = 0x444E454D;
	}
	else{
		return FALSE;
	}

	// 関数のパラメータを退避
	pBufBase = pBuf;
	nSizeBase = nSize;

	// 構造体の初期化
	ZeroMemory(cfi, sizeof(CPIFILEINFO));

	cnkcnt = 0;
	pBuf += 0x08; nSize -= 0x08;
	do{
		// コールバック関数の呼び出し
		if(lpfnProgressCallback){
			if(!(*lpfnProgressCallback)())
				return FALSE;
		}
		// チャンク名判定(アルファベット)
		cnksign = *(DWORD *)(pBuf + 0x04);
		for(i = 0; i < 0x20; i += 0x08){
			testchr = (BYTE)((cnksign >> i) & ~0x20);
			if(testchr < 'A' || testchr > 'Z') return FALSE;
		}
		// チャンクサイズ
		cnksize = ReadBigEndian(pBuf, 4);
		if(nSize < cnksize) return FALSE;
		// チャンクを飛ばす
		pBuf += (cnksize + 0x0C); nSize -= (cnksize + 0x0C);
		// 終端チャンクかどうかを判断
	} while(cnksign != lastcnk);

	cfi->nSize = (int) (pBuf - pBufBase);
	if(cfi->nSize > nSizeBase) return FALSE;

	cfi->szFileName = malloc(_MAX_FNAME);
	switch(png_file){
	case PNG_FILE_PNG:
		strcpy(cfi->szFileName, ".png");
		break;
	case PNG_FILE_MNG:
		strcpy(cfi->szFileName, ".mng");
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
