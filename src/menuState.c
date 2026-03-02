#include "menuState.h"
#include "renderer.h" 
#include "snakeState.h"


static void Menu_Enter(GameState *self, PlatformContext *Platform) {
    MenuState *state = (MenuState*)self;
    state->selectedOption = 0;
    
    Platform_EnableTextMode(Platform);
    Platform->ConOut->ClearScreen(Platform->ConOut);

    Platform->ConOut->SetCursorPosition(Platform->ConOut, 10, 2);
    Platform->ConOut->OutputString(Platform->ConOut, u"S N A K E   in   U E F I");

    Platform->ConOut->SetCursorPosition(Platform->ConOut, 10, 15);
    Platform->ConOut->SetAttribute(Platform->ConOut, EFI_BACKGROUND_BLACK | EFI_RED);
    Platform->ConOut->OutputString(Platform->ConOut, u"w - UP s - DOWN SPACE - SELECT");

    Platform->ConOut->SetAttribute(Platform->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    Platform->ConOut->SetCursorPosition(Platform->ConOut, 0, 20);
    Platform->ConOut->OutputString(Platform->ConOut, u"Author: Kacper");
}

static void Menu_HandleInput(GameState *self, PlatformContext *Platform, EFI_INPUT_KEY *Key, GameStateManager *Manager) {
    MenuState *state = (MenuState*)self; (void)Platform;

    if (Key) { 
        
        if (Key->UnicodeChar == u'w') state->selectedOption = 0;
        if (Key->UnicodeChar == u's') state->selectedOption = 1;

        if (Key->UnicodeChar == u' ') {
            if (state->selectedOption == 0) {
                StateManager_ChangeState(Manager, Manager->snakeState); 
            } else {
                Manager->currentState = NULL;
            }
        }
    }
}
static void Menu_Update(GameState *self, PlatformContext *Platform, GameStateManager *Manager) {
    MenuState *state = (MenuState*)self;
    EFI_INPUT_KEY key;


    (void)self;
    (void)Platform;
    (void)Manager;
}

static void Menu_Draw(GameState *self, PlatformContext *Platform) {
    MenuState *state = (MenuState*)self;
    
    Platform->ConOut->SetCursorPosition(Platform->ConOut, 10, 5);
    if (state->selectedOption == 0) {
        Platform->ConOut->SetAttribute(Platform->ConOut, EFI_BACKGROUND_WHITE | EFI_BLACK);
        Platform->ConOut->OutputString(Platform->ConOut, u"> PLAY     ");
    } else {
        Platform->ConOut->SetAttribute(Platform->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
        Platform->ConOut->OutputString(Platform->ConOut, u"  PLAY     ");
    }

    Platform->ConOut->SetCursorPosition(Platform->ConOut, 10, 6);
    if (state->selectedOption == 1) {
        Platform->ConOut->SetAttribute(Platform->ConOut, EFI_BACKGROUND_WHITE | EFI_BLACK);
        Platform->ConOut->OutputString(Platform->ConOut, u"> EXIT    ");
    } else {
        Platform->ConOut->SetAttribute(Platform->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
        Platform->ConOut->OutputString(Platform->ConOut, u"  EXIT    ");
    }
    
    Platform->ConOut->SetAttribute(Platform->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK); 
}

void MenuState_Init(MenuState *State) {
    State->base.Enter = Menu_Enter;
    State->base.Exit = NULL; 
    State->base.HandleInput = Menu_HandleInput;
    State->base.Update = Menu_Update;
    State->base.Draw = Menu_Draw;
    
    State->selectedOption = 0;
}
