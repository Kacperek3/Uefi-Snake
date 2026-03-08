#ifndef ARCH_LOGO_H
#define ARCH_LOGO_H
#include "efi.h"
#include "platform.h"



#define ARCH_COLOR (EFI_BLUE | EFI_GREEN | EFI_BRIGHT)

static const CHAR16 *ArchLogo[] = {
    L"       /\\       ",
    L"      /  \\      ",
    L"     /    \\     ",
    L"    /      \\    ",
    L"   /   ,,   \\   ",
    L"  /   |  |  -\\  ",
    L" /_-''    ''-_\\ ",
    L"                ",
    L"   ARCH LINUX   "
};
static const UINTN ArchLogoHeight = 9;

static void DrawArchLogo(PlatformContext *Platform, UINTN x, UINTN y) {
    Platform->ConOut->SetAttribute(Platform->ConOut, ARCH_COLOR | EFI_BACKGROUND_BLACK);
    for (UINTN i = 0; i < ArchLogoHeight; i++) {
        Platform->ConOut->SetCursorPosition(Platform->ConOut, x, y + i);
        Platform->ConOut->OutputString(Platform->ConOut, (CHAR16*)ArchLogo[i]);
    }
}

#endif // !ARCH_LOGO_H
