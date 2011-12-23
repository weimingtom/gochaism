/*
 * argument parser like setargv.obj
 * http://d.hatena.ne.jp/GOCHA/
 */

#ifndef SETARGV_AS
#define SETARGV_AS

#module "mod_setargv"

#uselib "kernel32.dll"
#cfunc GetCommandLine "GetCommandLineA"
#cfunc lstrlen "lstrlenA" sptr
#cfunc IsDBCSLeadByte "IsDBCSLeadByte" int

#define true    1
#define false   0

#deffunc setargv var cmdline, int argc_to_skip
  len_cmdline = lstrlen(cmdline)

  _argc@ = 0
  _argv@ = ""
  _arg_len_max@ = 0
  arg_len = 0
  quoted = false
  argc_to_skip_cnt = argc_to_skip
  i = 0
  repeat
    c = peek(cmdline, i)
    if ((c != ' ') & (c != 9)): break
    i++
  loop
  repeat len_cmdline + 1
    c = peek(cmdline, i)
    if (IsDBCSLeadByte(c)) {
      arg_len += 2: i += 2
      continue
    }
    else: if (c == '\"') {
      quoted = quoted ^ 1
      i++
    }
    else: if (((quoted == false) & ((c == ' ') | (c == 9))) | (c == 0)) {
      i++
      if (argc_to_skip_cnt) {
        argc_to_skip_cnt--
      }
      else: if (arg_len > 0) {
        _argc@++
      }
      if (_arg_len_max@ < arg_len) {
        _arg_len_max@ = arg_len
      }
      arg_len = 0

      if (c == 0): break
      repeat
        c = peek(cmdline, i)
        if ((c != ' ') & (c != 9)): break
        i++
      loop
      if (c == 0): break
    }
    else {
      arg_len++
      i++
    }
  loop

  if (_argc@ == 0): return
  sdim _argv@, _arg_len_max@ + 1, _argc@
  sdim arg, _arg_len_max@ + 1

  argi = 0
  quoted = false
  argc_to_skip_cnt = argc_to_skip
  i = 0
  repeat
    c = peek(cmdline, i)
    if ((c != ' ') & (c != 9)): break
    i++
  loop
  j = 0
  repeat len_cmdline + 1
    c = peek(cmdline, i)
    if (IsDBCSLeadByte(c)) {
      memcpy arg, cmdline, 2, j, i:
      i += 2: j += 2
      continue
    }
    else: if (c == '\"') {
      quoted = quoted ^ 1
      i++
    }
    else: if (((quoted == false) & ((c == ' ') | (c == 9))) | (c == 0)) {
      i++
      poke arg, j, 0
      if (argc_to_skip_cnt) {
        argc_to_skip_cnt--
      }
      else: if (j > 0) {
        _argv@.argi = arg
        argi++
      }
      j = 0

      if (c == 0): break
      repeat
        c = peek(cmdline, i)
        if ((c != ' ') & (c != 9)): break
        i++
      loop
      if (c == 0): break
    }
    else {
      memcpy arg, cmdline, 1, j, i: i++: j++
    }
  loop
return

#deffunc _setargv
  _pcmdline = GetCommandLine()
  _len_cmdline = lstrlen(_pcmdline)
  dupptr _cmdline, _pcmdline, _len_cmdline + 1, vartype("str")

#ifdef _DEBUG
  _argc_to_skip = 2
#else
  _argc_to_skip = 1
#endif

  setargv _cmdline, _argc_to_skip
return

#global

_setargv
argc = _argc
sdim argv, 64, 1
if (argc > 0) {
  sdim argv, _arg_len_max + 1, argc
  repeat argc
    argv(cnt) = _argv(cnt)
  loop
}

#endif /* not SETARGV_AS */
