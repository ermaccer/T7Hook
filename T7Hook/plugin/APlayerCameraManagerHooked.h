#pragma once
#include "..\unreal\APlayerCameraManager.h"
class APlayerCameraManagerHooked : public APlayerCameraManager{
public:
	void DoUpdateCameraHooked(float DeltaTime, bool unkBool);

	void SetFOV(float NewFOV);
};