// src/game_state.c
#include "gameState.h"


void StateManager_Init(GameStateManager *Manager, PlatformContext *Platform, GameState *initialState) {
    Manager->Platform = Platform;
    Manager->currentState = NULL;
    StateManager_ChangeState(Manager, initialState);
}

void StateManager_ChangeState(GameStateManager *Manager, GameState *newState) {
    if (Manager->currentState && Manager->currentState->Exit) {
        Manager->currentState->Exit(Manager->currentState, Manager->Platform);
    }


    Manager->currentState = newState;
    if (Manager->currentState && Manager->currentState->Enter) {
        Manager->currentState->Enter(Manager->currentState, Manager->Platform);
    }
}


void StateManager_HandleInput(GameStateManager *Manager, EFI_INPUT_KEY *Key) {
    if (Manager->currentState && Manager->currentState->HandleInput) {
        Manager->currentState->HandleInput(Manager->currentState, Manager->Platform, Key, Manager);
    }
}

void StateManager_Update(GameStateManager *Manager) {
    if (Manager->currentState && Manager->currentState->Update) {
        Manager->currentState->Update(Manager->currentState, Manager->Platform, Manager);
    }
}

void StateManager_Draw(GameStateManager *Manager) {
    if (Manager->currentState && Manager->currentState->Draw) {
        Manager->currentState->Draw(Manager->currentState, Manager->Platform);
    }
}