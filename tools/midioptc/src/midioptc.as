/*--------------------------------------------------------------------------
  MIDI Optimizer with Cherry                           Copyright (C) GOCHA
----------------------------------------------------------------------------
���ŏI�X�V          2003/12/26
���쐬��          Hot Soup Processor 2.6.1
--------------------------------------------------------------------------*/

#define _OBJEX_C_

#include "llmod.as"
#include "objex.as"
#include "progbar.as"
#include "dragdrop.as"
#include "ini_io.as"

#include "objrtns.as"
#include "midioptc_h.as"

/*--------------------------------------------------------------------------
  �ϐ��̐錾�Ə�����
--------------------------------------------------------------------------*/

#define HELPFILE    "readme.txt"

sdim s1, 260

sdim rundir, 260
if hspstat & 1: chdir "..\\": else: chdir exedir
rundir = curdir + "\\"

dim ddinfo, 4
sdim ddlist, 260

sdim midbuf, 64
sdim middir, 260
sdim midname, 260
sdim midpath, 260
sdim midfstr, 260
sdim midlist, 260

ini_setfile rundir + "midioptc.ini"
prm = 0: gosub *load_ini

cnfdlgf = 0

/*--------------------------------------------------------------------------
  �E�B���h�E�̍쐬�ƕ\��
--------------------------------------------------------------------------*/

dd_accept ddlist, ddinfo, 0
gosub *putwnd
onkey *okproc
onexit *end_t

/*--------------------------------------------------------------------------
  ���C�����[�v
--------------------------------------------------------------------------*/
*main
	if cnfdlgf == 0{
		if ddinfo{
			notesel ddlist
			noteget midfstr, 0
			objprm 0, midfstr
			ddinfo = 0
		}
	}

	objproclp
	wait 1
goto *main

/*--------------------------------------------------------------------------
  �L�[���荞�ݎ�
--------------------------------------------------------------------------*/
*okproc
	objprocok iparam
goto *main

/*--------------------------------------------------------------------------
  MIDI �t�@�C���I��
--------------------------------------------------------------------------*/
*selmidi
	dialog "mid", 16, "Standard MIDI File"
	if stat: midfstr = refstr: objprm 0, midfstr
goto *main

/*--------------------------------------------------------------------------
  �t�@�C���̕ϊ�
--------------------------------------------------------------------------*/
*midiopt
	getpath middir, midfstr, 32: middir += "\\"

	dirlist midlist, midfstr, 3: notesel midlist: notemax midmax
	if midlist == "": dialog "�t�@�C��������܂���", 0, SOFTNAME: goto *main

	ncnverr = 0
	repeat midmax
		notesel midlist
		noteget midpath, cnt
		midpath = middir + midpath
		getpath midname, midpath, 8

		if bakf: gosub *midibak
		gosub *midicnv
	loop
	prb_prm 64, 0

	if ncnverr{
		dialog "" + ncnverr + " �̃t�@�C��������ɕϊ��ł��܂���ł���", 0, SOFTNAME
	}
	else{
		if dlgf{
			s1 = ""
			if midmax == 1{
				t1 = midsize + maddsize * 100 / midsize
				if t1 < 100: s1 = "\n" + t1 + "% �ɂȂ�܂���(" + maddsize + " �o�C�g����)"
			}
			dialog "�ϊ����������܂���" + s1, 0, SOFTNAME
		}
		else{
			bell 0
		}
	}
goto *main

