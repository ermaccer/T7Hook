#include "SkeletalMeshComponent.h"
#include "FName.h"

#include "..\utils\addr.h"
#include "..\utils\Patterns.h"


void USkeletalMeshComponent::GetBoneMatrix(FMatrix* result, int BoneIndex)
{
	static void* pat = hook::txn::get_pattern("48 89 5C 24 10 48 89 74 24 18 57 48 81 EC 80 00 00 00 F6 81 A0 00 00 00 01");

	((void(__thiscall*)(USkeletalMeshComponent*, FMatrix*, int))pat)(this, result, BoneIndex);
}

void USkeletalMeshComponent::GetBoneLocation(FVector* result, int BoneIndex)
{
	FMatrix mat;
	GetBoneMatrix(&mat, BoneIndex);
	mat.GetPosition(result);
}

void USkeletalMeshComponent::GetBoneRotation(FRotator* result, int BoneIndex)
{
	FMatrix mat;
	GetBoneMatrix(&mat, BoneIndex);
	mat.Rotator(result);
}

void USkeletalMeshComponent::GetBoneForward(FVector* result, int BoneIndex)
{
	FMatrix mat;
	GetBoneMatrix(&mat, BoneIndex);
	mat.GetForward(result);
}

FName* USkeletalMeshComponent::GetBoneName(FName* result, int BoneIndex)
{
	static void* pat = hook::txn::get_pattern("48 8B 81 C8 06 00 00 48 85 C0 74 ? 45 85 C0 78 ?");
	
	return ((FName*(__thiscall*)(USkeletalMeshComponent*, FName*, int))pat)(this, result, BoneIndex);
}

int USkeletalMeshComponent::GetBoneIndexFromString(const wchar_t* BoneName)
{
	static FString boneName;

	for (int i = 0; i < SkeletalMesh->GetNumBones(); i++)
	{
		FName name;
		GetBoneName(&name, i);

		if (name.Index > 0)
		{
			name.ToString(&boneName);
			if (wcscmp(boneName.GetStr(), BoneName) == 0)
			{
				return i;
			}
		}
	}

	return -1;
}

int USkeletalMesh::GetNumBones()
{
	return *(int*)(this + 0x120);
}
