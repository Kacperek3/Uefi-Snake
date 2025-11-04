#include "platform.h"


EFI_STATUS Platform_Init(PlatformContext *Platform, EFI_SYSTEM_TABLE *SystemTable) {
    
    Platform->BS = SystemTable->BootServices;
    Platform->ConIn = SystemTable->ConIn;
    Platform->ConOut = SystemTable->ConOut;
    Platform->RT = SystemTable->RuntimeServices; 

    // initial values
    Platform->GOP = NULL;
    Platform->ScreenWidth = 0;
    Platform->ScreenHeight = 0;
    
    // Set text mode by default
    return Platform_EnableTextMode(Platform);
}

void Platform_DrawPixel(PlatformContext *Platform, UINTN x, UINTN y, UINT32 Color) {
    if (Platform->GOP == NULL || 
        x >= Platform->ScreenWidth || 
        y >= Platform->ScreenHeight) {
        return;
    }

    UINT32* FrameBuffer = (UINT32*)Platform->GOP->Mode->FrameBufferBase;
    UINTN Pitch = Platform->GOP->Mode->Info->PixelsPerScanLine;
    FrameBuffer[y * Pitch + x] = Color;
}


void Platform_Sleep(PlatformContext *Platform, UINTN milliseconds) {
    Platform->BS->Stall(milliseconds * 1000);
}

EFI_STATUS Platform_PollKey(PlatformContext *Platform, EFI_INPUT_KEY *Key) {
    EFI_STATUS Status = Platform->BS->CheckEvent(Platform->ConIn->WaitForKey);
    if (Status == EFI_SUCCESS) {
        return Platform->ConIn->ReadKeyStroke(Platform->ConIn, Key);
    }
    return Status;
}

EFI_STATUS Platform_EnableGraphicsMode(PlatformContext *Platform) {
    // Znajdź GOP (jeśli jeszcze go nie mamy)
    if (Platform->GOP == NULL) {
        EFI_GUID GopGuid = {0x9042a9de,0x23dc,0x4a38,\
        {0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}};
        
        EFI_STATUS Status = Platform->BS->LocateProtocol(&GopGuid, NULL, (VOID**)&Platform->GOP);
        if (Status != EFI_SUCCESS) {
            return Status; // Nie udało się znaleźć GOP
        }
    }

    // Odczytaj wymiary (GOP sam ustawił domyślny tryb)
    Platform->ScreenWidth = Platform->GOP->Mode->Info->HorizontalResolution;
    Platform->ScreenHeight = Platform->GOP->Mode->Info->VerticalResolution;
    
    return EFI_SUCCESS;
}

EFI_STATUS Platform_EnableTextMode(PlatformContext *Platform) {
    EFI_STATUS Status = Platform->ConOut->Reset(Platform->ConOut, FALSE);
    if (Status == EFI_SUCCESS) {
        Platform->GOP = NULL;
        Platform->ScreenWidth = 0;
        Platform->ScreenHeight = 0;
    }
    return Status;
}