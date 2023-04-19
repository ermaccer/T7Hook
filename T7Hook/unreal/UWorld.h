#pragma once
#include "TArray.h"
#include "ULevelStreaming.h"

class UWorld {
public:
	char pad[48];
	ULevel* PersistentLevel;
	char _pad[88];
	TArray<ULevelStreaming*> StreamingLevels;
};


UWorld* GetWorld();