/*--------------------------------------------------------------------------
  �ݒ�_�C�A���O�\��
--------------------------------------------------------------------------*/
*cnvconf
	clroi: obj_del -1

	screen 0, 320, 232, 7, dispx - 320 >> 1, dispy - 232 >> 1
	gsel 0, 1: cls: title SOFTNAME + " " + SOFTVER

	syspal 0, 18
	syspal 1, 15
	palfade 0

	objmode 1, 1
	objsize 100, 20
	pos   8,204: button "OK(&K)", *_exitcfg: setack  0, 'K'
	pos 112,204: button "�L�����Z��(&C)", *_exitcfg: setack  1, 'C'
	pos 216,204: button "�w���v(&H)", *openhelp: setack  2, 'H'
	objsize 304, 20
	pos   8,  8: chkbox "�ϊ��I�����Ƀ_�C�A���O��\������(&N)", dlgf: setack 3, 'N'
	pos   8, 32: chkbox "�ϊ����Ƀo�b�N�A�b�v(*.bak)���쐬����(&B)", bakf: setack  4, 'B'
	pos   8, 56: chkbox "�}�[�J�[�̃��^�C�x���g���폜����(&M)", mkrf: setack  5, 'M'
	pos   8, 80: chkbox "�����̃��^�C�x���g���폜����(&Y)", keyf: setack  6, 'Y'
	pos   8,104: chkbox "�e�L�X�g�Ɖ̎��̃��^�C�x���g���폜(&T)", txtf: setack  7, 'T'
	pos   8,128: chkbox "�Ȗ��ƃg���b�N���ƒ��쌠�̃e�L�X�g���폜(&S)", ttlf: setack  8, 'S'
	pos   8,152: chkbox "�g���b�N���̃e�L�X�g���폜(&R)", trnf: setack 9, 'R'
	pos   8,176: chkbox "�t�H�[�}�b�g 1 �̃f�[�^���t�H�[�}�b�g 0 �ɕϊ�����(&F)", fmtf: setack 10, 'F'
	setdefbtn 0

	obj_enbl 10, 0
	obj_tip "�ύX���e��ۑ����Ă��̃E�B���h�E����܂�", 0
	obj_tip "�ύX���e��j�����Ă��̃E�B���h�E����܂�", 1
	obj_tip "�w���v(" + HELPFILE + ")��\�����܂�", 2
	obj_tip "�ϊ��I�����m�F����_�C�A���O�̗L����ݒ肵�܂�", 3
	obj_tip "�ϊ����̃o�b�N�A�b�v�t�@�C���쐬�̗L����ݒ肵�܂�", 4
	obj_tip "�}�[�J�[�̃C�x���g���폜���邩�ǂ�����ݒ肵�܂�", 5
	obj_tip "�����̃C�x���g���폜���邩�ǂ�����ݒ肵�܂�", 6
	obj_tip "�e�L�X�g�Ɖ̎��̃��^�C�x���g���폜���邩�ǂ�����ݒ肵�܂�", 7
	obj_tip "�Ȗ��ƃg���b�N���ƒ��쌠�̏����폜���邩�ǂ�����ݒ肵�܂�", 8
	obj_tip "�g���b�N�������폜���邩�ǂ�����ݒ肵�܂�", 9
	obj_tip "�t�H�[�}�b�g 1 �̃f�[�^���t�H�[�}�b�g 0 �̃f�[�^�ɕϊ����܂�", 10

	redraw 2
	palcolor 1: boxf
	redraw 1

	cnfdlgf = 1
	dd_reject 0, 0
	winact 0
goto *main

/*--------------------------------------------------------------------------
  �w���v�t�@�C���̕\��
--------------------------------------------------------------------------*/
*openhelp
	exist rundir + HELPFILE
	if strsize < 0{
		dialog HELPFILE + " ���J�����Ƃ��ł��܂���", 0, SOFTNAME
	}
	else{
		exec rundir + HELPFILE, 16
	}
goto *main

/*--------------------------------------------------------------------------
  �ݒ�_�C�A���O�I������
--------------------------------------------------------------------------*/
*_exitcfg
	prm = stat
*exitcfg
	if prm == 1: prm = 1: gosub *load_ini

	clroi: obj_del -1
	cnfdlgf = 0: gosub *putwnd
goto *main


/*--------------------------------------------------------------------------
  �G���h�g���b�v
--------------------------------------------------------------------------*/
*end_t
	if cnfdlgf: prm = 1: goto *exitcfg
goto *_exit

/*--------------------------------------------------------------------------
  �I������
--------------------------------------------------------------------------*/
*_exit
	gosub *save_ini
	end

/*--------------------------------------------------------------------------
  sub.
----------------------------------------------------------------------------
  �E�B���h�E�̍쐬�ƕ\��
--------------------------------------------------------------------------*/
*putwnd
	screen 0, 424, 60, 7, dispx - 424 >> 1, dispy - 60 >> 1
	gsel 0, 1: cls: title SOFTNAME + " " + SOFTVER

	syspal 0, 18
	syspal 1, 15
	palfade 0

	objmode 1, 1
	pos   8,  8: s1 = midfstr: midfstr = "": input midfstr, 196, 20: objprm 0, s1
	objsize 100, 20
	pos 208,  8: button "�Q��(&R)...", *selmidi: setack 1, 'R'
	pos 316,  8: button "�ϊ�(&O)", *midiopt: setack 2, 'O'
	pos 316, 32: button "�ݒ�(&C)...", *cnvconf: setack 3, 'C'
	setdefbtn 2
	objsize 300, 20
	pos   8, 32: prb_put 1, 0, 1000

	obj_tip "�ϊ��Ώۃt�@�C���̃p�X���w�肵�܂�", 0
	obj_tip "�ϊ��Ώۃt�@�C�����_�C�A���O����I�����܂�", 1
	obj_tip "�ݒ�Ɋ�Â��ăt�@�C����ϊ����܂�", 2
	obj_tip "�ϊ��ݒ�_�C�A���O��\�����܂�", 3
	obj_tip "�������̃t�@�C���̕ϊ��󋵂������Ă��܂�", 64

	redraw 2
	palcolor 1: boxf
	redraw 1

	dd_reject 0, 1
	winact 0
