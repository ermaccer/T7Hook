#include "Menu.h"
#include "..\gui\imgui\imgui.h"
#include "..\tekken\CharacterInfo.h"
#include "..\tekken\ACharacterManager.h"
#include "..\unreal\UWorld.h"
#include "..\unreal\Math.h"
#include "Settings.h"
#include "..\helper\eKeyboardMan.h"
#include "..\gui\gui_impl.h"
#include "..\gui\notifications.h"
#include "..\utils\Patterns.h"
#include "..\utils\MemoryMgr.h"
#include "..\tekken\ATekkenGFxHUD.h"
#include <map>
#include <string>

using namespace Memory::VP;

T7Menu* TheMenu = new T7Menu();

static const char* cameraModes[] = {
	"First Person",
	"First Person (Look at P2 head)",
	"Third Person"
};

std::map<std::string, int> mCharacters = {
	{"Paul",0},
	{"Law",1},
	{"King",2},
	{"Yoshimitsu",3},
	{"Hwoarang",4},
	{"Xiaoyu",5},
	{"Jin",6},
	{"Bryan",7},
	{"Heihachi",8},
	{"Kazyua",9},
	{"Steve",10},
	{"Jack-7",11},
	{"Asuka",12},
	{"Devil Jin",13},
	{"Feng",14},
	{"Lili",15},
	{"Dragunov",16},
	{"Leo",17},
	{"Lars",18},
	{"Alisa",19},
	{"Claudio",20},
	{"Katarina",21},
	{"Lucky Chloe",22},
	{"Shaheen",23},
	{"Josie",24},
	{"Gigas",25},
	{"Kazumi",26},
	{"Devil Kazumi",27},
	{"Nina",28},
	{"Master Raven",29},
	{"Lee",30},
	{"Bob",31},
	{"Akuma",32},
	{"Kuma",33},
	{"Panda",34},
	{"Eddy",35},
	{"Eliza",36},
	{"Miguel",37},
	{"Tekken Force Soldier",38},
	{"Young Kazuya",39},
	{"Jack-4",40},
	{"Young Heihachi",41},
	{"Practice Dummy",42},
	{"Geese",43},
	{"Noctis",44},
	{"Anna",45},
	{"Lei",46},
	{"Marduk",47},
	{"Armor King",48},
	{"Julia",49},
	{"Negan",50},
	{"Zafina",51},
	{"Ganryu",52},
	{"Leroy",53},
	{"Fahkumram",54},
	{"Kunimitsu",55},
	{"Lidia",56},
};

static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

}



T7Menu::T7Menu()
{
	camPos = {};
	camAr[0] = 16;
	camAr[1] = 9;
	camFOV = 0.0f;
	
	sprintf(szCurrentCameraOption, cameraModes[CC_FirstPerson]);
	sprintf(szPlayer1ModifierCharacter, "Akuma");
	sprintf(szPlayer2ModifierCharacter, "Akuma");
}

void T7Menu::OnActivate()
{
	m_bIsActive ^= 1;
}

void T7Menu::OnToggleSlowMotion()
{
	if (m_bPressingKey)
		return;
	if (m_bIsActive)
		return;
	m_bSlowMotion ^= 1;
}

void T7Menu::OnToggleHUD()
{
	if (m_bPressingKey)
		return;
	if (m_bIsActive)
		return;
	m_bDisableHUD ^= 1;
}

