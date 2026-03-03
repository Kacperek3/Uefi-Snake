// src/main.c
#include "bootArch.h"
#include "gameState.h"
#include "menuState.h"
#include "platform.h"
#include "snakeState.h"

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    (void)ImageHandle;

    PlatformContext Platform;
    GameStateManager Manager;

    MenuState g_menuState;
    snakeState g_snakeState;

    LoadExt2Driver(ImageHandle, SystemTable);
    FindAndBootArch(ImageHandle, SystemTable);

    EFI_STATUS Status = Platform_Init(&Platform, SystemTable);
    if (Status != EFI_SUCCESS) {
        return Status;
    }

    MenuState_Init(&g_menuState, SystemTable);
    SnakeState_Init(&g_snakeState);

    StateManager_Init(&Manager, &Platform, (GameState *)&g_menuState);

    Manager.menuState = &g_menuState;
    Manager.snakeState = &g_snakeState;

    while (Manager.currentState != NULL) {

        EFI_INPUT_KEY key;
        EFI_STATUS keyStatus = Platform_PollKey(&Platform, &key);

        StateManager_HandleInput(&Manager, (keyStatus == EFI_SUCCESS) ? &key : NULL);
        StateManager_Update(&Manager);
        StateManager_Draw(&Manager);

        Platform_Sleep(&Platform, 33); //  ~30 FPS
    }

    SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    return EFI_SUCCESS;
}
