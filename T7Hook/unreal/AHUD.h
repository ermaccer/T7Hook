#pragma once
#include "UObject.h"

class AHUD {
public:
	char pad[896];
	UObject* PlayerOwner;
	unsigned __int8 bLostFocusPaused : 1;
	unsigned __int8 bShowHUD : 1;
	unsigned __int8 bShowDebugInfo : 1;
};