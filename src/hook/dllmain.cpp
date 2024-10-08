#include <StdInc.h>
#include "MinHook.h"
#include "cDebugRender.h"

//#define BLUE_MENU // it's not fun
//#define BORDERLESS_STANDALONE

void Log(const char* message, ...)
{
	va_list args;
	char buffer[1024];
	char buffer2[1024];

	va_start(args, message);
	_vsnprintf(buffer, sizeof(buffer), message, args);
	va_end(args);

	_snprintf(buffer2, sizeof(buffer2), "[KGDR2] %s\n", buffer);

	printf(buffer2);
	OutputDebugStringA(buffer2); // for debugview output if console isn't created
}

void DebugLog(const char* message, ...)
{
	va_list args;
	char buffer[1024];
	char buffer2[1024];

	va_start(args, message);
	_vsnprintf(buffer, sizeof(buffer), message, args);
	va_end(args);

	_snprintf(buffer2, sizeof(buffer2), "%s", buffer);

	printf(buffer2);
	OutputDebugStringA(buffer2); // for debugview output if console isn't created
}

void GetDesktopResolution(int& x, int& y)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	x = desktop.right;
	y = desktop.bottom;
}

void attach_console()
{
	// Console
	AllocConsole();
	AttachConsole(GetCurrentProcessId());

	// Console Window
	HWND hConsole = GetConsoleWindow();
	RECT rect;
	GetWindowRect(hConsole, &rect);

	freopen("CON", "w", stdout);
	freopen("CONIN$", "r", stdin);
}

bool& RenderLocalPlayerPoisition = *reinterpret_cast<bool*>(0xDDCAEE);

//void RenderText(float posX, float posY, float r, float g, float b, float a, const char *text, float idk, int s1, float scale)
//{
	//function_hooker_thiscall<0xA71900, DWORD, float, float, float, float, float, float, char*, float, int, int, scale>();

	//hook::ThisCall<void>(0xA71900, FontRoot, posX, posY, r, g, b, a, text, idk, s1, 0, scale);
	//injector::thiscall<void(cDebugRender*,float,float,float,float,float,float,const char*,float,int,int,float)>::call(0xA71900, &render_instance, posX, posY, r, g, b, a, text, idk, s1, 0, scale);
//}

double __cdecl OnRender(__int64 a1, __int64 a2)
{
	// 0.94f, 0.59f, 0.24f
	//RenderText(0.05f, 0.075f, 1.0f, 1.0f, 1.0f, 1.0f, "KGDR2? :)", 0.0f, 2, 1.0f);

	RenderText(0.05f, 0.075f, tColor(1.0f, 1.0f, 1.0f, 1.0f), (char*)"KGDR2!", 0.0, false, false, 4.0f);

	return injector::cstd<double(int64_t, int64_t)>::call(0xA3A500, a1, a2);
}

int* GetLabelText(int _this, int id)
{
	return injector::thiscall<int*(int, int)>::call(0xAC5400, _this, id); // might not need _this
}

BYTE *__stdcall QuietAssert(char *error, BYTE* a2, BYTE* a3, int file)
{
	BYTE *result;
	for (result = a2; *result; ++result)
	{
		if (result == a3)
			break;
	}
	Log("File %s load failed: %s | %d", file, error, result);
	
	//return 0;
	return result;
}

int __cdecl GetOutfitIdByName(const char* a1)
{
	return injector::stdcall<int(const char*)>::call(0x41FF30, a1);
}

#ifdef BORDERLESS_STANDALONE
bool bWindowed = true;
bool bBorderless = true;
#else
bool bDebugEnabled = GetPrivateProfileIntA("GLOBAL", "debug_enabled", 0, ".\\DR2Hookary.ini") == 1;
bool bEnableQuickieDebugMenu = GetPrivateProfileIntA("GLOBAL", "enable_quickie_debug_menu", 0, ".\\DR2Hookary.ini") == 1;
bool bEnableDebugJumpMenu = GetPrivateProfileIntA("GLOBAL", "enable_debug_jump_menu", 0, ".\\DR2Hookary.ini") == 1;
bool bSkipLogos = GetPrivateProfileIntA("GLOBAL", "skip_logos", 0, ".\\DR2Hookary.ini") == 1;
bool bWindowed = GetPrivateProfileIntA("GLOBAL", "windowed", 0, ".\\DR2Hookary.ini") == 1;
bool bBorderless = GetPrivateProfileIntA("GLOBAL", "borderless", 0, ".\\DR2Hookary.ini") == 1;
#endif

