#include "efi.h"
#include "functions.h"

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    (void) ImageHandle;

    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Start programu...\r\n");

    EFI_STATUS Status = GraphicsInit(SystemTable);

    if (Status != EFI_SUCCESS) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Blad inicjalizacji grafiki.\r\n");
    } else {
        UINT32 Czarny = 0x00000000;
        UINT32 ZielonyJasnY = 0x0000FF00; // Zielony

        DrawRectangle(0, 0, 
                      gGOP->Mode->Info->HorizontalResolution, 
                      gGOP->Mode->Info->VerticalResolution, 
                      Czarny);

        // --- Rysowanie węża ---
        UINTN segmentSize = SNAKE_HEAD_SPRITE_SIZE; // Używamy rozmiaru sprite'a jako rozmiaru segmentu
        UINTN headGridX = 30;   // Pozycja głowy w "segmentach"
        UINTN headGridY = 20;

        // Narysuj węża!
        // Zauważ, że nie przekazujemy już headColor
        DrawSnake(segmentSize, headGridX, headGridY, 5, ZielonyJasnY);
    }

    // Czekaj na klawisz przed wyjściem
    EFI_INPUT_KEY Key;
    while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) != EFI_SUCCESS)
        ;

    SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

    return EFI_SUCCESS;
}