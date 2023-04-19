// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>

#include "utils/addr.h"
#include "utils/MemoryMgr.h"
#include "utils/Trampoline.h"
#include "utils/Patterns.h"
#include "plugin/APlayerCameraManagerHooked.h"
#include "gui/dx11hook.h"
#include "gui/log.h"
#include "plugin/Settings.h"
#include "plugin/Menu.h"
#include "tekken/CharacterInfo.h"
#include "tekken/GameSpeed.h"
#include "gui/notifications.h"

using namespace Memory::VP;


void _hook_debug(const char* format, ...)
{
    printf(format);
}

void OnInitializeHook()
{
    if (SettingsMgr->bEnableConsoleWindow)
    {
        AllocConsole();
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
    Notifications->Init();

    eLog::Initialize();
    
    eLog::Message(__FUNCTION__, "INFO: T7Hook::Init()");
    

    Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
    Patch(_addr(0x14278FDB8), &APlayerCameraManagerHooked::DoUpdateCameraHooked);
    Patch(_addr(0x142CF3458), &APlayerCameraManagerHooked::DoUpdateCameraHooked);

    Patch(_addr(0x14278FD80), &APlayerCameraManagerHooked::SetFOV);
    Patch(_addr(0x142CF3420), &APlayerCameraManagerHooked::DoUpdateCameraHooked);

   // InjectHook(hook::txn::get_pattern("48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 C3 48 8D 05"), tramp->Jump(_hook_debug), PATCH_JUMP);
    InjectHook(hook::txn::get_pattern("89 51 10 89 91 A4 C0 00 00 C3"), tramp->Jump(SetPlayer1Character), PATCH_JUMP);
    InjectHook(hook::txn::get_pattern("89 51 14 C3"), tramp->Jump(SetPlayer2Character), PATCH_JUMP);
    InjectHook(hook::txn::get_pattern("48 8B 81 60 15 00 00 89 91 DC 00 00 00"), tramp->Jump(&CharacterInfo::SetIDHooked), PATCH_JUMP);
    InjectHook(hook::txn::get_pattern("89 0D ? ? ? ? C3 CC CC CC CC CC CC CC CC CC 88 0D"), tramp->Jump(SetGameSpeed), PATCH_JUMP);

    HANDLE h = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(DX11Hook_Thread), 0, NULL, 0);
    if (!(h == nullptr)) CloseHandle(h);
  

    CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Plugin_UpdateThread), nullptr, 0, nullptr);
}

bool ValidateGameVersion()
{
    wchar_t* gameName = (wchar_t*)_addr(0x142782218);

    if (wcscmp(gameName, L"Tekken 7") == 0)
        return true;
    else
    {
        MessageBoxA(0, "Invalid game version!\nDR3Hook only supports latest Steam executable,!\n\n"
            "If you still cannot run the plugin and made sure that the game is updated, DR3Hook needs to be updated.", 0, MB_ICONINFORMATION);
        return false;
    }
}


extern "C"
{
    __declspec(dllexport) void InitializeASI()
    {
        if (ValidateGameVersion())
          OnInitializeHook();
    }
}
