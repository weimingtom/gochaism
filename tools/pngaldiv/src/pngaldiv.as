; * PNG画像のアルファチャンネルを分離保存
; 
; フルカラーPNGのRGBと透明度を分離してBMPに保存します。
; どう見てもプラグインにすべてを投げています。本当に（ｒｙ）。
; 権利主張の類なんて恥ずかしくてできないので自由に利用してください。
; 
; HSP3が出てしばらくたっていますが、丸投げ目的で2.61を使っています。
;
;--------------------------------------------------------------

#packopt name    "pngaldiv"
#packopt runtime "hsp2c.hrt"
#packopt hide    1

#define  true    1
#define  false   0

title "pngaldiv [2007-10-21]"

;--------------------------------------------------------------

#include "AMpng.as"
#include "KazExt.as"
#include "llmod.as"
#include "dragdrop.as" ; xdim用修正済み

;--------------------------------------------------------------
; ** 初期化

*init
	dllproc "GetCommandLineA", p_cmdline, 0, D_KERNEL: p_cmdline = dllret
	dllproc "lstrlenA", p_cmdline, 1, D_KERNEL: cmdline_len = dllret

	sdim arg, cmdline_len + 1
	sdim _cmdline, cmdline_len + 1
	ll_peek _cmdline, p_cmdline

	sdim pngpath, 260
	sdim pngfname, 260
	sdim pngpath_noext, 260

	sdim dd_list, 65536
	dim dd_info, 4

	AMpngInitialize

;--------------------------------------------------------------
; ** コマンドラインを分割処理

*main
	i = 0
	j = 0
	argi = 0
	quoted = false

#ifdef _DEBUG
	arg_skip = 2
#else
	arg_skip = 1
#endif

	repeat: peek c, _cmdline, i: i++
		if (c == '"') {
			quoted = (quoted == false)
			continue
		}

		if (c == 0) | ((quoted == false) & ((c == 9) | (c == ' '))) {
			poke arg, j, 0: j = 0
			if (arg != "") {
				if (argi >= arg_skip): gosub *dispatch_arg
				arg = ""
				argi++
			}
			if (c == 0): break
		}
		else {
			dllproc "IsDBCSLeadByte", c, 1, D_KERNEL
			if (dllret) {
				peek d, _cmdline, i: i++
				poke arg, j, c: j++
				poke arg, j, d: j++
			}
			else {
				poke arg, j, c: j++
			}
		}
	loop

	if (argi <= arg_skip): gosub *nocmdline
	end

;--------------------------------------------------------------
; ** コマンドライン引数がない

*nocmdline
;	dialog "png", 16, "アルファチャンネル付き PNG イメージ"
;	if (stat != 0) {
;		pngpath = refstr
;		gosub *pngaldiv
;	}
;	return

	screen 0, 256, 224
	gsel 0, 1: cls 4

	font "Courier New", 14: color 204, 204, 204
	pos 34, 100: mes "Drop PNG files you like"

	dd_accept dd_list, dd_info, 0

	*nocmdline_main

		wait 5

		if (dd_info.0 > 0) {
			i = 0
			repeat dd_info.0
				getstr pngpath, dd_list, i: i += strsize
				gosub *pngaldiv
			loop
			dd_info.0 = 0
		}

	goto *nocmdline_main

;--------------------------------------------------------------
; ** コマンドライン引数を処理

*dispatch_arg
	pngpath = arg
	gosub *pngaldiv
return

;--------------------------------------------------------------
; ** PNGアルファチャンネル分割保存

*pngaldiv
	ampng_id = 5963
	rgb_wid = 3
	alpha_wid = 4
	getpath pngfname, pngpath, 8
	getpath pngpath_noext, pngpath, 1

	AMpngLoad pngpath, ampng_id
	if stat: dialog pngfname + " : AMpngエラー " + stat + " が発生しました", 0, pngpath: return

	x = 0 : AMpngGetInfo x, ampng_id, 0
	y = 0 : AMpngGetInfo y, ampng_id, 1
	colorType = 0 : AMpngGetInfo colorType, ampng_id, 3

	hasAlpha = (colorType & 4)
	if (hasAlpha == false) {
		dialog pngfname + " : アルファチャンネルを持たない画像です", 0, pngpath
		AMpngDelete ampng_id
		return
	}

	buffer alpha_wid, x, y
	pos 0, 0: AMpngGetFullAlpha ampng_id: redraw 1
	PicSave pngpath_noext + "_alpha.bmp"

	color 255, 255, 255: boxf
	AMpngSetFullAlpha ampng_id

	buffer rgb_wid, x, y
	pos 0, 0: AMpngPut ampng_id: redraw 1
	PicSave pngpath_noext + "_rgb.bmp"

	AMpngDelete ampng_id
return
