/*--------------------------------------------------------------------
	imgctl.dll ヘッダー for HSP3 (2006/10/21)
	by Kpan
	http://lhsp.s206.xrea.com/

	imgctl.dll
	by ルーチェ (ruche)
	http://www.ruche-home.net/
--------------------------------------------------------------------*/

#ifndef IMGCTL3_AS
#define global IMGCTL3_AS

/*--------------------------------------------------------------------
	imgctl.dll Macro
--------------------------------------------------------------------*/
;	Error Codes
#const global ICERR_NONE $000000
#const global ICERR_FILE_OPEN $010001
#const global ICERR_FILE_READ $010002
#const global ICERR_FILE_WRITE $010003
#const global ICERR_FILE_TYPE $010004
#const global ICERR_FILE_NULL $010005
#const global ICERR_FILE_SEEK $010006
#const global ICERR_FILE_SIZE $010007
#const global ICERR_PARAM_NULL $020001
#const global ICERR_PARAM_SIZE $020002
#const global ICERR_PARAM_TYPE $020003
#const global ICERR_PARAM_RANGE $020004
#const global ICERR_MEM_ALLOC $030001
#const global ICERR_MEM_SIZE $030002
#const global ICERR_IMG_COMPRESS $040001
#const global ICERR_IMG_RLESIZE $040002
#const global ICERR_IMG_BITCOUNT $040003
#const global ICERR_IMG_AREA $040004
#const global ICERR_IMG_RLETOP $040005
#const global ICERR_DIB_RLECOMP $050001
#const global ICERR_DIB_RLEEXP $050002
#const global ICERR_DIB_RLEBIT $050003
#const global ICERR_DIB_NULL $050004
#const global ICERR_DIB_UPPER16 $050005
#const global ICERR_DIB_AREAOUT $050006
#const global ICERR_BMP_FILEHEAD $060001
#const global ICERR_BMP_HEADSIZE $060002
#const global ICERR_BMP_IMGSIZE $060003
#const global ICERR_BMP_COMPRESS $060004
#const global ICERR_RLE_TOPDOWN $070001
#const global ICERR_RLE_DATASIZE $070002
#const global ICERR_JPEG_LIBERR $080001
#const global ICERR_PNG_LIBERR $090001
#const global ICERR_PNG_NOALPHA $090002
#const global ICERR_GIF_FILEHEAD $0B0001
#const global ICERR_GIF_BLOCK $0B0002
#const global ICERR_API_STRETCH $0A0001
#const global ICERR_API_SETMODE $0A0002
#const global ICERR_API_SECTION $0A0003
#const global ICERR_API_COMDC $0A0004
#const global ICERR_API_SELOBJ $0A0005
#const global ICERR_API_BITBLT $0A0006

;	Image Types
#const global IMG_BMP $000001
#const global IMG_JPEG $000002
#const global IMG_PNG	 $000003
#const global IMG_GIF $000004
#const global IMG_TIFF $000005
#const global IMG_PIC $000006
#const global IMG_MAG $000007
#const global IMG_PCX $000008
#const global IMG_ERROR $FFFFFF
#const global IMG_UNKNOWN $000000

;	Enough Buffer Size
#const global BUFSIZE_ENOUGH -1

;	DIB Compression
#const global BI_RGB 0
#const global BI_RLE8 1
#const global BI_RLE4 2
#const global BI_BITFIELDS 3

;	Turn Types
#const global TURN_90 90
#const global TURN_180 180
#const global TURN_270 270

;	DIBto16BitEx & DIBto8Bit Types
#const global TOBIT_DEFAULT $000000
#const global TOBIT_ORG $100000
#const global TOBIT_DIFF $000001
#const global TOBIT_DIFFFS $000002
#const global TOBIT_DIFFJJN $000003
#const global TOBIT_DIFFX $000101
#const global TOBIT_DIFFXFS $000102
#const global TOBIT_DIFFXJJN $000103
#const global TOBIT_DIFFDX $000201
#const global TOBIT_DIFFDXFS $000202
#const global TOBIT_DIFFDXJJN $000203

