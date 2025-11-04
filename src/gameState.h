#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "platform.h"

// Musimy zadeklarować wskaźniki "w przód"
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



// Definicja "interfejsu" (wskaźniki do funkcji)
typedef struct GameState {
    // "this" pointer jest przekazywany jako pierwszy argument
    
    // Wywoływane przy zmianie na ten stan
    ENTER           Enter;
    
    // Wywoływane przy opuszczaniu tego stanu
    EXIT            Exit;
    
    // Funkcje pętli gry
    HANDLE_INPUT    HandleInput;
    UPDATE          Update;
    DRAW            Draw;

} GameState;


// Definicja "Managera Stanów"
typedef struct GameStateManager {
    GameState       *currentState; // Wskaźnik na aktualny "interfejs"
    PlatformContext *Platform;     // Wskaźnik na nasz sprzęt
    
    // "Prywatne" pola - przechowujemy tu wskaźniki na konkretne stany
    // To pozwala nam uniknąć alokacji pamięci (malloc)
    void *menuState;
    void *snakeState;

} GameStateManager;

// Funkcje Managera
void StateManager_Init(GameStateManager *Manager, PlatformContext *Platform, GameState *initialState);
void StateManager_ChangeState(GameStateManager *Manager, GameState *newState);

// Funkcje głównej pętli
void StateManager_HandleInput(GameStateManager *Manager, EFI_INPUT_KEY *Key);
void StateManager_Update(GameStateManager *Manager);
void StateManager_Draw(GameStateManager *Manager);

#endif // GAME_STATE_H