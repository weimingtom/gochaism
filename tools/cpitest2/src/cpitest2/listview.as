
/*--------- ---------- ---------- ---------- ----------
; http://www.setsuki.com/
;---------- ---------- ---------- ---------- ----------
; 使用例）

#include "listview.as"

	; リストビューを設定
	pos 0,24 : listview 640, 456, 1

	; カラムの設定
	listaddc "漢字"    , 0, 100
	listaddc "ひらがな", 1, 100
	listaddc "English" , 2, 300

	; 最後に文字列を入れる
	listadd "林檎"  ,   -1, 0
	listadd "りんご", stat, 1
	listadd "apple" , stat, 2

	; 最後に文字列を入れる
	listadd "蜜柑"  ,   -1, 0
	listadd "みかん", stat, 1
	listadd "orange", stat, 2

	oncmd gosub *OnNotify, WM_NOTIFY
	stop

*OnNotify

	dupptr NMHDR, lparam, 12

	if NMHDR = hlistview {
		; 左クリック の取得
		if( NMHDR.2 == NM_CLICK ){

			; 選択中のリストボックスの値を取得
			data="" : listget data, listnow, 0

			; タイトルに表示
			title ""+listnow+" / "+listmax+"( "+data+" を選択中 )"
		}
		; 右クリック の取得
		if( NMHDR.2 == NM_RCLICK ){

			; リストビューのアイテムを全て削除
			listdel -1
		}
	}
	return

;---------- ---------- ---------- ---------- ----------

;hlistview ; 選択中のリストビューのハンドル
;listmax   ; 選択中のリストビューの最大行数
;listnow   ; 選択中のリストビューの選択行

;listview  width, height, style, menuID ; リストビューを作成
;liststyle style                        ; リストビューのスタイル変更
;listsel   objID                        ; リストビューを選択する
;listaddc  string, subitem, width       ; リストビューにカラムを追加
;listadd   string, item, width, subitem ; リストビューにアイテム追加
;listget   var, item, subitem           ; リストビューのアイテム文字列を取得
;listdel   item                         ; リストビューのアイテムを削除

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

#define global LVS_ICON                     $00000000 ;「大きいアイコン表示」
#define global LVS_REPORT                   $00000001 ;「詳細表示」
#define global LVS_SMALLICON                $00000002 ;「小さいアイコン表示」
#define global LVS_LIST                     $00000003 ;「一覧表示」
#define global LVS_SINGLESEL                $00000004 ;１つのみ選択可
#define global LVS_SHOWSELALWAYS            $00000008 ;フォーカスを失っても選択状態を表示
#define global LVS_SORTASCENDING            $00000010 ;昇順ソート
#define global LVS_SORTDESCENDING           $00000020 ;降順ソート
#define global LVS_SHAREIMAGELISTS          $00000040 ;コントロール削除時に、イメージリストを破棄しない
#define global LVS_NOLABELWRAP              $00000080 ;「大きいアイコン表示」テキストを１行表示
#define global LVS_AUTOARRANGE              $00000100 ;「アイコン表示」アイコンの自動整列
#define global LVS_EDITLABELS               $00000200 ;アイテムのテキストを編集可
#define global LVS_OWNERDATA                $00001000 ;仮想リストビュー
#define global LVS_NOSCROLL                 $00002000 ;スクロール不可
#define global LVS_ALIGNTOP                 $00000000 ;「アイコン表示」アイテムを上端に並べる
#define global LVS_ALIGNLEFT                $00000800 ;「アイコン表示」アイテムを左側に並べる
#define global LVS_OWNERDRAWFIXED           $00000400 ;「詳細表示」オーナードロー
#define global LVS_NOCOLUMNHEADER           $00004000 ;「詳細表示」カラムのヘッダを非表示
#define global LVS_NOSORTHEADER             $00008000 ;「詳細表示」カラムのヘッダの動作なし

#define global LVS_EX_GRIDLINES             $00000001 ;「詳細表示」罫線を表示
#define global LVS_EX_SUBITEMIMAGES         $00000002 ;「詳細表示」サブアイテムにイメージを表示可
#define global LVS_EX_CHECKBOXES            $00000004 ;アイテムにチェックボックス
#define global LVS_EX_TRACKSELECT           $00000008 ;マウスカーソルの一定時間停止で選択
#define global LVS_EX_HEADERDRAGDROP        $00000010 ;「詳細表示」ドラッグ＆ドロップでカラムの順序を入れ替える
#define global LVS_EX_FULLROWSELECT         $00000020 ;「詳細表示」アイテム選択時に列全体を強調表示
#define global LVS_EX_ONECLICKACTIVATE      $00000040 ;クリック時に LVN_ITEMACTIVATE 通知
#define global LVS_EX_TWOCLICKACTIVATE      $00000080 ;ダブルクリック時に LVN_ITEMACTIVATE 通知
#define global LVS_EX_FLATSB                $00000100 ;フラットスクロールバー
#define global LVS_EX_REGIONAL              $00000200 ;「大きいアイコン表示」リージョン使用 
#define global LVS_EX_INFOTIP               $00000400 ;「大きいアイコン表示」ツールヒップの表示時 LVN_GETINFOTIP 通知
#define global LVS_EX_UNDERLINEHOT          $00000800 ;カーソルが来たら下線を引く(LVS_EX_ONECLICKACTIVATE | LVS_EX_TWOCLICKACTIVATE )
#define global LVS_EX_UNDERLINECOLD         $00001000 ;下線を引く(LVS_EX_ONECLICKACTIVATE | LVS_EX_TWOCLICKACTIVATE )
#define global LVS_EX_MULTIWORKAREAS        $00002000 ;1つ以上の作業領域が定義されるまで自動整列しない ( LVS_AUTOARRANGE )
#endif

;---------- ---------- ---------- ---------- ----------
; 選択中のリストビューのハンドルを取得
;---------- ---------- ---------- ---------- ----------
#define global hlistview hlist@m_listview

;---------- ---------- ---------- ---------- ----------
; 選択中のリストビューのアイテム数を取得
;---------- ---------- ---------- ---------- ----------
#define global listmax getlistmax()
#define global listnow getlistnow()

;---------- ---------- ---------- ---------- ----------
#module m_listview

; 拡張デフォルトスタイル
#const default_style ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES )

;---------- ---------- ---------- ---------- ----------
; リストビューを作成
;---------- ---------- ---------- ---------- ----------
#deffunc listview int w, int h, int index, int sub

	; exstyle = WS_EX_CLIENTEDGE
	; style   = ( WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL )
	winobj "SysListView32","", $200, $50000005 | index, w, h, sub
	hlist = objinfo(stat, 2)

	; index に 1 があったら、拡張デフォルトスタイルを適応
	if( index & 1 ): sendmsg hlist, $00001036, 0, default_style
	return

;---------- ---------- ---------- ---------- ----------
; リストビューのスタイルを変更
;---------- ---------- ---------- ---------- ----------
#deffunc liststyle int index

	;( LVM_SETEXTENDEDLISTVIEWSTYLE )
	sendmsg hlist, $00001036, 0, index
	return

;---------- ---------- ---------- ---------- ----------
; リストビューを選択
;---------- ---------- ---------- ---------- ----------
#deffunc listsel int index

	hlist = objinfo(index, 2)
	return

;---------- ---------- ---------- ---------- ----------
; リストビューをアイテム数を返す
;---------- ---------- ---------- ---------- ----------
#defcfunc getlistmax

	;( LVM_GETITEMCOUNT )
	sendmsg hlist, $00001004, 0,0
	return stat

;---------- ---------- ---------- ---------- ----------
; リストビューの選択行を返す
;---------- ---------- ---------- ---------- ----------
#defcfunc getlistnow

    ;( LVM_GETNEXTITEM ) ( $0002 = LVNI_ALL | LVNI_SELECTED )
    sendmsg hlist, $100C, -1, $0002
    return stat

;---------- ---------- ---------- ---------- ----------
; リストビューにカラムを追加
;---------- ---------- ---------- ---------- ----------
#deffunc listaddc str sdata, int index, int w, int h

	sdim String, strlen(sdata)+1 : String = sdata

	;LVCOLUMN構造体 ( $000F = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM )
	dim prm,7 : prm = $000F, h, w, varptr(String), strlen(String), index
	if( w <= 0 ): prm.2 = 100

	;( LVM_INSERTCOLUMN )
	sendmsg hlist, $0000101B, index, varptr(prm)
	return

;---------- ---------- ---------- ---------- ----------
; リストビューにアイテム追加
;---------- ---------- ---------- ---------- ----------
#deffunc listadd str sdata, int index, int sub

	sdim String, strlen(sdata)+1 : String = sdata

	;LVITEM構造体 ( $0001 = LVIF_TEXT )
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
; リストビューのアイテム文字列を取得
;---------- ---------- ---------- ---------- ----------
#deffunc listget var sdata, int index, int sub

	;LVITEM構造体 ( $0001 = LVIF_TEXT )
	dim prm,9 : prm = $0001, index, sub, 0,0, varptr(sdata), 64

	;( LVM_GETITEM )
	sendmsg hlist, $00001005, 0, varptr(prm)
	return

;---------- ---------- ---------- ---------- ----------
; リストビューのアイテムを削除 ( index = -1 : 全消し )
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
