/*
 * ���蔲��CSV��͋@���ǂ�
 * http://d.hatena.ne.jp/GOCHA/
 *
 * �J���}�ŋ�؂�ꂽ�l��ǂނ��߂̃��W���[���ł��B
 * ����Cheetahmen���ɂ��������ł��B����A�������܂ł���Ȃ����ǁB
 * �K���ɍ���Ă���̂œK���Ɏg���̂����E�ł��B����͂Ђǂ��B
 */

#ifndef EZCSV_AS
#define global EZCSV_AS

#module "ezcsv"

#uselib "kernel32.dll"
#cfunc IsDBCSLeadByte "IsDBCSLeadByte" int

; �������i���[�U�͌Ăяo�����Ȃ����Ɓj
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

; �w��t�@�C����ǂݍ���
#deffunc ezcs_load str filename
  ; �t�@�C���̑��݂��m�F
  exist filename
  if (strsize == -1) : return 0
  ezcs_size = strsize

  ; �t�@�C����ǂݍ���
  sdim ezcs_buf, ezcs_size + 1
  bload filename, ezcs_buf, ezcs_size
  ; �V�[�N�ʒu��擪��
  ezcs_line_no@ = 0
  ezcs_ptr = 0
return 1

; �ǂݍ��݂�j���i���ɈӖ��͂Ȃ��j
#deffunc ezcs_unload
  sdim ezcs_buf, 64
  ezcs_size = -1
  ezcs_ptr = 0
return 1

; �����񂩂�l���擾
#deffunc ezcs_parse_ln str csv_text_ln
  ; �s�f�[�^���擾
  ezcs_ln_buf = csv_text_ln
  getstr ezcs_ln_buf, ezcs_ln_buf, 0
  ezcs_ln_size = strlen(ezcs_ln_buf)
  ; �l���i�[
  gosub *ezcs_set_argv
return 0

; �t�@�C�����݈ʒu����l���擾
#deffunc ezcs_parse_file
*ezcs_begin_parse_file
  ezcs_argc@ = 0

  ; �t�@�C���I�[�Ȃ�Η��E
  if (ezcs_ptr == ezcs_size) : return 1

  ; ���C���ǂݍ���
  getstr ezcs_ln_buf, ezcs_buf, ezcs_ptr: ezcs_ptr += strsize: ezcs_line_no@++
  ezcs_ln_size = strlen(ezcs_ln_buf)

  ; �������擾
  gosub *ezcs_set_argv
  ; �擾���Ȃ���ΌJ��Ԃ�
  if (ezcs_argc@ == 0) : goto *ezcs_begin_parse_file
return 0

; �s���e����l���i�[
*ezcs_set_argv
  ezcs_argc@ = 0
  ezcs_ln_ptr = 0

  repeat ezcs_prm_max : i = cnt
    ezcs_argv@(i) = ""
  loop

  repeat
    ; �I�[�`�F�b�N
    if (ezcs_ln_ptr == ezcs_ln_size) : break

    ; �󔒂�ǂݔ�΂�
    repeat
      c = peek(ezcs_ln_buf, ezcs_ln_ptr)
      if ((c != '\t') & (c != ' ')) {
        break
      }
      ezcs_ln_ptr++
    loop
    ; ���łɏI�[�ł���Η��E
    if (c == 0) : return

    ; �Ƃ肠�����l���擾
    getstr ezcs_token, ezcs_ln_buf, ezcs_ln_ptr, ',' : ezcs_ln_ptr += strsize

    ; �����̋󔒂�����
    j = strlen(ezcs_token)
    repeat j : i = cnt
      c = peek(ezcs_token, j - i - 1)
      if ((c != '\t') & (c != ' ')) {
        break
      }
      poke ezcs_token, j - i - 1, 0
    loop

    ; �N�I�[�g����Ă���Ή��� (���������̃L���~)
    c = peek(ezcs_token, 0)
    if (c == '"') {
      getstr ezcs_token, ezcs_token, 1, '"'
    }

    ; �������i�[����
    ezcs_argv@(ezcs_argc@) = ezcs_token
    ezcs_argc@++
  loop
return

#global
ezcs_init

#endif /* not EZCSV_AS */
