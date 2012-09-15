// Susie Plug-In wrapper for Lua

#include <stdint.h>

#define DllExport   __declspec( dllexport )

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

//------------------------------------------------------------------------------
// Error Code

#define SPI_NO_FUNCTION         -1
#define SPI_ALL_RIGHT           0
#define SPI_ABORT               1
#define SPI_NOT_SUPPORT         2
#define SPI_OUT_OF_ORDER        3
#define SPI_NO_MEMORY           4
#define SPI_MEMORY_ERROR        5
#define SPI_FILE_READ_ERROR     6
#define SPI_WINDOW_ERROR        7
#define SPI_OTHER_ERROR         8
#define SPI_FILE_WRITE_ERROR    9
#define SPI_END_OF_FILE         10

//------------------------------------------------------------------------------
// Structures

#pragma pack(push)
#pragma pack(1)

typedef struct PictureInfo
{
  int32_t left;
  int32_t top;
  int32_t width;
  int32_t height;
  uint16_t x_density;
  uint16_t y_density;
  int16_t colorDepth;
  HLOCAL hInfo;
} PictureInfo;

typedef struct fileInfo
{
  char method[8];
  uint32_t position;
  uint32_t compsize;
  uint32_t filesize;
  int32_t /*time_t*/ timestamp;
  char path[200];
  char filename[200];
  uint32_t crc;
} fileInfo;

#pragma pack(pop)

//------------------------------------------------------------------------------
// Functions

typedef int (CALLBACK *SPI_PROGRESS)(int32_t, int32_t, int32_t);

EXTERN_C {

int32_t DllExport PASCAL GetPluginInfo(int32_t infono, LPSTR buf, int32_t buflen);
int32_t DllExport PASCAL IsSupported(LPSTR filename, uint32_t dw);
int32_t DllExport PASCAL GetPictureInfo(LPSTR buf, int32_t len, uint32_t flag, PictureInfo *lpInfo);
int32_t DllExport PASCAL GetPicture(LPSTR buf, int32_t len, uint32_t flag, HANDLE *pHBInfo, HANDLE *pHBm, SPI_PROGRESS lpPrgressCallback, int32_t lData);
int32_t DllExport PASCAL GetArchiveInfo(LPSTR buf, int32_t len, uint32_t flag, HLOCAL *lphInf);
int32_t DllExport PASCAL GetFileInfo(LPSTR buf, int32_t len, LPSTR filename, uint32_t flag, fileInfo *lpInfo);
int32_t DllExport PASCAL GetFile(LPSTR src, int32_t len, LPSTR dest, uint32_t flag, SPI_PROGRESS prgressCallback, int32_t lData);
int32_t DllExport PASCAL GetPreview(LPSTR buf, int32_t len, uint32_t flag, HANDLE *pHBInfo, HANDLE *pHBm, SPI_PROGRESS lpPrgressCallback, int32_t lData);
int32_t DllExport PASCAL ConfigurationDlg(HWND parent, int32_t fnc);

}
