/**
 * Susie plug-in: OPTWORKS PMP Image
 * written by gocha, feel free to redistribute
 * 
 * based on spi00in_ex.c by Shimitei
 * http://www.asahi-net.or.jp/~kh4s-smz/spi/make_spi.html
 */

#include <windows.h>
#include <stdlib.h>
#include "ifpmp.h"

/**
 * エントリポイント
 */
BOOL APIENTRY SpiEntryPoint(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    break;

  case DLL_THREAD_ATTACH:
    break;

  case DLL_THREAD_DETACH:
    break;

  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

/**
 * ファイル先頭2KB以内から対応フォーマットか判断
 * (判断に使用するサイズはヘッダファイルで定義)
 * ファイル名も判断材料として渡されているみたい
 */
BOOL IsSupportedEx(char *filename, char *data)
{
  PMPFILEHEADER* pmpHead = (PMPFILEHEADER*) data;
  DWORD minFlgMapLen;

  /* お約束のシグネチャチェック、なのっ */
  if(pmpHead->pfType != 0x4d50) /* "PM" */
  {
    return FALSE;
  }

  /* フラグマップのサイズチェック、なのっ */
  minFlgMapLen = pmpHead->pfWidth * pmpHead->pfHeight / 8;
  if((minFlgMapLen == 0)
      || (pmpHead->pfFlgMapBLen < minFlgMapLen)
      || (pmpHead->pfFlgMapGLen < minFlgMapLen)
      || (pmpHead->pfFlgMapRLen < minFlgMapLen)
  )
  {
    return FALSE;
  }

  return TRUE;
}

/*
 * ファイルイメージから画像の情報を取得
 * (バッファサイズはヘッダファイルで定義)
 */
int GetPictureInfoEx(char *data, struct PictureInfo *lpInfo)
{
  PMPFILEHEADER* pmpHead = (PMPFILEHEADER*) data;

  lpInfo->left          = 0;
  lpInfo->top           = 0;
  lpInfo->width         = pmpHead->pfWidth;
  lpInfo->height        = pmpHead->pfHeight;
  lpInfo->x_density     = 0;
  lpInfo->y_density     = 0;
  lpInfo->colorDepth    = 24;
  lpInfo->hInfo         = NULL;

  return SPI_ALL_RIGHT;
}

/*
 * 画像ファイルをDIBに変換
 * dataはファイルイメージで,サイズはdatasizeバイト.
 */
int GetPictureEx(long datasize, HANDLE *pHBInfo, HANDLE *pHBm,
			 SPI_PROGRESS lpPrgressCallback, long lData, char *data)
{
  int result = SPI_ALL_RIGHT;

  *pHBInfo = LocalAlloc(LMEM_MOVEABLE, sizeof(BITMAPINFOHEADER));
  if(*pHBInfo)
  {
    PMPFILEHEADER* pmpHead = (PMPFILEHEADER*) data;
    DWORD width = pmpHead->pfWidth;
    DWORD height = pmpHead->pfHeight;
    DWORD imageSize = width * height * 3;

    *pHBm = LocalAlloc(LMEM_MOVEABLE, imageSize);
    if(*pHBm)
    {
      BITMAPINFO* bmpinfo = (BITMAPINFO*) LocalLock(*pHBInfo);
      BYTE* bmp = (BYTE*) LocalLock(*pHBm);

      if(bmpinfo && bmp)
      {
        BITMAPINFOHEADER* bmih = (BITMAPINFOHEADER*) bmpinfo;
        BYTE* pmp = (BYTE*) data;
        DWORD pxFlgOfs;
        DWORD sizeToScan = width * height / 16;
        DWORD bFlgMapOfs = sizeof(PMPFILEHEADER);
        DWORD bValOfs = bFlgMapOfs + pmpHead->pfFlgMapBLen;
        DWORD gFlgMapOfs = bValOfs + pmpHead->pfValMapBLen;
        DWORD gValOfs = gFlgMapOfs + pmpHead->pfFlgMapGLen;
        DWORD rFlgMapOfs = gValOfs + pmpHead->pfValMapGLen;
        DWORD rValOfs = rFlgMapOfs + pmpHead->pfFlgMapRLen;
        DWORD destOfs;
        BYTE bVal , gVal , rVal;
        BYTE bVal1, gVal1, rVal1;
        BYTE bVal2, gVal2, rVal2;

        /* BITMAPINFOに情報を格納 */
        bmih->biSize            = sizeof(BITMAPINFOHEADER);
        bmih->biWidth           = width;
        bmih->biHeight          = height;
        bmih->biPlanes          = 1;
        bmih->biBitCount        = 24;
        bmih->biCompression     = BI_RGB;
        bmih->biSizeImage       = imageSize;
        bmih->biXPelsPerMeter   = 0;
        bmih->biYPelsPerMeter   = 0;
        bmih->biClrUsed         = 0;
        bmih->biClrImportant    = 0;

        bVal = pmp[bValOfs];
        gVal = pmp[gValOfs];
        rVal = pmp[rValOfs];
        /* 画像を展開: 上位ビットのスキャン */
        destOfs = 0;
        bVal1 = (bVal & 0xf0);
        gVal1 = (gVal & 0xf0);
        rVal1 = (rVal & 0xf0);
        bVal2 = (bVal << 4);
        gVal2 = (gVal << 4);
        rVal2 = (rVal << 4);
        for(pxFlgOfs = 0; pxFlgOfs < sizeToScan; pxFlgOfs++)
        {
          BYTE bFlg = pmp[bFlgMapOfs + pxFlgOfs];
          BYTE gFlg = pmp[gFlgMapOfs + pxFlgOfs];
          BYTE rFlg = pmp[rFlgMapOfs + pxFlgOfs];
          BYTE bitMask = 0x80;

          do
          {
            /* フラグが立っていれば輝度を更新 */
            if(bFlg & bitMask){ bVal = pmp[bValOfs++]; bVal1 = (bVal & 0xf0); bVal2 = (bVal << 4); }
            if(gFlg & bitMask){ gVal = pmp[gValOfs++]; gVal1 = (gVal & 0xf0); gVal2 = (gVal << 4); }
            if(rFlg & bitMask){ rVal = pmp[rValOfs++]; rVal1 = (rVal & 0xf0); rVal2 = (rVal << 4); }

            /* ビットマップデータに転送 */
            bmp[destOfs    ] = bVal1;
            bmp[destOfs + 1] = gVal1;
            bmp[destOfs + 2] = rVal1;
            bmp[destOfs + 3] = bVal2;
            bmp[destOfs + 4] = gVal2;
            bmp[destOfs + 5] = rVal2;
            destOfs += 6;

            bitMask = (bitMask >> 1);
          } while(bitMask);
        }
        /* 画像を展開: 下位ビットのスキャン */
        destOfs = 0;
        bVal1 = (bVal >> 4);
        gVal1 = (gVal >> 4);
        rVal1 = (rVal >> 4);
        bVal2 = (bVal & 0x0f);
        gVal2 = (gVal & 0x0f);
        rVal2 = (rVal & 0x0f);
        for(; pxFlgOfs < (sizeToScan * 2); pxFlgOfs++)
        {
          BYTE bFlg = pmp[bFlgMapOfs + pxFlgOfs];
          BYTE gFlg = pmp[gFlgMapOfs + pxFlgOfs];
          BYTE rFlg = pmp[rFlgMapOfs + pxFlgOfs];
          BYTE bitMask = 0x80;

          do
          {
            /* フラグが立っていれば輝度を更新 */
            if(bFlg & bitMask){ bVal = pmp[bValOfs++]; bVal1 = (bVal >> 4); bVal2 = (bVal & 0x0f); }
            if(gFlg & bitMask){ gVal = pmp[gValOfs++]; gVal1 = (gVal >> 4); gVal2 = (gVal & 0x0f); }
            if(rFlg & bitMask){ rVal = pmp[rValOfs++]; rVal1 = (rVal >> 4); rVal2 = (rVal & 0x0f); }

            /* ビットマップデータに転送 */
            bmp[destOfs    ] |= bVal1;
            bmp[destOfs + 1] |= gVal1;
            bmp[destOfs + 2] |= rVal1;
            bmp[destOfs + 3] |= bVal2;
            bmp[destOfs + 4] |= gVal2;
            bmp[destOfs + 5] |= rVal2;
            destOfs += 6;

            bitMask = (bitMask >> 1);
          } while(bitMask);
        }
      }
      else
      {
        LocalFree(*pHBm);
        *pHBm = NULL;
        LocalFree(*pHBInfo);
        *pHBInfo = NULL;
        result = SPI_MEMORY_ERROR;
      }

      if(*pHBm) LocalUnlock(*pHBm);
    }
    else
    {
      LocalFree(*pHBInfo);
      *pHBInfo = NULL;
      result = SPI_NO_MEMORY;
    }
    if(*pHBInfo) LocalUnlock(*pHBInfo);
  }
  else
  {
    result = SPI_NO_MEMORY;
  }
  return result;
}

/*-------------------------------------------------------------------------*/
