#include "AModularTekkenPlayer.h"
#include "..\utils\addr.h"
#include "..\utils\Patterns.h"

bool AModularTekkenPlayer::PlayerOnRightSide()
{
	static void* pat = hook::txn::get_pattern("48 83 EC 28 8B 89 E0 03 00 00 E8 ? ? ? ? 83 B8");

    return 	((bool(__thiscall*)(AModularTekkenPlayer*))pat)(this);
}

USkeletalMeshComponent* AModularTekkenPlayer::GetMeshComponent(EMeshComponentType Type)
{
	static void* pat = hook::txn::get_pattern("40 53 48 83 EC 20 48 8B D9 83 FA 06");

	return 	((USkeletalMeshComponent * (__thiscall*)(AModularTekkenPlayer*, EMeshComponentType))pat)(this, Type);
}
