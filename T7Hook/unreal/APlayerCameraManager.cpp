#include "APlayerCameraManager.h"
#include "..\utils\addr.h"
#include "..\utils\Patterns.h"
APlayerCameraManager* APlayerCameraManager_Static = nullptr;


void APlayerCameraManager::UpdateViewTarget(FTViewTarget* OutVT)
{
	((void(__thiscall*)(APlayerCameraManager*, FTViewTarget*))_addr(0x1416C33F0))(this, OutVT);
}

void APlayerCameraManager::DoUpdateCamera(float DeltaTime, bool unkBool)
{
	static void* pat = hook::txn::get_pattern("4C 8B DC 55 49 8D AB 68 FC FF FF");
	APlayerCameraManager_Static = this;
	((void(__thiscall*)(APlayerCameraManager*, float, bool))pat)(this, DeltaTime, unkBool);
}