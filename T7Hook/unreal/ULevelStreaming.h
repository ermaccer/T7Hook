#pragma once
#include "ULevel.h"

class ULevelStreaming {
public:
	char pad[312];
	ULevel* LoadedLevel;
	ULevel* PendingUnloadLevel;
};