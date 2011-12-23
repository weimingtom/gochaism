
//
//		キャラクターなんとか機 おたすけプラグイン
//		gocha (http://d.hatena.ne.jp/GOCHA/)
//

#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "libpng.lib")
#pragma comment(lib, "imgctl.lib")

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hsp3plugin.h"

#include "png.h"
#include "imgctl.h"

#define INLINE	inline

// テキトーにC#風味な型
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

// 比較用テキトー
#define min2(a, b)                  (((a) < (b)) ? (a) : (b))
#define max2(a, b)                  (((a) > (b)) ? (a) : (b))
#define min3(a, b, c)               (min2(min2((a), (b)), (c)))
#define max3(a, b, c)               (max2(max2((a), (b)), (c)))
#define clip(min, val, max)         (((val) < (min)) ? (min) : (((val) > (max)) ? (max) : (val)))

// DIB処理用
#define DIB_PITCH_OF(width, bpp)    (((width * bpp + 31)>>3)&~3)

/*------------------------------------------------------------*/

INLINE void RGBtoHSV(int r, int g, int b, int *h, int *s, int *v)
{
	int maxdat, mindat, di;
	int hue;

	maxdat = max3(r, g, b);
	mindat = min3(r, g, b);
	*v = maxdat;

	if(maxdat == mindat) {
		*h = 0;
		*s = 0;
	}
	else {
		di = maxdat - mindat;
		*s = (di * 255) / maxdat;

		if(maxdat == r) {
			hue = (60 * (g - b)) / di;
			if (hue < 0) hue += 360;
		}
		else {
			if (maxdat == g) {
				hue = (60 * (b - r)) / di + 120;
			}
			else {
				hue = (60 * (r - g)) / di + 240;
			}
		}
		*h = hue;
	}
}

/*------------------------------------------------------------*/

INLINE void HSVtoRGB(int h, int s, int v, int *r, int *g, int *b)
{
	int takasa, haba, area;
	int maxdat, middat, mindat;

	takasa = v;
	haba   = (s * takasa) / 255;
	maxdat = takasa;
	mindat = takasa - haba;
	middat = (h % 60 * haba) / 60;
	area   = h / 60;

	switch(area)
	{
	case 0:
		*r = maxdat;
		*b = mindat;
		*g = mindat + middat;
		break;

	case 1:
		*g = maxdat;
		*b = mindat;
		*r = maxdat - middat;
		break;

	case 2:
		*r = mindat;
		*g = maxdat;
		*b = mindat + middat;
		break;

	case 3:
		*r = mindat;
		*b = maxdat;
		*g = maxdat - middat;
		break;

	case 4:
		*g = mindat;
		*b = maxdat;
		*r = mindat + middat;
		break;

	case 5:
		*r = maxdat;
		*g = mindat;
		*b = maxdat - middat;
		break;
	}
}

/*------------------------------------------------------------*/

INLINE void RGBtoHSVf(int r, int g, int b, double *h, double *s, double *v)
{
	double rr, gg, bb, maxdat, mindat, di, hue;

	rr = (double) r / 255.0;
	gg = (double) g / 255.0;
	bb = (double) b / 255.0;

	maxdat = max3(rr, gg, bb);
	mindat = min3(rr, gg, bb);

	*v = maxdat;

	if(maxdat == mindat) {
		*h = 0.0;
		*s = 0.0;
	}
	else {
		di = maxdat - mindat;
		*s = di / maxdat;

		if(maxdat == rr) {
			hue = 60.0 * (gg - bb) / di;
			if(hue < 0.0) hue += 360.0;
		}
		else {
			if(maxdat == gg) {
				hue = 60.0 * (bb - rr) / di + 120.0;
			}
			else {
				hue = 60.0 * (rr - gg) / di + 240.0;
			}
		}
		*h = hue;
	}
}

/*------------------------------------------------------------*/

