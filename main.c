#include "efi.h"

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    (void) ImageHandle;

    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_YELLOW | EFI_GREEN);

    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Hello, UEFI World!\r\n");

    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_RED | EFI_BLACK);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Press any key to continue...\r\n");

    EFI_INPUT_KEY Key;
    while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) != EFI_SUCCESS)
        ;

    SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

    return EFI_SUCCESS;
}