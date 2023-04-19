#pragma once
#include "..\utils\core.h"
#include "..\utils\addr.h"

#define TEKKENGFXHUD_CLASSNAME L"TekkenGFxHUD"

class ATekkenGFxHUD {
public:
	char pad[0x540];
	bool bDrawHUD;
};
VALIDATE_OFFSET(ATekkenGFxHUD, bDrawHUD, 0x540);

ATekkenGFxHUD* GetTekkenHUD();