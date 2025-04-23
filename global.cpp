#include "global.h"

// dllmain
PUSH_DLL
{
	GLO_H::Init(HMD);
	GLO_H::Loop();
	GLO_H::UnInit();
}
POP_DLL

namespace GLO_H
{
	const int targetFPS = 25;
	constexpr int sleepMS = 1000 / targetFPS;
	constexpr double staticDeltaTime = 1.0 / double(targetFPS);
	double lastTimeStamp = 0.0;
	double realDeltaTime = 0.0;
	uintptr_t PTRlocallayerAddrSig;
	uintptr_t playerCountSig;
	uintptr_t playerListSig;
	HMODULE thisModule = 0;

	void Init(HMODULE moduleHandle) 
	{
		thisModule = moduleHandle;
		U_MEM::Init();
		PTRlocallayerAddrSig = (uintptr_t)U_MEM::Scan("A1 ?? ?? ?? ?? 8B ?? E8 ?? ?? ?? ?? 85 C0 74 1F") + 1;
		playerCountSig = (uintptr_t)U_MEM::Scan("C7 05 ?? ?? ?? ?? 01 00 00 00 A1 ?? ?? ?? ?? 8B") + 2;
		playerListSig = playerCountSig + 30;
		U_COM::Init("Exaniba");
		U_COM::PushLog(std::to_string(playerCountSig), 15.f);
		U_WIN::Init();	
		G_HOK::Init();
	}

	void UnInit()
	{
		G_HOK::UnInit();
		U_COM::UnInit();
		U_WIN::UnInit();
	}

	C_Player* GetLocalPlayer()
	{
		return ***(C_Player****)PTRlocallayerAddrSig;
	}

	int GetPlayerCount()
	{
		return **(int**)playerCountSig;
	}

	C_Player* GetPlayer(int index)
	{
		return (C_Player*)(uintptr_t(*(uintptr_t**)(playerListSig)) + index * 0x4);
	}

	void FUN_00425838(float* param_1, float* param_2, float* param_3, float param_4)
	{
		*param_1 = *param_3 * param_4 + *param_2;
		param_1[1] = param_3[1] * param_4 + param_2[1];
		param_1[2] = param_4 * param_3[2] + param_2[2];
		return;
	}




