#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "gameState.h"



#define EFI_BLACK         0x00
#define EFI_WHITE         0x0F
#define EFI_YELLOW        0x0E
#define EFI_RED           0x04
#define EFI_BACKGROUND_BLACK 0x00
#define EFI_BACKGROUND_WHITE 0x70




typedef struct {
    GameState base;

    int selectedOption;

} MenuState;

void MenuState_Init(MenuState *State);

#endif