INLINE void HSVtoRGBf(double h, double s, double v, int *r, int *g, int *b)
{
	double takasa, haba, maxdat, middat, mindat;
	int area;

	takasa = v;
	haba   = s * takasa;
	maxdat = takasa;
	mindat = takasa - haba;
	middat = (h - 60.0 * floor(h / 60.0)) * haba / 60;
	area   = (int) (h / 60);

	switch(area)
	{
	case 0:
		*r = (int) ((maxdat * 255) + 0.5);
		*b = (int) ((mindat * 255) + 0.5);
		*g = (int) (((mindat + middat) * 255) + 0.5);
		break;
	case 1:
		*g = (int) ((maxdat * 255) + 0.5);
		*b = (int) ((mindat * 255) + 0.5);
		*r = (int) (((maxdat - middat) * 255) + 0.5);
		break;
	case 2:
		*r = (int) ((mindat * 255) + 0.5);
		*g = (int) ((maxdat * 255) + 0.5);
		*b = (int) (((mindat + middat) * 255) + 0.5);
		break;
	case 3:
		*r = (int) ((mindat * 255) + 0.5);
		*b = (int) ((maxdat * 255) + 0.5);
		*g = (int) (((maxdat - middat) * 255) + 0.5);
		break;
	case 4:
		*g = (int) ((mindat * 255) + 0.5);
		*b = (int) ((maxdat * 255) + 0.5);
		*r = (int) (((mindat + middat) * 255) + 0.5);
		break;
	case 5:
		*r = (int) ((maxdat * 255) + 0.5);
		*g = (int) ((mindat * 255) + 0.5);
		*b = (int) (((maxdat - middat) * 255) + 0.5);
		break;
	}
}

 /*------------------------------------------------------------*/
/*
		controller
*/
/*------------------------------------------------------------*/

