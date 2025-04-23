#ifndef G_HOK
#define G_HOK GAMEHOOKS
namespace G_HOK
{
	struct Hook_t
	{
		PBYTE originalFunction;
		PBYTE newFunction;
	};

	extern std::vector<Hook_t> hookStack;
	extern void Init();
	extern void UnInit();

	extern int Hook(PBYTE address, PBYTE newFunction); // returns hook index
	extern void UnHook(int hookIndex);
#define HOOK(address, newFunction) Hook((PBYTE)##address , (PBYTE)##newFunction )

	extern void HookWeaponHitEvent();
}
#endif






























/*using WepHandlerFn = void(__thiscall*)(int, int, int);
WepHandlerFn originalWepHandler = nullptr;
/*void __fastcall WepHandler(void* ECX, void* EDX)
{
	//U_COM::PushLog("wep calc hooked?", 5.f);
	//C_Player* player = (C_Player*)EAX;
	originalWepHandler(ECX, EDX);
}

void __stdcall CustomWepHandler(int ECX, int ESI, int EBX)
{
	U_COM::PushLog("wep calc hooked?", 5.f);
	originalWepHandler(ECX, ESI, EBX);
}

__declspec(naked) void WepHandler(int param1, int param2, int param3)
{
	__asm {

		push ebx
		mov ebx, eax
		push esi
		mov esi, edx
		push ecx

		//push dword ptr[param1]
		call CustomWepHandler

		retn 0Ch
		//ret
	}
}
*/

/*using WepHitInfoFn = int(__cdecl*)(void*);
WepHitInfoFn originalGetWepHitInfo = nullptr;
int __cdecl GetWeaponHitInfo(void* EAX)
{
	static BYTE* weaponCalcRetAddr = U_MEM::Scan("89 45 F4 83 7D ?? ?? 75 26 8B 1D ?? ?? ?? ??");

	if (_ReturnAddress() == weaponCalcRetAddr)
	{
		//U_COM::PushLog(std::format("weaponCalcRetAddr = {}", (uintptr_t)weaponCalcRetAddr), 5.f);
		uintptr_t weaponHitInfo = (uintptr_t)EAX;
		if (!weaponHitInfo)
		{
			//U_COM::PushLog("no hit info", 5.f);
			return originalGetWepHitInfo(EAX);
		}
		C_Player* localPlayer = GLO_H::GetLocalPlayer();
		if (!localPlayer)
		{
			//U_COM::PushLog("no local player", 5.f);
			return originalGetWepHitInfo(EAX);
		}

		C_WeaponHitInfo* localHitInfo = GLO_H::GetLocalPlayer()->GetWeaponHitInfo(true);

		if (weaponHitInfo == (uintptr_t)localHitInfo)
		{
			//U_COM::PushLog("found localplayer attack", 5.f);
		}
		else
		{
			//U_COM::PushLog("not localplayer attack?", 5.f);        its always this idk
		}
	}

	return originalGetWepHitInfo(EAX);

}

void HookFunctions()
{
	PBYTE weaponDamageCalculationsAddr = U_MEM::Scan("55 8B EC 81 C4 ?? ?? ?? ?? 53 56 57 89 ?? ?? ?? ?? 55 68 ?? ?? ?? ?? 64 FF 37");
	U_COM::PushLog(std::format("weaponDamageCalculationsAddr = {}", (uintptr_t)weaponDamageCalculationsAddr), 5.f);
	if (weaponDamageCalculationsAddr)
		originalWepCalc = reinterpret_cast<WepCalcFn>(DetourFunction(weaponDamageCalculationsAddr, (PBYTE)WepCalc));

	//PBYTE weaponHandlerAddr = U_MEM::Scan("53 56 51 8B F2 8B D8 83 BB ?? ?? ?? ?? ?? 76 0F");
	//U_COM::PushLog(std::format("weaponHandlerAddr = {}", (uintptr_t)weaponHandlerAddr), 5.f);
	//if (weaponHandlerAddr)
	//	originalWepHandler = reinterpret_cast<WepHandlerFn>(DetourFunction(weaponHandlerAddr, (PBYTE)WepHandler));


	//PBYTE getWepHitInfoAddr = U_MEM::Scan("8B 90 ?? ?? ?? ?? EB 14 8B 80 ?? ?? ?? ?? 3B 80 ?? ?? ?? ?? 74 0A 8B 90");
	//U_COM::PushLog(std::format("weaponDamageCalculationsAddr = {}", (uintptr_t)getWepHitInfoAddr), 5.f);
	//if (getWepHitInfoAddr)
	//	originalGetWepHitInfo = reinterpret_cast<WepHitInfoFn>(DetourFunction(getWepHitInfoAddr, (PBYTE)GetWeaponHitInfo));
}

void UnHookFunctions()
{
	if (originalWepCalc != nullptr)
		DetourRemove((PBYTE)originalWepCalc, (PBYTE)WepCalc);
	//if (originalWepHandler != nullptr)
	//	DetourRemove((PBYTE)originalWepHandler, (PBYTE)WepHandler);
	//if (originalGetWepHitInfo != nullptr)
	//	DetourRemove((PBYTE)originalGetWepHitInfo, (PBYTE)GetWeaponHitInfo);
}*/