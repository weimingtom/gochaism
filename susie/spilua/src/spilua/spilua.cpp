// Susie Plug-In wrapper for Lua

#include <string>
#include <sstream>

#include <windows.h>
#include <shlwapi.h>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
int luaopen_bit(lua_State *L);
}

#include "spilua.h"

//------------------------------------------------------------------------------
// Constants

#define LUA_SPILIB_NAME "spi"

//------------------------------------------------------------------------------
// Variables

#define Thread      __declspec( thread )

HINSTANCE hinstDLL = NULL;
struct DLLInfo {
	lua_State *L;
	bool LuaError;
	std::string LuaScriptPath;
} g;

//------------------------------------------------------------------------------
// Function Prototypes

void InitDllInfo();
std::string GetDefaultLuaScriptPath();
void PrintErrorMessage(std::string errorMessage);
bool RequireLuaScript();
bool OpenLuaScript(std::string luaScriptPath);
void RegisterLuaLib();
bool CallLuaLibFunc(const char *funcname, int nargs, int nresults);
void LuaScriptOnError(std::string errorMessage);
void CloseLuaScript();

//------------------------------------------------------------------------------
// Main Functions

BOOL WINAPI DllMain(HINSTANCE hinstDLL_, DWORD fdwReason, LPVOID lpvReserved)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		hinstDLL = hinstDLL_;
		InitDllInfo();
		RequireLuaScript();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		CloseLuaScript();
		break;
	}
	return TRUE;
}

int32_t DllExport PASCAL GetPluginInfo(int32_t infono, LPSTR buf, int32_t buflen)
{
	if (infono < 0)
		return 0;

	if (!RequireLuaScript())
		return 0;

	int type;
	switch(infono)
	{
	case 0:
		{
			if (!CallLuaLibFunc("GetPluginType", 0, 1))
				return 0;

			type = lua_type(g.L, -1);
			if (type != LUA_TSTRING)
			{
				lua_pop(g.L, 1);

				std::ostringstream os;
				os << "'GetPluginType' returned unexpected result "
					"(" << lua_typename(g.L, LUA_TSTRING) << " expected, got " << lua_typename(g.L, type) << ")";
				LuaScriptOnError(os.str());
				return 0;
			}

			const char *s = lua_tostring(g.L, -1);
			lstrcpyn(buf, s, buflen);
			lua_pop(g.L, 1);
			return strlen(s);
		}

	case 1:
		{
			if (!CallLuaLibFunc("GetPluginName", 0, 1))
				return 0;

			type = lua_type(g.L, -1);
			if (type != LUA_TSTRING)
			{
				lua_pop(g.L, 1);

				std::ostringstream os;
				os << "'GetPluginName' returned unexpected result "
					"(" << lua_typename(g.L, LUA_TSTRING) << " expected, got " << lua_typename(g.L, type) << ")";
				LuaScriptOnError(os.str());
				return 0;
			}

			const char *s = lua_tostring(g.L, -1);
			lstrcpyn(buf, s, buflen);
			lua_pop(g.L, 1);
			return strlen(s);
		}

	default:
		{
			const char *fieldName[] = { "filename", "description" };
			size_t tableIndex = 1 + ((infono - 2) / 2);
			size_t infoIndex = infono % 2;

			if (!CallLuaLibFunc("GetPluginFileTypes", 0, 1))
				return 0;

			type = lua_type(g.L, -1);
			if (type != LUA_TTABLE)
			{
				lua_pop(g.L, 1);

				std::ostringstream os;
				os << "'GetPluginFileTypes' returned unexpected result "
					"(" << lua_typename(g.L, LUA_TTABLE) << " expected, got " << lua_typename(g.L, type) << ")";
				LuaScriptOnError(os.str());
				return 0;
			}

			size_t tableLen = lua_objlen(g.L, -1);
			if (tableIndex > tableLen)
			{
				lua_pop(g.L, 1);
				return 0;
			}

			lua_pushinteger(g.L, tableIndex);
			lua_gettable(g.L, -2);
			type = lua_type(g.L, -1);
			if (type != LUA_TTABLE)
			{
				lua_pop(g.L, 2);

				std::ostringstream os;
				os << "index [" << tableIndex << "] of 'GetPluginFileTypes' returned unexpected result "
					"(" << lua_typename(g.L, LUA_TTABLE) << " expected, got " << lua_typename(g.L, type) << ")";
				LuaScriptOnError(os.str());
				return 0;
			}

			lua_pushstring(g.L, fieldName[infoIndex]);
			lua_gettable(g.L, -2);
			type = lua_type(g.L, -1);
			if (type != LUA_TSTRING)
			{
				lua_pop(g.L, 3);

				std::ostringstream os;
				os << "field '[" << tableIndex << "]." << fieldName[infoIndex] << "' of 'GetPluginFileTypes' returned unexpected result "
					"(" << lua_typename(g.L, LUA_TSTRING) << " expected, got " << lua_typename(g.L, type) << ")";
				LuaScriptOnError(os.str());
				return 0;
			}

			const char *s = lua_tostring(g.L, -1);
			lstrcpyn(buf, s, buflen);
			lua_pop(g.L, 3);
			return strlen(s);
		}
	}
}

