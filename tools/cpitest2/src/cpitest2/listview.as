
/*--------- ---------- ---------- ---------- ----------
; http://www.setsuki.com/
;---------- ---------- ---------- ---------- ----------
; �g�p��j

#include "listview.as"

	; ���X�g�r���[��ݒ�
	pos 0,24 : listview 640, 456, 1

	; �J�����̐ݒ�
	listaddc "����"    , 0, 100
	listaddc "�Ђ炪��", 1, 100
	listaddc "English" , 2, 300

	; �Ō�ɕ����������
	listadd "�ь�"  ,   -1, 0
	listadd "���", stat, 1
	listadd "apple" , stat, 2

	; �Ō�ɕ����������
	listadd "����"  ,   -1, 0
	listadd "�݂���", stat, 1
	listadd "orange", stat, 2

	oncmd gosub *OnNotify, WM_NOTIFY
	stop

*OnNotify

	dupptr NMHDR, lparam, 12

	if NMHDR = hlistview {
		; ���N���b�N �̎擾
		if( NMHDR.2 == NM_CLICK ){

			; �I�𒆂̃��X�g�{�b�N�X�̒l���擾
			data="" : listget data, listnow, 0

			; �^�C�g���ɕ\��
			title ""+listnow+" / "+listmax+"( "+data+" ��I�� )"
		}
		; �E�N���b�N �̎擾
		if( NMHDR.2 == NM_RCLICK ){

			; ���X�g�r���[�̃A�C�e����S�č폜
			listdel -1
		}
	}
	return

;---------- ---------- ---------- ---------- ----------

;hlistview ; �I�𒆂̃��X�g�r���[�̃n���h��
;listmax   ; �I�𒆂̃��X�g�r���[�̍ő�s��
;listnow   ; �I�𒆂̃��X�g�r���[�̑I���s

;listview  width, height, style, menuID ; ���X�g�r���[���쐬
;liststyle style                        ; ���X�g�r���[�̃X�^�C���ύX
;listsel   objID                        ; ���X�g�r���[��I������
;listaddc  string, subitem, width       ; ���X�g�r���[�ɃJ������ǉ�
;listadd   string, item, width, subitem ; ���X�g�r���[�ɃA�C�e���ǉ�
;listget   var, item, subitem           ; ���X�g�r���[�̃A�C�e����������擾
;listdel   item                         ; ���X�g�r���[�̃A�C�e�����폜

;---------- ---------- ---------- ---------- ---------*/

#ifndef WM_NOTIFY
#define global WM_NOTIFY     $004E
#endif

#ifndef NM_CLICK
#define global NM_CLICK       -2
#define global NM_DBLCLK      -3
#define global NM_KILLFOCUS   -8
#define global NM_OUTOFMEMORY -1
#define global NM_RCLICK      -5
#endif

#ifndef LVM_INSERTITEM
#define global LVM_GETITEMCOUNT             $00001004
#define global LVM_GETITEM                  $00001005
#define global LVM_SETITEM                  $00001006
#define global LVM_INSERTITEM               $00001007
#define global LVM_DELETEITEM               $00001008
#define global LVM_DELETEALLITEMS           $00001009
#define global LVM_INSERTCOLUMN             $0000101B
#define global LVM_SETEXTENDEDLISTVIEWSTYLE $00001036
#define global LVM_GETEXTENDEDLISTVIEWSTYLE $00001037

