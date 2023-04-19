#pragma once
#include "AModularTekkenPlayer.h"
#include "..\utils\core.h"
#include "..\utils\addr.h"

struct ACharacterLoadInfo {
public:
	int64 field0; // costume?
	AModularTekkenPlayer* pTekkenPlayer;
	char pad[80];
};
VALIDATE_SIZE(ACharacterLoadInfo, 96);

class ACharacterLoadHelper {
public:
	int GetNumCharacters();
	int64 GetInfoOffset();

	ACharacterLoadInfo* GetLoadInfo(int num);
};