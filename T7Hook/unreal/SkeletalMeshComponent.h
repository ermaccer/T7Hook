#pragma once
#include "FVector.h"
#include "FName.h"
#include "FMatrix.h"
#include "FTransform.h"
#include "FRotator.h"
#include "..\utils\core.h"

enum EBoneSpaces
{
	WorldSpace,
	ComponentSpace,
};

class USkeletalMesh {
public:
	int GetNumBones();
};


class USkeletalMeshComponent {
public:
	char pad[0x6C8];
	USkeletalMesh* SkeletalMesh;

	void GetBoneMatrix(FMatrix* result, int BoneIndex);

	// custom funcs

	void GetBoneLocation(FVector* result, int BoneIndex);
	void GetBoneRotation(FRotator* result, int BoneIndex);
	void GetBoneForward(FVector* result, int BoneIndex);

	FName* GetBoneName(FName* result, int BoneIndex);
	int GetBoneIndexFromString(const wchar_t* BoneName);
};
VALIDATE_OFFSET(USkeletalMeshComponent, SkeletalMesh, 0x6C8);

