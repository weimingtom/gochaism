/*--------------------------------------------------------------------------
  INI File I/O Routines                                Copyright (C) GOCHA
----------------------------------------------------------------------------
□最終更新          2003/10/18
□作成環境          Hot Soup Processor 2.6
--------------------------------------------------------------------------*/

#ifndef _INI_IO_
#define global _INI_IO_

#module "ini_io"

/*--------------------------------------------------------------------------
  初期化処理
--------------------------------------------------------------------------*/
*init
	mref stt, 64
return

/*--------------------------------------------------------------------------
  設定ファイル名を指定
--------------------------------------------------------------------------*/
#deffunc ini_setfile str
	mref p1, 32

	strlen t1, p1
	sdim ini_path, t1 + 1: ini_path = p1
return

/*--------------------------------------------------------------------------
  セクションを指定
--------------------------------------------------------------------------*/
#deffunc ini_section str
	mref p1, 32

	strlen t1, p1
	sdim ini_sect, t1 + 1: ini_sect = p1
return

/*--------------------------------------------------------------------------
  数値を取得
--------------------------------------------------------------------------*/
#deffunc ini_getint str, val
	mref p1, 32
	mref p2, 17

	strlen t1, p1
	sdim s1, t1 + 1: s1 = p1

	getptr prm  , ini_sect
	getptr prm.1, s1
	prm.2 = p2
	getptr prm.3, ini_path
	dllproc "GetPrivateProfileIntA", prm, 4, D_KERNEL
	p2 = dllret@
	sdim s1, 64
return

/*--------------------------------------------------------------------------
  文字列を取得
--------------------------------------------------------------------------*/
#deffunc ini_getstr str, val
	mref p1, 32
	mref p2, 25
	mref v2, 1025

	strlen t1, p1
	sdim s1, t1 + 1: s1 = p1

	getptr prm  , ini_sect
	getptr prm.1, s1
	getptr prm.2, p2
	getptr prm.3, p2
	prm.4 = v2.2 << 2
	getptr prm.5, ini_path
	dllproc "GetPrivateProfileStringA", prm, 6, D_KERNEL
	sdim s1, 64
return

/*--------------------------------------------------------------------------
  文字列を書き込み
--------------------------------------------------------------------------*/
#deffunc ini_setstr str, val
	mref p1, 32
	mref p2, 25

	strlen t1, p1
	sdim s1, t1 + 1: s1 = p1

	getptr prm  , ini_sect
	getptr prm.1, s1
	getptr prm.2, p2
	getptr prm.3, ini_path
	dllproc "WritePrivateProfileStringA", prm, 4, D_KERNEL
	stt = dllret@ == 0
	sdim s1, 64
return

/*--------------------------------------------------------------------------
  数値を書き込み
--------------------------------------------------------------------------*/
#deffunc ini_setint str, int
	mref p1, 32
	mref p2, 1

	sdat = "" + p2
	ini_setstr p1, sdat
return

#global
gosub *init@ini_io
#endif /* _INI_IO_ */
