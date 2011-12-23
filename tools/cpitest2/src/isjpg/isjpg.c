/*--------------------------------------------------------------------------
  isjpg plugin                                            written by gocha
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
#define CPI_NAME			"JPEG Check Plug-In"
#define CPI_VERSTR			"1.2.1"

/*--------------------------------------------------------------------------
  グローバル
--------------------------------------------------------------------------*/
char *pSupported[] = { "jpg" };

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
	/*
	JPEG の構造に少し反したアルゴリズムになってしまってます。
	私が書くと構造に基づいたつもりでも正常に判別できません。

	症状としては、データによっては一部しか切り取られないことです。
	ビューアで見ると小さい画像が出るし、サムネイルでも内包しているんでしょうか？

	今のところ、この書き方での不具合は発見されていません(たぶん)
	でもでも、より確実な書き方ができる方は助言してくださると助かります。
	*/

	int nSizeBase;
	BYTE *pBufBase;

	BYTE marker;
	int segsize;
	UINT segflag;

	// 領域チェック
	if(nSize < 0x0B) return FALSE;

	// SOI マーカ
	if(*(WORD *)pBuf != 0xD8FF) return FALSE;

	// 関数のパラメータを退避
	pBufBase = pBuf;
	nSizeBase = nSize;
	// SOI マーカ分をシーク
	pBuf += 2; nSize -= 2;

	// 構造体の初期化
	ZeroMemory(cfi, sizeof(CPIFILEINFO));

	// SOS のサーチ
	if(nSize < 0x02) return FALSE;
	// 主要セグメントチェック
	segflag = 0x00;
	do{
		// コールバック関数の呼び出し
		if(lpfnProgressCallback){
			if(!(*lpfnProgressCallback)())
				return FALSE;
		}
		// マーカ識別子
		if(*(BYTE *)pBuf != 0xFF) return FALSE;
		// マーカ(セグメント判別)
		marker = *(BYTE *)(pBuf + 0x01);
		pBuf += 2; nSize -= 2;
		// セグメントのスキップ
		switch(marker){
		// SOI マーカ + EOI マーカ(念のため)
		case 0xD8:
		case 0xD9:
			break;
		// SOS マーカ
		case 0xDA:
			// マーカがそろわない(SOS 以降に無いといいのですが)
			if(segflag != 0x03) return FALSE;
			break;
		default:
			// 主要セグメントチェック
			switch(marker){
			// DHT マーカ
			case 0xDB:
				segflag |= 0x01;
				break;
			// DQT マーカ
			case 0xC4:
				segflag |= 0x02;
				break;
			}

			segsize = ReadBigEndian(pBuf, 2);
			if(nSize < segsize) return FALSE;
			pBuf += segsize; nSize -= segsize;
		}
	} while(marker != 0xDA);
	// EOI までシーク(SOS 以降はイメージデータのみ？)
	while(*(WORD *)pBuf != 0xD9FF){
		// コールバック関数の呼び出し
		if(lpfnProgressCallback){
			if(!(*lpfnProgressCallback)())
				return FALSE;
		}
		// サイズチェック
		if(nSize <= 0) return FALSE;
		// 1 バイトずつ
		pBuf++; nSize--;
	}

	cfi->nSize = (int) (pBuf - pBufBase) + 0x02;
	cfi->szFileName = malloc(_MAX_FNAME);
	strcpy(cfi->szFileName, ".jpg");
	return TRUE;
}