;	DIBto8Bit Flags
#const global TO8_DIV_RGB $000000
#const global TO8_DIV_LIGHT $000001
#const global TO8_SEL_CENTER $000000
#const global TO8_SEL_AVGRGB $000100
#const global TO8_SEL_AVGPIX $000200
#const global TO8_PUT_RGB $000000
#const global TO8_PUT_LIGHT $010000
#const global TO8_PUT_YUV $020000

;	Resize Flags
#const global RESZ_SAME 0
#const global RESZ_RATIO -1

;	PNGOPT Flags
#const global POF_COMPLEVEL $000001
#const global POF_FILTER $000002
#const global POF_GAMMA $000004
#const global POF_TRNSCOLOR $000008
#const global POF_BACKCOLOR $000010
#const global POF_TEXT $000020
#const global POF_TEXTCOMP $000040
#const global POF_INTERLACING $000080
#const global POF_TIME $000100
#const global POF_TRNSPALETTE $020000
#const global POF_TRNSALPHA $040000
#const global POF_BACKPALETTE $010000

;	PNGOPT Filters
#const global PO_FILTER_NONE $000008
#const global PO_FILTER_SUB $000010
#const global PO_FILTER_UP $000020
#const global PO_FILTER_AVG $000040
#const global PO_FILTER_PAETH $000080
#const global PO_FILTER_ALL $0000F8

;	PNGOPT Gamma
#const global PO_GAMMA_NORMAL 45455
#const global PO_GAMMA_WIN PO_GAMMA_NORMAL
#const global PO_GAMMA_MAC 55556

;	GIFOPT Flags
#const global GOF_LOGICAL $000001
#const global GOF_TRNSCOLOR $000008
#const global GOF_BACKCOLOR $000010
#const global GOF_INTERLACING $000080
#const global GOF_BACKPALETTE $010000
#const global GOF_TRNSPALETTE $020000
#const global GOF_LZWCLRCOUNT $080000
#const global GOF_LZWNOTUSE $100000
#const global GOF_BITCOUNT $200000

;	GIFANISCENE Flags
#const global GSF_LOGICAL $00000001
#const global GSF_TRNSCOLOR $00000008
#const global GSF_BITCOUNT $00200000
#const global GSF_LZWCLRCOUNT $00080000
#const global GSF_LZWNOTUSE $00100000
#const global GSF_DISPOSAL $00001000
#const global GSF_INTERLACING $00000080
#const global GSF_TRNSPALETTE $00020000
#const global GSF_USERINPUT $00002000

;	GIFANISCENE Disposal Methods
#const global GS_DISP_NONE 0
#const global GS_DISP_LEAVE 1
#const global GS_DISP_BACK 2
#const global GS_DISP_PREV 3

;	GIFANIOPT Flags
#const global GAF_LOGICAL $00000001
#const global GAF_BACKCOLOR $00000010
#const global GAF_LOOPCOUNT $00000400
#const global GAF_NOTANI $00000800

;	Replace Colors
#const global REP_R 0
#const global REP_RED REP_R
#const global REP_G 1
#const global REP_GREEN REP_G
#const global REP_B 2
#const global REP_BLUE REP_B

;	Raster Operations
#const global SRCCOPY $00CC0020
#const global SRCPAINT $00EE0086
#const global SRCAND $008800C6
#const global SRCINVERT $00660046
#const global SRCERASE $00440328
#const global ROP_NOTSRCCOPY $00330008
#const global ROP_NOTSRCERASE $001100A6
#const global MERGECOPY $00C000CA
#const global MERGEPAINT $00BB0226
#const global PATCOPY $00F00021
#const global PATPAINT $00FB0A09
#const global PATINVERT $005A0049
#const global DSTINVERT $00550009
#const global BLACKNESS $00000042
#const global WHITENESS $00FF0062


/*--------------------------------------------------------------------
	imgctl.dll API
--------------------------------------------------------------------*/
#uselib "imgctl"

;	Standard Functions
#func global ImgctlVersion "ImgctlVersion"
#func global ImgctlBeta "ImgctlBeta"
#func global ImgctlError "ImgctlError"
#func global ImgctlErrorClear "ImgctlErrorClear"
#func global GetImageType "GetImageType" str, int
#func global GetImageMType "GetImageMType" int, int ,int
#func global ToDIB "ToDIB" str
#func global MtoDIB "MtoDIB" int, int

