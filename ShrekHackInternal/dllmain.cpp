// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "include.h"

BOOL WINAPI triggerThread(HMODULE hModule) {
    uintptr_t gameModule = (uintptr_t)GetModuleHandle("client.dll");

    while (!GetAsyncKeyState(VK_END))
    {
        uintptr_t localPlayer = *(uintptr_t*)(gameModule + dwLocalPlayer);
        if (localPlayer == NULL) continue;

        uint32_t localTeam = *(uint32_t*)(localPlayer + m_iTeamNum);
        uint32_t CrosshairId = *(uint32_t*)(localPlayer + m_iCrosshairId);

        if (CrosshairId != 0 && CrosshairId < 32) {
            uintptr_t Enemy = *(uintptr_t*)(gameModule + dwEntityList + (CrosshairId - 1) * 0x10);
            if (Enemy == NULL)continue;

            uint32_t eHealth = *(uint32_t*)(Enemy + m_iHealth);
            uint32_t EnemyTeam = *(uintptr_t*)(Enemy + m_iTeamNum);

            if (EnemyTeam != localTeam && eHealth > 0 && eHealth <= 100) {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                Sleep(20);
            }
            Sleep(1);
        }

    }
    FreeLibraryAndExitThread(hModule,0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)triggerThread, 0, 0, 0));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

