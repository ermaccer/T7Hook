#pragma once
#include "..\unreal\AActor.h"
#include "..\unreal\SkeletalMeshComponent.h"

enum EMeshComponentType {
	MCT_HairMesh = 0,
	MCT_UpperMesh = 2,
	MCT_LowerMesh = 3,
	MCT_FaceMesh = 6,
	MCT_FaceHairMesh = 10,


};


class AModularTekkenPlayer : public AActor{
public:
	bool PlayerOnRightSide();

	USkeletalMeshComponent* GetMeshComponent(EMeshComponentType Type);
};