#include "snakeState.h"
#include "renderer.h"
//#include "menuState.h"




static unsigned int g_seed = 55;
int pseudo_rand(void) {
    g_seed = g_seed * 1103515245 + 12345;
    return (g_seed / 65536) % 32768;
}

static void Snake_PlaceFood(snakeState *state) {
    // Simple random position generation
    state->food.x = pseudo_rand() % SNAKE_GRID_WIDTH;
    state->food.y = pseudo_rand() % SNAKE_GRID_HEIGHT;

    // TODO: Check if food didn't appear on the snake
}

static void snakeState_Enter(GameState *self, PlatformContext *Platform) {
    snakeState *state = (snakeState*)self;
    // Initialize snake state here
    Platform_EnableGraphicsMode(Platform);
    ClearScreen(Platform, 0x000000);


    int boardWidthPixels = SNAKE_GRID_WIDTH * SNAKE_SEGMENT_SIZE;
    int boardHeightPixels = SNAKE_GRID_HEIGHT * SNAKE_SEGMENT_SIZE;


    state->boardOffsetX = (Platform->ScreenWidth - boardWidthPixels) / 2;
    state->boardOffsetY = (Platform->ScreenHeight - boardHeightPixels) / 2;

    state->snakeLength = 3;
    state->body[0].x = SNAKE_GRID_WIDTH / 2;     
    state->body[0].y = SNAKE_GRID_HEIGHT / 2;
    state->body[1].x = SNAKE_GRID_WIDTH / 2 - 1;
    state->body[1].y = SNAKE_GRID_HEIGHT / 2;
    state->body[2].x = SNAKE_GRID_WIDTH / 2 - 2; 
    state->body[2].y = SNAKE_GRID_HEIGHT / 2;

    state->snakeDX = 1;  
    state->snakeDY = 0;

    Snake_PlaceFood(state);
}



static void snakeState_Exit(GameState *self, PlatformContext *Platform) {
    // To be implemented
}

static void snakeState_HandleInput(GameState *self, PlatformContext *Platform, EFI_INPUT_KEY *Key, GameStateManager *Manager) {
    snakeState *state = (snakeState*)self;
    
    if (Key) { 
        if (Key->UnicodeChar == u'q') {
            StateManager_ChangeState(Manager, Manager->menuState); 
        }

        if (Key->UnicodeChar == u'w' && state->snakeDY == 0) {
            state->snakeDX = 0;
            state->snakeDY = -1;
        }
        else if (Key->UnicodeChar == u's' && state->snakeDY == 0) {
            state->snakeDX = 0;
            state->snakeDY = 1;
        }
        else if (Key->UnicodeChar == u'a' && state->snakeDX == 0) {
            state->snakeDX = -1;
            state->snakeDY = 0;
        }
        else if (Key->UnicodeChar == u'd' && state->snakeDX == 0) {
            state->snakeDX = 1;
            state->snakeDY = 0;
        }
    }
}


static void snakeState_Update(GameState *self, PlatformContext *Platform, GameStateManager *Manager) {
    snakeState *state = (snakeState*)self;


    Point nextHead;
    nextHead.x = state->body[0].x + state->snakeDX;
    nextHead.y = state->body[0].y + state->snakeDY;

    // 2. Check collision with walls
    if (nextHead.x < 0 || nextHead.x >= SNAKE_GRID_WIDTH ||
        nextHead.y < 0 || nextHead.y >= SNAKE_GRID_HEIGHT) {
        StateManager_ChangeState(Manager, Manager->menuState); // Game over
        return;
    }

    // 3. Check collision with itself
    for (int i = 1; i < state->snakeLength; i++) {
        if (nextHead.x == state->body[i].x && nextHead.y == state->body[i].y) {
            StateManager_ChangeState(Manager, Manager->menuState); // Game over
            return;
        }
    }

    // 4. Check if snake ate food
    BOOLEAN ateFood = FALSE;
    if (nextHead.x == state->food.x && nextHead.y == state->food.y) {
        ateFood = TRUE;
        if (state->snakeLength < SNAKE_MAX_LENGTH) {
            state->snakeLength++; // Snake grows!
        }
        Snake_PlaceFood(state); // Place new food
    }

    // 5. Move snake body
    // If snake ate food, the tail "stays in place" (we don't move it)
    // If it didn't eat, we need to move the entire body
    
    // Save tail position (before overwriting it)
    //Point lastTail = state->body[state->snakeLength - 1];

    if (!ateFood) {
        // Move segments from tail to head
        for (int i = state->snakeLength - 1; i > 0; i--) {
            state->body[i] = state->body[i - 1];
        }
    } else {
        // If it ate, body grows, so we need to "make room"
        // in the array for the new head by shifting everything
        for (int i = state->snakeLength - 1; i > 0; i--) {
            state->body[i] = state->body[i - 1];
        }
    }
    // (In both cases above, the loop is the same, 
    // but logically we think that with "ateFood" the tail doesn't disappear)

    // 6. Set new head position
    state->body[0] = nextHead;
}


static void snakeState_Draw(GameState *self, PlatformContext *Platform) {
    snakeState *state = (snakeState*)self;
    ClearScreen(Platform, 0x00000000);


    int frameWidth = SNAKE_GRID_WIDTH * SNAKE_SEGMENT_SIZE;
    int frameHeight = SNAKE_GRID_HEIGHT * SNAKE_SEGMENT_SIZE;
    
    DrawFrame(Platform, 
              state->boardOffsetX - 2,
              state->boardOffsetY - 2,
              frameWidth + 4,
              frameHeight + 4,
              2, 
              0x00AAAAAA); 
    // ------------

    // 2. draw food and 
    DrawRectangle(Platform, 
                  state->boardOffsetX + (state->food.x * SNAKE_SEGMENT_SIZE), 
                  state->boardOffsetY + (state->food.y * SNAKE_SEGMENT_SIZE), 
                  SNAKE_SEGMENT_SIZE, SNAKE_SEGMENT_SIZE, 
                  0x000000FF); //red

    // 3. draw snake
    for (int i = 0; i < state->snakeLength; i++) {
        UINT32 color = 0x0000FF00; // green
        if (i == 0) {
            color = 0x0033FF33; // head
        }
        
        DrawRectangle(Platform, 
                      state->boardOffsetX + (state->body[i].x * SNAKE_SEGMENT_SIZE), 
                      state->boardOffsetY + (state->body[i].y * SNAKE_SEGMENT_SIZE), 
                      SNAKE_SEGMENT_SIZE, SNAKE_SEGMENT_SIZE, 
                      color);
    }
}

//constructor
void SnakeState_Init(snakeState *State) {
    // Set function pointers
    State->base.Enter = snakeState_Enter;
    State->base.Exit = snakeState_Exit;
    State->base.HandleInput = snakeState_HandleInput; 
    State->base.Update = snakeState_Update; 
    State->base.Draw = snakeState_Draw;   

    // Initialize snake-specific data here

}
