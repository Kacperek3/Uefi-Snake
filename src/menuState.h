#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "archLogo.h"
#include "gameState.h"
#include "optionsManager.h"

#define EFI_BLACK 0x00
#define EFI_WHITE 0x0F
#define EFI_YELLOW 0x0E
#define EFI_RED 0x04
#define EFI_BACKGROUND_BLACK 0x00
#define EFI_BACKGROUND_WHITE 0x70

#define EASY 0
#define MEDIUM 1
#define HARD 2
#define HARDCORE 3

typedef struct {
    GameState base;
    int selectedLevel;
    OptionsManager optMgr;
    OptionsManager levMgr;
    OptionsManager *currentOption;
} MenuState;

void MenuState_Init(MenuState *State, EFI_SYSTEM_TABLE *SystemTable);

#endif
