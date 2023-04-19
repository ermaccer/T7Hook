#pragma once
#include "UObject.h"
#include "FVector.h"
#include "FRotator.h"
#include "USceneComponent.h"
#include "..\utils\core.h"

class AActor : public UObject {
public:
	char pad[352];
	USceneComponent* RootComponent;

	void GetActorScale3D(FVector* Out);
	void SetActorScale3D(FVector* NewScale);

	void GetActorLocation(FVector* result);
	void GetActorRotation(FRotator* result);

	void GetActorEyesViewPoint(FVector* OutLocation, FRotator* OutRotation);
};

VALIDATE_OFFSET(AActor, RootComponent, 0x160);