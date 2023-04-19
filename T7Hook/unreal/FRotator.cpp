#include "FRotator.h"
#include "..\utils\addr.h"
#include "..\utils\Patterns.h"

FRotator::FRotator()
{
	Pitch = 0;
	Yaw = 0;
	Roll = 0;
}

FRotator::FRotator(FQuat* Quat)
{
	static void* pat = hook::txn::get_pattern("40 53 48 83 EC 30 48 8B C2 48 8B D9 48 8B C8 48 8D 54 24 20 E8 ? ? ? ? F2 0F");
	((void(__thiscall*)(FRotator*, FQuat*))pat)(this, Quat);
}

void FRotator::FromQuat(FQuat* Quat)
{
	static void* pat = hook::txn::get_pattern("40 53 48 83 EC 30 48 8B C2 48 8B D9 48 8B C8 48 8D 54 24 20 E8 ? ? ? ? F2 0F");
	((void(__thiscall*)(FRotator*, FQuat*))pat)(this, Quat);
}

void FRotator::Quaternion(FQuat* result)
{
	static void* pat = hook::txn::get_pattern("48 83 EC 18 F3 0F 10 41 08 0F 57 C9");
	((void(__fastcall*)(FRotator*, FQuat*))pat)(this, result);
}