	void Loop()
	{
		C_Player* localPlayer = nullptr;
		C_WeaponHitInfo* lastHitInfo = nullptr;
		C_WeaponHitInfo* oldHitInfo = nullptr;


		while (GLO_H::PTRlocallayerAddrSig)
		{
			double currentTimeStamp = GLO_H::lastTimeStamp + GLO_H::staticDeltaTime; // not accurate at all but it works

			if (GLO_H::lastTimeStamp != currentTimeStamp)
			{
				GLO_H::realDeltaTime = currentTimeStamp - GLO_H::lastTimeStamp;
				GLO_H::lastTimeStamp = currentTimeStamp;

				localPlayer = GLO_H::GetLocalPlayer();
				if (localPlayer)
				{
					int localPlayerIndex = -1;
					//for (int i = 0; i < 1000; i++)
					//{
					//	if (GetPlayer(i) == localPlayer)
					//	{
					//		localPlayerIndex = i;
					//	}
					//}

					if (GetAsyncKeyState('X'))
					{
						//Vector3 idk = Vector3{ 1.f, -20000000.f,1.f };
						//FUN_00425838((float*)(uintptr_t(localPlayer) + 0x868), (float*)(uintptr_t(localPlayer) + 0x850), (float*)&idk, 1.f);
						//PushPlayer((uintptr_t)localPlayer, 2, Vector3{ 1000.f, 1000.f, 1000.f });
						U_COM::Print("idkidkidkidkidkidkidkidkidkidkidk\n");
					}

					//if (GetAsyncKeyState('F'))
					{
						localPlayer->GetYellowHealth() = .25f;
						localPlayer->GetRedHealth() = .25f;
						localPlayer->GetDamageQueue() = .25f;

						
					}

					if (localPlayer->GetWeaponHitInfo() != lastHitInfo)
					{
						oldHitInfo = lastHitInfo;
						lastHitInfo = localPlayer->GetWeaponHitInfo();
					}

					std::string finalStr = std::format("youalive playercount = {} localindex = {}\n"
						"	GLO_H::lastTimeStamp = {}\n"
						"	yellowHealth = {}\n"
						"	redHealth = {}\n"
						"	GetDamageQueue = {}\n"
						"	GetActiveWeapon = {}\n"
						"{}\n"
						"	GetWeaponHitInfo = {}\n"
						"{}\n"
						"	GetOldWeaponHitInfo = {}\n"
						"{}\n"
						"	GetState = {} {} {}\n"
						"	GetOldState = {} {}\n"
						"	GetUnknown1 = {}\n"
						"	GetUnknownFuncAddr1 = {}\n"
						"	GetUnknownFuncAddr2 = {}\n"
						"	GetAttackForce = {}\n"
						"	GetBlockForce = {}\n"
						"	GetUnknown4 = {}\n"
						"	GetAttackCooldownTimer = {}\n"
						"	GetUnknownFlags6 = {} {}\n"
						"	GetUnknownFlags7 = {} {}\n"
						"	GetUnknownFlags8 = {} {}\n"
						"	GetUnknownFlags9 = {} {}\n"
						"	GetSomeCount = {}\n",
						GetPlayerCount(),
						localPlayerIndex,
						GLO_H::lastTimeStamp,
						localPlayer->GetYellowHealth(),
						localPlayer->GetRedHealth(),
						localPlayer->GetDamageQueue(),
						(uintptr_t)localPlayer->GetActiveWeapon(),
						((localPlayer->GetActiveWeapon() != nullptr) ? localPlayer->GetActiveWeapon()->DebugString() : std::string(" ")),
						(uintptr_t)localPlayer->GetWeaponHitInfo(),
						((localPlayer->GetWeaponHitInfo() != nullptr) ? localPlayer->GetWeaponHitInfo()->DebugString() : std::string(" ")),
						(uintptr_t)oldHitInfo,
						((oldHitInfo != nullptr) ? oldHitInfo->DebugString() : std::string(" ")),
						(uintptr_t)localPlayer->GetState(),
						stateStrings.at(std::clamp((int)localPlayer->GetState(), 0, (int)stateStrings.size())),
						U_MEM::ToBitString(localPlayer->GetState()),
						(uintptr_t)localPlayer->GetOldState(),
						U_MEM::ToBitString(localPlayer->GetOldState()),
						localPlayer->GetUnknown1(),
						localPlayer->GetUnknownFuncAddr1(),
						localPlayer->GetUnknownFuncAddr2(),
						localPlayer->GetAttackForce(),
						localPlayer->GetBlockForce(),
						localPlayer->GetUnknown4(),
						localPlayer->GetAttackCooldownTimer(),
						localPlayer->GetUnknownFlags6(),
						U_MEM::ToBitString(localPlayer->GetUnknownFlags6()),
						localPlayer->GetUnknownFlags7(),
						U_MEM::ToBitString(localPlayer->GetUnknownFlags7()),
						localPlayer->GetUnknownFlags8(),
						U_MEM::ToBitString(localPlayer->GetUnknownFlags8()),
						(uintptr_t)localPlayer->GetUnknownFlags9(),
						U_MEM::ToBitString(localPlayer->GetUnknownFlags9()),
						localPlayer->GetSomeCount());
					U_COM::Print(finalStr);
				}
			}


			U_COM::PopLogs();
			U_COM::FlushConsole();
			U_WIN::FlushKeys();
			Sleep(GLO_H::sleepMS);
			if (GetAsyncKeyState(VK_DELETE)) // unhook
			{
				break;
			}
		}
	}

}