return

/*--------------------------------------------------------------------------
  �ݒ�t�@�C���̓Ǎ�
--------------------------------------------------------------------------*/
*load_ini
	bakf = 1: dlgf = 1: mkrf = 1
	keyf = 1: txtf = 1: ttlf = 0: trnf = 0

	ini_section SOFTNAME
	ini_getint "ConvertFormat1", fmtf
	ini_getint "CreateBackup", bakf
	ini_getint "NotifyDialog", dlgf
	ini_getint "RemoveCopyrights", ttlf
	ini_getint "RemoveKey", keyf
	ini_getint "RemoveMarker", mkrf
	ini_getint "RemoveTextEvent", txtf
	ini_getint "RemoveTrackName", trnf
	if prm == 0: ini_getstr "LastFilePath", midfstr
return

/*--------------------------------------------------------------------------
  �ݒ�t�@�C����ۑ�
--------------------------------------------------------------------------*/
*save_ini
	ini_section SOFTNAME
	ini_setint "ConvertFormat1", fmtf
	ini_setint "CreateBackup", bakf
	ini_setint "NotifyDialog", dlgf
	ini_setstr "LastFilePath", midfstr
	ini_setint "RemoveCopyrights", ttlf
	ini_setint "RemoveKey", keyf
	ini_setint "RemoveMarker", mkrf
	ini_setint "RemoveTextEvent", txtf
	ini_setint "RemoveTrackName", trnf
return

/*--------------------------------------------------------------------------
  �o�b�N�A�b�v���쐬
--------------------------------------------------------------------------*/
*midibak
	bcopy midpath, midpath + ".bak"
return

