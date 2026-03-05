#include "snakeState.h"
#include "bootArch.h"
#include "renderer.h"

#define POINTS_TO_BOOT 15

static unsigned int g_seed = 55;
int pseudo_rand(void) {
    g_seed = g_seed * 1103515245 + 12345;
    return (g_seed / 65536) % 32768;
}

static void Snake_PlaceFood(snakeState *state) {
    state->food.x = pseudo_rand() % state->boardSizeX;
    state->food.y = pseudo_rand() % state->boardSizeY;
}

static void snakeState_Enter(GameState *self, PlatformContext *Platform) {
    snakeState *state = (snakeState *)self;

    Platform_EnableGraphicsMode(Platform);
    ClearScreen(Platform, 0x000000);

    if (Platform->Level == EASY) {
        state->boardSizeX = 40;
        state->boardSizeY = 40;
    } else if (Platform->Level == MEDIUM) {
        state->boardSizeX = 30;
        state->boardSizeY = 30;
    } else {
        state->boardSizeX = 20;
        state->boardSizeY = 20;
    }

    int boardWidthPixels = state->boardSizeX * SNAKE_SEGMENT_SIZE;
    int boardHeightPixels = state->boardSizeY * SNAKE_SEGMENT_SIZE;

    // Perfect board centralization
    state->boardOffsetX = (Platform->ScreenWidth - boardWidthPixels) / 2;
    state->boardOffsetY = (Platform->ScreenHeight - boardHeightPixels) / 2;

    // Defaulght lenght is 3
    state->snakeLength = 3;
    state->body[0].x = state->boardSizeX / 2;
    state->body[0].y = state->boardSizeY / 2;
    state->body[1].x = (state->boardSizeX / 2) - 1;
    state->body[1].y = state->boardSizeY / 2;
    state->body[2].x = (state->boardSizeX / 2) - 2;
    state->body[2].y = state->boardSizeY / 2;
    // Snake start to moving right by deafault
    state->snakeDX = 1;
    state->snakeDY = 0;

    Snake_PlaceFood(state);
}

static void snakeState_Exit(GameState *self, PlatformContext *Platform) {}

static void snakeState_HandleInput(GameState *self, PlatformContext *Platform, EFI_INPUT_KEY *Key,
                                   GameStateManager *Manager) {
    snakeState *state = (snakeState *)self;

    if (Key) {
        if (Key->UnicodeChar == u'q') {
            StateManager_ChangeState(Manager, Manager->menuState);
        }

        if (Key->UnicodeChar == u'w' && state->snakeDY == 0) {
            state->snakeDX = 0;
            state->snakeDY = -1;
        } else if (Key->UnicodeChar == u's' && state->snakeDY == 0) {
            state->snakeDX = 0;
            state->snakeDY = 1;
        } else if (Key->UnicodeChar == u'a' && state->snakeDX == 0) {
            state->snakeDX = -1;
            state->snakeDY = 0;
        } else if (Key->UnicodeChar == u'd' && state->snakeDX == 0) {
            state->snakeDX = 1;
            state->snakeDY = 0;
        }
    }
}

