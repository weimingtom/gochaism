/*
 * ���蔲���X�N���v�g���ǂ�
 * http://d.hatena.ne.jp/GOCHA/
 *
 * �o�b�`�t�@�C�����ǂ��ȊȈՃX�N���v�g��ǂނ��߂̃��W���[���ł��B
 * �󔒋�؂�ŃR�}���h�ƈ������L�q���܂��B;�Ŏn�܂�s�̓R�����g�ł��B
 * �Ȃ�ƂȂ��ȒP�Ȏd�������������Ƃ��Ɏg���邩������܂���B
 *
 * �������@�͂��������Ȃ̂ŁA�������₽�璷���t�@�C���ȂǁA
 * �Ӑ}���Ȃ��t�@�C���𑊎�ɂ���Ɨ����܂��i�܂��Ȃ��Ǝv���܂����j�B
 * �d���t�@�C���̏����ɂ͌����܂���B�W�����v�Ƃ��擪����T���Ȃ̂ŁB
 * �������肵�����̂����߂�l�ɂ͓��ɂ������߂ł��܂���B����͂Ђǂ��B
 */

#ifndef EZSCRIPT_AS
#define global EZSCRIPT_AS

#module "ezscript"

#uselib "kernel32.dll"
#cfunc IsDBCSLeadByte "IsDBCSLeadByte" int