int32_t DllExport PASCAL IsSupported(LPSTR filename, uint32_t dw)
{
	if (!RequireLuaScript())
		return FALSE;

	const int HeadBufSize = 2048;
	char buf_[HeadBufSize];
	ZeroMemory(buf_, HeadBufSize);
	int ActualHeadBufSize = HeadBufSize;

	char *buf;
	bool dwIsFile = ((dw & 0xFFFF0000) == 0);
	if (dwIsFile)
	{
		DWORD ReadBytes;
		if (!ReadFile((HANDLE)dw, buf_, HeadBufSize, &ReadBytes, NULL)) {
			return FALSE;
		}
		buf = buf_;
		ActualHeadBufSize = ReadBytes;
	}
	else
	{
		buf = (char*)dw;
	}

	lua_pushstring(g.L, filename);
	lua_pushlstring(g.L, buf, ActualHeadBufSize);
	if (!CallLuaLibFunc("IsSupported", 0, 1))
		return FALSE;
	int32_t result = lua_toboolean(g.L, -1);
	lua_pop(g.L, 1);
	return result;
}

int32_t DllExport PASCAL GetPictureInfo(LPSTR buf, int32_t len, uint32_t flag, PictureInfo *lpInfo)
{
	if (!RequireLuaScript())
		return SPI_OTHER_ERROR;
	return SPI_NO_FUNCTION;
}

int32_t DllExport PASCAL GetPicture(LPSTR buf, int32_t len, uint32_t flag, HANDLE *pHBInfo, HANDLE *pHBm, SPI_PROGRESS lpPrgressCallback, int32_t lData)
{
	if (!RequireLuaScript())
		return SPI_OTHER_ERROR;
	return SPI_NO_FUNCTION;
}

int32_t DllExport PASCAL GetArchiveInfo(LPSTR buf, int32_t len, uint32_t flag, HLOCAL *lphInf)
{
	if (!RequireLuaScript())
		return SPI_OTHER_ERROR;
	return SPI_NO_FUNCTION;
}

int32_t DllExport PASCAL GetFileInfo(LPSTR buf, int32_t len, LPSTR filename, uint32_t flag, fileInfo *lpInfo)
{
	if (!RequireLuaScript())
		return SPI_OTHER_ERROR;
	return SPI_NO_FUNCTION;
}

int32_t DllExport PASCAL GetFile(LPSTR src, int32_t len, LPSTR dest, uint32_t flag, SPI_PROGRESS prgressCallback, int32_t lData)
{
	if (!RequireLuaScript())
		return SPI_OTHER_ERROR;
	return SPI_NO_FUNCTION;
}

int32_t DllExport PASCAL GetPreview(LPSTR buf, int32_t len, uint32_t flag, HANDLE *pHBInfo, HANDLE *pHBm, SPI_PROGRESS lpPrgressCallback, int32_t lData)
{
	if (!RequireLuaScript())
		return SPI_OTHER_ERROR;
	return SPI_NO_FUNCTION;
}

int32_t DllExport PASCAL ConfigurationDlg(HWND parent, int32_t fnc)
{
	if (!RequireLuaScript())
		return SPI_OTHER_ERROR;
	return SPI_NO_FUNCTION;
}

//------------------------------------------------------------------------------
// Subroutines

void InitDllInfo()
{
	g.L = NULL;
	g.LuaScriptPath = "";
	g.LuaError = false;
}

