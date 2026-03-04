#ifndef BOOT_ARCH_H
#define BOOT_ARCH_H

#include <efi.h>
#include <efilib.h>
#include <efistdarg.h>
#include <efifs.h>


EFI_STATUS FindAndBootArch(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);
EFI_STATUS LoadExt2Driver(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);



#endif //BOOT_ARCH_H
