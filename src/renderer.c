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