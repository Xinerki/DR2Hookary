#include <windows.h>
#include <stdio.h>
#include <hook\HookingUtils.hpp>
#include <hook\HookFunction.hpp>

#include "MinHook.h"
#pragma comment(lib, "MinHook.lib")

#include <wtsapi32.h>
#pragma comment(lib, "Wtsapi32.lib")

using namespace Util;

void Log(char* message, ...)
{
	va_list args;
	char buffer[1024];
	char buffer2[1024];

	va_start(args, message);
	_vsnprintf(buffer, sizeof(buffer), message, args);
	va_end(args);

	_snprintf(buffer2, sizeof(buffer2), "[DR2ML] %s\n", buffer);

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

	// If there's a 2nd monitor, Put it there
	if (GetSystemMetrics(SM_CMONITORS) > 1)
	{
		// Desktop
		HWND hDesktop = GetDesktopWindow();
		RECT rDesktopRect;
		GetWindowRect(hDesktop, &rDesktopRect);

		SetWindowPos(hConsole, nullptr, rDesktopRect.left - rect.right - 50, 200, rect.right - rect.left, rect.bottom - rect.top, 0);
	}

	freopen("CON", "w", stdout);
	freopen("CONIN$", "r", stdin);
}

//float& somethingX = *reinterpret_cast<float*>(0xD53184);
//float& somethingY = *reinterpret_cast<float*>(0xD53188);
DWORD& FontRoot = *reinterpret_cast<DWORD*>(0xE5FEEC);
//DWORD& DatafileRoot = *reinterpret_cast<DWORD*>(0xE125C8);
//DWORD FontRoot = *(DWORD*)0xE5FEEC;
bool& RenderLocalPlayerPoisition = *reinterpret_cast<bool*>(0xDDCAEE);

void RenderText(float posX, float posY, float r, float g, float b, float a, char *text, float idk, int s1, float scale)
{
	ThisCall<void>(0xA71900, FontRoot, posX, posY, r, g, b, a, text, idk, s1, 0, scale);
}

void OnRender(int *a1, double *a2)
{
	// 0.94f, 0.59f, 0.24f
	RenderText(0.5, 0.5, 1.0f, 1.0f, 1.0f, 1.0f, "BIG", 0.0, 2, 5.0);

	//Call(0x4449C0, a1, a2);
}

/*

//*(_DWORD **)(*((_DWORD *)0xDD2D2C + 14) + 428)

*/

/*
DWORD& DatafileRoot = *reinterpret_cast<DWORD*>(0xDDBEBC);
char LoadDatafile(char* a2, int a3, BYTE* a4, int a5)
{
	return ThisCall<char>(0x741E40, &DatafileRoot, a2, a3, a4, a5);
}

*/

DWORD& DatafileRoot2 = *reinterpret_cast<DWORD*>(0xDCB0FC);

BYTE* AnotherLoadDatafile(BYTE* _this, size_t FILE, int a3, int a4, int a5, int a6)
{
	DWORD DatafileRoot2 = MemRead<DWORD>(0xE125C8);

	//return ThisCall<BYTE*>(0xA36B30, DatafileRoot, FILE, a3, a4, a5, a6, a7);
	return ThisCall<BYTE*>(0x8C0350, _this, (size_t)&FILE, a3, a4, a5, a6);
}


void* fileLoader__inst;
void* fileProcessor__inst;

/*BYTE* LoadDatafile(DWORD *_this, char* FILE, int a3, int a4, int a5, int a6, int a7)
{
	return ThisCall<BYTE*>(0xA36B30, _this, FILE, a3, a4, a5, a6, a7);
}

char ProcessDatafile(DWORD *_this, int a2, char **a3, int a4, int a5, int a6)
{
	return ThisCall<char>(0x727DD0, _this, a2, a3, a4, a5, a6);
}*/

static char*(__fastcall*g_loadDataFileOrig)(void*& _this, char* FILE, int a3, int a4, int a5, int a6, int a7);
static int(__fastcall*g_processDataFileOrig)(int a2, char** a3, int a4, int a5, char* a6);

