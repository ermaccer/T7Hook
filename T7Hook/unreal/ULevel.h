#pragma once
#include "TArray.h"
#include "UObject.h"
#include "UWorld.h"
#include "AActor.h"

class UWorld;

class ULevel {
public:
	char pad[160];
	TArray<AActor*> ActorsForGC;
	UWorld* OwningWorld;
	UObject* Model;
};