static void gcopy8( void )
{
	//		gcopy8 命令の処理
	//
	//		(gcopy8 p1, p2, p3, p4, p5)
	//		gcopyの要領でRGBA同士の合成を行います。
	//		コピー先、コピー元ともに、gmode 7のブレンドのように、
	//		RGB画像の右側にアルファチャンネルがあるものとします。
	//
	//		コピーサイズに応じてアルファの位置が決まる都合から、
	//		実質的に同サイズの画像同士の合成にしか使えません。
	//		どう見ても汎用性皆無です。本当にありがとうございました。
	//
	//		それにしてもセンスのない命令名だね。本家に8出来たらどうするの。
	//
	BMSCR *bm_dst, *bm_src;                             // BMSCR
	int wid;                                            // コピー元ウィンドウID
	int cx_dst, cy_dst;                                 // コピー先座標
	int cx_src, cy_src;                                 // コピー元座標
	int sx_dst, sy_dst;                                 // コピー先ウィンドウサイズ
	int sx_src, sy_src;                                 // コピー元ウィンドウサイズ
	int sx_blt, sy_blt;                                 // 画像転送サイズ
	int sx2_dst, sx2_src;                               // ラインあたりのバイト数
	int sx_to_a;                                        // RGBとアルファの横方向の距離
	int to_next_ln_dst, to_next_ln_src;                 // ライン移動に加算するバイト数
	int opacity;                                        // 半透明合成レート
	bool hasOpacity;                                    // 半透明合成の有無
	byte *pb_dst, *pb_src;                              // RGB処理ポインタ
	byte *pba_dst, *pba_src;                            // アルファ処理ポインタ
	int x, y;
	int a_dst, b_dst, g_dst, r_dst, a_dst2;
	int a_src, b_src, g_src, r_src, a_new;              // ピクセル処理ループ用

	// 引数の取得
	wid = code_getdi(0);                                // ウィンドウID
	bm_dst = (BMSCR *) getbmscr(active_window);         // コピー先のBMSCR
	bm_src = (BMSCR *) getbmscr(wid);                   // コピー元のBMSCR
	cx_dst = bm_dst->cx;                                // コピー先の左上X座標
	cy_dst = bm_dst->cy;                                // コピー先の左上X座標
	cx_src = code_getdi(cx_dst);                        // コピー元の左上X座標
	cy_src = code_getdi(cy_dst);                        // コピー元の左上Y座標
	sx_blt = code_getdi(bm_dst->gx);                    // コピーする大きさX（ドット単位）
	sy_blt = code_getdi(bm_dst->gy);                    // コピーする大きさY（ドット単位）

	opacity = bm_dst->gfrate;                           // 半透明合成レート
	if(opacity == 0) {
		return;
	}
	hasOpacity = (opacity < 256);

	// ピクセル操作のための準備
	sx_to_a = sx_blt;                                   // アルファへの距離
	sx_dst = bm_dst->sx;
	sy_dst = bm_dst->sy;
	sx_src = bm_src->sx;
	sy_src = bm_src->sy;                                // ウィンドウサイズ
	sx2_dst = bm_dst->sx2;
	sx2_src = bm_src->sx2;                              // ラインあたりのバイト数
	to_next_ln_dst = sx2_dst - (sx_blt * 3);
	to_next_ln_src = sx2_src - (sx_blt * 3);            // ライン移動に加算するバイト数
	pb_dst = ((byte *) bm_dst->pBit) + (sy_dst - cy_dst - sy_blt) * sx2_dst + (cx_dst * 3);
	pb_src = ((byte *) bm_src->pBit) + (sy_src - cy_src - sy_blt) * sx2_src + (cx_src * 3);
	pba_dst = pb_dst + (sx_to_a * 3);
	pba_src = pb_src + (sx_to_a * 3);                   // 転送開始位置

	// コピー処理
	for(y = 0; y < sy_blt; y++)
	{
		for(x = 0; x < sx_blt; x++)
		{
			// アルファをドロー！
			a_src = *pba_src;
			a_dst = *pba_dst;

      // 透明合成レートによる調整
      if (hasOpacity) {
        a_src = a_src * opacity / 256;
      }

			if(a_src == 0)
			{
				// 転送しない
				pba_src += 3;
				pba_dst += 3;
				pb_src += 3;
				pb_dst += 3;
			}
			else if(a_src == 255 || a_dst == 0)
			{
				// そのまま転送
				*(pba_dst) = (byte) a_src;
				*(pba_dst + 1) = (byte) a_src;
				*(pba_dst + 2) = (byte) a_src;
				*(pb_dst) = *(pb_src);
				*(pb_dst + 1) = *(pb_src + 1);
				*(pb_dst + 2) = *(pb_src + 2);

				pba_src += 3;
				pba_dst += 3;
				pb_src += 3;
				pb_dst += 3;
			}
			else
			{
				// 合成処理にはRGBを使わざるを得ない！
				b_src = *(pb_src);
				b_dst = *(pb_dst);
				g_src = *(pb_src + 1);
				g_dst = *(pb_dst + 1);
				r_src = *(pb_src + 2);
				r_dst = *(pb_dst + 2);

				// アルファ合成
				a_dst2 = ((255 - a_src) * a_dst + 128) / 255;
				if (a_src != 0) {
					a_new = a_dst2 + a_src;

					// RGB合成
					b_dst = ((b_dst * a_dst2 + b_src * a_src) + (a_new / 2)) / a_new;
					g_dst = ((g_dst * a_dst2 + g_src * a_src) + (a_new / 2)) / a_new;
					r_dst = ((r_dst * a_dst2 + r_src * a_src) + (a_new / 2)) / a_new;

					// ピクセル転送DADADADADA～！
					*(pba_dst) = (byte) a_new;
					*(pba_dst + 1) = (byte) a_new;
					*(pba_dst + 2) = (byte) a_new;
					*(pb_dst) = (byte) b_dst;
					*(pb_dst + 1) = (byte) g_dst;
					*(pb_dst + 2) = (byte) r_dst;
				}

					/*
					*(pba_dst) = (byte) a_src;
					*(pba_dst + 1) = (byte) a_src;
					*(pba_dst + 2) = (byte) a_src;
					*(pb_dst) = *(pb_src);
					*(pb_dst + 1) = *(pb_src + 1);
					*(pb_dst + 2) = *(pb_src + 2);

					pba_src += 3;
					pba_dst += 3;
					pb_src += 3;
					pb_dst += 3;
					*/

				// とっとと引っ越し
				pba_src += 3;
				pba_dst += 3;
				pb_src += 3;
				pb_dst += 3;
			}
		}
		pba_src += to_next_ln_src;
		pba_dst += to_next_ln_dst;
		pb_src  += to_next_ln_src;
		pb_dst  += to_next_ln_dst;
	}

	bms_send(bm_dst, cx_dst, cy_dst, sx_blt, sy_blt);
	bms_send(bm_dst, cx_dst, cy_dst, sx_blt + sx_to_a, sy_blt);
}

