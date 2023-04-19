#pragma once
#include "ACharacterLoadHelper.h"
#include "..\unreal\UObject.h"

#define CHARACATERMANAGER_CLASSNAME L"BP_CharacterManager_C"

class ACharacterManager {
public:
	ACharacterLoadHelper* GetLoadHelper();
};

ACharacterManager* GetCharacterManager();