#ifndef BOOT_ARCH_H
#define BOOT_ARCH_H

#include <efi.h>
#include <efifs.h>
#include <efilib.h>
#include <efistdarg.h>

EFI_STATUS FindAndBootArch(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);
EFI_STATUS LoadExt2Driver(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);
EFI_STATUS FindAndBootWindows(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);

void ScanAndPrintOSInfo(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);
#endif // BOOT_ARCH_H
