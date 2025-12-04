#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "platform.h"

struct GameStateManager;
struct GameState;

typedef
void
(*ENTER)(
    struct GameState *self,
    PlatformContext *Platform
);

typedef
void 
(*EXIT)(
    struct GameState *self,
    PlatformContext *Platform
);

typedef
void 
(*HANDLE_INPUT)(
    struct GameState *self,
    PlatformContext *Platform,
    EFI_INPUT_KEY *Key,
    struct GameStateManager *Manager
);

typedef
void
(*UPDATE)(
    struct GameState *self,
    PlatformContext *Platform,
    struct GameStateManager *Manager
);

typedef
void 
(*DRAW)(
    struct GameState *self,
    PlatformContext *Platform
);



typedef struct GameState {
    ENTER           Enter;
    EXIT            Exit;
    HANDLE_INPUT    HandleInput;
    UPDATE          Update;
    DRAW            Draw;

} GameState;


typedef struct GameStateManager {
    GameState       *currentState; 
    PlatformContext *Platform; 
    
    void *menuState;
    void *snakeState;

} GameStateManager;

void StateManager_Init(GameStateManager *Manager, PlatformContext *Platform, GameState *initialState);
void StateManager_ChangeState(GameStateManager *Manager, GameState *newState);

void StateManager_HandleInput(GameStateManager *Manager, EFI_INPUT_KEY *Key);
void StateManager_Update(GameStateManager *Manager);
void StateManager_Draw(GameStateManager *Manager);

#endif 