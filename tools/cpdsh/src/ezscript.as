/*
 * 超手抜きスクリプトもどき
 * http://d.hatena.ne.jp/GOCHA/
 *
 * バッチファイルもどきな簡易スクリプトを読むためのモジュールです。
 * 空白区切りでコマンドと引数を記述します。;で始まる行はコメントです。
 * なんとなく簡単な仕事をさせたいときに使えるかもしれません。
 *
 * 処理方法はいい加減なので、引数がやたら長いファイルなど、
 * 意図しないファイルを相手にすると落ちます（まずないと思いますが）。
 * 重いファイルの処理には向きません。ジャンプとか先頭から探索なので。
 * しっかりしたものを求める人には特におすすめできません。これはひどい。
 */

#ifndef EZSCRIPT_AS
#define global EZSCRIPT_AS

#module "ezscript"

#uselib "kernel32.dll"
#cfunc IsDBCSLeadByte "IsDBCSLeadByte" int

; 初期化（ユーザは呼び出ししないこと）
#deffunc ezs_init
  ezs_prm_max = 16
  ezs_prm_size_max = 512
  ezs_call_max = 32

  ezs_call_level = 0
  dim ezs_ptr_to_ret, ezs_call_max
  dim ezs_line_to_ret, ezs_call_max

  sdim ezs_buf, 64
  sdim ezs_token, ezs_prm_size_max
  sdim ezs_ln_buf, ezs_prm_size_max * ezs_prm_max
  ezs_size = -1
  ezs_ptr = 0

  ezs_argc@ = 0
  sdim ezs_argv@, ezs_prm_size_max, ezs_prm_max
  ezs_line_no@ = 0
return

; 指定ファイルを読み込み
#deffunc ezs_load str filename
  ; ファイルの存在を確認
  exist filename
  if (strsize == -1) : return 0
  ezs_size = strsize

  ; ファイルを読み込む
  sdim ezs_buf, ezs_size + 1
  bload filename, ezs_buf, ezs_size
  ; シーク位置を先頭に
  ezs_line_no@ = 0
  ezs_ptr = 0
  ezs_call_level = 0
return 1

; 読み込みを破棄（特に意味はない）
#deffunc ezs_unload
  sdim ezs_buf, 64
  ezs_size = -1
  ezs_ptr = 0
return 1

; 指定行にジャンプ
#deffunc ezs_goto_line int line_no
  ; 現在の位置を退避、破棄
  ezs_line_no_old = ezs_line_no@
  ezs_ptr_old = ezs_ptr
  ezs_line_no@ = 0
  ezs_ptr = 0

  ; 行番号範囲チェック
  if (line_no <= 0) {
    return 0
  }

  ; 指定行までシーク
  result = 0
  repeat
    if ((ezs_line_no@ + 1) == line_no) {
      ; 指定行なので離脱
      result = 1
      break
    }

    if (ezs_ptr == ezs_size) {
      ; 指定行がなかったので離脱
      break
    }

    ; ライン読み込み
    getstr ezs_ln_buf, ezs_buf, ezs_ptr: ezs_ptr += strsize: ezs_line_no@++
    ezs_ln_size = strlen(ezs_ln_buf)
  loop

  /*
  ; 範囲外なら移動を破棄
  if (result == 0) {
    ezs_line_no@ = ezs_line_no_old
    ezs_ptr = ezs_ptr_old
  }
  */

return result

; 指定ラベルにジャンプ
#deffunc ezs_goto str label_name
  ; 現在の位置を退避、破棄
  ezs_line_no_old = ezs_line_no@
  ezs_ptr_old = ezs_ptr
  ezs_line_no@ = 0
  ezs_ptr = 0

  ; 指定ラベルを検索
  result = 0
  repeat
    ; ファイル終端ならば離脱
    if (ezs_ptr == ezs_size) : break

    ; ライン読み込み
    getstr ezs_ln_buf, ezs_buf, ezs_ptr: ezs_ptr += strsize: ezs_line_no@++
    ezs_ln_size = strlen(ezs_ln_buf)

    ezs_ln_ptr = 0
    gosub *ezs_get_token
    ; 指定ラベルならば離脱
    if (ezs_token == (":"+label_name)) {
      result = 1
      break
    }
  loop

  ; 失敗時ならば位置を復帰
  if (result == 0) {
    ezs_line_no@ = ezs_line_no_old
    ezs_ptr = ezs_ptr_old
  }

