#pragma once
#include "FTViewTarget.h"
#include "APlayerController.h"

#include "..\utils\core.h"
#include "FMatrix.h"

struct FCameraCacheEntry {
	float TimeStamp;
	FMinimalViewInfo POV;
};
VALIDATE_SIZE(FCameraCacheEntry, 1012);

class APlayerCameraManager {
public:
	char pad[0x380];
	APlayerController* PCOwner;
	UObject* TransformComponent;
	int		 CameraStyle[2];
	float    DefaultFOV;
	float    LockedFOV;
	char     _pad[80]; // 3A0
	FCameraCacheEntry CameraCache; // 410
	char	__pad[8 + 1012];
	FTViewTarget ViewTarget;
	FTViewTarget PendingViewTarget;

	void UpdateViewTarget(FTViewTarget* OutVT);
	void DoUpdateCamera(float DeltaTime, bool unkBool);

};

VALIDATE_OFFSET(APlayerCameraManager, PCOwner, 0x380);
VALIDATE_OFFSET(APlayerCameraManager, CameraCache, 0x3F0);
VALIDATE_OFFSET(APlayerCameraManager, ViewTarget, 0xBE0);
VALIDATE_OFFSET(APlayerCameraManager, PendingViewTarget, 0xFE0);

extern APlayerCameraManager* APlayerCameraManager_Static;