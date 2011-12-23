/*--------- ---------- ---------- ---------- ----------
; http://www.setsuki.com/
;---------- ---------- ---------- ---------- ----------
; 使用例）

#include "progbar.as"

	; プログレスバーを設置
	objsize 200,24 : progbar 0, 0, 200

		; プログレスバーの色を変更
		progcolor 0, 0x00FFFF, 0xFF0000

		; プログレスバーの値を変更
		progprm 0, 20

	; スムーズ・プログレスバーを設置
	objsize 200,24 : progbar PBS_SMOOTH, 0, 200

		progprm 1, 150

	; 縦プログレスバーを設置
	objsize 50, 200 : progbar PBS_VERTICAL, 0, 200

		progprm 2, 100

;---------- ---------- ---------- ---------- ---------*/


#define client_pos(%1) \
 : GetCursorPos varptr(rect) \
 : ScreenToClient %1,varptr(rect)

#define global ctype MAKELPARAM(%1,%2)   (%2<<16)+%1

;---------- --------- ---------- ---------- ----------

#define PBS_SMOOTH          0x01   ;スムーズ
#define PBS_VERTICAL        0x04   ;縦表示

#module object
;---------- --------- ---------- ---------- ----------
; プログレスバーの設定
;---------- --------- ---------- ---------- ----------
#define PBM_SETRANGE        0x0401 ;範囲の設定
#define PBM_SETPOS          0x0402
#define PBM_DELTAPOS        0x0403
#define PBM_SETSTEP         0x0404
#define PBM_STEPIT          0x0405
#define PBM_SETRANGE32      0x0406
#define PBM_GETRANGE        0x0407
#define PBM_GETPOS          0x0408
#define PBM_SETBARCOLOR     0x0409 ;背景色
#define PBM_SETBKCOLOR      0x2001 ;バーの色

;---------- --------- ---------- ---------- ----------
#deffunc progcolor int id, int bg, int fg

	; 背景色とバーの色を設定
	if bg >= 0 : sendmsg objinfo(id,2), PBM_SETBKCOLOR,  0, bg
	if fg >= 0 : sendmsg objinfo(id,2), PBM_SETBARCOLOR, 0, fg
	return

;---------- --------- ---------- ---------- ----------
#deffunc progprm int id, int max

	; バーの値を変更
	sendmsg objinfo(id,2), PBM_SETPOS, max, 0
	return

;---------- --------- ---------- ---------- ----------
#deffunc progrng int id, int min, int max

	; 範囲の設定
	sendmsg objinfo(id,2), PBM_SETRANGE, 0, MAKELPARAM(min, max)
	return id

;---------- --------- ---------- ---------- ----------
#deffunc progbar int style, int min, int max

	; プログレスバーの作成
	winobj "msctls_progress32","",0,0x50000000|style
	id = stat

	; 範囲の設定
	sendmsg objinfo(id,2), PBM_SETRANGE, 0, MAKELPARAM(min, max)
	return id

;---------- --------- ---------- ---------- ----------
#global
