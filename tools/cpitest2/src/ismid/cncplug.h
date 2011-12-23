/*--------------------------------------------------------------------------
  Cut'n'Convert Plug-In Header                            written by gocha
----------------------------------------------------------------------------
□最終更新      2004/01/23
□作成言語      Visual C++ 6.0
--------------------------------------------------------------------------*/

#ifndef _CNCPLUG_H_
#define _CNCPLUG_H_

#include <windows.h>

/*--------------------------------------------------------------------------
  DLL 用マクロ定義
--------------------------------------------------------------------------*/
#if defined(__cplusplus) || defined(c_plusplus) || defined(_LANGUAGE_C_PLUS_PLUS)
#ifndef __cplusplus
#define __cplusplus
#endif // !__cplusplus
#ifndef cplusplus
#define cplusplus
#endif // !cplusplus
#ifndef _LANGUAGE_C_PLUS_PLUS
#define _LANGUAGE_C_PLUS_PLUS
#endif // !_LANGUAGE_C_PLUS_PLUS
#endif // C++

#ifdef EXPORT
#undef EXPORT
#endif // EXPORT
#ifdef IMPORT
#undef IMPORT
#endif // IMPORT
#ifdef __cplusplus
#define _EXTERN_C_  extern "C"
#else
#define _EXTERN_C_  extern
#endif // __cplusplus
#define EXPORT      EXTERN_C __declspec(dllexport)
#define IMPORT      EXTERN_C __declspec(dllimport)

/*--------------------------------------------------------------------------
  定数宣言
--------------------------------------------------------------------------*/
#define CPI_SYSVER			0x00000100

// プラグインの種類
#define CPI_TYPE_CHECK		0x00000001
#define CPI_TYPE_CONVERT	0x00000002

/*--------------------------------------------------------------------------
  構造体の宣言
--------------------------------------------------------------------------*/
typedef struct _CPISYSTEMINFO{
	UINT uSystemVersion;
	UINT uSystemType;
} CPISYSTEMINFO;

typedef struct _CPIVERSIONINFO{
	char *szPluginName;
	char *szPluginVersion;
} CPIVERSIONINFO;

typedef struct _CPIFILEINFO{
	char *szFileName;
	int nSize;
} CPIFILEINFO;

/*--------------------------------------------------------------------------
  関数のプロトタイプ宣言
--------------------------------------------------------------------------*/
EXPORT void cpiGetPluginSystemInfo(CPISYSTEMINFO *csi);
EXPORT void cpiGetPluginInfo(CPIVERSIONINFO *cvi);
EXPORT LPSTR cpiGetSupportedList(void);
EXPORT BOOL cpiCheckData(CPIFILEINFO *cfi, BYTE *pBuf, int nSize, FARPROC lpfnProgressCallback);

// コールバック関数
BOOL ProgressCallback(void);

/*--------------------------------------------------------------------------
  関数ポインタ用
--------------------------------------------------------------------------*/
typedef void (*FPCPIGETPLUGINSYSTEMINFO)(CPISYSTEMINFO *);
typedef void (*FPCPIGETPLUGININFO)(CPIVERSIONINFO *);
typedef LPSTR (*FPCPIGETSUPPORTEDLIST)();
typedef BOOL (*FPCPICHECKDATA)(CPIFILEINFO *, BYTE *, int, FARPROC);

typedef BOOL (*FPPROGRESSCALLBACK)();

#endif // !_CNCPLUG_H_
