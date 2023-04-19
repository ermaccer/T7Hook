#include "AActor.h"
#include "..\utils\addr.h"
#include "..\utils\Patterns.h"

void AActor::GetActorScale3D(FVector* Out)
{
	static void* pat = hook::txn::get_pattern("48 8B 81 60 01 00 00 48 85 C0 74 ? 0F 28 88 50 01 00 00");
	((void(__thiscall*)(AActor*, FVector*))pat)(this, Out);
}

void AActor::SetActorScale3D(FVector* NewScale)
{
	static void* pat = hook::txn::get_pattern("48 83 EC 38 48 8B 89 60 01 00 00 48 85 C9 74 ? F2 0F 10 02");
	((void(__thiscall*)(AActor*, FVector*))pat)(this, NewScale);
}

void AActor::GetActorLocation(FVector* result)
{
	static void* pat = hook::txn::get_pattern("48 83 EC 18 48 8B 81 60 01 00 00 48 85 C0");
	((void(__thiscall*)(AActor*, FVector*))pat)(this, result);
}

void AActor::GetActorRotation(FRotator* result)
{
	static void* pat = hook::txn::get_pattern("48 89 5C 24 08 57 48 83 EC 40 48 8B 99 60 01 00 00");
	((void(__thiscall*)(AActor*, FRotator*))pat)(this, result);
}

void AActor::GetActorEyesViewPoint(FVector* OutLocation, FRotator* OutRotation)
{
	static void* pat = hook::txn::get_pattern("48 89 5C 24 08 57 48 83 EC 40 48 8B 81 60 01 00 00");
	((void(__thiscall*)(AActor*, FVector*, FRotator*))pat)(this, OutLocation, OutRotation);
}
