#include <windows.h>
#include <stdio.h>
#include <experimental/filesystem>
#include <hook\HookingUtils.hpp>
#include <hook\HookFunction.hpp>

#include "MinHook.h"
#pragma comment(lib, "MinHook.lib")

#include <wtsapi32.h>
#pragma comment(lib, "Wtsapi32.lib")

using namespace Util;

char* OutfitTable[1024] = {
	"OUTFIT_DEFAULT",
	"OUTFIT_COWBOY",
	"OUTFIT_NAKED",
	"OUTFIT_MEGAMAN",
	"OUTFIT_MALLPATROL",
	"OUTFIT_TIR",
	"OUTFIT_RYU",
	"OUTFIT_PROTOMAN",
	"OUTFIT_PROXY",
	"OUTFIT_DLC1",
	"OUTFIT_ARTHUR",
	"OUTFIT_BILLYBOB",
	"OUTFIT_SWAT_HELMET",
	"OUTFIT_RUSSIAN_HAT",
	"OUTFIT_SHOW_GIRL_HEAD_PIECE",
	"OUTFIT_FLOWER_HEAD_PIECE",
	"OUTFIT_KNIGHT_HELMET",
	"OUTFIT_LONG_BLOND_HAIR_KEN",
	"OUTFIT_HOCKEY_MASK",
	"OUTFIT_GHOST_MASK",
	"OUTFIT_BALD_NO_HAIR",
	"OUTFIT_MOHAWK_HAIR",
	"OUTFIT_CHUCK_COLOURED_HAIR_PINK",
	"OUTFIT_CHUCK_COLOURED_HAIR_GREY",
	"OUTFIT_CHUCK_COLOURED_HAIR_BLUE",
	"OUTFIT_WILD_FRONTIER_HAT",
	"OUTFIT_FEDORA",
	"OUTFIT_BASEBALL_CAP_TIR",
	"OUTFIT_BASEBALL_CAP_SPORT",
	"OUTFIT_TUQUE",
	"OUTFIT_WHITE_COWBOY_HAT",
	"OUTFIT_BLACK_COWBOY_HAT",
	"OUTFIT_FOOTBALL_HELMET",
	"OUTFIT_DEALER_VISOR",
	"OUTFIT_TOURIST_BOAT_HAT",
	"OUTFIT_TENNIS_HEAD_BAND",
	"OUTFIT_AMERICAN_SHOWMAN_HELMET",
	"OUTFIT_LADIES_HAT",
	"OUTFIT_FUNNY_MASK_ZOMBIE",
	"OUTFIT_FUNNY_MASK_LIZARD",
	"OUTFIT_FUNNY_MASK_SERVBOT",
	"OUTFIT_FUNNY_MASK_BLANKA",
	"OUTFIT_BLACK_RIMMED_GLASSES",
	"OUTFIT_YELLOW_TINTED_GLASSES",
	"OUTFIT_DARK_AVIATOR_GLASSES",
	"OUTFIT_KIDS_SUPER_HERO_EYE_MASK",
	"OUTFIT_NOVELTY_GLASSES",
	"OUTFIT_FULL_BEARD_MOUSTACHE",
	"OUTFIT_SPORT_GLASSES",
	"OUTFIT_LARGE_RHINESTONE_GLASSES",
	"OUTFIT_HYGIENE_MASK",
	"OUTFIT_ARMY_JACKET_PANTS",
	"OUTFIT_YELLOW_TRACK_SUIT",
	"OUTFIT_HIP_HOP_OUTFIT",
	"OUTFIT_FOOTBALL_UNIFORM",
	"OUTFIT_BASEBALL_UNIFORM",
	"OUTFIT_BASKETBALL_UNIFORM",
	"OUTFIT_SWAT_OUTFIT",
	"OUTFIT_BLACK_TUXEDO_BOW_TIE",
	"OUTFIT_SPORTY_TRACK_SUIT",
	"OUTFIT_BLUE_OYSTER_BIKER_OUTFIT",
	"OUTFIT_ONE_PIECE_PAJAMA",
	"OUTFIT_TUBE_TOP_MINI_SKIRT",
	"OUTFIT_FISH_NET_SHIRT_WHITE_PANTS",
	"OUTFIT_COCONUT_BRA_HOLA_DRESS",
	"OUTFIT_KNOTTED_TOP_CUTOFFS",
	"OUTFIT_BANANA_HAMMOCK",
	"OUTFIT_SUMMER_DRESS",
	"OUTFIT_KIDS_SUPER_HERO_COSTUME",
	"OUTFIT_ARTHURS_BOXERS",
	"OUTFIT_KEN_RED_FIGHTING_OUTFIT",
	"OUTFIT_TATTERED_CLOTHES",
	"OUTFIT_ORANGE_PRISON_OUTFIT",
	"OUTFIT_TODDLER_OUTFIT",
	"OUTFIT_HAWAIIAN_SHIRT_1_SHORTS",
	"OUTFIT_HAWAIIAN_SHIRT_2_SHORTS",
	"OUTFIT_HAWAIIAN_SHIRT_3_PANTS",
	"OUTFIT_HAWAIIAN_SHIRT_4_PANTS",
	"OUTFIT_DEALER_OUTFIT",
	"OUTFIT_WHITE_COWBOY_OUTFIT",
	"OUTFIT_BLACK_COWBOY_OUTFIT",
	"OUTFIT_WHITE_LEISURE_SUIT",
	"OUTFIT_AMERICAN_SHOWMAN_OUTFIT",
	"OUTFIT_CARDIGAN_SLACKS",
	"OUTFIT_TENNIS_OUTFIT",
	"OUTFIT_BLUE_GREY_WORK_OVERALLS",
	"OUTFIT_SURF_WETSUIT",
	"OUTFIT_COMEDIAN_OUTFIT",
	"OUTFIT_POLO_SHIRT_BLUE_JEANS",
	"OUTFIT_FORTUNE_TSHIRT_1_BLUE_JEANS",
	"OUTFIT_FORTUNE_TSHIRT_2_BLACK_JEANS",
	"OUTFIT_PLAID_SUIT_BROWN_SHIRT_TIE",
	"OUTFIT_TAN_JACKET_SLACKS_TURTLENECK",
	"OUTFIT_SKATER_OUTFIT",
	"OUTFIT_MID_LENGTH_TSHIRT_JEANS",
	"OUTFIT_BARE_FEET_CHUCK",
	"OUTFIT_YELLOW_SNEAKERS",
	"OUTFIT_BASKETBALL_HIGH_TOPS",
	"OUTFIT_BLACK_CANVAS_SNEAKERS",
	"OUTFIT_WHITE_LOW_HEELS",
	"OUTFIT_HOOKER_BOOTS",
	"OUTFIT_SANDALS_WHITE_SOCKS",
	"OUTFIT_KIDS_SUPER_HERO_BOOTS",
	"OUTFIT_BUNNY_SLIPPERS",
	"OUTFIT_KNIGHT_BOOTS",
	"OUTFIT_BARE_FEET_KEN",
	"OUTFIT_FLIP_FLOPS",
	"OUTFIT_BLACK_MILITARY_BOOTS",
	"OUTFIT_CLEATS",
	"OUTFIT_BLACK_DRESS_SHOES",
	"OUTFIT_WHITE_COWBOY_BOOTS",
	"OUTFIT_BLACK_COWBOY_BOOTS",
	"OUTFIT_WHITE_TENNIS_SHOES_SOCKS",
	"OUTFIT_BARE_FEET_CHUCK_UNDER",
	"OUTFIT_YELLOW_SNEAKERS_UNDER",
	"OUTFIT_BASKETBALL_HIGH_TOPS_UNDER",
	"OUTFIT_BLACK_CANVAS_SNEAKERS_UNDER",
	"OUTFIT_WHITE_LOW_HEELS_UNDER",
	"OUTFIT_HOOKER_BOOTS_UNDER",
	"OUTFIT_SANDALS_WHITE_SOCKS_UNDER",
	"OUTFIT_KIDS_SUPER_HERO_BOOTS_UNDER",
	"OUTFIT_BUNNY_SLIPPERS_UNDER",
	"OUTFIT_KNIGHT_BOOTS_UNDER",
	"OUTFIT_BARE_FEET_KEN_UNDER",
	"OUTFIT_FLIP_FLOPS_UNDER",
	"OUTFIT_BLACK_MILITARY_BOOTS_UNDER",
	"OUTFIT_CLEATS_UNDER",
	"OUTFIT_BLACK_DRESS_SHOES_UNDER",
	"OUTFIT_WHITE_COWBOY_BOOTS_UNDER",
	"OUTFIT_BLACK_COWBOY_BOOTS_UNDER",
	"OUTFIT_WHITE_TENNIS_SHOES_SOCKS_UNDER",
	"OUTFIT_DEFAULT_UNDER",
	"OUTFIT_COWBOY_UNDER",
	"OUTFIT_NAKED_UNDER",
	"OUTFIT_MEGAMAN_UNDER",
	"OUTFIT_MALLPATROL_UNDER",
	"OUTFIT_TIR_UNDER",
	"OUTFIT_RYU_UNDER",
	"OUTFIT_PROTOMAN_UNDER",
	"OUTFIT_ARTHUR_UNDER",
	"OUTFIT_BILLYBOB_UNDER",
	"OUTFIT_KNIGHT_ARMOUR",
	"OUTFIT_TIR_FAN",
	"OUTFIT_SHOCK_COLLAR",
	"OUTFIT_DLC_NINJA",
	"OUTFIT_DLC_SPORTSFAN",
	"OUTFIT_DLC_PSYCHOPATH",
	"OUTFIT_DLC_SOLDIEROFFORTUNE",
	"OUTFIT_DEMO_FIRST_DEFAULT",
	"OUTFIT_SECOND_DEMO_DEFAULT",
	"OUTFIT_COOP_DEFAULT",
	"OUTFIT_TIR_YELLOW",
	"OUTFIT_CHAMPIONS_JACKET",
	"OUTFIT_BOWLING_SHIRT_JEANS",
	"OUTFIT_DINER_WAITRESS_OUTFIT",
	"OUTFIT_GREASY_OVERALL_PLAID_SHIRT",
	"OUTFIT_HUNTING_JACKET_ORANGE_VEST",
	"OUTFIT_CHAMPIONS_JACKET2",
	"OUTFIT_COOP_DEFAULT_UNDER"
};

