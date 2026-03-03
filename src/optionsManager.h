#ifndef OPTIONS_MANAGER_H
#define OPTIONS_MANAGER_H

#include <efi.h>

typedef struct GameStateManager GameStateManager;

typedef void (*MenuAction)(GameStateManager *Manager);

typedef struct {
    CHAR16 *Text;
    UINTN xPos;
    UINTN yPos;
    MenuAction Action;
} Option;

typedef struct {
    UINTN AmountOfOptions;
    UINTN SelectedOption;
    Option *Options;
} OptionsManager;

EFI_STATUS OptionsManager_Init(OptionsManager *Mgr, UINTN Count, EFI_SYSTEM_TABLE *SystemTable);
void OptionUp(OptionsManager *Mgr);
void OptionDown(OptionsManager *Mgr);
void OptionAction(OptionsManager *Mgr, GameStateManager *Gamestate);
void FillOption(OptionsManager *Mgr, UINTN Index, CHAR16 *Text, UINTN x, UINTN y,
                MenuAction Action);
void CleanOptionsManager(OptionsManager *Mgr, EFI_SYSTEM_TABLE *SystemTable);

#endif