; �������i���[�U�͌Ăяo�����Ȃ����Ɓj
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

; �w��t�@�C����ǂݍ���
#deffunc ezs_load str filename
  ; �t�@�C���̑��݂��m�F
  exist filename
  if (strsize == -1) : return 0
  ezs_size = strsize

  ; �t�@�C����ǂݍ���
  sdim ezs_buf, ezs_size + 1
  bload filename, ezs_buf, ezs_size
  ; �V�[�N�ʒu��擪��
  ezs_line_no@ = 0
  ezs_ptr = 0
  ezs_call_level = 0
return 1

; �ǂݍ��݂�j���i���ɈӖ��͂Ȃ��j
#deffunc ezs_unload
  sdim ezs_buf, 64
  ezs_size = -1
  ezs_ptr = 0
return 1

; �w��s�ɃW�����v
#deffunc ezs_goto_line int line_no
  ; ���݂̈ʒu��ޔ��A�j��
  ezs_line_no_old = ezs_line_no@
  ezs_ptr_old = ezs_ptr
  ezs_line_no@ = 0
  ezs_ptr = 0

  ; �s�ԍ��͈̓`�F�b�N
  if (line_no <= 0) {
    return 0
  }

  ; �w��s�܂ŃV�[�N
  result = 0
  repeat
    if ((ezs_line_no@ + 1) == line_no) {
      ; �w��s�Ȃ̂ŗ��E
      result = 1
      break
    }

    if (ezs_ptr == ezs_size) {
      ; �w��s���Ȃ������̂ŗ��E
      break
    }

    ; ���C���ǂݍ���
    getstr ezs_ln_buf, ezs_buf, ezs_ptr: ezs_ptr += strsize: ezs_line_no@++
    ezs_ln_size = strlen(ezs_ln_buf)
  loop

  /*
  ; �͈͊O�Ȃ�ړ���j��
  if (result == 0) {
    ezs_line_no@ = ezs_line_no_old
    ezs_ptr = ezs_ptr_old
  }
  */

return result

; �w�胉�x���ɃW�����v
#deffunc ezs_goto str label_name
  ; ���݂̈ʒu��ޔ��A�j��
  ezs_line_no_old = ezs_line_no@
  ezs_ptr_old = ezs_ptr
  ezs_line_no@ = 0
  ezs_ptr = 0

  ; �w�胉�x��������
  result = 0
  repeat
    ; �t�@�C���I�[�Ȃ�Η��E
    if (ezs_ptr == ezs_size) : break

    ; ���C���ǂݍ���
    getstr ezs_ln_buf, ezs_buf, ezs_ptr: ezs_ptr += strsize: ezs_line_no@++
    ezs_ln_size = strlen(ezs_ln_buf)

    ezs_ln_ptr = 0
    gosub *ezs_get_token
    ; �w�胉�x���Ȃ�Η��E
    if (ezs_token == (":"+label_name)) {
      result = 1
      break
    }
  loop

  ; ���s���Ȃ�Έʒu�𕜋A
  if (result == 0) {
    ezs_line_no@ = ezs_line_no_old
    ezs_ptr = ezs_ptr_old
  }

return result

; �w�胉�x���ɃT�u���[�`���W�����v
#deffunc ezs_call str label_name
  ; �X�^�b�N���x���̃`�F�b�N
  if (ezs_call_level >= ezs_call_max) {
    return 0;
  }

  ; �߂���ޔ�
  ezs_ptr_to_ret(ezs_call_level) = ezs_ptr
  ezs_line_to_ret(ezs_call_level) = ezs_line_no@
  ; �W�����v
  ezs_goto label_name
  if (stat == 0) {
    ; ���s�Ȃ�߂�悲�Ɣj��
    return 0
  }
  ; �R�[���X�^�b�N��i�߂�
  ezs_call_level++
return 1

; �T�u���[�`�����畜�A
#deffunc ezs_ret
  ; �X�^�b�N���x���̃`�F�b�N
  if (ezs_call_level == 0) {
    return 0;
  }

  ; �R�[���X�^�b�N��߂�
  ezs_call_level--
  ; �T�u���[�`�����畜�A
  ezs_ptr = ezs_ptr_to_ret(ezs_call_level)
  ezs_line_no@ = ezs_line_to_ret(ezs_call_level)
return 1

; ���݈ʒu����R�}���h���擾
#deffunc ezs_parse
*ezs_begin_parse
  ezs_argc@ = 0
  repeat ezs_prm_max : i = cnt
    ezs_argv@(i) = ""
  loop

  ; �t�@�C���I�[�Ȃ�Η��E
  if (ezs_ptr == ezs_size) : return 1

  ; ���C���ǂݍ���
  getstr ezs_ln_buf, ezs_buf, ezs_ptr: ezs_ptr += strsize: ezs_line_no@++
  ezs_ln_size = strlen(ezs_ln_buf)

  ezs_ln_ptr = 0
  repeat
    ; �P����擾
    gosub *ezs_get_token
    c = peek(ezs_token, 0)
    ; �P�ꂪ�Ȃ���Η��E
    if (c == 0) {
      break
    }
    ; ���x���s�Ȃ�Η��E
    if ((ezs_argc@ == 0) & (c == ':')) {
      break
    }
    ; �������i�[����
    ezs_argv@(ezs_argc@) = ezs_token
    ezs_argc@++
  loop
  ; �擾���Ȃ���ΌJ��Ԃ�
  if (ezs_argc@ == 0) : goto *ezs_begin_parse
return 0

; ���ݍs����P����擾
*ezs_get_token
  ezs_token = ""

  ; �󔒂�ǂݔ�΂�
  repeat
    c = peek(ezs_ln_buf, ezs_ln_ptr)
    if ((c != '\t') & (c != ' ')) {
      break
    }
    ezs_ln_ptr++
  loop
  ; ���łɏI�[�ł���Η��E
  if (c == 0) : return
  ; �R�����g�s�ł���Η��E
  if (c == ';') : return

  ezs_quoted = 0
  ezs_token_ptr = 0
  repeat
    c = peek(ezs_ln_buf, ezs_ln_ptr)

    if (IsDBCSLeadByte(c)) {
      ; �S�p�����͂��̂܂܃R�s�[
      c = wpeek(ezs_ln_buf, ezs_ln_ptr): ezs_ln_ptr += 2
      wpoke ezs_token, ezs_token_ptr, c: ezs_token_ptr += 2
      continue
    }

    if (c == 0) {
      ; �I�[���������Ď擾�I��
      poke ezs_token, ezs_token_ptr, 0
      break
    }
    ezs_ln_ptr++

    if ((ezs_quoted == 0) & ((c == '\t') | (c == ' '))) {
      ; �P��̐؂�ځA�I�[���������Ď擾�I��
      poke ezs_token, ezs_token_ptr, 0: ezs_token_ptr++
      break
    }
    else: if (c == '"') {
      ; �N�I�[�e�[�V�����؂�ւ�
      ezs_quoted = ezs_quoted ^ 1
    }
    else {
      ; �����Ƃ��ď���
      poke ezs_token, ezs_token_ptr, c: ezs_token_ptr++
    }
  loop
return

#global
ezs_init

#endif /* not EZSCRIPT_AS */
