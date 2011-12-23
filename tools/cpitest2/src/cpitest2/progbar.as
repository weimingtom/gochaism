/*--------- ---------- ---------- ---------- ----------
; http://www.setsuki.com/
;---------- ---------- ---------- ---------- ----------
; �g�p��j

#include "progbar.as"

	; �v���O���X�o�[��ݒu
	objsize 200,24 : progbar 0, 0, 200

		; �v���O���X�o�[�̐F��ύX
		progcolor 0, 0x00FFFF, 0xFF0000

		; �v���O���X�o�[�̒l��ύX
		progprm 0, 20

	; �X���[�Y�E�v���O���X�o�[��ݒu
	objsize 200,24 : progbar PBS_SMOOTH, 0, 200

		progprm 1, 150

	; �c�v���O���X�o�[��ݒu
	objsize 50, 200 : progbar PBS_VERTICAL, 0, 200

		progprm 2, 100

;---------- ---------- ---------- ---------- ---------*/


#define client_pos(%1) \
 : GetCursorPos varptr(rect) \
 : ScreenToClient %1,varptr(rect)

#define global ctype MAKELPARAM(%1,%2)   (%2<<16)+%1

;---------- --------- ---------- ---------- ----------

#define PBS_SMOOTH          0x01   ;�X���[�Y
#define PBS_VERTICAL        0x04   ;�c�\��

#module object
;---------- --------- ---------- ---------- ----------
; �v���O���X�o�[�̐ݒ�
;---------- --------- ---------- ---------- ----------
#define PBM_SETRANGE        0x0401 ;�͈͂̐ݒ�
#define PBM_SETPOS          0x0402
#define PBM_DELTAPOS        0x0403
#define PBM_SETSTEP         0x0404
#define PBM_STEPIT          0x0405
#define PBM_SETRANGE32      0x0406
#define PBM_GETRANGE        0x0407
#define PBM_GETPOS          0x0408
#define PBM_SETBARCOLOR     0x0409 ;�w�i�F
#define PBM_SETBKCOLOR      0x2001 ;�o�[�̐F

;---------- --------- ---------- ---------- ----------
#deffunc progcolor int id, int bg, int fg

	; �w�i�F�ƃo�[�̐F��ݒ�
	if bg >= 0 : sendmsg objinfo(id,2), PBM_SETBKCOLOR,  0, bg
	if fg >= 0 : sendmsg objinfo(id,2), PBM_SETBARCOLOR, 0, fg
	return

;---------- --------- ---------- ---------- ----------
#deffunc progprm int id, int max

	; �o�[�̒l��ύX
	sendmsg objinfo(id,2), PBM_SETPOS, max, 0
	return

;---------- --------- ---------- ---------- ----------
#deffunc progrng int id, int min, int max

	; �͈͂̐ݒ�
	sendmsg objinfo(id,2), PBM_SETRANGE, 0, MAKELPARAM(min, max)
	return id

;---------- --------- ---------- ---------- ----------
#deffunc progbar int style, int min, int max

	; �v���O���X�o�[�̍쐬
	winobj "msctls_progress32","",0,0x50000000|style
	id = stat

	; �͈͂̐ݒ�
	sendmsg objinfo(id,2), PBM_SETRANGE, 0, MAKELPARAM(min, max)
	return id

;---------- --------- ---------- ---------- ----------
#global
