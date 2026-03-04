#ifndef _WINDOWS_LOGO_H
#define _WINDOWS_LOGO_H

#include "efi.h"
#include "platform.h"

#define BG_RED (EFI_RED << 4)
#define BG_GREEN (EFI_GREEN << 4)
#define BG_BLUE (EFI_BLUE << 4)
#define BG_YELLOW ((EFI_RED | EFI_GREEN) << 4)

static void DrawTile(PlatformContext *Platform, UINTN x, UINTN y, UINTN colorAttr) {
    Platform->ConOut->SetAttribute(Platform->ConOut, colorAttr); // Ustawiamy tło

    for (int i = 0; i < 2; i++) {
        Platform->ConOut->SetCursorPosition(Platform->ConOut, x, y + i);
        Platform->ConOut->OutputString(Platform->ConOut, L"      ");
    }
}

static void DrawWindowsLogo(PlatformContext *Platform, UINTN x, UINTN y) {

    UINTN w = 6;
    UINTN h = 2;

    DrawTile(Platform, x, y, BG_RED);

    DrawTile(Platform, x + w, y, BG_GREEN);

    DrawTile(Platform, x, y + h, BG_BLUE);

    DrawTile(Platform, x + w, y + h, BG_YELLOW);

    Platform->ConOut->SetAttribute(Platform->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    Platform->ConOut->SetCursorPosition(Platform->ConOut, x + 3, y + (h * 2) + 1);
    Platform->ConOut->OutputString(Platform->ConOut, L"WINDOWS");
}

#endif //_WINDOWS_LOGO_H
