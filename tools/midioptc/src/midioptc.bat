@REM -----------------------------------------------------------------------
@REM   MIDI Optimizer with Cherry -- Build Option
@REM -----------------------------------------------------------------------
@REM 
@REM   hcc                                      [HSP Commandline Compiler]  
@REM 
@REM   -O:xxx                                   �o�̓t�@�C�����w��          
@REM   -A:xxx                                   AX�t�@�C�����w��            
@REM   -R:xxx                                   �ݒ�t�@�C�����w��(*.hrc)   
@REM   -S0 -S1                                  �N�����E�B���h�E��\��SW    
@REM   -Ga                                      �I�u�W�F�N�g�t�@�C��(AX)    
@REM   -Gx                                      �W�����s�t�@�C��(EXE)       
@REM   -Gf                                      �t���X�N���[�����[�h(EXE)   
@REM   -Gs                                      �X�N���[���Z�[�o�[(SCR)     
@REM 
@REM -----------------------------------------------------------------------
HCC -O:midioptc.exe -R:midioptc.hrc -S0 -Gx midioptc.as

@REM -----------------------------------------------------------------------
@REM   �N���[���A�b�v
@REM -----------------------------------------------------------------------
DEL hccrt midioptc.dpm
DEL start.ax midioptc.as.i

@REM -----------------------------------------------------------------------
@REM   HSP �ꎞ�^���ԃt�@�C���폜
@REM -----------------------------------------------------------------------
IF EXIST hsptmp DEL hsptmp
IF EXIST hsptmp.i DEL hsptmp.i
IF EXIST obj DEL obj