char* fileData;

char* __stdcall LoadDatafile(DWORD*& _this, char* FILE, int a3, int a4, int a5, int a6, int a7)
{
	static bool loadedDataFile = false;

	//printf("[KGDR2] Loading datafile %s", FILE);
	Log("Loading datafile %s", FILE);

	if (!loadedDataFile)
	{
		//loadedDataFile = true;
		//fileData = ((char* (__thiscall*)(void*, char*, int, int, int, int, int))0xA36B30)(fileLoader__inst, "mods/data/fortune_exterior.txt", 21, 0, 1, 4, 1);
	}

	return ((char* (__thiscall*)(void*, char*, int, int, int, int, int))0xA36B30)(_this, FILE, a3, a4, a5, a6, a7);
	//return g_loadDataFileOrig(_this, FILE, a3, a4, a5, a6, a7);
	//return ThisCall<char*>(0xA36B30, _this, FILE, a3, a4, a5, a6, a7);
}

char* __stdcall ProcessDatafile(void*& _this, int a2, char** a3, int a4, int a5, char* a6)
{
	/*
	static bool processedDataFile = false;

	if (fileData && !processedDataFile)
	{
		((int (__thiscall*)(void*, int, char**, int, int, char*))0xA36B30)(fileProcessor__inst, a2, &fileData, a4, a5, "mods/data/fortune_exterior.txt");
		processedDataFile = true;
	}
	*/

	Log("Loading datafile %s", a3);

	return ((char* (__thiscall*)(void*, int, char**, int, int, char*))0x727DD0)(fileProcessor__inst, a2, a3, a4, a5, a6);
	//return ThisCall<char>(0x727DD0, _this, a2, a3, a4, a5, a6);
}

int CatchThis(int _, DWORD *_this)
{
	fileLoader__inst = _this;
	Log("Gotcha! fileLoader__inst = 0x%x\n", (int)_this);
	//return ThisCall<int>(0x703EF0, _this);
	return 1;
}

void CatchThis2(DWORD* _this)
{
	fileProcessor__inst = _this;
	Log("Gotcha! fileProcessor__inst = 0x%x", (int)_this);
	//ThisCall<void>(0x743A70, _this);
	//((void* (__thiscall*)(void*))0x703EF0)(_this);
}

int* GetLabelText(int _this, int id)
{
	return ThisCall<int*>(0xAC5400, _this, id);
}

int OnLevelLoad(int a1)
{

	//Log("Label test: %s", GetLabelText(0456));

	/*

	Log("Loading gameplay");

	Log("Attempting mod load");

	DWORD* v6 = Call<DWORD*>(0xA12050);

	// LoadDatafile(char* FILE, int a3, int a4, int a5, int a6, int a7)
	char* file = ((char* (__thiscall*)(void*, char*, int, int, int, int, int))0xA36B30)(v6, "mods/data/fortune_exterior.txt", 21, 0, 1, 4, 1);

	char* file2 = file;

	// ProcessDatafile(int a2, char** a3, int a4, int a5, char* a6)
	((char* (__thiscall*)(void*, int, char**, int, int, char*))0x727DD0)(fileProcessor__inst, (int)file, &file2, 0, 0, "mods/data/fortune_exterior.txt");

	Call<void>(0xA20FC0, file);

	*/

	//auto v6 = Call<DWORD*>(0xA12050);
	//auto v7 = LoadDatafile(v6, "mods/data/fortune_exterior.txt", 21, 0, 1, 4, 1);
	//ProcessDatafile(fakeThis, (int)v7, (char**)&v7, 0, 0, (int)"mods/data/fortune_exterior.txt");


	/*if (fakeThis)
	{
		DWORD* v1;
		int v4;
		char v5;
		DWORD *v6;
		BYTE* v7;
		void* v8;

		float v16;

		v1 = fakeThis;
		//v1 = MemRead<DWORD *>(0xDDBEBC);

		//v4 = dword_E12718[dword_E12654];
		u32 tempPtr = MemRead<u32>(0xE12718);
		v4 = tempPtr+4;
		v5 = 0;
		v6 = Call<DWORD *>(0xA12050);
		v7 = LoadDatafile(v6, "mods/data/fortune_exterior.txt", 21, 0, 1, 4, 1);
		v8 = v7;
		if (v7)
		{
			Log("Attempting ProcessDatafile\n");
			v16 = *(float*)&v7;
			v5 = ProcessDatafile(v1, (int)v7, (char**)&v16, 0, 0, (int)"mods/data/fortune_exterior.txt");
			Call<void>(0xA20FC0, v8);
		
		}
		else
		{
			Log("v7 failed, file is missing\n");
		}
	}*/

	Log("Mod load attempt done");

	return Call<int>(0xA56D30, a1);
}

