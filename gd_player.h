#ifndef GD_PLY
#define GD_PLY
enum State
{
	Moving = 0x0,
	CombatStance = 0x3, // can be moving if using wasd, when you enter combat mode this is the stance before attackstance/before you actually attack
	AttackStance = 0x4, // can be moving if using wasd, the stance your in after attacking // you stay in this stance even if you leave combat mode until you move 
	RightAttack1 = 0x5,
	RightAttack2 = 0x6,
	RightAttack3 = 0x7,
	LeftAttack1 = 0x8,
	LeftAttack2 = 0x9,
	LeftAttack3 = 0xA,
	AlternativeAttack = 0xB, // stab
	HeavyAttack = 0xC,
	GettingUpFromStomach = 0xE,
	GettingUpFromBack = 0xF,
	FallingOver1 = 0x10, // not sure
	FallingOver2 = 0x11, // not sure
	Resting = 0x13, // not moving or in combat
	Block1 = 0x15,// blocking side (idk what side)
	Block2 = 0x16,// blocking side (idk what side)
	Block3 = 0x17,// blocking side (idk what side)
	Block4 = 0x18,// blocking side (idk what side)
	BalancingMaybe = 0x1D, // idk
	SwapingWeapons = 0x1F,
	BalancingOrFallenOver = 0x27
};

#define STRIKE_POWER_PTR 0x5A029A00
static const std::vector<std::string>stateStrings =
{
	"Moving",
	"UNK1","UNK2",
	"CombatStance", // can be moving if using wasd, when you enter combat mode this is the stance before attackstance/before you actually attack
	"AttackStance", // can be moving if using wasd, the stance your in after attacking // you stay in this stance even if you leave combat mode until you move 
	"RightAttack1",
	"RightAttack2",
	"RightAttack3",
	"LeftAttack1",
	"LeftAttack2",
	"LeftAttack3",
	"AlternativeAttack",
	"HeavyAttack",
	"UNK13",
	"GettingUpFromStomach",
	"GettingUpFromBack",
	"FallingOver1", // not sure
	"FallingOver2", // not sure
	"UNK18",
	"Resting", // not moving or in combat
	"UNK20",
	"Block1",// blocking side (idk what side)
	"Block2",// blocking side (idk what side)
	"Block3",// blocking side (idk what side)
	"Block4",// blocking side (idk what side)
	"UNK25","UNK26","UNK27","UNK28",
	"BalancingMaybe",//idk
	"UNK30",
	"SwapingWeapons",
	"Magic1","Magic2","UNK34","UNK35","UNK36","UNK37","UNK38",
	"BalancingOrFallenOver"
};
struct HitboxInfo_t
{
	Vector3 position;
	float modifier;
};

class C_Player;
class C_Player
{
public:
	float& GetYellowHealth()// 0.0 - 0.25
	{
		return *(float*)(uintptr_t(this) + 0xB30);
	}
	float& GetRedHealth()// 0.0 - 0.25
	{
		return *(float*)(uintptr_t(this) + 0xB34);
	}
	float& GetDamageQueue()
	{
		return *(float*)(uintptr_t(this) + 0xDE0);
	}
	C_Weapon* GetActiveWeapon()
	{
		return *(C_Weapon**)(uintptr_t(this) + 0x7DC);
	}
	C_WeaponHitInfo* GetWeaponHitInfo(bool prev = false) //return (BYTE*)FUN_005673e0(*(uintptr_t*)(uintptr_t(this) + 0xA08));
	{
		uintptr_t this_ptr = *(uintptr_t*)(uintptr_t(this) + 0xA08);
		if (!this_ptr)
			return nullptr;
		if (prev)
			return (C_WeaponHitInfo*)this_ptr;

		int var1;
		var1 = *(int*)(this_ptr + 0x5B4);
		while ((var1 == 0 && (this_ptr = *(int*)(this_ptr + 0x34C), this_ptr != *(int*)(this_ptr + 0x34C)))
			) {
			var1 = *(int*)(this_ptr + 0x5B4);
		}

		if (!var1)
			return nullptr;

		return (C_WeaponHitInfo*)var1;
	}

	int& GetSomeCount() // if you swing with a weapon then it = 1 but if you swing with fist it = 2 else 0
	{
		return *(int*)(uintptr_t(this) + 0x7D0);
	}

	State& GetState() // these are not flags
	{
		return *(State*)(uintptr_t(this) + 0x7FC);
	}
	State& GetOldState() // these are not flags
	{
		return *(State*)(uintptr_t(this) + 0x804);
	}
	int& GetUnknown1()
	{
		return *(int*)(uintptr_t(this) + 0xC88);
	}
	uintptr_t& GetUnknownFuncAddr1()
	{
		return *(uintptr_t*)(uintptr_t(this) + 0xFA4);
	}
	uintptr_t& GetUnknownFuncAddr2()
	{
		return *(uintptr_t*)(uintptr_t(this) + 0xFAC);
	}
	float& GetAttackForce() // how hard your attacks hit (can be any number)
	{
		return *(float*)(uintptr_t(this) + 0xFB4);
	}
	float& GetBlockForce() // 0 - 1 (1 means you take no GetAttackForce and no damage from blunt objects)
	{
		return *(float*)(uintptr_t(this) + 0xFB0);
	}
	int& GetUnknown4()
	{
		return *(int*)(uintptr_t(this) + 0x5A4);
	}
	float& GetAttackCooldownTimer() // when you finish attacking this starts counting up from 0 until it reaches 1 where it then resets to 0 until you attack again
	{
		return *(float*)(uintptr_t(this) + 0x7EC);
	}
	uintptr_t& GetUnknownFlags6()
	{
		return *(uintptr_t*)(uintptr_t(this) + 0xFBC);
	}
	uintptr_t& GetUnknownFlags7()
	{
		return *(uintptr_t*)(uintptr_t(this) + 0xA0C);
	}
	uintptr_t& GetUnknownFlags8()
	{
		return *(uintptr_t*)(uintptr_t(this) + 0xBC4);
	}
	BYTE& GetUnknownFlags9()
	{
		return *(BYTE*)(uintptr_t(this) + 0xE40);
	}
	C_Player* GetHitTarget()
	{
		uintptr_t this_ptr = *(uintptr_t*)(uintptr_t(this) + 0x9f8);
		if (!this_ptr)
			return nullptr;
		this_ptr = *(uintptr_t*)(this_ptr + 0x348);
		if (!this_ptr)
			return nullptr;
		return (C_Player*)this_ptr;
	}
	void SetHitTarget(uintptr_t ptr)
	{
		uintptr_t this_ptr = *(uintptr_t*)(uintptr_t(this) + 0x9f8);
		if (!this_ptr)
			return;
		*(uintptr_t*)(this_ptr + 0x348) = ptr;
	}
};
#endif