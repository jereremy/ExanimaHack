#include "global.h"

namespace G_HOK
{
	std::vector<Hook_t> hookStack = std::vector<Hook_t>{};

	void Init()
	{
		HookWeaponHitEvent();
	}

	void UnInit()
	{
		for (int i = 0; i < hookStack.size(); i++)
		{
			if (hookStack[i].originalFunction && hookStack[i].newFunction)
			{
				DetourRemove(hookStack[i].originalFunction, hookStack[i].newFunction);
			}
		}
		hookStack.clear();
	}

	int Hook(PBYTE address, PBYTE newFunction) // returns hook index
	{
		if (PBYTE original = DetourFunction(address, newFunction))
		{
			hookStack.push_back({ original, newFunction });
			return hookStack.size() - 1;
		}
		U_COM::PushError("couldnt detour func", 15.f);
		return -1;
	}

	void UnHook(int hookIndex)
	{
		if (hookStack[hookIndex].originalFunction && hookStack[hookIndex].newFunction)
		{
			DetourRemove(hookStack[hookIndex].originalFunction, hookStack[hookIndex].newFunction);
			hookStack.erase(hookStack.begin() + hookIndex);
		}
	}
}