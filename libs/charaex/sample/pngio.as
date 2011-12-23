;---------------------------------------------------------------------
; PNG I/O VIA IMGCTL.DLL
;---------------------------------------------------------------------

#ifndef PNGIO_AS
#define global PNGIO_AS

#module "pngio"

; PNGâÊëúÇÃRGBïîï™ÇæÇØì«Ç›çûÇ›
#deffunc png_load int p1, str filename
	sel = ginfo_sel

	exist filename: filesize = strsize
	if (filesize == -1) {
		return 0
	}

	sdim filebuf, filesize
	bload filename, filebuf

	MtoDIB varptr (filebuf), filesize: hDib = stat
	if (hDib == 0) {
		return 0
	}

	dim BITMAPINFOHEADER, 10
	HeadDIB hDib, varptr (BITMAPINFOHEADER)

	img_width = BITMAPINFOHEADER.1
	img_height = BITMAPINFOHEADER.2
	buffer p1, img_width, img_height, 0
	DIBtoDC hdc, 0, 0, img_width, img_height, hDib, 0, 0, $CC0020: redraw 1

	DeleteDIB hDib

;	gsel sel
return 1

; PNGâÊëúÇÃÉAÉãÉtÉ@ïîï™ÇæÇØì«Ç›çûÇ›
#deffunc png_loada int p1, str filename
	sel = ginfo_sel

	exist filename: filesize = strsize
	if (filesize == -1) {
		return 0
	}

	sdim filebuf, filesize
	bload filename, filebuf

	PNGMAtoDIB varptr (filebuf), filesize: hDib = stat
	if (hDib == 0) {
		return 0
	}

	dim BITMAPINFOHEADER, 10
	HeadDIB hDib, varptr (BITMAPINFOHEADER)

	img_width = BITMAPINFOHEADER.1
	img_height = BITMAPINFOHEADER.2
	buffer p1, img_width, img_height, 1
	repeat 256
		p = 255 - cnt
		palette p, p, p, p, (p == 0)
	loop
	DIBtoDC hdc, 0, 0, img_width, img_height, hDib, 0, 0, $CC0020: redraw 1

	DeleteDIB hDib

;	gsel sel
return 1

; PNGâÊëúÇï€ë∂
#deffunc png_save int p1, str filename
	sel = ginfo_sel

	gsel p1
	img_width = ginfo_sx
	img_height = ginfo_sy
	DCtoDIB hdc, 0, 0, img_width, img_height: hDib = stat

	dim pngOpt, 9
	DIBtoPNGex filename, hDib, varptr(pngOpt)

	gsel sel
return 1

#global

#endif
