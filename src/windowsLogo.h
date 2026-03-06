#ifndef WINDOWS_LOGO_H
#define WINDOWS_LOGO_H

#include "art.h" // Upewnij się, że masz to zainkludowane, żeby widzieć WindowsAsset i WindowsLines
#include "platform.h"
#include <efi.h>

static void DrawWindowsLogo(PlatformContext *Platform, UINTN startCol, UINTN startRow) {
    // Mapa cięcia lewej i prawej strony
    UINTN leftTileEnd[11] = {15, 12, 13, 13, 13, 12, 11, 11, 11, 10, 10};

    for (UINTN line = 0; line < WindowsLines; line++) {
        // Ustawiamy kursor na początek linii
        Platform->ConOut->SetCursorPosition(Platform->ConOut, startCol, startRow + line);

        // Pobieramy całą linię (np. 18 znaków)
        const CHAR16 *rowStr = WindowsAsset[line];

        // Zmienne na bufory dla lewej i prawej strony
        CHAR16 leftPart[40] = {0};
        CHAR16 rightPart[40] = {0};

        // Wyliczamy, na którym indeksie tniemy stringa
        UINTN cutIndex = leftTileEnd[line] + 1; // +1 bo leftTileEnd to ostatni indeks lewej strony

        // 1. Kopiujemy lewą część
        UINTN i;
        for (i = 0; i < cutIndex && rowStr[i] != L'\0'; i++) {
            leftPart[i] = rowStr[i];
        }
        leftPart[i] = L'\0'; // Zamykamy stringa

        // 2. Kopiujemy prawą część
        UINTN j = 0;
        for (; rowStr[i] != L'\0'; i++, j++) {
            rightPart[j] = rowStr[i];
        }
        rightPart[j] = L'\0'; // Zamykamy stringa

        // --- KOLORY LEWEJ STRONY ---
        UINTN leftColor = (line <= 4) ? EFI_RED : EFI_BLUE;
        Platform->ConOut->SetAttribute(Platform->ConOut, leftColor | EFI_BACKGROUND_BLACK);
        Platform->ConOut->OutputString(Platform->ConOut, leftPart);

        // --- KOLORY PRAWEJ STRONY ---
        UINTN rightColor = (line <= 5) ? EFI_GREEN : EFI_YELLOW;
        Platform->ConOut->SetAttribute(Platform->ConOut, rightColor | EFI_BACKGROUND_BLACK);
        Platform->ConOut->OutputString(Platform->ConOut, rightPart);
    }

    // Zawsze posprzątaj po sobie na końcu!
    Platform->ConOut->SetAttribute(Platform->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
}

#endif
