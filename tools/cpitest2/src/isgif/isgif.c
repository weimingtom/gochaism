/*--------------------------------------------------------------------------
  isgif plugin                                            written by gocha
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
#define CPI_NAME			"GIF Check Plug-In"
#define CPI_VERSTR			"1.0"

/*--------------------------------------------------------------------------
  グローバル
--------------------------------------------------------------------------*/
char *pSupported[] = { "gif" };

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

	int i;
	BYTE bf;
	int ctsize;
	BYTE blksize;
	BYTE blktype;

	// 領域チェック
	if(nSize < 0x0D) return FALSE;

	// シグネチャ("GIF")
	if((*(DWORD *)pBuf & 0xFFFFFF) != 0x464947) return FALSE;

	// バージョンチェック
	switch(*(DWORD *)(pBuf + 0x03) & 0xFFFFFF){
	// 87a("87a")
	case 0x613738:
	// 89a("89a")
	case 0x613938:
		break;
	default:
		return FALSE;
	}

	// 関数のパラメータを退避
	pBufBase = pBuf;
	nSizeBase = nSize;

	// 構造体の初期化
	ZeroMemory(cfi, sizeof(CPIFILEINFO));

	// 画像サイズチェック
	if(!*(WORD *)(pBuf + 0x06)) return FALSE;// 画像の幅
	if(!*(WORD *)(pBuf + 0x08)) return FALSE;// 画像の高さ

	// ビットフラグ判定
	bf = *(BYTE *)(pBuf + 0x0A);
	pBuf += 0x0D; nSize -= 0x0D;

	// グローバルカラーテーブル
	if(bf & 0x80){
		ctsize = 3;
		for(i = 0; i <= (bf & 7); i++){
			ctsize *= 2;
		}
		if(nSize < ctsize) return FALSE;
		pBuf += ctsize; nSize -= ctsize;
	}

	do{
		// コールバック関数の呼び出し
		if(lpfnProgressCallback){
			if(!(*lpfnProgressCallback)())
				return FALSE;
		}
		// ブロックの種類
		blktype = *(BYTE *)pBuf++; nSize--;
		switch(blktype){
		// イメージブロック
		case 0x2C:
			// ビットフラグ判定
			bf = *(BYTE *)(pBuf + 0x08);
			pBuf += 0x09; nSize -= 0x09;
			// ローカルカラーテーブル
			if(bf & 0x80){
				ctsize = 3;
				for(i = 0; i <= (bf & 7); i++){
					ctsize *= 2;
				}
				if(nSize < ctsize) return FALSE;
				pBuf += ctsize; nSize -= ctsize;
			}
			// LZW コードの最小ビット数
			pBuf++; nSize--;
			break;
		// 拡張ブロック
		case 0x21:
			pBuf++; nSize--;
			break;
		}
		// ブロックのシーク
		if(blktype != 0x3B){
			do{
				// コールバック関数の呼び出し
				if(lpfnProgressCallback){
					if(!(*lpfnProgressCallback)())
						return FALSE;
				}
				// サイズチェック
				if(nSize <= 1) return FALSE;
				// ブロックサイズを読んでシークしていく
				blksize = *(BYTE *)pBuf;
				pBuf += (blksize + 0x01); nSize -= (blksize + 0x01);
				// 0x00 でブロック終了
			} while (blksize != 0x00);
		}
	} while(blktype != 0x3B);

	cfi->nSize = (int) (pBuf - pBufBase);
	if(cfi->nSize > nSizeBase) return FALSE;

	cfi->szFileName = malloc(_MAX_FNAME);
	strcpy(cfi->szFileName, ".gif");
	return TRUE;
}
