#ifndef GLO_H
#define GLO_H GLOBAL

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "windowsx.h"
#include "Psapi.h"
#pragma comment(lib, "Psapi.lib")
#include <iostream>
#include <chrono>
#include <string>
#include <cstring>
#include <cstdlib> 
#include <cstdio>
#include <string_view>
#include <algorithm>
#include <bitset>
#include "detours.hpp"
#include "intrin.h"
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;

#define HMD thisModule
#define MOD HMODULE
#define HMOD MOD HMD
#define PUSH_DLL int z(HMOD){
#define POP_DLL Sleep(0x3E8);MOD m{HMD};FreeLibraryAndExitThread(m,0);}int WINAPI DllMain(HMOD,int r,int* l){if(!(r-1)){DisableThreadLibraryCalls(HMD);CreateThread(0,0,(LPTHREAD_START_ROUTINE)z,HMD,0,0);}return TRUE;}
#define DLLMAIN(CODE) PUSH_DLL{ CODE }POP_DLL

#include "utils.h"
#include "game_data.h"
#include "game_hooks.h"

namespace GLO_H
{
	extern const int targetFPS;
	extern const int sleepMS;
	extern const double staticDeltaTime;
	extern double lastTimeStamp;
	extern double realDeltaTime;

	extern C_Player* GetLocalPlayer();
	extern void Init(HMODULE moduleHandle);
	extern void UnInit();
	extern void Loop();
}

#endif




