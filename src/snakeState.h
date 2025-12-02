#ifndef SNAKE_STATE_H
#define SNAKE_STATE_H

#include "gameState.h"


#define SNAKE_SEGMENT_SIZE 16
#define SNAKE_MAX_LENGTH 100

#define SNAKE_GRID_WIDTH  40 
#define SNAKE_GRID_HEIGHT 40

typedef struct Point {
    int x;
    int y;
} Point;


typedef struct snakeState
{
    GameState base;

    //Data specific to snake state
    Point body[SNAKE_MAX_LENGTH];
    int snakeLength;

    Point food;

    int snakeDX;
    int snakeDY;

    int boardOffsetX;
    int boardOffsetY;
} snakeState;


//constructor
void SnakeState_Init(snakeState *State);


#endif // SNAKE_STATE_H