void Log(char* message, ...)
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

void DebugLog(char* message, ...)
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

double __cdecl OnRender(__int64 a1, __int64 a2)
{
	// 0.94f, 0.59f, 0.24f
	RenderText(0.05f, 0.075f, 1.0f, 1.0f, 1.0f, 1.0f, "KGDR2? :)", 0.0f, 2, 1.0f);

	return Cdecl<double>(0xA3A500, a1, a2);
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

char* LoadDatafile(DWORD* _this, int a3, int a4, int a5, int a6, int a7)
{
	/*
	static bool loadedDataFile = false;

	if (!loadedDataFile)
	{
		//loadedDataFile = true;
		//fileData = ((char* (__thiscall*)(void*, char*, int, int, int, int, int))0xA36B30)(fileLoader__inst, "mods/data/fortune_exterior.txt", 21, 0, 1, 4, 1);
	}
	*/

	Log("Loading datafile %s", "data/datafile/items.txt");

	return ((char* (__thiscall*)(void*, char*, int, int, int, int, int))0xA36B30)(_this, "data/datafile/items.txt", 21, 0, 1, 4, 1);
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

	//Log("Loading datafile %s", a3);

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

bool mmmmmmno()
{
	return false;
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

int __cdecl GetOutfitIdByName(const char* a1)
{
	return Call<int>(0x41FF30, a1);
}

bool bDebugEnabled = GetPrivateProfileIntA("GLOBAL", "debug_enabled", 0, ".\\DR2Hookary.ini") == 1;
bool bEnableQuickieDebugMenu = GetPrivateProfileIntA("GLOBAL", "enable_quickie_debug_menu", 0, ".\\DR2Hookary.ini") == 1;
bool bEnableDebugJumpMenu = GetPrivateProfileIntA("GLOBAL", "enable_debug_jump_menu", 0, ".\\DR2Hookary.ini") == 1;
bool bSkipLogos = GetPrivateProfileIntA("GLOBAL", "skip_logos", 0, ".\\DR2Hookary.ini") == 1;
bool bWindowed = GetPrivateProfileIntA("GLOBAL", "windowed", 0, ".\\DR2Hookary.ini") == 1;
bool bBorderless = GetPrivateProfileIntA("GLOBAL", "borderless", 0, ".\\DR2Hookary.ini") == 1;

//#define BLUE_MENU // it's not fun

void ApplyDebugPatches(int __)
{
	Call<void>(0x7BC490, __); // LoadIniConfig

#ifdef BLUE_MENU
	//MemWrite(0xDDCB28, 1); // debug_show_loading_time
	MemWrite(0xDDCAD4, 1); // enable_dev_features
	MemWrite(0xDDC96F, 1); // limited_debug_menu
	MemWrite(0xDDCB1F, 1); // enable_one_button_debug_menu
#endif

	MemWrite(0xD63FCC, !bDebugEnabled); // IsRetail
	MemWrite(0xDDCAD4, bDebugEnabled); // enable_dev_features
	MemWrite(0xDDCB21, bEnableQuickieDebugMenu); // enable_quickie_debug_menu - needs to be toggled later?
	MemWrite(0xDDCB1E, bEnableQuickieDebugMenu); // enable_dev_only_debug_tiwwchnt
	MemWrite(0xDDCB29, bEnableDebugJumpMenu); // enable_debug_jump_menu
	MemWrite(0xDDCC5B, bSkipLogos); // skip_logos

	if (bWindowed == true)
	{
		MemWrite(0xDDCADD, 1); // OverrideRenderSettings
		MemWrite(0xDDCADE, 0); // RenderFullScreen
	}

	MemWrite(0xDE25F2, 1); // SilentExitOnCrash, fixes quit 'crash'
	MemWrite(0xE5B73C, 1); // dont debug break from inside exe

	/*
	for (int i = 0; i < 192; i++)
		Log("Outfit ID %d = %s", i, OutfitTable[i]);
	*/

	// this works :D
	//Log("OUTFIT_FEDORA = %d", GetOutfitIdByName("OUTFIT_FEDORA"));

	//MakeJMP(0xA36B30, LoadDatafile, 3);
	
	//MakeCALL(0x743B26, LoadDatafile);

	// attempt to load ini files on startup didnt work
	//MakeNOP(0x7AD107, 2);
	//MemWrite(0x7AD107, 0xEB);

					//"CL: 3221 (pc) (Regression) UK []"
	//CopyStr(0xCA4500, "CL: 3221 (ps5) (mom of mia) NO []");
	//CopyStr(0xDDC438, "CL: 3221 (ps5) (mom of mia) UK []");

	/*
	MakeCALL(0x7448E8, CatchThis);
	*/
	//MakeCALL(0x743AB2, CatchThis2);
	//MakeCALL(0x83EAD3, OnLevelLoad);

	// fix a missing newline
	CopyStr(0xCEA790, "User: %d change login status! P2P will be shutdown!\n");

	// put crashes in separate folder
	std::experimental::filesystem::create_directory("crashes");
	CopyStr(0xCC8B64, "crashes\\crash_%m%d_%H%M%S.txt");

	//LoadDatafile("mods/data/safehouse.txt", 0, (BYTE*)1, 1);

	/*
	BYTE* v4 = LoadDataFile("mods\data\safehouse.txt", 21, 0, 129, 4, 1);
	char* v10[24];
	printf("[KGDR2] Attempting mod process\n");
	ProcessDataFile(v4, v10, 0, 0, "mods\data\safehouse.txt");
	Call(0xA20FC0, v4);
	*/
}

HWND __cdecl InitializeGameWindow(LPCWSTR lpWindowName, int xRight, int yBottom, DWORD dwStyle)
{
	if (bBorderless)
		dwStyle = WS_VISIBLE | WS_POPUP;

	return Cdecl<HWND>(0x8C4D80, lpWindowName, xRight, yBottom, dwStyle);
}

BOOL WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID lpvReserved)
{
	if (nReason == DLL_PROCESS_ATTACH)
	{
		MakeCALL(0x7AD13C, ApplyDebugPatches);

		MakeCALL(0x8CA6A1, InitializeGameWindow);
		//MakeCALL(0x8CA6A1, CreateWindowHk);
		//MakeJMP(0x8C4D80, CreateWindowHk);
		//Call<int>(, CreateWindowHk);

		MakeCALL(0xA532D0, mmmmmmno); // no more XLiveRender it die 1950-2020
		//MakeNOP(0x8C4CBB, 40);

		MakeJMP(0xA3A0E0, DebugLog);

		MakeCALL(0x728246, QuietAssert);
		MakeCALL(0x7282F7, QuietAssert);

		//MakeCALL(0x743B26, LoadDatafile);

		//MakeCALL(0x814291, idkman);
		//MakeCALL(0x744925, snprintfHook);

		MH_Initialize();
		//MH_CreateHook((void*)0x8C4D80, CreateWindowHk, (void**)&g_createWindowOrig);
		//MH_CreateHook(&DefWindowProcW, wndProxy, (void**)&g_origWndProc);
		//MH_CreateHook((void*)0xA36B30, LoadDatafile, (void**)&g_loadDataFileOrig);
		//MH_CreateHook((void*)0x727DD0, ProcessDatafile, (void**)&g_processDataFileOrig);
		MH_EnableHook(MH_ALL_HOOKS);

		MakeCALL(0x7B3002, OnRender);
		attach_console();
	}
	return TRUE;
}
