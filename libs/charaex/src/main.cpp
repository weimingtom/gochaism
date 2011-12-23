
//
//		�L�����N�^�[�Ȃ�Ƃ��@ ���������v���O�C��
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

// �e�L�g�[��C#�����Ȍ^
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

// ��r�p�e�L�g�[
#define min2(a, b)                  (((a) < (b)) ? (a) : (b))
#define max2(a, b)                  (((a) > (b)) ? (a) : (b))
#define min3(a, b, c)               (min2(min2((a), (b)), (c)))
#define max3(a, b, c)               (max2(max2((a), (b)), (c)))
#define clip(min, val, max)         (((val) < (min)) ? (min) : (((val) > (max)) ? (max) : (val)))

// DIB�����p
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
	//		gcopy8 ���߂̏���
	//
	//		(gcopy8 p1, p2, p3, p4, p5)
	//		gcopy�̗v�̂�RGBA���m�̍������s���܂��B
	//		�R�s�[��A�R�s�[���Ƃ��ɁAgmode 7�̃u�����h�̂悤�ɁA
	//		RGB�摜�̉E���ɃA���t�@�`�����l����������̂Ƃ��܂��B
	//
	//		�R�s�[�T�C�Y�ɉ����ăA���t�@�̈ʒu�����܂�s������A
	//		�����I�ɓ��T�C�Y�̉摜���m�̍����ɂ����g���܂���B
	//		�ǂ����Ă��ėp���F���ł��B�{���ɂ��肪�Ƃ��������܂����B
	//
	//		����ɂ��Ă��Z���X�̂Ȃ����ߖ����ˁB�{�Ƃ�8�o������ǂ�����́B
	//
	BMSCR *bm_dst, *bm_src;                             // BMSCR
	int wid;                                            // �R�s�[���E�B���h�EID
	int cx_dst, cy_dst;                                 // �R�s�[����W
	int cx_src, cy_src;                                 // �R�s�[�����W
	int sx_dst, sy_dst;                                 // �R�s�[��E�B���h�E�T�C�Y
	int sx_src, sy_src;                                 // �R�s�[���E�B���h�E�T�C�Y
	int sx_blt, sy_blt;                                 // �摜�]���T�C�Y
	int sx2_dst, sx2_src;                               // ���C��������̃o�C�g��
	int sx_to_a;                                        // RGB�ƃA���t�@�̉������̋���
	int to_next_ln_dst, to_next_ln_src;                 // ���C���ړ��ɉ��Z����o�C�g��
	int opacity;                                        // �������������[�g
	bool hasOpacity;                                    // �����������̗L��
	byte *pb_dst, *pb_src;                              // RGB�����|�C���^
	byte *pba_dst, *pba_src;                            // �A���t�@�����|�C���^
	int x, y;
	int a_dst, b_dst, g_dst, r_dst, a_dst2;
	int a_src, b_src, g_src, r_src, a_new;              // �s�N�Z���������[�v�p

	// �����̎擾
	wid = code_getdi(0);                                // �E�B���h�EID
	bm_dst = (BMSCR *) getbmscr(active_window);         // �R�s�[���BMSCR
	bm_src = (BMSCR *) getbmscr(wid);                   // �R�s�[����BMSCR
	cx_dst = bm_dst->cx;                                // �R�s�[��̍���X���W
	cy_dst = bm_dst->cy;                                // �R�s�[��̍���X���W
	cx_src = code_getdi(cx_dst);                        // �R�s�[���̍���X���W
	cy_src = code_getdi(cy_dst);                        // �R�s�[���̍���Y���W
	sx_blt = code_getdi(bm_dst->gx);                    // �R�s�[����傫��X�i�h�b�g�P�ʁj
	sy_blt = code_getdi(bm_dst->gy);                    // �R�s�[����傫��Y�i�h�b�g�P�ʁj

	opacity = bm_dst->gfrate;                           // �������������[�g
	if(opacity == 0) {
		return;
	}
	hasOpacity = (opacity < 256);

	// �s�N�Z������̂��߂̏���
	sx_to_a = sx_blt;                                   // �A���t�@�ւ̋���
	sx_dst = bm_dst->sx;
	sy_dst = bm_dst->sy;
	sx_src = bm_src->sx;
	sy_src = bm_src->sy;                                // �E�B���h�E�T�C�Y
	sx2_dst = bm_dst->sx2;
	sx2_src = bm_src->sx2;                              // ���C��������̃o�C�g��
	to_next_ln_dst = sx2_dst - (sx_blt * 3);
	to_next_ln_src = sx2_src - (sx_blt * 3);            // ���C���ړ��ɉ��Z����o�C�g��
	pb_dst = ((byte *) bm_dst->pBit) + (sy_dst - cy_dst - sy_blt) * sx2_dst + (cx_dst * 3);
	pb_src = ((byte *) bm_src->pBit) + (sy_src - cy_src - sy_blt) * sx2_src + (cx_src * 3);
	pba_dst = pb_dst + (sx_to_a * 3);
	pba_src = pb_src + (sx_to_a * 3);                   // �]���J�n�ʒu

	// �R�s�[����
	for(y = 0; y < sy_blt; y++)
	{
		for(x = 0; x < sx_blt; x++)
		{
			// �A���t�@���h���[�I
			a_src = *pba_src;
			a_dst = *pba_dst;

      // �����������[�g�ɂ�钲��
      if (hasOpacity) {
        a_src = a_src * opacity / 256;
      }

			if(a_src == 0)
			{
				// �]�����Ȃ�
				pba_src += 3;
				pba_dst += 3;
				pb_src += 3;
				pb_dst += 3;
			}
			else if(a_src == 255 || a_dst == 0)
			{
				// ���̂܂ܓ]��
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
				// ���������ɂ�RGB���g�킴��𓾂Ȃ��I
				b_src = *(pb_src);
				b_dst = *(pb_dst);
				g_src = *(pb_src + 1);
				g_dst = *(pb_dst + 1);
				r_src = *(pb_src + 2);
				r_dst = *(pb_dst + 2);

				// �A���t�@����
				a_dst2 = ((255 - a_src) * a_dst + 128) / 255;
				if (a_src != 0) {
					a_new = a_dst2 + a_src;

					// RGB����
					b_dst = ((b_dst * a_dst2 + b_src * a_src) + (a_new / 2)) / a_new;
					g_dst = ((g_dst * a_dst2 + g_src * a_src) + (a_new / 2)) / a_new;
					r_dst = ((r_dst * a_dst2 + r_src * a_src) + (a_new / 2)) / a_new;

					// �s�N�Z���]��DADADADADA�`�I
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

				// �Ƃ��Ƃƈ����z��
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
	//		pngsave_a ���߂̏���
	//
	//		(pngsave_a "filename")
	//		bmpsave�̗v�̂�PNG�C���[�W���o�͂��܂��B
	//		�������A���̖��߂͓����x���������摜��O��Ƃ��܂��B
	//		RGB�摜�̉E���ɃA���t�@�`�����l����������̂Ƃ��܂��B
	//		�����𖞂����Ȃ��w�������Ɨ�ɂ���ė����܂��B
	//
	//		�����x�ǂ��ł��悯���imgctl.dll��������g���Ă��������B
	//		�\�[�X�R�[�h�������H�@�֑������ł��B
	//
	BMSCR *bm = (BMSCR *) getbmscr(active_window);      // BMSCR
	char *png_name = code_gets();                       // �t�@�C����
	int width = bm->sx / 2;                             // �摜�̕�
	int height = bm->sy;                                // �摜�̍���
	int pitch = DIB_PITCH_OF(width, 32);                // ���C���̃o�C�g��
	int to_next_ln_24 = bm->sx2 - (width * 3);          // ���C���ړ��p (24bpp)
	int to_next_ln_32 = pitch - (width * 4);            // ���C���ړ��p (32bpp)
	byte *dib32, *rgba;                                 // �摜�o�b�t�@ (32bpp)
	byte *rgb = (byte *) bm->pBit;                      // RGB�`�����l��
	byte *alpha = rgb + (width * 3);                    // �A���t�@�`�����l��
	FILE *png_fp;                                       // PNG�t�@�C���|�C���^
	png_structp png_ptr;
	png_infop info_ptr;                                 // libpng�p�\����
    png_bytepp row_pointers;
	int x, y;                                           // ���[�v�p���W�ėp

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
	//		ToneDIBHSV ���߂̏���
	//
	//		(ToneDIBHSV hDib, hMove, sMove, vMove)
	//		imgctl.dll��ToneDIB�֐���HSV�łł��B���������B
	//		�F��Ԃ̕ϊ��͉t�ł͂���܂���B�s�K�v�ɕϊ�������
	//		�J��Ԃ��Ɖ摜���򉻂��邩������܂���B�����ӂ��B
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

				// RGBtoHSV�ϊ�
				RGBtoHSVf(r, g, b, &h, &s, &v);

				// �ϒ�
				h += hMovef;
				if (h > 360.0) h -= 360.0;
				s = clip(0.0, s + sMovef, 1.0);
				v = clip(0.0, v + vMovef, 1.0);

				// HSVtoRGB�ϊ�
				HSVtoRGBf(h, s, v, &r, &g, &b);

				// RGB�]��
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
	//		���s���� (���ߎ��s���ɌĂ΂�܂�)
	//
	code_next();							// ���̃R�[�h���擾(�ŏ��ɕK���K�v�ł�)

	switch( cmd ) {							// �T�u�R�}���h���Ƃ̕���

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
	//		DLL�G���g���[ (��������K�v�͂���܂���)
	//
	return TRUE;
}


EXPORT void WINAPI hsp3cmdinit( HSP3TYPEINFO *info )
{
	//		�v���O�C�������� (���s�E�I��������o�^���܂�)
	//
	hsp3sdk_init( info );			// SDK�̏�����(�ŏ��ɍs�Ȃ��ĉ�����)
	info->cmdfunc = cmdfunc;		// ���s�֐�(cmdfunc)�̓o�^
}

/*----------------------------------------------------------------*/
