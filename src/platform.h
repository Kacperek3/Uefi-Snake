// platform.h

#ifndef PLATFORM_H
#define PLATFORM_H

#include <efi.h>
#include <efifs.h>
#include <efilib.h>
#include <efistdarg.h>

typedef struct PlatformContext {
    EFI_HANDLE ImageHandle;
    EFI_SYSTEM_TABLE *SystemTable;
    EFI_BOOT_SERVICES *BS;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
    SIMPLE_INPUT_INTERFACE *ConIn;
    EFI_SIMPLE_TEXT_OUT_PROTOCOL *ConOut;
    EFI_RUNTIME_SERVICES *RT;

    UINTN ScreenWidth;
    UINTN ScreenHeight;

    int Level;
} PlatformContext;

EFI_STATUS Platform_Init(PlatformContext *Platform, EFI_SYSTEM_TABLE *SystemTable);

void Platform_DrawPixel(PlatformContext *Platform, UINTN x, UINTN y, UINT32 Color);

void Platform_Sleep(PlatformContext *Platform, UINTN milliseconds);

EFI_STATUS Platform_PollKey(PlatformContext *Platform, EFI_INPUT_KEY *Key);

EFI_STATUS Platform_EnableGraphicsMode(PlatformContext *Platform);

EFI_STATUS Platform_EnableTextMode(PlatformContext *Platform);

#endif // PLATFORM_H
