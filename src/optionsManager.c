#include "optionsManager.h"
#include <efilib.h>

EFI_STATUS OptionsManager_Init(OptionsManager *Mgr, UINTN Count, EFI_SYSTEM_TABLE *SystemTable) {
    Mgr->AmountOfOptions = Count;
    Mgr->SelectedOption = 0;

    EFI_STATUS Status = SystemTable->BootServices->AllocatePool(
        EfiBootServicesData, Count * sizeof(Option), (void **)&Mgr->Options);

    return Status;
}

void OptionUp(OptionsManager *Mgr) {
    if (Mgr->SelectedOption > 0) {
        Mgr->SelectedOption--;
    }
}

void OptionDown(OptionsManager *Mgr) {
    if (Mgr->SelectedOption < Mgr->AmountOfOptions - 1) {
        Mgr->SelectedOption++;
    }
}

void OptionAction(OptionsManager *Mgr, GameStateManager *Gamestate) {
    if (Mgr->Options[Mgr->SelectedOption].Action != NULL) {
        Mgr->Options[Mgr->SelectedOption].Action(Gamestate);
    }
}

void FillOption(OptionsManager *Mgr, UINTN Index, CHAR16 *Text, UINTN x, UINTN y,
                MenuAction Action) {
    if (Index >= Mgr->AmountOfOptions)
        return;

    Mgr->Options[Index].Text = Text;
    Mgr->Options[Index].xPos = x;
    Mgr->Options[Index].yPos = y;
    Mgr->Options[Index].Action = Action;
}

void CleanOptionsManager(OptionsManager *Mgr, EFI_SYSTEM_TABLE *SystemTable) {
    if (Mgr->Options != NULL) {
        SystemTable->BootServices->FreePool(Mgr->Options);
        Mgr->Options = NULL;
    }
}
