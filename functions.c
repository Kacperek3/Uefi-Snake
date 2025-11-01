#include "functions.h"

// --- Definicje globalnych wskaźników ---
EFI_BOOT_SERVICES *gBS = NULL;
EFI_GRAPHICS_OUTPUT_PROTOCOL *gGOP = NULL;

// ... (tutaj funkcje dodaj i PrintInt) ...

/**
 * Inicjalizuje tryb graficzny poprzez znalezienie protokołu GOP.
 */
EFI_STATUS GraphicsInit(EFI_SYSTEM_TABLE *SystemTable) {
    // 1. Zapisz globalny wskaźnik na Boot Services
    gBS = SystemTable->BootServices;

    // 2. Definicja GUID dla Graphics Output Protocol
    // Można ją znaleźć w specyfikacji UEFI
    EFI_GUID GopGuid = {0x9042a9de,0x23dc,0x4a38,\
    {0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}};

    EFI_STATUS Status;

    // 3. Znajdź protokół
    // Przekazujemy adres wskaźnika gGOP, aby usługa
    // mogła zapisać w nim adres znalezionego protokołu.
    Status = gBS->LocateProtocol(&GopGuid, NULL, (VOID**)&gGOP);

    if (Status != EFI_SUCCESS) {
        // Nie udało się znaleźć protokołu graficznego
        SystemTable->ConOut->OutputString(SystemTable->ConOut, 
            u"Nie mozna zlokalizowac GOP (Graphics Output Protocol).\r\n");
        gGOP = NULL; // Upewnij się, że jest NULL w razie błędu
    } else {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, 
            u"Protokol graficzny (GOP) znaleziony!\r\n");
        // (Opcjonalnie: można tutaj ustawić konkretny tryb graficzny
        // wywołując gGOP->SetMode(), ale domyślny zwykle działa)
    }

    return Status;
}

/**
 * Rysuje pojedynczy piksel na ekranie.
 */
void DrawPixel(UINTN x, UINTN y, UINT32 Color) {
    // Sprawdź, czy GOP został poprawnie zainicjowany
    // i czy jesteśmy w granicach ekranu
    if (gGOP == NULL || 
        x >= gGOP->Mode->Info->HorizontalResolution || 
        y >= gGOP->Mode->Info->VerticalResolution) {
        return;
    }

    // Pobierz adres bufora ramki (framebuffer)
    // Jest to adres w pamięci, gdzie zaczyna się obraz ekranu
    UINT32* FrameBuffer = (UINT32*)gGOP->Mode->FrameBufferBase;

    // Oblicz adres piksela:
    // adres = baza + (y * liczba_pikseli_w_linii) + x
    // WAŻNE: Używamy PixelsPerScanLine, a NIE HorizontalResolution!
    // Czasem mogą być różne (np. dla wyrównania pamięci).
    UINTN Pitch = gGOP->Mode->Info->PixelsPerScanLine;
    
    FrameBuffer[y * Pitch + x] = Color;
}

/**
 * Rysuje prostokąt o zadanych współrzędnych i rozmiarze.
 *
 * @param x         Współrzędna X lewego górnego rogu.
 * @param y         Współrzędna Y lewego górnego rogu.
 * @param width     Szerokość prostokąta w pikselach.
 * @param height    Wysokość prostokąta w pikselach.
 * @param Color     Kolor prostokąta (format 0xAABBGGRR).
 */
void DrawRectangle(UINTN x, UINTN y, UINTN width, UINTN height, UINT32 Color) {
    if (gGOP == NULL) return;

    // Rysuj wiersz po wierszu
    for (UINTN currentY = y; currentY < y + height; ++currentY) {
        for (UINTN currentX = x; currentX < x + width; ++currentX) {
            DrawPixel(currentX, currentY, Color);
        }
    }
}

/**
 * Rysuje obrazek (sprite) na ekranie.
 *
 * @param startX       Współrzędna X lewego górnego rogu sprite'a.
 * @param startY       Współrzędna Y lewego górnego rogu sprite'a.
 * @param spriteWidth  Szerokość sprite'a w pikselach.
 * @param spriteHeight Wysokość sprite'a w pikselach.
 * @param spriteData   Wskaźnik na tablicę kolorów pikseli sprite'a.
 */
void DrawSprite(UINTN startX, UINTN startY, UINTN spriteWidth, UINTN spriteHeight, const UINT32* spriteData) {
    if (gGOP == NULL || spriteData == NULL) return;

    for (UINTN y = 0; y < spriteHeight; ++y) {
        for (UINTN x = 0; x < spriteWidth; ++x) {
            UINT32 color = spriteData[y * spriteWidth + x];
            if (color != TRANSPARENT_COLOR) { // Rysuj tylko, jeśli piksel nie jest "przezroczysty"
                DrawPixel(startX + x, startY + y, color);
            }
        }
    }
}

