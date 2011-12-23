; * PNG画像のアルファチャンネルを結合保存 (png+pna)
; 
; サーフェスpng+pna=32ビットPNGなツールです。
; どう見てもプラグインにすべてを投げています。本当に（ｒｙ）。
; 権利主張の類なんて恥ずかしくてできないので自由に利用してください。
; 
; HSP3が出てしばらくたっていますが、丸投げ目的で2.61を使っています。
;
;--------------------------------------------------------------

#packopt name    "pngalcat"
#packopt runtime "hsp2c.hrt"
#packopt hide    1

#define  true    1
#define  false   0

title "pngalcat [2007-10-21]"

;--------------------------------------------------------------

#include "AMpng.as"
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
				gosub *pngalcat
			loop
			dd_info.0 = 0
		}

	goto *nocmdline_main

;--------------------------------------------------------------
; ** コマンドライン引数を処理

*dispatch_arg
	pngpath = arg
	gosub *pngalcat
return

;--------------------------------------------------------------
; ** PNGアルファチャンネル結合保存

*pngalcat
	ampng_rgb_id = 5963
	ampng_pna_id = ampng_rgb_id + 1

	rgb_wid = 3
	alpha_wid = 4
	getpath pngfname, pngpath, 8
	getpath pngpath_noext, pngpath, 1

	AMpngLoad pngpath_noext + ".png", ampng_rgb_id
	if stat {
		dialog pngfname_noext + ".png " + " : AMpngエラー " + stat + " が発生しました", 0, pngpath_noext + ".png"
		return
	}
	AMpngLoad pngpath_noext + ".pna", ampng_pna_id
	if stat {
		dialog pngfname_noext + ".pna " + " : AMpngエラー " + stat + " が発生しました", 0, pngpath_noext + ".pna"
		AMpngDelete ampng_rgb_id
		return
	}

	x = 0 : AMpngGetInfo x, ampng_rgb_id, 0
	y = 0 : AMpngGetInfo y, ampng_rgb_id, 1
	x_verify = 0 : AMpngGetInfo x_verify, ampng_pna_id, 0
	y_verify = 0 : AMpngGetInfo y_verify, ampng_pna_id, 1

	if ((x != x_verify) | (y != y_verify)) {
		dialog pngfname_noext + " : png と pna でサイズが異なります", 0, pngpath_noext
		AMpngDelete ampng_rgb_id
		AMpngDelete ampng_pna_id
		return
	}

	buffer rgb_wid, x, y
	pos 0, 0: AMpngPut ampng_rgb_id: redraw 1

	buffer alpha_wid, x, y
	pos 0, 0: AMpngPut ampng_pna_id: redraw 1

	pos 0, 0: AMpngSetFullAlpha ampng_rgb_id
	AMpngSave pngpath_noext + ".pna.png", ampng_rgb_id, 0

	AMpngDelete ampng_rgb_id
	AMpngDelete ampng_pna_id
return