void ApplyDebugPatches(int __)
{
	injector::cstd<void(int)>::call(0x7BC490, __); // LoadIniConfig

#ifdef BLUE_MENU
	//MemWrite(0xDDCB28, 1); // debug_show_loading_time
	injector::WriteMemory(0xDDCAD4, 1); // enable_dev_features
	injector::WriteMemory(0xDDC96F, 1); // limited_debug_menu
	injector::WriteMemory(0xDDCB1F, 1); // enable_one_button_debug_menu
#endif
	
#ifndef BORDERLESS_STANDALONE
	injector::WriteMemory(0xD63FCC, !bDebugEnabled); // IsRetail
	injector::WriteMemory(0xDDCAD4, bDebugEnabled); // enable_dev_features
	injector::WriteMemory(0xDDCB21, bEnableQuickieDebugMenu); // enable_quickie_debug_menu - needs to be toggled later?
	injector::WriteMemory(0xDDCB1E, bEnableQuickieDebugMenu); // enable_dev_only_debug_tiwwchnt
	injector::WriteMemory(0xDDCB29, bEnableDebugJumpMenu); // enable_debug_jump_menu
	injector::WriteMemory(0xDDCC5B, bSkipLogos); // skip_logos
#else
	injector::WriteMemory(0xDDCC5B, 1); // skip_logos
#endif

	if (bWindowed == true)
	{
		injector::WriteMemory(0xDDCADD, 1); // OverrideRenderSettings
		injector::WriteMemory(0xDDCADE, 0); // RenderFullScreen
	}

	injector::WriteMemory(0xDE25F2, 1); // SilentExitOnCrash, fixes quit 'crash'
	injector::WriteMemory(0xE5B73C, 1); // dont debug break from inside exe
}

void OTR_ApplyDebugPatches(int __)
{
	injector::WriteMemory(0xDEC1D5, 1); // skip_logos
	if (bWindowed == true)
	{
		injector::WriteMemory(0xDEC043, 1); // OverrideRenderSettings
		injector::WriteMemory(0xDEC044, 0); // RenderFullScreen
	}
}

bool isOtr;

HWND __cdecl InitializeGameWindow(LPCWSTR lpWindowName, int xRight, int yBottom, DWORD dwStyle)
{
	if (bWindowed && bBorderless)
	{
		dwStyle = WS_VISIBLE | WS_POPUP;
		Log("Made the window borderless, enjoy!");
	}

	if (isOtr)
		return injector::cstd<HWND(LPCWSTR, int, int, DWORD)>::call(0x906820, lpWindowName, xRight, yBottom, dwStyle);

	return injector::cstd<HWND(LPCWSTR, int, int, DWORD)>::call(0x8C4D80, lpWindowName, xRight, yBottom, dwStyle);
}

BOOL OTR_AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu)
{
	if (bWindowed && bBorderless)
		dwStyle = WS_VISIBLE | WS_POPUP;

	return AdjustWindowRect(lpRect, dwStyle, bMenu);
}

LONG __stdcall OTR_SetWindowLongW(HWND hWnd, int nIndex, LONG dwNewLong)
{
	//if (bWindowed && bBorderless)
		//dwNewLong = WS_VISIBLE | WS_POPUP;

	return SetWindowLongW(hWnd, nIndex, dwNewLong);
}

int addrToCheck = 0x6C2FFD;
BYTE dr2Val = 0x8B;
BYTE dr2OtrVal = 0x0F;

BOOL WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID lpvReserved)
{
	if (nReason == DLL_PROCESS_ATTACH)
	{
#ifndef BORDERLESS_STANDALONE
		attach_console();
#endif

		Log("Detecting game...");

		BYTE randomAddr = injector::ReadMemory<BYTE>(addrToCheck);

		isOtr = randomAddr == dr2OtrVal;

		if (randomAddr == dr2Val)
		{
			Log("Detected game: DR2");

			injector::MakeCALL(0x7AD13C, ApplyDebugPatches);

			injector::MakeCALL(0x8CA6A1, InitializeGameWindow);

			injector::MakeJMP(0xA3A0E0, DebugLog);

			injector::MakeCALL(0x728246, QuietAssert);
			injector::MakeCALL(0x7282F7, QuietAssert);

			MH_Initialize();
			// ...
			MH_EnableHook(MH_ALL_HOOKS);

			injector::MakeCALL(0x7B3002, OnRender);
		}
		else if (randomAddr == dr2OtrVal) 
		{
			Log("Detected game: DR2OTR");

			injector::MakeCALL(0x7E7322, OTR_ApplyDebugPatches);
			injector::MakeCALL(0x909AE0, InitializeGameWindow);
		}
		else
		{
			Log("Detected game: Unknown. What on earth are you doing?");
		}
	}
	return TRUE;
}