#define global LVS_ICON                     $00000000 ;�u�傫���A�C�R���\���v
#define global LVS_REPORT                   $00000001 ;�u�ڍו\���v
#define global LVS_SMALLICON                $00000002 ;�u�������A�C�R���\���v
#define global LVS_LIST                     $00000003 ;�u�ꗗ�\���v
#define global LVS_SINGLESEL                $00000004 ;�P�̂ݑI����
#define global LVS_SHOWSELALWAYS            $00000008 ;�t�H�[�J�X�������Ă��I����Ԃ�\��
#define global LVS_SORTASCENDING            $00000010 ;�����\�[�g
#define global LVS_SORTDESCENDING           $00000020 ;�~���\�[�g
#define global LVS_SHAREIMAGELISTS          $00000040 ;�R���g���[���폜���ɁA�C���[�W���X�g��j�����Ȃ�
#define global LVS_NOLABELWRAP              $00000080 ;�u�傫���A�C�R���\���v�e�L�X�g���P�s�\��
#define global LVS_AUTOARRANGE              $00000100 ;�u�A�C�R���\���v�A�C�R���̎�������
#define global LVS_EDITLABELS               $00000200 ;�A�C�e���̃e�L�X�g��ҏW��
#define global LVS_OWNERDATA                $00001000 ;���z���X�g�r���[
#define global LVS_NOSCROLL                 $00002000 ;�X�N���[���s��
#define global LVS_ALIGNTOP                 $00000000 ;�u�A�C�R���\���v�A�C�e������[�ɕ��ׂ�
#define global LVS_ALIGNLEFT                $00000800 ;�u�A�C�R���\���v�A�C�e���������ɕ��ׂ�
#define global LVS_OWNERDRAWFIXED           $00000400 ;�u�ڍו\���v�I�[�i�[�h���[
#define global LVS_NOCOLUMNHEADER           $00004000 ;�u�ڍו\���v�J�����̃w�b�_���\��
#define global LVS_NOSORTHEADER             $00008000 ;�u�ڍו\���v�J�����̃w�b�_�̓���Ȃ�

#define global LVS_EX_GRIDLINES             $00000001 ;�u�ڍו\���v�r����\��
#define global LVS_EX_SUBITEMIMAGES         $00000002 ;�u�ڍו\���v�T�u�A�C�e���ɃC���[�W��\����
#define global LVS_EX_CHECKBOXES            $00000004 ;�A�C�e���Ƀ`�F�b�N�{�b�N�X
#define global LVS_EX_TRACKSELECT           $00000008 ;�}�E�X�J�[�\���̈�莞�Ԓ�~�őI��
#define global LVS_EX_HEADERDRAGDROP        $00000010 ;�u�ڍו\���v�h���b�O���h���b�v�ŃJ�����̏��������ւ���
#define global LVS_EX_FULLROWSELECT         $00000020 ;�u�ڍו\���v�A�C�e���I�����ɗ�S�̂������\��
#define global LVS_EX_ONECLICKACTIVATE      $00000040 ;�N���b�N���� LVN_ITEMACTIVATE �ʒm
#define global LVS_EX_TWOCLICKACTIVATE      $00000080 ;�_�u���N���b�N���� LVN_ITEMACTIVATE �ʒm
#define global LVS_EX_FLATSB                $00000100 ;�t���b�g�X�N���[���o�[
#define global LVS_EX_REGIONAL              $00000200 ;�u�傫���A�C�R���\���v���[�W�����g�p 
#define global LVS_EX_INFOTIP               $00000400 ;�u�傫���A�C�R���\���v�c�[���q�b�v�̕\���� LVN_GETINFOTIP �ʒm
#define global LVS_EX_UNDERLINEHOT          $00000800 ;�J�[�\���������牺��������(LVS_EX_ONECLICKACTIVATE | LVS_EX_TWOCLICKACTIVATE )
#define global LVS_EX_UNDERLINECOLD         $00001000 ;����������(LVS_EX_ONECLICKACTIVATE | LVS_EX_TWOCLICKACTIVATE )
#define global LVS_EX_MULTIWORKAREAS        $00002000 ;1�ȏ�̍�Ɨ̈悪��`�����܂Ŏ������񂵂Ȃ� ( LVS_AUTOARRANGE )
#endif