return result

; 指定ラベルにサブルーチンジャンプ
#deffunc ezs_call str label_name
  ; スタックレベルのチェック
  if (ezs_call_level >= ezs_call_max) {
    return 0;
  }

  ; 戻り先を退避
  ezs_ptr_to_ret(ezs_call_level) = ezs_ptr
  ezs_line_to_ret(ezs_call_level) = ezs_line_no@
  ; ジャンプ
  ezs_goto label_name
  if (stat == 0) {
    ; 失敗なら戻り先ごと破棄
    return 0
  }
  ; コールスタックを進める
  ezs_call_level++
return 1

; サブルーチンから復帰
#deffunc ezs_ret
  ; スタックレベルのチェック
  if (ezs_call_level == 0) {
    return 0;
  }

  ; コールスタックを戻す
  ezs_call_level--
  ; サブルーチンから復帰
  ezs_ptr = ezs_ptr_to_ret(ezs_call_level)
  ezs_line_no@ = ezs_line_to_ret(ezs_call_level)
return 1

; 現在位置からコマンドを取得
#deffunc ezs_parse
*ezs_begin_parse
  ezs_argc@ = 0
  repeat ezs_prm_max : i = cnt
    ezs_argv@(i) = ""
  loop

  ; ファイル終端ならば離脱
  if (ezs_ptr == ezs_size) : return 1

  ; ライン読み込み
  getstr ezs_ln_buf, ezs_buf, ezs_ptr: ezs_ptr += strsize: ezs_line_no@++
  ezs_ln_size = strlen(ezs_ln_buf)

  ezs_ln_ptr = 0
  repeat
    ; 単語を取得
    gosub *ezs_get_token
    c = peek(ezs_token, 0)
    ; 単語がなければ離脱
    if (c == 0) {
      break
    }
    ; ラベル行ならば離脱
    if ((ezs_argc@ == 0) & (c == ':')) {
      break
    }
    ; 引数を格納する
    ezs_argv@(ezs_argc@) = ezs_token
    ezs_argc@++
  loop
  ; 取得がなければ繰り返し
  if (ezs_argc@ == 0) : goto *ezs_begin_parse
return 0

; 現在行から単語を取得
*ezs_get_token
  ezs_token = ""

  ; 空白を読み飛ばす
  repeat
    c = peek(ezs_ln_buf, ezs_ln_ptr)
    if ((c != '\t') & (c != ' ')) {
      break
    }
    ezs_ln_ptr++
  loop
  ; すでに終端であれば離脱
  if (c == 0) : return
  ; コメント行であれば離脱
  if (c == ';') : return

  ezs_quoted = 0
  ezs_token_ptr = 0
  repeat
    c = peek(ezs_ln_buf, ezs_ln_ptr)

    if (IsDBCSLeadByte(c)) {
      ; 全角文字はそのままコピー
      c = wpeek(ezs_ln_buf, ezs_ln_ptr): ezs_ln_ptr += 2
      wpoke ezs_token, ezs_token_ptr, c: ezs_token_ptr += 2
      continue
    }

    if (c == 0) {
      ; 終端処理をして取得終了
      poke ezs_token, ezs_token_ptr, 0
      break
    }
    ezs_ln_ptr++

    if ((ezs_quoted == 0) & ((c == '\t') | (c == ' '))) {
      ; 単語の切れ目、終端処理をして取得終了
      poke ezs_token, ezs_token_ptr, 0: ezs_token_ptr++
      break
    }
    else: if (c == '"') {
      ; クオーテーション切り替え
      ezs_quoted = ezs_quoted ^ 1
    }
    else {
      ; 文字として処理
      poke ezs_token, ezs_token_ptr, c: ezs_token_ptr++
    }
  loop
return

#global
ezs_init

#endif /* not EZSCRIPT_AS */
