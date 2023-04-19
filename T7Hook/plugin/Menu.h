#pragma once
#include "..\unreal\FVector.h"
#include "..\unreal\FRotator.h"
#include "APlayerCameraManagerHooked.h"

#define T7HOOK_VERSION "0.1"

// bone names
// HEAD
// L_Eye_Joint
// R_Eye_Joint


enum eCustomCameras {
	CC_FirstPerson,
	CC_FirstPersonLookAtHead,
	CC_ThirdPerson,
	CC_Total
};



enum eMenuSubMenus {
	SM_Settings,
	SM_Total
};

class T7Menu {
public:
	bool	 m_bIsActive = false;
	bool	 m_bSubmenuActive[SM_Total] = {};
	bool     m_bPressingKey = false;
	bool	 m_bSlowMotion = false;
	bool	 m_bIsFocused = true;

	bool	 m_bCustomCameraPos = false;
	bool	 m_bCustomCameraRot = false;
	bool	 m_bCustomCameraFOV = false;
	bool	 m_bCustomCameraAR = false;
	bool	 m_bCustomCameras = false;
	bool	 m_bFreeCam = false;
	bool	 m_bDisableHUD = false;

	bool	 m_bConstrainAROverride = true;

	bool	 m_bMouseControl = false;

	bool	 m_bPlayer1Modifier = false;
	bool	 m_bPlayer2Modifier = false;
	int		 m_nPlayer1Character = 0;
	int		 m_nPlayer2Character = 0;

	FVector	 camPos;
	FRotator camRot;
	float	 camFOV;
	
	int		 camAr[2];

	int  m_nCurrentCustomCamera = CC_FirstPerson;
	
	bool	m_bStabilizeFirstPerson = true;
	bool	m_bUseP2AsSource = false;

	float	m_fFirstPersonDistance = 5.0f;
	float	m_fThirdPersonDistance = 325.0f;
	float	m_fThirdPersonHeight = 90.0f;
	float	m_fSlowMotionSpeed = 0.5f;
	float	m_fFreeCamSpeed = 5.25f;
	float   m_fFreeCamRotationSpeed = 2.25f;

	int*	m_pCurrentVarToChange = nullptr;

	char	szCurrentCameraOption[128] = {};
	char	szPlayer1ModifierCharacter[128] = {};
	char	szPlayer2ModifierCharacter[128] = {};

	T7Menu();
	
	void	 OnActivate();
	void	 OnToggleSlowMotion();
	void	 OnToggleHUD();


	void	 Draw();
	void	 DrawCharacterTab();
	void	 DrawCameraTab();
	void	 DrawPlayerTab();
	void	 DrawSpeedTab();
	void	 DrawWorldTab();
	void	 DrawMiscTab();

	void	 DrawSettings();


	void	 DrawKeyBind(char* name, int* var);
	void	 KeyBind(int* var, char* bindName, char* name);

	void	 UpdateCustomCameras();
	void	 UpdateFreeCamera();
	void	 UpdateHUDToggle();
	void	 UpdateCharacters();
};

float GetDeltaTime();
void Plugin_UpdateThread();

extern T7Menu* TheMenu;