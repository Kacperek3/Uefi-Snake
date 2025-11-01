#include "efi.h"
#include "platform.h"
#include "renderer.h"

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    (void) ImageHandle;

    PlatformContext Platform;
    EFI_STATUS Status = Platform_Init(&Platform, SystemTable);
    ClearScreen(&Platform, 0x00000000); // Czarny ekran
    DrawRectangle(&Platform, 50, 50, 200, 100, 0x0000FF00); // Zielony prostokąt
    

    // Czekaj na klawisz przed wyjściem
    EFI_INPUT_KEY Key;
    while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) != EFI_SUCCESS)
        ;

    SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

    return EFI_SUCCESS;
}