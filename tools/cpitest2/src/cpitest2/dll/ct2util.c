//----------------------------------------------------------
// ct2util.c -- written by gocha
//----------------------------------------------------------

#include <windows.h>
#include "cncplug.h"

BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  return TRUE;
}

typedef int (*PFNCHECKDATA) (int, char*);

EXPORT BOOL _cpi_match(int cpi_num, FPCPICHECKDATA* cpiCheckData, int search_size, 
                       CPIFILEINFO* cfi, BYTE *buf, int* offset, int buffer_size)
{
  CPIFILEINFO cfi_t;
  BOOL match_result;
  BYTE *buf_t = buf + *offset;
  int search_offset;
  int cpi_cnt;

  for(search_offset = 0; search_offset < search_size; search_offset++) {
    for(cpi_cnt = 0; cpi_cnt < cpi_num; cpi_cnt++) {
      match_result = cpiCheckData[cpi_cnt](&cfi_t, &buf_t[search_offset], buffer_size - search_offset, NULL);
      if(match_result) {
        memcpy(cfi, &cfi_t, sizeof(CPIFILEINFO));
        *offset += search_offset;
        return TRUE;
      }
    }
  }
  *offset += search_offset;
  return FALSE;
}