#define T TRANSPARENT_COLOR
#define H SNAKE_HEAD_COLOR
#define E SNAKE_EYE_COLOR
#define P SNAKE_PUPIL_COLOR
#define O SNAKE_TONGUE_COLOR
#define N SNAKE_NOSTRIL_COLOR

const UINT32 gSnakeHeadSprite[SNAKE_HEAD_SPRITE_SIZE * SNAKE_HEAD_SPRITE_SIZE] = {
    //  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  (Kolumny)
    // --- Wiersz 0 ---
    T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,
    // --- Wiersz 1 ---
    H, H, H, H, H, H, H, H, H, H, H, H, T, T, T, T,
    // --- Wiersz 2 ---
    H, H, H, H, H, H, H, H, H, H, H, H, H, H, T, T,
    // --- Wiersz 3 ---
    H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, T,
    // --- Wiersz 4 ---
    H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, T,
    // --- Wiersz 5 ---
    H, H, H, H, H, E, P, H, H, H, H, E, P, H, H, H,
    // --- Wiersz 6 ---
    H, H, H, H, H, E, P, H, H, H, H, E, P, H, H, H,
    // --- Wiersz 7 ---
    H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H,
    // --- Wiersz 8 ---
    H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, H,
    // --- Wiersz 9 ---
    H, H, H, H, H, N, H, H, H, H, N, H, H, H, H, T,
    // --- Wiersz 10 ---
    H, H, H, H, H, H, H, H, H, H, H, H, H, H, H, T,
    // --- Wiersz 11 ---
    H, H, H, H, H, H, H, H, H, H, H, H, H, H, T, T,
    // --- Wiersz 12 ---
    H, H, H, H, H, H, H, H, O, O, H, H, H, T, T, T,
    // --- Wiersz 13 ---
    H, H, H, H, H, H, H, H, O, O, H, H, T, T, T, T,
    // --- Wiersz 14 ---
    H, H, H, H, H, H, H, H, O, H, H, T, T, T, T, T,
    // --- Wiersz 15 ---
    T, T, T, T, T, T, T, T, O, H, T, T, T, T, T, T
};

// Usuń definicje skrótów, aby nie "wyciekły" do innych plików
#undef T
#undef H
#undef E
#undef P
#undef O
#undef N

// Funkcja DrawSnake (bez zmian)
void DrawSnake(UINTN segmentSize, UINTN headGridX, UINTN headGridY, UINTN length, UINT32 bodyColor) {
    if (gGOP == NULL || length == 0) return;

    // Współrzędne głowy (w pikselach)
    UINTN pixelHeadX = headGridX * segmentSize;
    UINTN pixelHeadY = headGridY * segmentSize;

    // Narysuj głowę jako sprite
    DrawSprite(pixelHeadX, pixelHeadY, SNAKE_HEAD_SPRITE_SIZE, SNAKE_HEAD_SPRITE_SIZE, gSnakeHeadSprite);

    // ... (reszta funkcji DrawSnake jest taka sama jak poprzednio, 
    //      rysując ciało za pomocą DrawRectangle) ...
    // Narysuj resztę ciała (idąc "wstecz" od głowy)
    for (UINTN i = 1; i < length; ++i) {
        UINTN currentSegmentX;
        UINTN currentSegmentY;

        // Tutaj nadal definiujesz kształt ciała
        if (i <= 2) { // Pierwsze dwa segmenty ciała idą w lewo
            currentSegmentX = headGridX - i;
            currentSegmentY = headGridY;
        } else if (i == 3) { // Kolejny segment w dół
            currentSegmentX = headGridX - 2;
            currentSegmentY = headGridY + 1;
        } else { // Ostatni segment w dół
            currentSegmentX = headGridX - 2;
            currentSegmentY = headGridY + 2;
        }
        
        UINTN pixelSegmentX = currentSegmentX * segmentSize;
        UINTN pixelSegmentY = currentSegmentY * segmentSize;

        if (pixelSegmentX + segmentSize > gGOP->Mode->Info->HorizontalResolution ||
            pixelSegmentY + segmentSize > gGOP->Mode->Info->VerticalResolution) {
            break; 
        }

        // Rysuj ciało jako kwadrat
        DrawRectangle(pixelSegmentX, pixelSegmentY, segmentSize, segmentSize, bodyColor);
    }
}