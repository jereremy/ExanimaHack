#ifndef GD_WEP
#define GD_WEP

enum WeaponEffect
{
	E_Electric = 0x4E9BD8
};

enum WeaponEffectEnd
{
	F_Electric = 0x4E9AA4
};


class C_Weapon
{
public:
	std::string GetEffectName()
	{
		switch ((int)GetEffect())
		{
		case WeaponEffect::E_Electric: return "Electric";
		}
		return "UNKNOWN";
	}

	uintptr_t& GetWeaponType()// 4 = fist, 0 = any weapon
	{
		return *(uintptr_t*)(uintptr_t(this) + 0x55C);
	}
	uintptr_t& GetUnknown1()//something??? the game only compares it with 0xC
	{
		return *(uintptr_t*)(uintptr_t(this) + 0x558);
	}
	uintptr_t& GetUnknown2()
	{
		return *(uintptr_t*)(uintptr_t(this) + 0x10);
	}
	uintptr_t& GetEffect() // pointer to a function (!WE CAN OVERRIDE THIS)
	{
		return *(uintptr_t*)(uintptr_t(this) + 0x5e4);
	}

	uintptr_t& GetEffectEnd() // pointer to a function (!WE CAN OVERRIDE THIS)
	{
		return *(uintptr_t*)(uintptr_t(this) + 0x5e8);
	}

	std::string DebugString()
	{
		return std::format( "		GetWeaponType = {}\n"
							"		GetUnknown1 = {}\n"
							"		GetUnknown2 = {}\n"
							"		GetEffect = {} {}\n"
							"		GetUnkown3 = {}",
							GetWeaponType(),
							GetUnknown1(),
							GetUnknown2(),
							GetEffect(),
							GetEffectName(),
							GetEffectEnd());
	}
};

class C_WeaponHitInfo
{
public:

	uintptr_t& GetUnknownFlags1()
	{
		return *(uintptr_t*)(uintptr_t(this) + 0x0);
	}

	float& GetUnknown2()
	{
		return *(float*)(uintptr_t(this) + 0x10);
	}

	float& GetUnknown3()
	{
		return *(float*)(uintptr_t(this) + 0x14);
	}

	float& GetUnknown4()
	{
		return *(float*)(uintptr_t(this) + 0x20);
	}

	float& GetUnknown5()
	{
		return *(float*)(uintptr_t(this) + 0x28);
	}

	float& GetWeight() // higher weight = more dmg
	{
		return *(float*)(uintptr_t(this) + 0x54);
	}

	Vector3& GetUnknownVec7()
	{
		return *(Vector3*)(uintptr_t(this) + 0x2C);
	}

	uintptr_t& GetUnknown8()
	{
		return *(uintptr_t*)(uintptr_t(this) + 0x4);
	}


	std::string DebugString()
	{
		return std::format( "		GetUnknownFlags1 = {} {}\n"
							"		GetUnknown2 = {}\n"
							"		GetUnknown3 = {}\n"
							"		GetUnknown4 = {}\n"
							"		GetUnknown5 = {}\n"
							"		GetWeight = {}\n"
							"		GetUnknown7 = {}\n"
							"		GetUnknown8 = {}",
							GetUnknownFlags1(),
							U_MEM::ToBitString(GetUnknownFlags1()),
							GetUnknown2(),
							GetUnknown3(),
							GetUnknown4(),
							GetUnknown5(),
							GetWeight(),
							GetUnknownVec7().ToString(),
							GetUnknown8());
	}
};

#endif