/*
void CatchThis(int *_this, signed int a2)
{
	ThisCall<void>(0x7448D0, _this, a2);
}
*/

static HWND(*g_createWindowOrig)(LPCWSTR lpWindowName, int xRight, int yBottom, DWORD dwStyle);

HWND CreateWindowHk(LPCWSTR lpWindowName, int xRight, int yBottom, DWORD dwStyle)
{
	if (xRight == 0 || yBottom == 0)
	{
		Log("Window ded, resurrecting");
		GetDesktopResolution(xRight, yBottom);
	}

	return g_createWindowOrig(lpWindowName, xRight, yBottom, dwStyle);

	//HINSTANCE v4; // esi
	HWND result; // eax
	struct tagRECT rc; // [esp+4h] [ebp-38h]
	WNDCLASSW WndClass; // [esp+14h] [ebp-28h]

	auto ClassName = "DeadRising2WindowClass";
	auto ClassNameWide = L"DeadRising2WindowClass";
	auto X = *(int*)0xDDC9D8;
	auto Y = *(int*)0xDDC9DC;

	Log("CREATING %s WINDOW WITH X = %d Y = %d W = %d H = %d\n", ClassName, X, Y, xRight, yBottom);

	//v4 = GetModuleHandleW(0);
	WndClass.style = 8;
	WndClass.lpfnWndProc = *(WNDPROC*)0x8C4C40;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = NULL;
	//WndClass.hIcon = LoadIconW(NULL, (LPCWSTR)0x65);
	//WndClass.hCursor = LoadCursorW(0, (LPCWSTR)0x7F00);
	//WndClass.hbrBackground = (HBRUSH)GetStockObject(4);
	WndClass.lpszMenuName = 0;
	WndClass.lpszClassName = ClassNameWide;

	if (RegisterClassW(&WndClass) || (result = (HWND)GetLastError(), result == (HWND)1410))
	{
		SetRect(&rc, 0, 0, xRight, yBottom);
		AdjustWindowRect(&rc, dwStyle, 0);
		result = CreateWindowExW(0, ClassNameWide, lpWindowName, dwStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 0, 0, NULL, 0);
		
		// set global hWnd
		//*(HWND*)0xDE25E8 = result;
		MemWrite(0xDE25E8, result);

		if (result)
		{
			MoveWindow(result, X, Y, rc.right - rc.left, rc.bottom - rc.top, 1);
			result = (HWND)WTSRegisterSessionNotification(result, 0);
		}
	}
	return result;
}

bool mmmmmmno()
{
	return false;
}

static LRESULT(WINAPI *g_origWndProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT wndProxy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == 6) // focus loss
	{
		return false;
	}
	return g_origWndProc(hWnd, uMsg, wParam, lParam);
}

BYTE *WeirdAssert(char* a1, BYTE* a2, BYTE* a3, int a4)
{
	BYTE *result;
	for (result = a2; *result; ++result)
	{
		if (result == a3)
			break;
	}
	Log("File %s load failed: %s", a4, a1);
	return 0;
	//return result;
}

//int idkman(char* Buffer, const char* _Format, ...)
int idkman(char* Buffer, const char* _Format, int kills)
{
	/*
	va_list args;
	int result;
	//return (int*)"hot single horses in your area";
	//return &*(int*)0xCB2E84;

	va_start(args, _Format);
	result = _vsnprintf(Buffer, sizeof(Buffer), "%d hot single horses in your area", args);
	//sprintf(Buffer, _Format, args);
	Log("%s", Buffer);
	va_end(args);

	return result;
	*/

	int result;

	result = sprintf(Buffer, "%d hot single horses in your area", kills);
	Log("%s", Buffer);

	return result;
}

