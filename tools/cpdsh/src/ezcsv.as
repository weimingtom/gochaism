/*
 * 超手抜きCSV解析機もどき
 * http://d.hatena.ne.jp/GOCHA/
 *
 * カンマで区切られた値を読むためのモジュールです。
 * 作りはCheetahmen並にいい加減です。いや、あそこまでじゃないけど。
 * 適当に作られているので適当に使うのが限界です。これはひどい。
 */

#ifndef EZCSV_AS
#define global EZCSV_AS

#module "ezcsv"

#uselib "kernel32.dll"
#cfunc IsDBCSLeadByte "IsDBCSLeadByte" int

; 初期化（ユーザは呼び出ししないこと）
#deffunc ezcs_init
  ezcs_prm_max = 16
  ezcs_prm_size_max = 512

  sdim ezcs_buf, 64
  sdim ezcs_token, ezcs_prm_size_max
  sdim ezcs_ln_buf, ezcs_prm_size_max * ezcs_prm_max
  ezcs_size = -1
  ezcs_ptr = 0

  ezcs_argc@ = 0
  sdim ezcs_argv@, ezcs_prm_size_max, ezcs_prm_max
  ezcs_line_no@ = 0
return

; 指定ファイルを読み込み
#deffunc ezcs_load str filename
  ; ファイルの存在を確認
  exist filename
  if (strsize == -1) : return 0
  ezcs_size = strsize

  ; ファイルを読み込む
  sdim ezcs_buf, ezcs_size + 1
  bload filename, ezcs_buf, ezcs_size
  ; シーク位置を先頭に
  ezcs_line_no@ = 0
  ezcs_ptr = 0
return 1

; 読み込みを破棄（特に意味はない）
#deffunc ezcs_unload
  sdim ezcs_buf, 64
  ezcs_size = -1
  ezcs_ptr = 0
return 1

; 文字列から値を取得
#deffunc ezcs_parse_ln str csv_text_ln
  ; 行データを取得
  ezcs_ln_buf = csv_text_ln
  getstr ezcs_ln_buf, ezcs_ln_buf, 0
  ezcs_ln_size = strlen(ezcs_ln_buf)
  ; 値を格納
  gosub *ezcs_set_argv
return 0

; ファイル現在位置から値を取得
#deffunc ezcs_parse_file
*ezcs_begin_parse_file
  ezcs_argc@ = 0

  ; ファイル終端ならば離脱
  if (ezcs_ptr == ezcs_size) : return 1

  ; ライン読み込み
  getstr ezcs_ln_buf, ezcs_buf, ezcs_ptr: ezcs_ptr += strsize: ezcs_line_no@++
  ezcs_ln_size = strlen(ezcs_ln_buf)

  ; 引数を取得
  gosub *ezcs_set_argv
  ; 取得がなければ繰り返し
  if (ezcs_argc@ == 0) : goto *ezcs_begin_parse_file
return 0

; 行内容から値を格納
*ezcs_set_argv
  ezcs_argc@ = 0
  ezcs_ln_ptr = 0

  repeat ezcs_prm_max : i = cnt
    ezcs_argv@(i) = ""
  loop

  repeat
    ; 終端チェック
    if (ezcs_ln_ptr == ezcs_ln_size) : break

    ; 空白を読み飛ばす
    repeat
      c = peek(ezcs_ln_buf, ezcs_ln_ptr)
      if ((c != '\t') & (c != ' ')) {
        break
      }
      ezcs_ln_ptr++
    loop
    ; すでに終端であれば離脱
    if (c == 0) : return

    ; とりあえず値を取得
    getstr ezcs_token, ezcs_ln_buf, ezcs_ln_ptr, ',' : ezcs_ln_ptr += strsize

    ; 末尾の空白を除去
    j = strlen(ezcs_token)
    repeat j : i = cnt
      c = peek(ezcs_token, j - i - 1)
      if ((c != '\t') & (c != ' ')) {
        break
      }
      poke ezcs_token, j - i - 1, 0
    loop

    ; クオートされていれば解除 (いい加減のキワミ)
    c = peek(ezcs_token, 0)
    if (c == '"') {
      getstr ezcs_token, ezcs_token, 1, '"'
    }

    ; 引数を格納する
    ezcs_argv@(ezcs_argc@) = ezcs_token
    ezcs_argc@++
  loop
return

#global
ezcs_init

#endif /* not EZCSV_AS */
