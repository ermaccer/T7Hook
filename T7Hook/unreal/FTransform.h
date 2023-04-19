#pragma once
#include "FVector.h"
#include "FQuat.h"

class FTransform {
public:
	FQuat Rotation;
	FQuat Transform;
	FQuat Scale3D;
};