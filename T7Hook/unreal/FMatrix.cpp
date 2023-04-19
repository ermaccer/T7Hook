#include "FMatrix.h"
#include "..\utils\Patterns.h"
#include "..\utils\addr.h"

FMatrix::FMatrix()
{
	memset(M, 0, sizeof(M));
}

void FMatrix::GetForward(FVector* result)
{
	result->X = M[0][0];
	result->Y = M[0][1];
	result->Z = M[0][2];
}

void FMatrix::GetRight(FVector* result)
{
	result->X = M[1][0];
	result->Y = M[1][1];
	result->Z = M[1][2];
}

void FMatrix::GetUp(FVector* result)
{
	result->X = M[2][0];
	result->Y = M[2][1];
	result->Z = M[2][2];
}

void FMatrix::GetPosition(FVector* result)
{
	result->X = M[3][0];
	result->Y = M[3][1];
	result->Z = M[3][2];
}

void FMatrix::Rotator(FRotator* result)
{
	static void* pat = hook::txn::get_pattern("48 8B C4 53 48 81 EC 00 01 00 00 0F 29 70 E8");
	((void(__thiscall*)(FMatrix*, FRotator*))pat)(this, result);
}

void FMatrix::FromQuat(FQuat* Quat)
{
	static void* pat = hook::txn::get_pattern("48 83 EC 68 F3 0F 10 52 0C 33 C0");
	((void(__thiscall*)(FMatrix*, FQuat*))pat)(this, Quat);
}

void FMatrix::MakeFromX(FVector* XAxis)
{
	static void* pat = hook::txn::get_pattern("48 8B C4 55 48 8D 68 A1 48 81 EC B0 00 00 00 F3 0F 10 1D ? ? ? ? F3 0F 10 25 ? ? ? ? 0F 29 70 E8");
	((void(__thiscall*)(FMatrix*, FVector*))pat)(this, XAxis);
}