;---------- ---------- ---------- ---------- ----------
; �I�𒆂̃��X�g�r���[�̃n���h�����擾
;---------- ---------- ---------- ---------- ----------
#define global hlistview hlist@m_listview

;---------- ---------- ---------- ---------- ----------
; �I�𒆂̃��X�g�r���[�̃A�C�e�������擾
;---------- ---------- ---------- ---------- ----------
#define global listmax getlistmax()
#define global listnow getlistnow()

;---------- ---------- ---------- ---------- ----------
#module m_listview

; �g���f�t�H���g�X�^�C��
#const default_style ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES )

;---------- ---------- ---------- ---------- ----------
; ���X�g�r���[���쐬
;---------- ---------- ---------- ---------- ----------
#deffunc listview int w, int h, int index, int sub

	; exstyle = WS_EX_CLIENTEDGE
	; style   = ( WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL )
	winobj "SysListView32","", $200, $50000005 | index, w, h, sub
	hlist = objinfo(stat, 2)

	; index �� 1 ����������A�g���f�t�H���g�X�^�C����K��
	if( index & 1 ): sendmsg hlist, $00001036, 0, default_style
	return

;---------- ---------- ---------- ---------- ----------
; ���X�g�r���[�̃X�^�C����ύX
;---------- ---------- ---------- ---------- ----------
#deffunc liststyle int index

	;( LVM_SETEXTENDEDLISTVIEWSTYLE )
	sendmsg hlist, $00001036, 0, index
	return

;---------- ---------- ---------- ---------- ----------
; ���X�g�r���[��I��
;---------- ---------- ---------- ---------- ----------
#deffunc listsel int index

	hlist = objinfo(index, 2)
	return

;---------- ---------- ---------- ---------- ----------
; ���X�g�r���[���A�C�e������Ԃ�
;---------- ---------- ---------- ---------- ----------
#defcfunc getlistmax

	;( LVM_GETITEMCOUNT )
	sendmsg hlist, $00001004, 0,0
	return stat

;---------- ---------- ---------- ---------- ----------
; ���X�g�r���[�̑I���s��Ԃ�
;---------- ---------- ---------- ---------- ----------
#defcfunc getlistnow

    ;( LVM_GETNEXTITEM ) ( $0002 = LVNI_ALL | LVNI_SELECTED )
    sendmsg hlist, $100C, -1, $0002
    return stat

;---------- ---------- ---------- ---------- ----------
; ���X�g�r���[�ɃJ������ǉ�
;---------- ---------- ---------- ---------- ----------
#deffunc listaddc str sdata, int index, int w, int h

	sdim String, strlen(sdata)+1 : String = sdata

	;LVCOLUMN�\���� ( $000F = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM )
	dim prm,7 : prm = $000F, h, w, varptr(String), strlen(String), index
	if( w <= 0 ): prm.2 = 100

	;( LVM_INSERTCOLUMN )
	sendmsg hlist, $0000101B, index, varptr(prm)
	return

;---------- ---------- ---------- ---------- ----------
; ���X�g�r���[�ɃA�C�e���ǉ�
;---------- ---------- ---------- ---------- ----------
#deffunc listadd str sdata, int index, int sub

	sdim String, strlen(sdata)+1 : String = sdata

	;LVITEM�\���� ( $0001 = LVIF_TEXT )
	dim prm,9 : prm = $0001, index, sub, 0,0, varptr(String), strlen(String)
	if( index == -1 ){
		;( LVM_GETITEMCOUNT )
		sendmsg hlist, $00001004, 0,0 : prm.1 = stat
	}

	if( sub > 0 ){
		;( LVM_SETITEM )
		sendmsg hlist, $00001006, 0, varptr(prm)
	} else {
		;( LVM_INSERTITEM )
		sendmsg hlist, $00001007, 0, varptr(prm)
	}
	return prm.1

;---------- ---------- ---------- ---------- ----------
; ���X�g�r���[�̃A�C�e����������擾
;---------- ---------- ---------- ---------- ----------
#deffunc listget var sdata, int index, int sub

	;LVITEM�\���� ( $0001 = LVIF_TEXT )
	dim prm,9 : prm = $0001, index, sub, 0,0, varptr(sdata), 64

	;( LVM_GETITEM )
	sendmsg hlist, $00001005, 0, varptr(prm)
	return

;---------- ---------- ---------- ---------- ----------
; ���X�g�r���[�̃A�C�e�����폜 ( index = -1 : �S���� )
;---------- ---------- ---------- ---------- ----------
#deffunc listdel int index

	if( index == -1 ){
		;( LVM_DELETEALLITEMS )
		sendmsg hlist, $00001009, 0, 0
	} else {
		;( LVM_DELETEITEM )
		sendmsg hlist, $00001008, index, 0
	}
	return

;---------- ---------- ---------- ---------- ----------
#global
