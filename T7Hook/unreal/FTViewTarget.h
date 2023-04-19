#pragma once
#include "FRotator.h"
#include "FVector.h"
#include "AActor.h"
#include "UObject.h"

#include "..\utils\core.h"

struct FMinimalViewInfo
{
	char pad[4];
	FVector Location;
	FRotator Rotation;
	float FOV;
	// TODO: check all this?
	float DesiredFOV;
	float OrthoWidth;
	float OrthoNearClipPlane;
	float AspectRatio;
	unsigned __int32 bConstrainAspectRatio : 1;
	unsigned __int32 bUseFieldOfViewForLOD : 1;


	// FTViewTarget size in T7 is 400, so minimalview info is 3F0
	char _pad[956];
};
VALIDATE_SIZE(FMinimalViewInfo, 1008);


class FTViewTarget {
public:
	AActor* Target;
	FMinimalViewInfo POV;
	UObject* PlayerState;

};
VALIDATE_SIZE(FTViewTarget, 1024);