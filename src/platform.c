#include "platform.h"


EFI_STATUS Platform_Init(PlatformContext *Platform, EFI_SYSTEM_TABLE *SystemTable) {
    
    Platform->BS = SystemTable->BootServices;
    Platform->ConIn = SystemTable->ConIn;
    Platform->ConOut = SystemTable->ConOut;

    // Locate Graphics Output Protocol (GOP)
    EFI_GUID GopGuid = {0x9042a9de,0x23dc,0x4a38,\
    {0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}};


    EFI_STATUS Status = Platform->BS->LocateProtocol(&GopGuid, NULL, (VOID**)&Platform->GOP);

    if (Status != EFI_SUCCESS) {
        return Status;
    }

    // Take screen dimensions from GOP mode info
    Platform->ScreenWidth = Platform->GOP->Mode->Info->HorizontalResolution;
    Platform->ScreenHeight = Platform->GOP->Mode->Info->VerticalResolution;
    
    return EFI_SUCCESS;
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
    //Platform->BS->Stall(milliseconds * 1000);
    return;
}

EFI_STATUS Platform_PollKey(PlatformContext *Platform, EFI_INPUT_KEY *Key) {
    EFI_STATUS Status = 0; //Platform->BS->CheckEvent(Platform->ConIn->WaitForKey);
    if (Status == EFI_SUCCESS) {
        return Platform->ConIn->ReadKeyStroke(Platform->ConIn, Key);
    }
    return Status;
}