void PrintErrorMessage(std::string errorMessage)
{
	if (!g.LuaError)
	{
		std::string title = std::string(PathFindFileName(g.LuaScriptPath.c_str())) + " - Lua Error";
		MessageBox(NULL, errorMessage.c_str(), title.c_str(), MB_ICONERROR);
		g.LuaError = true;
	}
}

std::string GetDefaultLuaScriptPath()
{
	std::string luaScriptPath = "";
	char szModuleFileName[_MAX_PATH + 1];
	char szLuaScriptPath[_MAX_PATH + 1];
	if (GetModuleFileName(hinstDLL, szModuleFileName, _MAX_PATH) != 0)
	{
		lstrcpy(szLuaScriptPath, szModuleFileName);
		PathRemoveExtension(szLuaScriptPath);
		PathAddExtension(szLuaScriptPath, ".lua");
		luaScriptPath = szLuaScriptPath;
	}
	return luaScriptPath;
}

bool RequireLuaScript()
{
	if (g.L == NULL)
		return OpenLuaScript(GetDefaultLuaScriptPath());
	else
		return TRUE;
}

bool OpenLuaScript(std::string luaScriptPath)
{
	CloseLuaScript();

	g.LuaScriptPath = luaScriptPath;
	g.L = lua_open();
	if (g.L == NULL)
	{
		PrintErrorMessage("Lua initialization error");
		CloseLuaScript();
		return false;
	}

	luaL_openlibs(g.L);
	luaopen_bit(g.L);
	RegisterLuaLib();
	if (luaL_dofile(g.L, luaScriptPath.c_str()) != 0)
	{
		std::string errorMessage = lua_tostring(g.L, -1);
		LuaScriptOnError(errorMessage);
		return false;
	}
	g.LuaError = false;
	return true;
}

void RegisterLuaLib()
{
	lua_newtable(g.L);

	lua_pushstring(g.L, "PluginType");
	lua_newtable(g.L);
	lua_pushstring(g.L, "Image");
	lua_pushstring(g.L, "00IN");
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "Archive");
	lua_pushstring(g.L, "00AM");
	lua_settable(g.L, -3);
	lua_settable(g.L, -3);

	lua_pushstring(g.L, "ErrorCode");
	lua_newtable(g.L);
	lua_pushstring(g.L, "NoFunction");
	lua_pushinteger(g.L, SPI_NO_FUNCTION);
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "AllRight");
	lua_pushinteger(g.L, SPI_ALL_RIGHT);
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "Abort");
	lua_pushinteger(g.L, SPI_ABORT);
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "NotSupport");
	lua_pushinteger(g.L, SPI_NOT_SUPPORT);
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "OutOfOrder");
	lua_pushinteger(g.L, SPI_OUT_OF_ORDER);
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "NoMemory");
	lua_pushinteger(g.L, SPI_NO_MEMORY);
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "MemoryError");
	lua_pushinteger(g.L, SPI_MEMORY_ERROR);
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "ReadError");
	lua_pushinteger(g.L, SPI_FILE_READ_ERROR);
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "WindowError");
	lua_pushinteger(g.L, SPI_WINDOW_ERROR);
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "OtherError");
	lua_pushinteger(g.L, SPI_OTHER_ERROR);
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "FileWriteError");
	lua_pushinteger(g.L, SPI_FILE_WRITE_ERROR);
	lua_settable(g.L, -3);
	lua_pushstring(g.L, "EndOfFile");
	lua_pushinteger(g.L, SPI_END_OF_FILE);
	lua_settable(g.L, -3);
	lua_settable(g.L, -3);

	lua_setglobal(g.L, LUA_SPILIB_NAME);
}

bool CallLuaLibFunc(const char *funcname, int nargs, int nresults)
{
	lua_getglobal(g.L, LUA_SPILIB_NAME);
	lua_pushstring(g.L, funcname);
	lua_gettable(g.L, -2);
	lua_remove(g.L, -2);
	lua_insert(g.L, nargs);
	if (lua_pcall(g.L, nargs, nresults, 0) != 0) {
		std::string errorMessage = lua_tostring(g.L, -1);
		LuaScriptOnError(errorMessage);
		return false;
	}
	return true;
}

void LuaScriptOnError(std::string errorMessage)
{
	PrintErrorMessage(errorMessage);
	CloseLuaScript();
}

void CloseLuaScript()
{
	if (g.L != NULL)
	{
		lua_close(g.L);
		g.L = NULL;
	}
	g.LuaScriptPath = "";
}
