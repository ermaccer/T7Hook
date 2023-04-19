#include "APlayerCameraManagerHooked.h"
#include "..\tekken\CharacterInfo.h"
#include <iostream>
#include <Windows.h>
#include "Menu.h"

void APlayerCameraManagerHooked::DoUpdateCameraHooked(float DeltaTime, bool unkBool)
{
	FMinimalViewInfo POV = CameraCache.POV;

	DoUpdateCamera(DeltaTime, unkBool);

	if (TheMenu->m_bCustomCameraPos || TheMenu->m_bCustomCameras)
		CameraCache.POV.Location = TheMenu->camPos;
	else
		TheMenu->camPos = CameraCache.POV.Location;

	if (TheMenu->m_bCustomCameraPos || TheMenu->m_bCustomCameras)
		CameraCache.POV.Rotation = TheMenu->camRot;
	else
		TheMenu->camRot = CameraCache.POV.Rotation;

	if (TheMenu->m_bCustomCameraAR)
	{
		float X = TheMenu->camAr[0];
		float Y = TheMenu->camAr[1];
		CameraCache.POV.AspectRatio = X / Y;
	}

	CameraCache.POV.bConstrainAspectRatio = TheMenu->m_bConstrainAROverride;

	TheMenu->UpdateFreeCamera();
	TheMenu->UpdateCustomCameras();
	TheMenu->UpdateHUDToggle();
	TheMenu->UpdateCharacters();

}

void APlayerCameraManagerHooked::SetFOV(float NewFOV)
{
	if (!TheMenu->m_bCustomCameraFOV)
		TheMenu->camFOV = LockedFOV = NewFOV;
	else
		LockedFOV = TheMenu->camFOV;
}
