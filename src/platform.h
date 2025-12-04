// platform.h

#ifndef PLATFORM_H
#define PLATFORM_H

#include "efi.h"


typedef struct PlatformContext {
    EFI_BOOT_SERVICES               *BS;
    EFI_GRAPHICS_OUTPUT_PROTOCOL    *GOP;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *ConIn;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_RUNTIME_SERVICES            *RT;

    UINTN                           ScreenWidth;
    UINTN                           ScreenHeight;
} PlatformContext;


EFI_STATUS Platform_Init(
    PlatformContext *Platform,
    EFI_SYSTEM_TABLE *SystemTable
);

void Platform_DrawPixel(
    PlatformContext *Platform,
    UINTN x,
    UINTN y,
    UINT32 Color
);

void Platform_Sleep(
    PlatformContext *Platform,
    UINTN milliseconds
);


EFI_STATUS Platform_PollKey(
    PlatformContext *Platform,
    EFI_INPUT_KEY *Key
);

EFI_STATUS Platform_EnableGraphicsMode(
    PlatformContext *Platform
);

EFI_STATUS Platform_EnableTextMode(
    PlatformContext *Platform
);

#endif // PLATFORM_H