int snprintfHook(char* Buffer, size_t Size, char* _Format, ...)
{
	va_list args;
	int result;
	//return (int*)"hot single horses in your area";
	//return &*(int*)0xCB2E84;

	va_start(args, _Format);
	result = _vsnprintf(Buffer, Size, _Format, args);
	//sprintf(Buffer, _Format, args);
	Log("Loading datafile %s", args);
	va_end(args);

	return result;
}

void ApplyDebugPatches(int __)
{
	//MakeJMP(0x8C4D80, CreateWindowHk);
	//Call<int>(, CreateWindowHk);

	MakeCALL(0xA532D0, mmmmmmno); // no more XLiveRender it die 1950-2020
	//MakeNOP(0x8C4CBB, 40);

	//MakeCALL(0x814291, idkman);
	//MakeCALL(0x744925, snprintfHook);

	MH_Initialize();
	//MH_CreateHook((void*)0x8C4D80, CreateWindowHk, (void**)&g_createWindowOrig);
	//MH_CreateHook(&DefWindowProcW, wndProxy, (void**)&g_origWndProc);
	//MH_CreateHook((void*)0xA36B30, LoadDatafile, (void**)&g_loadDataFileOrig);
	//MH_CreateHook((void*)0x727DD0, ProcessDatafile, (void**)&g_processDataFileOrig);
	MH_EnableHook(MH_ALL_HOOKS);

	Call<void>(0x7BC490, __); // LoadIniConfig
	MemWrite(0xD63FCC, 0); // IsRetail
	MemWrite(0xDDCB21, 1); // enable_quickie_debug_menu
	MemWrite(0xDDCB1E, 1); // enable_dev_only_debug_tiwwchnt
	MemWrite(0xDDCB29, 1); // enable_debug_jump_menu

#ifdef WINDOWED
	MemWrite(0xDDCADD, 1); // OverrideRenderSettings
	MemWrite(0xDDCADE, 0); // RenderFullScreen
#endif

	//MakeCALL(0x728246, WeirdAssert);
	//MakeCALL(0x7282F7, WeirdAssert);

	//MakeJMP(0xA36B30, LoadDatafile, 3);
	
	//MakeCALL(0x743B26, LoadDatafile);

	//MakeNOP(0x7AD107, 2);
	MemWrite(0x7AD107, 0xEB);

	//MemWrite(0xCA4500, "CL: 3221 (pc) (Regression) UK []")

	/*
	MemWrite(0xDDCB28, 1);
	MemWrite(0xDDCAD4, 1); // enable_dev_features
	MemWrite(0xDDC96F, 1); // limited_debug_menu
	MemWrite(0xDDCB1F, 1); // enable_one_button_debug_menu
	*/

	/*
	MakeCALL(0x7448E8, CatchThis);
	*/
	//MakeCALL(0x743AB2, CatchThis2);
	//MakeCALL(0x83EAD3, OnLevelLoad);

	CopyStr(0xCEA790, "User: %d change login status! P2P will be shutdown!\n");

	//LoadDatafile("mods/data/safehouse.txt", 0, (BYTE*)1, 1);

	/*
	BYTE* v4 = LoadDataFile("mods\data\safehouse.txt", 21, 0, 129, 4, 1);
	char* v10[24];
	printf("[KGDR2] Attempting mod process\n");
	ProcessDataFile(v4, v10, 0, 0, "mods\data\safehouse.txt");
	Call(0xA20FC0, v4);
	*/
}

BOOL WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID lpvReserved)
{
	if (nReason == DLL_PROCESS_ATTACH)
	{
		MakeCALL(0x7AD13C, ApplyDebugPatches);
		//MakeCALL(0x48F392, OnRender);
		attach_console();
	}
	return TRUE;
}
