#include "global.h"

namespace G_HOK
{ 
	int hookIndex = 0;
	
	using WepCalcFn = void(__cdecl*)();
	WepCalcFn originalWepHitEvent = nullptr;
	void __stdcall CustWepHitEvent(int EAX)
	{
		C_Player* player = (C_Player*)EAX;

		if (player)
		{
			if (player == GLO_H::GetLocalPlayer())
			{
				U_COM::PushLog("local player attack!", 2.5f);
				C_WeaponHitInfo* hitInfo = player->GetWeaponHitInfo();
				if (hitInfo && player->GetActiveWeapon())
				{
					//hitInfo->GetUnknown2() = 100.f;
					//hitInfo->GetUnknown3() = 100.f;
					//hitInfo->GetUnknown4() = 100.f;
					//hitInfo->GetHitVolume() = 1.f;
					//player->GetActiveWeapon()->GetEffect() = WeaponEffect::E_Electric;

					player->GetHitTarget()->GetBlockForce() = 0.f;
					player->GetAttackForce() = 25.f;
					//player->GetUnknownFuncAddr1() = STRIKE_POWER_PTR;
					//player->GetUnknownFuncAddr2() = STRIKE_POWER_PTR;
					//player->GetActiveWeapon()->GetEffectEnd() = WeaponEffectEnd::F_Electric;
					//hitInfo->GetWeight() = 500.f;
					//hitInfo->GetUnknownVec7() = Vector3{ 999999.f, 999999.f, 999999.f };
				}
			}
			else if (player->GetHitTarget() == GLO_H::GetLocalPlayer())
			{
				U_COM::PushLog("attack on local player!", 2.5f);
				player->GetHitTarget()->GetBlockForce() = 1.f;
			//	player->SetHitTarget(0);
			}
		}
	}

	__declspec(naked) void WepCalc()
	{
		__asm
		{
			pushad
			pushfd

			push eax
			call CustWepHitEvent

			popfd
			popad

			jmp[originalWepHitEvent]
		}
	}

	void HookWeaponHitEvent()
	{
		hookIndex = HOOK(U_MEM::Scan("55 8B EC 81 C4 ?? ?? ?? ?? 53 56 57 89 ?? ?? ?? ?? 55 68 ?? ?? ?? ?? 64 FF 37"), WepCalc);
		originalWepHitEvent = (WepCalcFn)hookStack[hookIndex].originalFunction;
	}
}