/*--------------------------------------------------------------------------
  �t�@�C���ϊ�����
--------------------------------------------------------------------------*/
*midicnv
	exist midpath: midsize = strsize
	sdim midbuf, midsize + 0x1000
	bload midpath, midbuf

	gosub *midichk
	if ret: ncnverr++: return

	dim midtrkptr, midtrk
	_mprog = 0: prb_prm 64, 0
	mmsgsize = 0: maddsize = 0

	seqttlf = 0
	midptr = 0x0E
	repeat midtrk
		mtrkaddsize = 0

		mnowtrk = cnt
		midptr += mmsgsize: mmsgsize = 0

		strmid s1, midbuf, midptr, 4: midptr += 4
		if s1 != "MTrk": ret = 1: break

		ptrksize = midptr: midptr += 4
		lpeekb mtrksize, midbuf, ptrksize

		ret = 0
		_mmsg = -1
		mtickque = 0
		ptrkbegin = midptr
		midtrkptr.cnt = midptr
		repeat
			midptr += mmsgsize: mmsgsize = 0
			if midptr >= (ptrkbegin + mtrksize + mtrkaddsize): ret = 1: break

			mprog = midptr + 1 * 1000 / (midsize + mtrksize + mtrkaddsize) >> cnvfmtf
			if mprog != _mprog: prb_prm 64, mprog

			/* �f���^�^�C�� */
			xpeekb mtick, midbuf, midptr
			ticksize = stat: mmsgsize += ticksize

			/* ���b�Z�[�W�R�}���h */
			peek mmsg, midbuf, midptr + mmsgsize
			if mmsg & 0x80: rl = 0: mmsgsize++: else: mmsg = _mmsg: rl = 1
			mmsgcmd = mmsg >> 4 & 15: mmsgtrk = mmsg & 15

			/* ���^�C�x���g */
			mmetamsg = -1
			if mmsg == 0xFF{
				peek mmetamsg, midbuf, midptr + mmsgsize: mmsgsize++
				xpeekb mmetasize, midbuf, midptr + mmsgsize: mmsgsize += stat

				pmetamsg = midptr + mmsgsize
				mmsgsize += mmetasize
				if (mmetamsg == 0x06) & (mkrf != 0){
					/* �}�[�J�[ */
					gosub *mmsgdel
					continue
				}

				if (mmetamsg == 0x59) & (keyf != 0){
					/* ���� */
					gosub *mmsgdel
					continue
				}

				if (mmetamsg == 0x01) & (txtf != 0){
					/* �e�L�X�g */
					gosub *mmsgdel
					continue
				}

				if (mmetamsg == 0x02) & (ttlf != 0){
					/* ���쌠�\�� */
					gosub *mmsgdel
					continue
				}

				if mmetamsg == 0x03{
					/* �V�[�P���X�� */
					if (ttlf != 0){
						gosub *mmsgdel
					}
					else{
						if (seqttlf != 0) & (trnf != 0): gosub *mmsgdel
					}
					if seqttlf == 0: seqttlf = 1
					continue
				}

				if (mmetamsg == 0x05) & (txtf != 0){
					/* �̎� */
					gosub *mmsgdel
					continue
				}
			}
			else{ if mmsg == 0xF0{
				/* �G�N�X�N���[�V�u���b�Z�[�W */
				xpeekb t1, midbuf, midptr + mmsgsize
				mmsgsize += t1 + stat
			}
			else{ if (mmsg >= 0xF1){
				/* MIDI �^�C���R�[�h */
				mmsgsize += 1
			}
			else{ if (mmsg >= 0xF2){
				/* �\���O�|�W�V�����|�C���^ */
				mmsgsize += 2
			}
			else{ if (mmsg >= 0xF3){
				/* �\���O�Z���N�g */
				mmsgsize += 1
			}
			else{ if mmsgcmd == 0x9{
				/* �m�[�g�I�� */
				mmsgsize += 2
			}
			else{ if mmsgcmd == 0x8{
				/* �m�[�g�I�t */
				mmsgsize += 2
			}
			else{ if mmsgcmd == 0xA{
				/* �|���t�H�j�b�N�L�[�v���b�V�� */
				mmsgsize += 2
			}
			else{ if mmsgcmd == 0xC{
				/* �v���O�����`�F���W */
				mmsgsize++
			}
			else{ if mmsgcmd == 0xD{
				/* �`�����l���v���b�V�� */
				mmsgsize += 1
			}
			else{ if mmsgcmd == 0xE{
				/* �s�b�`�x���h�`�F���W */
				mmsgsize += 2
			}
			else{ if mmsgcmd == 0xB{
				/* �R���g���[���`�F���W */
				mmsgsize += 2
			}
			}}}}}}}}}}} /* (^^;) */
			_mmsg = mmsg
			if mtickque{
				t1 = mtick + mtickque: xintlen t2, t1
				memdel midbuf, midptr, ticksize
				memins midbuf, midptr, t2
				xpokeb midbuf, midptr, t1
				mmsgsize += t2 - ticksize
				mtrkaddsize += t2 - ticksize
				ticksize = t2
				mtickque = 0
			}
			if mmetamsg == 0x2F: ret = 0: break
		loop
		if ret: break

		lpokeb midbuf, ptrksize, mtrksize + mtrkaddsize
		maddsize += mtrkaddsize
	loop

	if cnvfmtf{
		/*
		Convert format 1 to 0
		*/
	}

	prb_prm 64, 1000
;	if ret: ncnverr++: return

	bsave midpath, midbuf, midsize + maddsize
return

/*--------------------------------------------------------------------------
  �t�@�C���ȈՃ`�F�b�N����
--------------------------------------------------------------------------*/
*midichk
	strmid s1, midbuf, 0x00, 4
	if s1 != "MThd": ret = 1: return

	lpeekb t1, midbuf, 0x04
	if t1 != 6: ret = 1: return

	wpeekb midfmt, midbuf, 0x08
	if midfmt >> 1 != 0: ret = 1: return
	cnvfmtf = (midfmt == 1) & (fmtf != 0)

	wpeekb midtrk, midbuf, 0x0A
	wpeekb midtbs, midbuf, 0x0C
	ret = 0
return

/*--------------------------------------------------------------------------
  MIDI ���b�Z�[�W�폜
--------------------------------------------------------------------------*/
*mmsgdel
	mtickque += mtick
	memdel midbuf, midptr, mmsgsize
	mtrkaddsize -= mmsgsize
	mmsgsize = 0
return