void T7Menu::Draw()
{
	if (!m_bIsActive)
		return;

	ImGui::GetIO().MouseDrawCursor = true;

	ImGui::Begin("T7Hook by ermaccer", &m_bIsActive, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Settings"))
			{
				m_bSubmenuActive[SM_Settings] = true;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::BeginMenu("About"))
				{
					ImGui::MenuItem("Version: " T7HOOK_VERSION);
					ImGui::MenuItem("Date: " __DATE__);
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginTabBar("##tabs"))
		{
			if (ImGui::BeginTabItem("Character"))
			{
				DrawCharacterTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Camera"))
			{
				DrawCameraTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Player"))
			{
				DrawPlayerTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Speed"))
			{
				DrawSpeedTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc."))
			{
				DrawMiscTab();
				ImGui::EndTabItem();
			}
#ifdef _DEBUG
			if (ImGui::BeginTabItem("World"))
			{
				DrawWorldTab();
				ImGui::EndTabItem();
			}
#endif // _DEBUG


			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	if (m_bSubmenuActive[SM_Settings])
		DrawSettings();
}

void T7Menu::DrawCharacterTab()
{
	ImGui::Checkbox("Player 1 Modifier", &m_bPlayer1Modifier);
	ImGui::SameLine();
	ShowHelpMarker("Special outfits aren't applied if a character requires them. Only affects character select, does NOT change character in already loaded games.");
	ImGui::PushItemWidth(-FLT_MIN);
	if (ImGui::BeginCombo("##clistp1", szPlayer1ModifierCharacter))
	{
		for (auto m : mCharacters)
		{
			bool is_selected = strcmp(szPlayer1ModifierCharacter, m.first.c_str()) == 0;
			if (ImGui::Selectable(m.first.c_str(), is_selected))
			{
				m_nPlayer1Character = m.second;
				sprintf(szPlayer1ModifierCharacter, m.first.c_str());
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	ImGui::Checkbox("Player 2 Modifier", &m_bPlayer2Modifier);
	ImGui::SameLine();
	ShowHelpMarker("Special outfits aren't applied if a character requires them. Only affects character select, does NOT change character in already loaded games.");

	ImGui::PushItemWidth(-FLT_MIN);
	if (ImGui::BeginCombo("##clistp2", szPlayer2ModifierCharacter))
	{
		for (auto m : mCharacters)
		{
			bool is_selected = strcmp(szPlayer2ModifierCharacter, m.first.c_str()) == 0;
			if (ImGui::Selectable(m.first.c_str(), is_selected))
			{
				m_nPlayer2Character = m.second;
				sprintf(szPlayer2ModifierCharacter, m.first.c_str());
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
}

void T7Menu::DrawCameraTab()
{
	if (!APlayerCameraManager_Static)
		return;

	ImGui::Checkbox("Set Camera Position", &m_bCustomCameraPos);
	ImGui::InputFloat3("X | Y | Z", &camPos.X);
	ImGui::Checkbox("Set Camera Rotation", &m_bCustomCameraRot);
	ImGui::InputFloat3("Pitch | Yaw | Roll", &camRot.Pitch);
	ImGui::Checkbox("Set FOV", &m_bCustomCameraFOV);
	ImGui::InputFloat("FOV", &camFOV);

	ImGui::Separator();
	ImGui::Checkbox("Free Camera", &m_bFreeCam);
	ImGui::SameLine(); ShowHelpMarker("Check controls in the Settings menu!");
	if (m_bFreeCam)
	{
		if (!m_bCustomCameraPos || !m_bCustomCameraRot || !m_bCustomCameraFOV)
			ImGui::TextColored(ImVec4(1.f, 0.3f, 0.3f, 1.f), "Check rest of the Set Camera options!");

		{
			ImGui::InputFloat("Free Camera Speed", &m_fFreeCamSpeed);
			ImGui::InputFloat("Free Camera Rotation Speed", &m_fFreeCamRotationSpeed);
		}
	}


	ImGui::Separator();
	ImGui::Checkbox("Custom Cameras", &m_bCustomCameras);
	if (m_bCustomCameras)
	{
		if (ImGui::BeginCombo("Camera Mode", szCurrentCameraOption))
		{
			for (int n = 0; n < IM_ARRAYSIZE(cameraModes); n++)
			{
				bool is_selected = (szCurrentCameraOption == cameraModes[n]);
				if (ImGui::Selectable(cameraModes[n], is_selected))
				{
					sprintf(szCurrentCameraOption, cameraModes[n]);
					m_nCurrentCustomCamera = n;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();

			}
			ImGui::EndCombo();
		}
		ImGui::Checkbox("Use Player Two As Source", &m_bUseP2AsSource);

		if (m_nCurrentCustomCamera == CC_FirstPerson)
			ImGui::Checkbox("Stabilize View", &m_bStabilizeFirstPerson);

		if (m_nCurrentCustomCamera == CC_FirstPerson || m_nCurrentCustomCamera == CC_FirstPersonLookAtHead)
			ImGui::SliderFloat("Distance", &m_fFirstPersonDistance, -10.0, 10.0f);

		if (m_nCurrentCustomCamera == CC_ThirdPerson)
		{
			ImGui::SliderFloat("Distance##tp", &m_fThirdPersonDistance, 100.0, 400.0f);
			ImGui::SliderFloat("Height##tp", &m_fThirdPersonHeight, -20.0, 200.0f);
		}

	}
	ImGui::Separator();
	ImGui::Checkbox("Set Aspect Ratio", &m_bCustomCameraAR);
	ImGui::InputInt2("Aspect Ratio", &camAr[0]);

	ImGui::Checkbox("Constrain Aspect Ratio", &m_bConstrainAROverride);
#ifdef _DEBUG
	ImGui::Text("APlayerCameraManager: 0x%p", APlayerCameraManager_Static);
#endif
}

void T7Menu::DrawPlayerTab()
{
	ACharacterManager* cm = GetCharacterManager();
	if (!cm)
		return;

	ACharacterLoadHelper* clh = cm->GetLoadHelper();
	if (!clh)
		return;

	AModularTekkenPlayer* p1 = clh->GetLoadInfo(0)->pTekkenPlayer;
	AModularTekkenPlayer* p2 = clh->GetLoadInfo(1)->pTekkenPlayer;


	if (p1 && p2)
	{
		ImGui::Text("Scale");
		static FVector p1Scale = { 1.0f, 1.0f, 1.0f };
		static FVector p2Scale = { 1.0f, 1.0f, 1.0f };

		ImGui::InputFloat3("P1 X | Y | Z", &p1Scale.X);
		ImGui::InputFloat3("P2 X | Y | Z", &p2Scale.X);

		if (ImGui::Button("Set Scale", { -FLT_MIN, 0 }))
		{
			p1->SetActorScale3D(&p1Scale);
			p2->SetActorScale3D(&p2Scale);
		}

		if (ImGui::Button("Reset Scale", { -FLT_MIN, 0 }))
		{
			p1Scale = { 1.0f, 1.0f, 1.0f };
			p2Scale = { 1.0f, 1.0f, 1.0f };
			p1->SetActorScale3D(&p1Scale);
			p2->SetActorScale3D(&p2Scale);
		}
		ImGui::Separator();
		ImGui::Text("Position");
		ImGui::SameLine(); ShowHelpMarker("Read only!");
		static FVector plrPos, plrPos2;

		p1->GetActorLocation(&plrPos);
		ImGui::InputFloat3("X | Y | Z", &plrPos.X);
		p2->GetActorLocation(&plrPos2);
		ImGui::InputFloat3("X | Y | Z", &plrPos2.X);

	}
}

void T7Menu::DrawSpeedTab()
{
	ImGui::Text("Gamespeed Control");

	ImGui::InputFloat("##sms", &m_fSlowMotionSpeed, 0.1f);

	ImGui::Checkbox("Enable", &m_bSlowMotion);
}

void T7Menu::DrawWorldTab()
{
	UWorld* world = GetWorld();

	if (!world)
		return;

	if (!world->PersistentLevel)
		return;
	ULevel* p = world->PersistentLevel;

	ImGui::Text("%p", world);
	ImGui::Text("StreamingNum %d/%d", world->StreamingLevels.Count, world->StreamingLevels.Max);
	ImGui::Text("Persistent level: %p", p);
	ImGui::Text("Persistent level actors: %d/%d", p->ActorsForGC.Count, p->ActorsForGC.Max);
	if (ImGui::Button("Print ALL Names to console"))
	{
		for (int i = 0; i < world->StreamingLevels.Count; i++)
		{
			ULevelStreaming* ls = world->StreamingLevels.Get(i);
			if (ls)
			{
				ULevel* l = ls->LoadedLevel;
				if (l)
				{
					for (int k = 0; k < l->ActorsForGC.Count; k++)
					{
						AActor* a = l->ActorsForGC.Get(k);

						if (a)
						{
							FString str;
							a->GetFullName(&str, 0);

							wprintf(L"SL%02d | %03d | %s | %p\n", i + 1, k + 1, str.GetStr(), a);
						}
					}
				}
			}

		}
	}
	ImGui::Separator();

	for (int i = 0; i < p->ActorsForGC.Count; i++)
	{
		AActor* a = p->ActorsForGC.Get(i);
		if (a)
		{
			FString str;
			a->GetFullName(&str, 0);
			if (a->GetClass())
			{
				FString classStr;
				FName className = a->GetClass()->GetName();

				className.ToString(&classStr);
				ImGui::Text("%03d | 0x%p | %ws", i + 1, a, classStr.GetStr());
			}

		}


	}

	ImGui::Separator();
}

void T7Menu::DrawMiscTab()
{
	ImGui::Checkbox("Disable HUD", &m_bDisableHUD);
}

void T7Menu::DrawSettings()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.5f, 0.5f });
	ImGui::SetNextWindowPos({ ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f }, ImGuiCond_Once, { 0.5f, 0.5f });
	ImGui::SetNextWindowSize({ 700,700 }, ImGuiCond_Once);
	ImGui::Begin("Settings", &m_bSubmenuActive[SM_Settings]);

	static int settingID = 0;
	static const char* settingNames[] = {
		"Menu",
		"INI",
		"Keys",
	};

	enum eSettings {
		MENU,
		INI,
		KEYS,
	};

	ImGui::BeginChild("##settings", { 12 * ImGui::GetFontSize(), 0 }, true);

	for (int n = 0; n < IM_ARRAYSIZE(settingNames); n++)
	{
		bool is_selected = (settingID == n);
		if (ImGui::Selectable(settingNames[n], is_selected))
			settingID = n;
		if (is_selected)
			ImGui::SetItemDefaultFocus();
	}

	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild("##content", { 0, -ImGui::GetFrameHeightWithSpacing() });

	switch (settingID)
	{
	case MENU:
		ImGui::TextWrapped("All user settings are saved to t7hook_user.ini.");
		ImGui::Text("Menu Scale");
		ImGui::PushItemWidth(-FLT_MIN);
		ImGui::InputFloat("##scale", &SettingsMgr->fMenuScale);
		ImGui::PopItemWidth();
		break;
	case INI:
		ImGui::TextWrapped("These settings control T7Hook.ini options. Any changes require game restart to take effect.");
		ImGui::LabelText("", "Core");
		ImGui::Separator();
		ImGui::Checkbox("Debug Console", &SettingsMgr->bEnableConsoleWindow);
		ImGui::Separator();

		break;
	case KEYS:
		if (m_bPressingKey)
			ImGui::TextColored(ImVec4(0.f, 1.f, 0.3f, 1.f), "Press a key!");

		if (ImGui::Button("Reset Keys", { -FLT_MIN, 0 }))
		{
			SettingsMgr->ResetKeys();
			Notifications->SetNotificationTime(2500);
			Notifications->PushNotification("Keys reset! Remember to save.");
		}

		ImGui::Separator();
		ImGui::LabelText("", "Core");
		ImGui::Separator();
		KeyBind(&SettingsMgr->iMenuOpenKey, "Open/Close Menu", "menu");
		KeyBind(&SettingsMgr->iToggleSlowMoKey, "Toggle Slowmotion", "slomo");
		KeyBind(&SettingsMgr->iToggleHUDKey, "Toggle HUD", "hud");
		ImGui::Separator();
		ImGui::LabelText("", "Camera/Airbreak");
		ImGui::Separator();

		KeyBind(&SettingsMgr->iFreeCameraKeyFOVPlus, "FOV+", "fov_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyFOVMinus, "FOV-", "fov_minus");

		KeyBind(&SettingsMgr->iFreeCameraKeyYawPlus, "Yaw+", "ya_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyYawMinus, "Yaw-", "ya_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyPitchPlus, "Pitch+", "pi_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyPitchMinus, "Pitch-", "pi_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRollPlus, "Roll+", "r_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRollMinus, "Roll-", "r_minus");

		KeyBind(&SettingsMgr->iFreeCameraKeyForward, "Forward", "x_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyBack, "Back", "x_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyLeft, "Left", "y_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRight, "Right", "y_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyUp, "Up", "z_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyDown, "Down", "z_minus");
		ImGui::Separator();

		if (m_bPressingKey)
		{
			eVKKeyCode result = eKeyboardMan::GetLastKey();

			if (result >= VK_BACKSPACE && result < VK_KEY_NONE)
			{
				*m_pCurrentVarToChange = result;
				m_bPressingKey = false;
			}

		}
		break;
	default:
		break;
	}

	if (ImGui::Button("Save", { -FLT_MIN, 0 }))
	{
		Notifications->SetNotificationTime(2500);
		Notifications->PushNotification("Settings saved to T7Hook.ini and t7hook_user.ini!");
		GUIImplementation::RequestFontReload();
		SettingsMgr->SaveSettings();
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();

	ImGui::End();
}

void T7Menu::DrawKeyBind(char* name, int* var)
{
	ImGui::SameLine();

	static char butName[256] = {};
	sprintf(butName, "%s##key%s", eKeyboardMan::KeyToString(*var), name);
	if (ImGui::Button(butName))
	{
		m_bPressingKey = true;
		m_pCurrentVarToChange = var;
	}
}

void T7Menu::KeyBind(int* var, char* bindName, char* name)
{
	ImGui::LabelText("", bindName);
	DrawKeyBind(name, var);
}

void T7Menu::UpdateCustomCameras()
{
	if (!m_bCustomCameras)
		return;

	if (!GetWorld())
		return;

	if (!GetWorld()->PersistentLevel)
		return;

	if (!(GetWorld()->StreamingLevels.Count > 0))
		return;

	ACharacterManager* characterManager = GetCharacterManager();

	if (!characterManager)
		return;

	ACharacterLoadHelper* clh = characterManager->GetLoadHelper();
	if (!clh)
		return;

	if (!clh->GetInfoOffset())
		return;

	if (clh->GetNumCharacters() < 2)
		return;

	AModularTekkenPlayer* player = clh->GetLoadInfo(TheMenu->m_bUseP2AsSource ? SIDE_RIGHT : SIDE_LEFT)->pTekkenPlayer;

	if (!player)
		return;

	AModularTekkenPlayer* enemy = clh->GetLoadInfo(TheMenu->m_bUseP2AsSource ? SIDE_LEFT : SIDE_RIGHT)->pTekkenPlayer;

	if (!enemy)
		return;

	if (m_nCurrentCustomCamera == CC_FirstPerson)
	{
		FVector finalPos;
		FVector eyePos[2];
		FVector forward;
		FRotator headRot;
		int leftEyeID, rightEyeID, headID;
		USkeletalMeshComponent* mesh;

		mesh = player->GetMeshComponent(MCT_FaceMesh);
		if (mesh && mesh->SkeletalMesh)
		{
			leftEyeID = mesh->GetBoneIndexFromString(L"L_Eye_Joint");
			rightEyeID = mesh->GetBoneIndexFromString(L"R_Eye_Joint");
			headID = mesh->GetBoneIndexFromString(L"HEAD");

			if (leftEyeID >= 0 && rightEyeID >= 0 && headID >= 0)
			{
				mesh->GetBoneLocation(&eyePos[0], leftEyeID);
				mesh->GetBoneLocation(&eyePos[1], rightEyeID);

				mesh->GetBoneForward(&forward, headID);

				finalPos = (eyePos[0] + eyePos[1]) / 2.0f;

				mesh->GetBoneRotation(&headRot, headID);

				finalPos += forward * TheMenu->m_fFirstPersonDistance;

				camPos = finalPos;
				if (m_bStabilizeFirstPerson)
					headRot.Roll = 0;
				camRot = headRot;
			}
		}	
	}
	else if (m_nCurrentCustomCamera == CC_FirstPersonLookAtHead)
	{
		FVector finalPos;
		FVector eyePos[2];
		FVector forward;
		FVector headPos_p2;
		int leftEyeID, rightEyeID, headID, headID_p2;
		USkeletalMeshComponent* mesh, *enemy_mesh;

		mesh = player->GetMeshComponent(MCT_FaceMesh);
		enemy_mesh = enemy->GetMeshComponent(MCT_FaceMesh);

		if (mesh && mesh->SkeletalMesh && enemy_mesh && enemy_mesh->SkeletalMesh)
		{
			leftEyeID = mesh->GetBoneIndexFromString(L"L_Eye_Joint");
			rightEyeID = mesh->GetBoneIndexFromString(L"R_Eye_Joint");
			headID = mesh->GetBoneIndexFromString(L"HEAD");
			headID_p2 = enemy_mesh->GetBoneIndexFromString(L"HEAD");

			if (leftEyeID >= 0 && rightEyeID >= 0 && headID >= 0 && headID_p2 >= 0)
			{
				mesh->GetBoneLocation(&eyePos[0], leftEyeID);
				mesh->GetBoneLocation(&eyePos[1], rightEyeID);

				mesh->GetBoneForward(&forward, headID);
				enemy_mesh->GetBoneLocation(&headPos_p2, headID_p2);
				finalPos = (eyePos[0] + eyePos[1]) / 2.0f;

				finalPos += forward * TheMenu->m_fFirstPersonDistance * -1;

				camPos = finalPos;
				camRot = FindLookAtRotation(&finalPos, &headPos_p2);
				
			}
		}
	}
	else if (m_nCurrentCustomCamera == CC_ThirdPerson)
	{
		FVector finalPos, target;
		FVector spinePos, spinePos_p2;
		FVector forward;
		int spineID, spineID_p2;
		static FQuat* quatRot = new FQuat();
		USkeletalMeshComponent* mesh, * enemy_mesh;

		mesh = player->GetMeshComponent(MCT_UpperMesh);
		enemy_mesh = enemy->GetMeshComponent(MCT_UpperMesh);

		if (mesh && mesh->SkeletalMesh && enemy_mesh && enemy_mesh->SkeletalMesh)
		{
			spineID = mesh->GetBoneIndexFromString(L"Spine1");
			spineID_p2 = mesh->GetBoneIndexFromString(L"Spine1");

			if (spineID >= 0 && spineID_p2 >= 0)
			{
				mesh->GetBoneLocation(&spinePos, spineID);
				enemy_mesh->GetBoneLocation(&spinePos_p2, spineID_p2);

				finalPos = spinePos;
				finalPos.Z += TheMenu->m_fThirdPersonHeight;


				target = spinePos_p2;
				target.Z += TheMenu->m_fThirdPersonHeight / 2.0f;

				camRot = FindLookAtRotation(&finalPos, &target);


				camRot.Quaternion(quatRot);

				FMatrix camMat;
				camMat.FromQuat(quatRot);

				camMat.GetForward(&forward);
				finalPos += forward * TheMenu->m_fThirdPersonDistance * -1;

				camPos = finalPos;

			}
		}
	}
}

void T7Menu::UpdateFreeCamera()
{
	if (!m_bFreeCam)
		return;

	static FQuat* quatRot = new FQuat();
	camRot.Quaternion(quatRot);

	FMatrix camMat;
	camMat.FromQuat(quatRot);

	FVector fwd, right;

	camMat.GetForward(&fwd);
	camMat.GetRight(&right);

	float camSpeed = TheMenu->m_fFreeCamSpeed * 100.0f;
	float camRotSpeed = TheMenu->m_fFreeCamRotationSpeed * 100.0f;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyForward))
		camPos += fwd * camSpeed * GetDeltaTime() * 1;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyBack))
		camPos += fwd * camSpeed * GetDeltaTime() * -1;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyLeft))
		camPos += right * camSpeed * GetDeltaTime() * -1;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRight))
		camPos += right * camSpeed * GetDeltaTime() * 1;

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyUp))
		camPos.Z += camSpeed * GetDeltaTime() * 1;
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyDown))
		camPos.Z += camSpeed * GetDeltaTime() * -1;


	
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVMinus))
		camFOV -= 100.0f * GetDeltaTime();
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVPlus))
		camFOV += 100.0f * GetDeltaTime();



	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyPitchPlus))
		camRot.Pitch += camRotSpeed * GetDeltaTime();
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyPitchMinus))
		camRot.Pitch -= camRotSpeed * GetDeltaTime();

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYawPlus))
		camRot.Yaw += camRotSpeed * GetDeltaTime();
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYawMinus))
		camRot.Yaw -= camRotSpeed * GetDeltaTime();

	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRollPlus))
		camRot.Roll += camRotSpeed * GetDeltaTime();
	if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRollMinus))
		camRot.Roll -= camRotSpeed * GetDeltaTime();
	
}

void T7Menu::UpdateHUDToggle()
{
	static void* pat = hook::txn::get_pattern("40 56 48 83 EC 30 0F B6 51 08 48 8B F1");

	Patch<char>(pat, TheMenu->m_bDisableHUD ? 0xC3 : 0x40);

	static void* pat2 = hook::txn::get_pattern("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 83 EC 70 44 8B 15 ? ? ? ? 4C 8B F1");

	Patch<char>(pat2, TheMenu->m_bDisableHUD ? 0xC3 : 0x48);

	ATekkenGFxHUD* hud = GetTekkenHUD();
	if (hud && TheMenu->m_bDisableHUD)
	{
		hud->bDrawHUD = false;
	}
}

void T7Menu::UpdateCharacters()
{
	CharacterInfo* p1_info = GetCharacterInfo(0);

}

float GetDeltaTime()
{
	return GUIImplementation::GetDeltaTime();
}

void Plugin_UpdateThread()
{
	while (true)
	{
		Notifications->Update();
		Sleep(1);
	}
}