/*------------------------------------------------------------*/

static void pngsave_a( void )
{
	//		pngsave_a 命令の処理
	//
	//		(pngsave_a "filename")
	//		bmpsaveの要領でPNGイメージを出力します。
	//		ただし、この命令は透明度を持った画像を前提とします。
	//		RGB画像の右側にアルファチャンネルがあるものとします。
	//		条件を満たさない指定をすると例によって落ちます。
	//
	//		透明度どうでもよければimgctl.dllあたりを使ってください。
	//		ソースコードが汚い？　禁則事項です。
	//
	BMSCR *bm = (BMSCR *) getbmscr(active_window);      // BMSCR
	char *png_name = code_gets();                       // ファイル名
	int width = bm->sx / 2;                             // 画像の幅
	int height = bm->sy;                                // 画像の高さ
	int pitch = DIB_PITCH_OF(width, 32);                // ラインのバイト数
	int to_next_ln_24 = bm->sx2 - (width * 3);          // ライン移動用 (24bpp)
	int to_next_ln_32 = pitch - (width * 4);            // ライン移動用 (32bpp)
	byte *dib32, *rgba;                                 // 画像バッファ (32bpp)
	byte *rgb = (byte *) bm->pBit;                      // RGBチャンネル
	byte *alpha = rgb + (width * 3);                    // アルファチャンネル
	FILE *png_fp;                                       // PNGファイルポインタ
	png_structp png_ptr;
	png_infop info_ptr;                                 // libpng用構造体
    png_bytepp row_pointers;
	int x, y;                                           // ループ用座標汎用

	dib32 = (byte *) malloc(pitch * height);
	if(dib32 == NULL) {
		return;
	}

	rgba = dib32;
	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			*(rgba)     = *(rgb + 2);
			*(rgba + 1) = *(rgb + 1);
			*(rgba + 2) = *(rgb);
			*(rgba + 3) = *(alpha);

			alpha += 3;
			rgb += 3;
			rgba += 4;
		}
		alpha += to_next_ln_24;
		rgb += to_next_ln_24;
		rgba += to_next_ln_32;
	}

	row_pointers = (png_bytepp) malloc(height * sizeof(png_bytep));
	for(y = 0; y < height; y++) {
		row_pointers[height - y - 1] = (png_bytep) dib32 + (y * pitch);
	}

	png_fp = fopen(png_name, "wb");
	if(png_fp == NULL) {
		free(row_pointers);
		free(dib32);
		return;
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL) {
		fclose(png_fp);
		free(row_pointers);
		free(dib32);
		return;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
		fclose(png_fp);
		free(row_pointers);
		free(dib32);
		return;
	}

	if (setjmp(png_ptr->jmpbuf)) {
		png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
		fclose(png_fp);
		free(row_pointers);
		free(dib32);
		return;
	}

	png_init_io(png_ptr, png_fp);

    png_set_IHDR(png_ptr, info_ptr,	
		width, 
		height, 
		8, 
		PNG_COLOR_TYPE_RGB_ALPHA, 
		PNG_INTERLACE_NONE, 
		PNG_COMPRESSION_TYPE_BASE, 
		PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);

	png_write_image(png_ptr, row_pointers);

	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, (png_infopp) NULL);

	fclose(png_fp);
	free(row_pointers);
	free(dib32);
}

