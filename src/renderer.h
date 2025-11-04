#ifndef RENDERER_H
#define RENDERER_H

#include "platform.h"

void ClearScreen(PlatformContext *Platform, UINT32 Color);
void DrawRectangle(PlatformContext *Platform, UINTN x, UINTN y, UINTN width, UINTN height, UINT32 Color);
void DrawSprite(PlatformContext *Platform, UINTN startX, UINTN startY, UINTN spriteWidth, UINTN spriteHeight, const UINT32* spriteData);
void DrawFrame(PlatformContext *Platform, UINTN x, UINTN y, UINTN width, UINTN height, UINTN thickness, UINT32 Color);




#endif // RENDERER_H