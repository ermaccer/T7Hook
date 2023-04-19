#pragma once
#include "FQuat.h"

class FRotator {
public:
	float Pitch;
	float Yaw;
	float Roll;

	FRotator();
	FRotator(FQuat* Quat);
	void FromQuat(FQuat* Quat);
	void Quaternion(FQuat* result);
};

struct FRotatorCompressed
{
	short Pitch;
	short Yaw;
	short Roll;
};