static void snakeState_Update(GameState *self, PlatformContext *Platform,
                              GameStateManager *Manager) {
    snakeState *state = (snakeState *)self;

    Point nextHead;
    nextHead.x = state->body[0].x + state->snakeDX;
    nextHead.y = state->body[0].y + state->snakeDY;

    // Check collision with the wall
    if (nextHead.x < 0 || nextHead.x >= state->boardSizeX || nextHead.y < 0 ||
        nextHead.y >= state->boardSizeY) {
        StateManager_ChangeState(Manager, Manager->menuState); // game over
        return;
    }

    // Check collision with myself
    for (int i = 1; i < state->snakeLength; i++) {
        if (nextHead.x == state->body[i].x && nextHead.y == state->body[i].y) {
            StateManager_ChangeState(Manager, Manager->menuState); // game over
            return;
        }
    }

    // Check if snake ate
    BOOLEAN ateFood = FALSE;
    if (nextHead.x == state->food.x && nextHead.y == state->food.y) {
        ateFood = TRUE;
        if (state->snakeLength < SNAKE_MAX_LENGTH) {
            state->snakeLength++; // Snake grows
        }
        int currentScore = state->snakeLength - 3;
        if (currentScore >= POINTS_TO_BOOT) {
            Platform_EnableTextMode(Platform);
            Platform->ConOut->ClearScreen(Platform->ConOut);

            LoadExt2Driver(Platform->ImageHandle, Platform->SystemTable);
            FindAndBootArch(Platform->ImageHandle, Platform->SystemTable);

            StateManager_ChangeState(Manager, Manager->menuState);
            return;
        }
        Snake_PlaceFood(state); // Place new food
    }

    // moving the snake body
    if (!ateFood) {
        // Move segments from tail to head
        for (int i = state->snakeLength - 1; i > 0; i--) {
            state->body[i] = state->body[i - 1];
        }
    } else {
        // Right i dont support dynamic memory management so the loop is the same
        for (int i = state->snakeLength - 1; i > 0; i--) {
            state->body[i] = state->body[i - 1];
        }
    }

    // Lastly we set a head to new point
    state->body[0] = nextHead;
}

static void snakeState_Draw(GameState *self, PlatformContext *Platform) {
    snakeState *state = (snakeState *)self;

    int frameWidth = state->boardSizeX * SNAKE_SEGMENT_SIZE;
    int frameHeight = state->boardSizeY * SNAKE_SEGMENT_SIZE;

    DrawRectangle(Platform, state->boardOffsetX, state->boardOffsetY, frameWidth, frameHeight,
                  0x00000000);
    DrawFrame(Platform, state->boardOffsetX - 2, state->boardOffsetY - 2, frameWidth + 4,
              frameHeight + 4, 2, 0x00AAAAAA);
    // ------------

    // Draw food and
    DrawRectangle(Platform, state->boardOffsetX + (state->food.x * SNAKE_SEGMENT_SIZE),
                  state->boardOffsetY + (state->food.y * SNAKE_SEGMENT_SIZE), SNAKE_SEGMENT_SIZE,
                  SNAKE_SEGMENT_SIZE, 0x000000FF);

    // Draw snake
    for (int i = 0; i < state->snakeLength; i++) {
        UINTN posX = state->boardOffsetX + (state->body[i].x * SNAKE_SEGMENT_SIZE);
        UINTN posY = state->boardOffsetY + (state->body[i].y * SNAKE_SEGMENT_SIZE);

        if (i == 0) {
            DrawSnakeHead(Platform, posX, posY, SNAKE_SEGMENT_SIZE);
        } else {
            DrawRectangle(Platform, posX, posY, SNAKE_SEGMENT_SIZE, SNAKE_SEGMENT_SIZE, 0x00099000);
        }
    }
    int currentScore = state->snakeLength - 3;
    if (currentScore < 0)
        currentScore = 0;

    int barMaxWidth = Platform->ScreenWidth / 2;
    int barHeight = 20;
    int barX = (Platform->ScreenWidth - barMaxWidth) / 2;
    int barY = 30;

    DrawRectangle(Platform, barX, barY, barMaxWidth, barHeight, 0x00000000);
    DrawFrame(Platform, barX - 2, barY - 2, barMaxWidth + 4, barHeight + 4, 2, 0x00FFFFFF);

    int fillWidth = (currentScore * barMaxWidth) / POINTS_TO_BOOT;
    if (fillWidth > barMaxWidth)
        fillWidth = barMaxWidth;

    if (fillWidth > 0) {
        DrawRectangle(Platform, barX, barY, fillWidth, barHeight, 0x001793D1);
    }
}

// constructor
void SnakeState_Init(snakeState *State) {
    // Set function pointers
    State->base.Enter = snakeState_Enter;
    State->base.Exit = snakeState_Exit;
    State->base.HandleInput = snakeState_HandleInput;
    State->base.Update = snakeState_Update;
    State->base.Draw = snakeState_Draw;
}
