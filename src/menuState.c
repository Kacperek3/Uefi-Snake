#include "menuState.h"
#include "archLogo.h"
#include "efiapi.h"
#include "gameState.h"
#include "optionsManager.h"

//---------------------------------------------------------
//                  optMgr
void Action_StartGame(GameStateManager *Manager) {
    StateManager_ChangeState(Manager, Manager->snakeState);
}
void Action_ChoseLevel(GameStateManager *self) {
    MenuState *state = (MenuState *)self->currentState;
    state->currentOption = &state->levMgr;
}
void Action_Exit(GameStateManager *Manager) {
    Manager->currentState = NULL;
}
//----------------------------------------------------------

//----------------------------------------------------------
//                  levMgr

void Action_Easy(GameStateManager *self) {
    MenuState *state = (MenuState *)self->currentState;
    state->currentOption = &state->optMgr;
}
void Action_Medium(GameStateManager *self) {
    MenuState *state = (MenuState *)self->currentState;
    state->currentOption = &state->optMgr;
}
void Action_Hard(GameStateManager *self) {
    MenuState *state = (MenuState *)self->currentState;
    state->currentOption = &state->optMgr;
}
//----------------------------------------------------------

static void Menu_Enter(GameState *self, PlatformContext *Platform) {
    MenuState *state = (MenuState *)self;

    Platform_EnableTextMode(Platform);
    Platform->ConOut->ClearScreen(Platform->ConOut);

    Platform->ConOut->SetCursorPosition(Platform->ConOut, 10, 2);
    Platform->ConOut->SetAttribute(Platform->ConOut, EFI_BACKGROUND_BLACK | EFI_YELLOW);
    Platform->ConOut->OutputString(Platform->ConOut, u"S N A K E   in   U E F I");

    Platform->ConOut->SetCursorPosition(Platform->ConOut, 0, 16);
    Platform->ConOut->SetAttribute(Platform->ConOut, EFI_BACKGROUND_BLACK | EFI_RED);
    Platform->ConOut->OutputString(Platform->ConOut, u"[ w ] - MOVE UP");
    Platform->ConOut->SetCursorPosition(Platform->ConOut, 0, 17);
    Platform->ConOut->OutputString(Platform->ConOut, u"[ s ] - MOVE DOWN");

    Platform->ConOut->SetCursorPosition(Platform->ConOut, 0, 18);
    Platform->ConOut->SetAttribute(Platform->ConOut, EFI_BACKGROUND_BLACK | EFI_RED);
    Platform->ConOut->OutputString(Platform->ConOut, u"SPACE - SELECT");

    Platform->ConOut->SetAttribute(Platform->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    Platform->ConOut->SetCursorPosition(Platform->ConOut, 0, 20);
    Platform->ConOut->OutputString(Platform->ConOut, u"Author: Kacperek3");
    DrawArchLogo(Platform, 30, 5);
}

static void Menu_HandleInput(GameState *self, PlatformContext *Platform, EFI_INPUT_KEY *Key,
                             GameStateManager *Manager) {
    MenuState *state = (MenuState *)self;
    (void)Platform;

    if (Key) {

        if (Key->UnicodeChar == u'w')
            OptionUp(state->currentOption);
        if (Key->UnicodeChar == u's')
            OptionDown(state->currentOption);
        if (Key->UnicodeChar == u' ') {
            OptionAction(state->currentOption, Manager);
        }
    }
}
static void Menu_Update(GameState *self, PlatformContext *Platform, GameStateManager *Manager) {
    MenuState *state = (MenuState *)self;
    EFI_INPUT_KEY key;

    (void)self;
    (void)Platform;
    (void)Manager;
}

static void Menu_Draw(GameState *self, PlatformContext *Platform) {
    MenuState *state = (MenuState *)self;

    for (UINTN i = 0; i < state->currentOption->AmountOfOptions; i++) {
        Option *opt = &state->currentOption->Options[i];

        Platform->ConOut->SetCursorPosition(Platform->ConOut, opt->xPos, opt->yPos);

        if (i == state->currentOption->SelectedOption) {
            Platform->ConOut->SetAttribute(Platform->ConOut, EFI_BACKGROUND_WHITE | EFI_BLACK);
            Platform->ConOut->OutputString(Platform->ConOut, L"> ");
        } else {
            Platform->ConOut->SetAttribute(Platform->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
            Platform->ConOut->OutputString(Platform->ConOut, L"  ");
        }

        Platform->ConOut->OutputString(Platform->ConOut, opt->Text);
    }
    Platform->ConOut->SetAttribute(Platform->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
}

void MenuState_Init(MenuState *State, EFI_SYSTEM_TABLE *SystemTable) {
    State->base.Enter = Menu_Enter;
    State->base.Exit = NULL;
    State->base.HandleInput = Menu_HandleInput;
    State->base.Update = Menu_Update;
    State->base.Draw = Menu_Draw;

    OptionsManager_Init(&State->optMgr, 3, SystemTable);
    FillOption(&State->optMgr, 0, L"GAME", 10, 5, Action_StartGame);
    FillOption(&State->optMgr, 1, L"LEVEL ", 10, 7, Action_ChoseLevel);
    FillOption(&State->optMgr, 2, L"EXIT", 10, 9, Action_Exit);
    State->currentOption = &State->optMgr;

    OptionsManager_Init(&State->levMgr, 3, SystemTable);
    FillOption(&State->levMgr, 0, L"EASY", 10, 5, Action_Easy);
    FillOption(&State->levMgr, 1, L"MEDIUM", 10, 7, Action_Medium);
    FillOption(&State->levMgr, 2, L"HARD", 10, 9, Action_Hard);
}
