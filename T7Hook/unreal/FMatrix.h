#pragma once
#include "FVector.h"
#include "FRotator.h"

class FMatrix {
public:
	float M[4][4];	

	FMatrix();

	void GetForward(FVector* result);
	void GetRight(FVector* result);
	void GetUp(FVector* result);
	void GetPosition(FVector* result);
	void Rotator(FRotator* result);

	void FromQuat(FQuat* Quat);
	void MakeFromX(FVector* XAxis);
};