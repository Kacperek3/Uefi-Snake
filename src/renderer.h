#ifndef RENDERER_H
#define RENDERER_H

#include "platform.h"

void ClearScreen(PlatformContext *Platform, UINT32 Color);
void DrawRectangle(PlatformContext *Platform, UINTN x, UINTN y, UINTN width, UINTN height, UINT32 Color);
void DrawFrame(PlatformContext *Platform, UINTN x, UINTN y, UINTN width, UINTN height, UINTN thickness, UINT32 Color);
void DrawSnakeHead(PlatformContext *Platform, UINTN x, UINTN y, UINTN size); 
#endif // RENDERER_H