/*------------------------------------------------------------*/

static void ToneDIBHSV( void )
{
	//		ToneDIBHSV 命令の処理
	//
	//		(ToneDIBHSV hDib, hMove, sMove, vMove)
	//		imgctl.dllのToneDIB関数のHSV版です。説明割愛。
	//		色空間の変換は可逆ではありません。不必要に変換処理を
	//		繰り返すと画像が劣化するかもしれません。ご注意を。
	//
	HDIB hDIB = (HDIB) code_geti();
	int hMove = code_getdi(0);
	int sMove = code_getdi(0);
	int vMove = code_getdi(0);
	double hMovef, sMovef, vMovef;
	LPCIMGDATA pimg = DataDIB(hDIB);
	LPBITMAPINFOHEADER pbmih = &pimg->pbmi->bmiHeader;
	byte *pdib = (byte *) pimg->pData;
	int width = pbmih->biWidth;
	int height = pbmih->biHeight;
	int bpp = pbmih->biBitCount;
	int lLen = DIB_PITCH_OF(width, bpp);
	int r, g, b;
	double h, s, v;
	int x, y;
	byte *pt;

	hMove %= 360;
	if(hMove < 0) {
		hMove = 360 - hMove;
	}
	sMove = clip(-255, sMove, 255);
	vMove = clip(-255, vMove, 255);

	hMovef = (double) hMove;
	sMovef = (double) sMove / 255;
	vMovef = (double) vMove / 255;

	if(hDIB == NULL) {
		return;
	}

	switch (bpp)
	{
	case 24:
		for(y = 0; y < height; y++) {
			pt = pdib + y * lLen;
			for(x = 0; x < pbmih->biWidth; x++) {
				b = *(pt);
				g = *(pt + 1);
				r = *(pt + 2);

				// RGBtoHSV変換
				RGBtoHSVf(r, g, b, &h, &s, &v);

				// 変調
				h += hMovef;
				if (h > 360.0) h -= 360.0;
				s = clip(0.0, s + sMovef, 1.0);
				v = clip(0.0, v + vMovef, 1.0);

				// HSVtoRGB変換
				HSVtoRGBf(h, s, v, &r, &g, &b);

				// RGB転送
				*(pt)     = (byte) b;
				*(pt + 1) = (byte) g;
				*(pt + 2) = (byte) r;
				pt += 3;
			}
		}
	}
}

static int cmdfunc( int cmd )
{
	//		実行処理 (命令実行時に呼ばれます)
	//
	code_next();							// 次のコードを取得(最初に必ず必要です)

	switch( cmd ) {							// サブコマンドごとの分岐

	case 0x00:								// gcopy8
		gcopy8();
		break;

	case 0x01:								// pngsave_a
		pngsave_a();
		break;

	case 0x02:								// ToneDIBHSV
		ToneDIBHSV();
		break;

	default:
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}
	return RUNMODE_RUN;
}

/*------------------------------------------------------------*/
/*
		interface
*/
/*------------------------------------------------------------*/

int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	//		DLLエントリー (何もする必要はありません)
	//
	return TRUE;
}


EXPORT void WINAPI hsp3cmdinit( HSP3TYPEINFO *info )
{
	//		プラグイン初期化 (実行・終了処理を登録します)
	//
	hsp3sdk_init( info );			// SDKの初期化(最初に行なって下さい)
	info->cmdfunc = cmdfunc;		// 実行関数(cmdfunc)の登録
}

/*----------------------------------------------------------------*/
