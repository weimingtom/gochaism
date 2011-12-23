/*--------------------------------------------------------------------------
  MIDI Optimizer Header                                Copyright (C) GOCHA
--------------------------------------------------------------------------*/

#ifndef _MIDIOPT_
#define global _MIDIOPT_

#define global  SOFTNAME    "MIDI Optimizer"
#define global  SOFTVER     "1.0.2 with Cherry"

#module "midiopt"

/*--------------------------------------------------------------------------
  初期化処理
--------------------------------------------------------------------------*/
*init
	mref stt, 64
return

/*--------------------------------------------------------------------------
  n バイト読み込み (Big Endian)
--------------------------------------------------------------------------*/
*npeekb
	p1 = 0
	repeat n
		peek t1, p2, p3 + cnt
		p1 = p1 << 8 | t1
	loop
return

/*--------------------------------------------------------------------------
  n バイト書き込み (Big Endian)
--------------------------------------------------------------------------*/
*npokeb
	repeat n
		poke p1, p2 + cnt, p3 >> (n - cnt - 1 << 3)
	loop
return

/*--------------------------------------------------------------------------
  可変長数値サイズ取得
--------------------------------------------------------------------------*/
*getxlen
	ret = 0
	repeat
		ret++: prm = prm >> 7
		if prm == 0: break
	loop
return

/*--------------------------------------------------------------------------
  4 バイト読み込み (Little Endian)
--------------------------------------------------------------------------*/
#deffunc lpeek val, val, int
	mref p1, 16
	mref p2, 25
	mref p3, 2

	wpeek p1, p2, p3
	wpeek t1, p2, p3 + 2: p1 += t1 << 16
return

/*--------------------------------------------------------------------------
  4 バイト書き込み (Little Endian)
--------------------------------------------------------------------------*/
#deffunc lpoke val, int, int
	mref p1, 24
	mref p2, 1
	mref p3, 2

	wpoke p1, p2, p3 & 0xFFFF
	wpoke p1, p2 + 2, p3 >> 16 & 0xFFFF
return

/*--------------------------------------------------------------------------
  2 バイト読み込み (Big Endian)
--------------------------------------------------------------------------*/
#deffunc wpeekb val, val, int
	mref p1, 16
	mref p2, 25
	mref p3, 2

	n = 2: gosub *npeekb
return

/*--------------------------------------------------------------------------
  4 バイト読み込み (Big Endian)
--------------------------------------------------------------------------*/
#deffunc lpeekb val, val, int
	mref p1, 16
	mref p2, 25
	mref p3, 2

	n = 4: gosub *npeekb
return

/*--------------------------------------------------------------------------
  2 バイト書き込み (Big Endian)
--------------------------------------------------------------------------*/
#deffunc wpokeb val, int, int
	mref p1, 24
	mref p2, 1
	mref p3, 2

	n = 2: gosub *npokeb
return

/*--------------------------------------------------------------------------
  4 バイト書き込み (Big Endian)
--------------------------------------------------------------------------*/
#deffunc lpokeb val, int, int
	mref p1, 24
	mref p2, 1
	mref p3, 2

	n = 4: gosub *npokeb
return

/*--------------------------------------------------------------------------
  可変長数値読み込み (Big Endian)
--------------------------------------------------------------------------*/
#deffunc xpeekb val, val, int
	mref p1, 16
	mref p2, 25
	mref p3, 2

	n = 0
	p1 = 0
	repeat
		peek t1, p2, p3 + n: n++
		p1 = p1 << 7 | (t1 & 0x7F)
		if t1 & 0x80 == 0: break
	loop
	stt = n
return

/*--------------------------------------------------------------------------
  可変長数値書き込み (Big Endian)
--------------------------------------------------------------------------*/
#deffunc xpokeb val, int, int
	mref p1, 24
	mref p2, 1
	mref p3, 2

	prm = p3: gosub *getxlen
	n = ret
	repeat n
		poke p1, p2 + cnt, p3 >> (n - cnt - 1 * 7) & 0x7F | ((cnt + 1 < n) << 7)
	loop
	stt = n
return

/*--------------------------------------------------------------------------
  可変長数値のサイズを取得
--------------------------------------------------------------------------*/
#deffunc xintlen val, int
	mref p1, 16
	mref p2, 1

	prm = p2: gosub *getxlen: p1 = ret
	stt = p1
return

/*--------------------------------------------------------------------------
  メモリから任意バイト削除
--------------------------------------------------------------------------*/
#deffunc memdel val, int, int
	mref p1, 24
	mref v1, 1024
	mref p2, 1
	mref p3, 2

	if p3 < 1: stt = 0: return

	nSize = v1.2 << 2
	dsize = nSize - p2 - p3
	sdim buf, dsize
	memcpy buf, p1, dsize, 0, p2 + p3
	memcpy p1, buf, dsize, p2, 0
	memset p1, 0x00, p3, nSize - p3

	sdim buf, 64
	stt = p3
return

/*--------------------------------------------------------------------------
  メモリに空白を挿入
--------------------------------------------------------------------------*/
#deffunc memins val, int, int
	mref p1, 24
	mref v1, 1024
	mref p2, 1
	mref p3, 2

	if p3 < 1: stt = 0: return

	nSize = v1.2 << 2
	dsize = nSize - p2
	sdim buf, dsize
	memcpy buf, p1, dsize, 0, p2
	memset p1, 0x00, p3, p2
	memcpy p1, buf, dsize - p3, p2 + p3

	sdim buf, 64
	stt = p3
return

/*--------------------------------------------------------------------------
  システムビープ音再生
--------------------------------------------------------------------------*/
#deffunc bell int
	mref p1, 0

	prm = p1 << 4
	dllproc "MessageBeep", prm, 1, D_USER
return

/*--------------------------------------------------------------------------
  システムカラーをパレットに設定
--------------------------------------------------------------------------*/
#deffunc syspal int, int
	mref p1, 0
	mref p2, 1

	prm = p2
	dllproc "GetSysColor", prm, 1, D_USER

	col = dllret@
	palette p1, col & 255, col >> 8 & 255, col >> 16 & 255

	stt = 0
return

/*--------------------------------------------------------------------------
  ウィンドウをアクティブにする
--------------------------------------------------------------------------*/
#deffunc winact int
	mref p1, 0
	mref bm, 96 + p1

	getptr p, timeout: timeout = 0
	prm = 0x2000, 0, p, 0: dllproc "SystemParametersInfoA", prm, 4, D_USER
	prm = 0x2001, 0, 0, 0: dllproc "SystemParametersInfoA", prm, 4, D_USER
	prm = bm.13: dllproc "SetForegroundWindow", prm, 1, D_USER
	prm = 0x2001, 0, p, 0: dllproc "SystemParametersInfoA", prm, 4, D_USER
return

#global
gosub *init@midiopt
#endif /* _MIDIOPT_ */
