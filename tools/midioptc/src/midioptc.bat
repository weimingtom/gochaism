@REM -----------------------------------------------------------------------
@REM   MIDI Optimizer with Cherry -- Build Option
@REM -----------------------------------------------------------------------
@REM 
@REM   hcc                                      [HSP Commandline Compiler]  
@REM 
@REM   -O:xxx                                   出力ファイル名指定          
@REM   -A:xxx                                   AXファイル名指定            
@REM   -R:xxx                                   設定ファイル名指定(*.hrc)   
@REM   -S0 -S1                                  起動時ウィンドウ非表示SW    
@REM   -Ga                                      オブジェクトファイル(AX)    
@REM   -Gx                                      標準実行ファイル(EXE)       
@REM   -Gf                                      フルスクリーンモード(EXE)   
@REM   -Gs                                      スクリーンセーバー(SCR)     
@REM 
@REM -----------------------------------------------------------------------
HCC -O:midioptc.exe -R:midioptc.hrc -S0 -Gx midioptc.as

@REM -----------------------------------------------------------------------
@REM   クリーンアップ
@REM -----------------------------------------------------------------------
DEL hccrt midioptc.dpm
DEL start.ax midioptc.as.i

@REM -----------------------------------------------------------------------
@REM   HSP 一時／中間ファイル削除
@REM -----------------------------------------------------------------------
IF EXIST hsptmp DEL hsptmp
IF EXIST hsptmp.i DEL hsptmp.i
IF EXIST obj DEL obj
