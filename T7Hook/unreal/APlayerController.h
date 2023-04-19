#pragma once
#include "..\utils\core.h"
#include "AHUD.h"

class APlayerController {
public:
	char pad[1040];
	AHUD* MyHUD;
};

VALIDATE_OFFSET(APlayerController, MyHUD, 0x410);