/*--------------------------------------------------------------------------
  GUI Object Routines                                  Copyright (C) GOCHA
----------------------------------------------------------------------------
□最終更新          2003/10/14
□作成環境          Hot Soup Processor 2.6
--------------------------------------------------------------------------*/

#ifndef _OBJRTNS_
#define global _OBJRTNS_

;#include "llmod.as"

#module "objrtns"

#ifndef _OBJEX_
#define OBJKAZZ             64
#else
#const  OBJKAZZ             (64 + OBJEXKAZZ)
#endif ; _OBJEX_

#define WM_GETTEXT          0x000D
#define BM_SETSTYLE         0x00F4
#define BM_CLICK            0x00F5

#define BS_PUSHBUTTON       0x0000
#define BS_DEFPUSHBUTTON    0x0001
#define BS_CHECKBOX         0x0002
#define BS_AUTOCHECKBOX     0x0003
#define BS_RADIOBUTTON      0x0004
#define BS_3STATE           0x0005
#define BS_AUTO3STATE       0x0006
#define BS_GROUPBOX         0x0007
#define BS_AUTORADIOBUTTON  0x0009
#define BS_OWNERDRAW        0x000B

/*--------------------------------------------------------------------------
  初期化処理
--------------------------------------------------------------------------*/
*init
	mref stt, 64
	sdim s1, 256

	nowdef = -1
	defbtn = -1
	_aoid = OBJKAZZ
	dim acktbl, OBJKAZZ
return

/*--------------------------------------------------------------------------
  ボタン自動ジャンプ
--------------------------------------------------------------------------*/
*gobtn
	#ifndef _OBJEX_
	_hspobjhandle prm
	#else
	obj_get prm
	#endif ; _OBJEX_
	prm = stat: prm.1 = BM_CLICK, 0, 0
	dllproc "PostMessageA", prm, 4, D_USER
return

/*--------------------------------------------------------------------------
  ボタンであるか判定
--------------------------------------------------------------------------*/
*isbtn
	if prm < 0: ret = 0: return

	#ifndef _OBJEX_
	_hspobjhandle prm
	#else
	obj_get prm
	#endif ; _OBJEX_

	hwnd = stat
	prm = hwnd: getptr prm.1, s1: prm.2 = 256
	dllproc "GetClassNameA", prm, 3, D_USER

	charupper s1
	if s1 != "BUTTON": ret = 0: return

	prm = hwnd, -16
	setwndlong prm, 1
	if dllret@ & 0xFF > 2: ret = -1: return

	ret = 1
return

/*--------------------------------------------------------------------------
  入力ボックスであるか判定
--------------------------------------------------------------------------*/
*isedit
	if prm < 0: ret = 0: return

	#ifndef _OBJEX_
	_hspobjhandle prm
	#else
	obj_get prm
	#endif ; _OBJEX_

	hwnd = stat
	prm = hwnd: getptr prm.1, s1: prm.2 = 256
	dllproc "GetClassNameA", prm, 3, D_USER

	charupper s1
	if s1 != "EDIT": ret = 0: return

	ret = 1
return

/*--------------------------------------------------------------------------
  オブジェクトの状態取得
--------------------------------------------------------------------------*/
*isenbl
	if prm < 0: ret = 0: return

	#ifndef _OBJEX_
	_hspobjhandle prm
	#else
	obj_get prm
	#endif ; _OBJEX_

	prm = stat
	dllproc "IsWindowEnabled", prm, 1, D_USER
	ret = dllret@ != 0
return

/*--------------------------------------------------------------------------
  オブジェクトのフォーカス設定
--------------------------------------------------------------------------*/
*objselr
	#ifndef _OBJEX_
	objsel prm
	#else
	obj_focus prm
	#endif ; _OBJEX_

	aoid = prm
return

/*--------------------------------------------------------------------------
  内部情報の破棄
--------------------------------------------------------------------------*/
#deffunc clroi
	nowdef = -1
	defbtn = -1
	_aoid = OBJKAZZ
	dim acktbl, OBJKAZZ
return

/*--------------------------------------------------------------------------
  アクセラレータキー設定
--------------------------------------------------------------------------*/
#deffunc setack int, int
	mref p1, 0; oid
	mref p2, 1; key code

	acktbl.p1 = p2
return

/*--------------------------------------------------------------------------
  デフォルトボタン設定
--------------------------------------------------------------------------*/
#deffunc setdefbtn int
	mref p1, 0; oid

	defbtn = p1
	nowdef = defbtn

	prm = -1
	objsend nowdef, BM_SETSTYLE, BS_DEFPUSHBUTTON, prm, 1
	prm = nowdef: gosub *objselr
return

/*--------------------------------------------------------------------------
  オブジェクト処理(ループに入れて使う)
--------------------------------------------------------------------------*/
#deffunc objproclp
	#ifndef _OBJEX_
	objsel -1
	#else
	obj_focus -1
	#endif ; _OBJEX_

	aoid = stat
	if aoid != _aoid{
		_aoid = aoid
		prm = aoid: gosub *isbtn
		if ret != 1{
			if defbtn < 0{
				aoid = nowdef
			}
			else{
				aoid = defbtn
			}
		}
		if aoid == nowdef: return
		prm = -1
		if nowdef >= 0: objsend nowdef, BM_SETSTYLE, BS_PUSHBUTTON, prm, 1
		objsend aoid, BM_SETSTYLE, BS_DEFPUSHBUTTON, prm, 1
		nowdef = aoid
	}
return

/*--------------------------------------------------------------------------
  オブジェクト処理(onkey 先処理)
--------------------------------------------------------------------------*/
#deffunc objprocok int
	mref p1, 0; key code

	if p1 == 0x0D{
		prm = nowdef: gosub *gobtn
	}
	else{
		objsel -1
		prm = stat: gosub *isedit
		if ret: return

		repeat OBJKAZZ
			if acktbl.cnt == p1{
				prm = cnt: gosub *isenbl
				if ret == 0: continue
				prm = cnt: gosub *objselr
				prm = cnt: gosub *isbtn
				if ret != 0: prm = cnt: gosub *gobtn
				break
			}
		loop
	}
return

/*--------------------------------------------------------------------------
  システムカラー選択
--------------------------------------------------------------------------*/
#deffunc syscol int
	mref p1, 0
	mref p2, 1
	mref bm, 67

	prm = p1: dllproc "GetSysColor", prm, 1, D_USER
	r = dllret@ & 255: g = dllret@ >> 8 & 255: b = dllret@ >> 16 & 255

	if bm.3 == 1{
		palette  p2,  r,  g,  b
		palfade 0: palcolor p2
	}
	else{
		color   r,  g,  b
	}
return

#undef OBJKAZZ

#undef WM_GETTEXT
#undef BM_SETSTYLE
#undef BM_CLICK

#undef BS_PUSHBUTTON
#undef BS_DEFPUSHBUTTON
#undef BS_CHECKBOX
#undef BS_AUTOCHECKBOX
#undef BS_RADIOBUTTON
#undef BS_3STATE
#undef BS_AUTO3STATE
#undef BS_GROUPBOX
#undef BS_AUTORADIOBUTTON
#undef BS_OWNERDRAW

#global
gosub *init@objrtns

#endif ; _OBJRTNS_
