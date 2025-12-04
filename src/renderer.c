#include "renderer.h"



void ClearScreen(PlatformContext *Platform, UINT32 Color) {
    DrawRectangle(Platform, 0, 0, Platform->ScreenWidth, Platform->ScreenHeight, Color);
}

void DrawRectangle(PlatformContext *Platform, UINTN x, UINTN y, UINTN width, UINTN height, UINT32 Color) {
    if (Platform->GOP == NULL) return;

    for (UINTN row = 0; row < height; ++row) {
        for (UINTN col = 0; col < width; ++col) {
            UINTN currentX = x + col;
            UINTN currentY = y + row;

            if (currentX >= Platform->ScreenWidth || currentY >= Platform->ScreenHeight) {
                continue;
            }

            Platform_DrawPixel(Platform, currentX, currentY, Color);
        }
    }
}
// renderer.c

void DrawSnakeHead(PlatformContext *Platform, UINTN x, UINTN y, UINTN size) {
    DrawRectangle(Platform, x, y, size, size, 0x0000FF00);

    UINT32 eyeColor = 0x00FF0000;
    
    UINTN eyeSize = size / 4;
    if (eyeSize == 0) eyeSize = 1;

    UINTN margin = size / 5;

    DrawRectangle(Platform, 
                  x + margin,          
                  y + margin,           
                  eyeSize, eyeSize,
                  eyeColor);

    DrawRectangle(Platform, 
                  x + size - margin - eyeSize, 
                  y + margin,                  
                  eyeSize, eyeSize, 
                  eyeColor);
}


void DrawFrame(PlatformContext *Platform, UINTN x, UINTN y, UINTN width, UINTN height, UINTN thickness, UINT32 Color) {
    if (Platform->GOP == NULL) return;
    
    
    //horizontal
    for (UINTN t = 0; t < thickness; ++t) {
        DrawRectangle(Platform, x, y + t, width, 1, Color);
        DrawRectangle(Platform, x, y + height - 1 - t, width, 1, Color);
    }

    // vertical
    for (UINTN t = 0; t < thickness; ++t) {
        DrawRectangle(Platform, x + t, y, 1, height, Color);
        DrawRectangle(Platform, x + width - 1 - t, y, 1, height, Color);
    }
}