;	DIB Functions
#func global DeleteDIB "DeleteDIB" int
#func global HeadDIB "HeadDIB" int, int
#func global PaletteDIB "PaletteDIB" int, int, int
#func global PixelDIB "PixelDIB" int, int, int
#func global ColorDIB "ColorDIB" int
#func global GetDIB "GetDIB" int, int, int, int, int
#func global MapDIB "MapDIB" int, int, int, int, int
#func global DataDIB "DataDIB" int
#func global CreateDIB "CreateDIB" int, int
#func global CopyDIB "CopyDIB" int
#func global CutDIB "CutDIB" int, int, int ,int, int
#func global TurnDIB "TurnDIB" int, int
#func global DIBto24Bit "DIBto24Bit" int
#func global DIBto16Bit "DIBto16Bit" int, int
#func global DIBto16BitEx "DIBto16BitEx" int, int, int
#func global DIBto8Bit "DIBto8Bit" int, int, int

;	24Bit DIB Functions
#func global PasteDIB "PasteDIB" int, int, int, int, int, int, int, int, int, int
#func global ResizeDIB "ResizeDIB" int, int, int
#func global TurnDIBex "TurnDIBex" int, int, int

;	RLE-DIB Functions
#func global IsRLE "IsRLE" int
#func global DIBtoRLE "DIBtoRLE" int
#func global RLEtoDIB "RLEtoDIB" int

;	Bitmap Functions
#func global DIBtoBMP "DIBtoBMP" str, int
#func global BMPtoDIB "BMPtoDIB" str
#func global BMPMtoDIB "BMPMtoDIB" int, int

;	JPEG Functions
#func global DIBtoJPG "DIBtoJPG" str, int, int, int
#func global JPGtoDIB "JPGtoDIB" str
#func global JPGMtoDIB "JPGMtoDIB" int, int

;	PNG Functions
#func global DIBtoPNG "DIBtoPNG" str, int, int
#func global DIBtoPNGex "DIBtoPNGex" str, int, int
#func global PNGtoDIB "PNGtoDIB" str
#func global PNGMtoDIB "PNGMtoDIB" int, int
#func global PNGAtoDIB "PNGAtoDIB" str
#func global PNGMAtoDIB "PNGMAtoDIB" int, int
#func global InfoPNG "InfoPNG" str, int, int, int
#func global InfoPNGM "InfoPNGM" int, int, int, int, int

;	GIF Functions
#func global DIBtoGIF "DIBtoGIF" str, int, int
#func global DIBtoGIFex "DIBtoGIFex" str, int, int
#func global DIBtoGIFAni "DIBtoGIFAni" str, int, int, int
#func global DIBtoGIFAniEx "DIBtoGIFAniEx" str, int, int, int
#func global GIFtoDIB "GIFtoDIB" str
#func global GIFMtoDIB "GIFMtoDIB" int, int
#func global GIFtoDIBex "GIFtoDIBex" str, int
#func global GIFMtoDIBex "GIFMtoDIBex" int, int, int

;	Filter Functions
#func global GrayDIB "GrayDIB" int, int
#func global ReplaceDIB "ReplaceDIB" int, int, int, int
#func global RepaintDIB "RepaintDIB" int, int, int
#func global TableDIB "TableDIB" int, int

;	Convert Table Functions
#func global ToneDIB "ToneDIB" int, int, int, int
#func global ShadeDIB "ShadeDIB" int, int, int, int
#func global GammaDIB "GammaDIB" int, int, int, int
#func global ContrastDIB "ContrastDIB" int, int, int, int

;	Device Context Functions
#func global DIBtoDC "DIBtoDC" int, int, int, int, int, int, int, int, int
#func global DIBtoDCex "DIBtoDCex" int, int, int, int, int, int, int, int, int, int, int
#func global DIBtoDCex2 "DIBtoDCex2" int, int, int, int, int, int, int, int, int, int, int, int
#func global DCtoDIB "DCtoDIB" int, int, int, int, int

#endif /* not IMGCTL3_AS */
