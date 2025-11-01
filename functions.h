#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "efi.h"

extern EFI_BOOT_SERVICES *gBS;
extern EFI_GRAPHICS_OUTPUT_PROTOCOL *gGOP;

// Nowe funkcje graficzne
EFI_STATUS GraphicsInit(EFI_SYSTEM_TABLE *SystemTable);
void DrawPixel(UINTN x, UINTN y, UINT32 Color);


void DrawPixel(UINTN x, UINTN y, UINT32 Color);
void DrawRectangle(UINTN x, UINTN y, UINTN width, UINTN height, UINT32 Color);

// Nowa funkcja do rysowania obrazków (sprite'ów)
void DrawSprite(UINTN startX, UINTN startY, UINTN spriteWidth, UINTN spriteHeight, const UINT32* spriteData);

// Możemy też od razu zdefiniować tablicę dla głowy węża
// (lub przenieść ją do functions.c, jeśli nie chcemy jej w h)
#define SNAKE_HEAD_SPRITE_SIZE 16 // Rozmiar sprite'a to 10x10 pikseli

#define TRANSPARENT_COLOR 0x00FFFFFF // Kolor, który będzie ignorowany
#define SNAKE_HEAD_COLOR  0x0000FFFF // Żółty
#define SNAKE_EYE_COLOR   0x000000FF // Czerwony
#define SNAKE_PUPIL_COLOR 0x00000000 // Czarny
#define SNAKE_TONGUE_COLOR 0x000050A5 // Brązowy (język)
#define SNAKE_NOSTRIL_COLOR 0x00008080 // Ciemnożółty/Oliwkowy (nozdrza)

// Dane sprite'a dla głowy węża (10x10 pikseli)
// Kolejność pikseli: wiersz po wierszu, od lewej do prawej.
// Musi to pasować do SNAKE_HEAD_SPRITE_SIZE * SNAKE_HEAD_SPRITE_SIZE elementów.
extern const UINT32 gSnakeHeadSprite[SNAKE_HEAD_SPRITE_SIZE * SNAKE_HEAD_SPRITE_SIZE];


// Zmieniona deklaracja DrawSnake, aby rysować sprite głowy
void DrawSnake(UINTN segmentSize, UINTN headGridX, UINTN headGridY, UINTN length, UINT32 bodyColor);

#endif