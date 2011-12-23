#ifndef mod_onlyone
#module mod_onlyone

#uselib "kernel32.dll"
#cfunc GetLastError "GetLastError"
#cfunc CreateFileMapping "CreateFileMappingA" int,int,int,int,int,sptr
#cfunc MapViewOfFile "MapViewOfFile" int,int,int,int,int
#func UnmapViewOfFile "UnmapViewOfFile" int
#func CloseHandle "CloseHandle" int
#uselib "user32.dll"
#func SetForegroundWindow "SetForegroundWindow" int
#deffunc OnlyOne str _key
  tmpstr = dir_cmdline
  cds = 0, strlen(tmpstr)+1, varptr(tmpstr)
  hFileMap = CreateFileMapping(-1, 0, 4, 0, 4, _key)
  Second = (GetLastError() == 183)
  ptr = MapViewOfFile(hFileMap, 2, 0, 0, 0)
  dupptr v, ptr, 4, vartype("int")
  if (Second) {
    sendmsg v, $4A, hwnd, varptr(cds)
    SetForegroundWindow v
    UnmapViewOfFile ptr : end
  }
  v = hwnd
  UnmapViewOfFile ptr
  mref rstr,65
return

;// Å´Ç§ÇÌÅ[ÅcÅc
#define global OnlyOne_SetProc(%1) %tOOSP goto *%i:*%i:\
  dupptr cdsr@mod_onlyone,lparam,12:\
  dupptr cdslpdata@mod_onlyone,cdsr@mod_onlyone.2,1,2 :\
  rstr@mod_onlyone=cdslpdata@mod_onlyone :\
  gosub %1:return 1:*%p1:oncmd gosub *%o, $4A %o0

#deffunc OnlyOneOnExit onexit
  CloseHandle hFileMap
  return 0
#global
#endif
