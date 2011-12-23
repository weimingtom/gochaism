/*--------------------------------------------------------------------------
  isogg plugin                                            written by gocha
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
#define CPI_NAME			"Ogg Check Plug-In"
#define CPI_VERSTR			"1.1"

// データの種類
#define OGG_FILE_UNKNOWN	0x00
#define OGG_FILE_VORBIS		0x01
#define OGG_FILE_VIDEO		0x02
#define OGG_FILE_SPEEX      0x03

/*--------------------------------------------------------------------------
  グローバル
--------------------------------------------------------------------------*/
char *pSupported[] = { "ogg,ogm" };

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
	BYTE *pBufBase;

	int segsize;
	UINT pagecnt;
	UINT pagelev;
	BYTE pagetype;
	UINT ogg_file;
	BYTE segcnt, segmax;

	// 領域チェック
	if(nSize <= 0x1C) return FALSE;

	// シグネチャ("OggS")
	if(*(DWORD *)pBuf != 0x5367674F) return FALSE;

	// 関数のパラメータを退避
	pBufBase = pBuf;

	// 構造体の初期化
	ZeroMemory(cfi, sizeof(CPIFILEINFO));

	pagecnt = 0;
	pagelev = 0;
	ogg_file = OGG_FILE_UNKNOWN;
	do{
		// コールバック関数の呼び出し
		if(lpfnProgressCallback){
			if(!(*lpfnProgressCallback)())
				return FALSE;
		}

		// 領域チェック
		if(nSize <= 0x1C) return FALSE;

		// シグネチャ("OggS")
		if(*(DWORD *)pBuf != 0x5367674F) return FALSE;

		// ページ属性
		pagetype = *(BYTE *)(pBuf + 0x05);
		// 最初のページ
		if(pagecnt == 0 && !(pagetype & 2)) return FALSE;
		// ページのネスト処理
		if(pagetype & 2) pagelev++;
		if(pagetype & 4) pagelev--;

		// セグメント数
		segmax = *(BYTE *)(pBuf + 0x1A);
		if(!segmax) return FALSE;

		pBuf += 0x1B; nSize -= 0x1B;
		if(nSize < segmax) return FALSE;

		// セグメント総サイズ算出
		segsize = 0;
		for(segcnt = 0; segcnt < segmax; segcnt++){
			segsize += *(BYTE *)pBuf++;
		}
		nSize -= segmax;
		if(nSize < segsize) return FALSE;

		// ファイルの種類をチェック
		if(pagecnt == 0){
			if(nSize < 0x09) return FALSE;
			// Ogg Vorbis("vorbis\0\0")
			if(*(DWORD *)(pBuf + 0x01) == 0x62726F76 && *(DWORD *)(pBuf + 0x05) == 0x00007369)
				ogg_file = OGG_FILE_VORBIS;
			// OGM 動画 ("video\0\0\0")
			else if(*(DWORD *)(pBuf + 0x01) == 0x65646976 && *(DWORD *)(pBuf + 0x05) == 0x0000006F)
				ogg_file = OGG_FILE_VIDEO;
			// Speex ("Speex   ")
			else if(*(DWORD *)(pBuf) == 0x65657053 && *(DWORD *)(pBuf + 0x04) == 0x20202078)
				ogg_file = OGG_FILE_SPEEX;
			/*
			// 不明
			else
				return FALSE;
			*/
		}

		// セグメントをスキップ
		pBuf += segsize; nSize -= segsize;

		// 次のページへ
		pagecnt++;
	} while(pagelev > 0);

	cfi->nSize = (int) (pBuf - pBufBase);
	cfi->szFileName = malloc(_MAX_FNAME);
	switch(ogg_file){
	// Ogg Vorbis
	case OGG_FILE_VORBIS:
		strcpy(cfi->szFileName, ".ogg");
		break;
	// OGM 動画
	case OGG_FILE_VIDEO:
		strcpy(cfi->szFileName, ".ogm");
		break;
	// Speex
	case OGG_FILE_SPEEX:
		strcpy(cfi->szFileName, ".spx");
		break;
	// 未知の形式
	default:
		strcpy(cfi->szFileName, ".ogg.unknown");
	}
	return TRUE;
}
