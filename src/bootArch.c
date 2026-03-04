#include "bootArch.h"

EFI_GUID gEfiLoadedImageProtocolGuid = {
    0x5B1B31A1, 0x9562, 0x11d2, {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}};

EFI_GUID gEfiSimpleFileSystemProtocolGuid = {
    0x964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

EFI_GUID gEfiFileInfoGuid = {
    0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

EFI_GUID gEfiBlockIoProtocolGuid = {
    0xCE345171, 0xBA0B, 0x11D2, {0x8E, 0x4F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}};

EFI_STATUS LoadExt2Driver(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_BOOT_SERVICES *BS = SystemTable->BootServices;
    EFI_STATUS Status;
    UINTN HandleCount = 0;
    EFI_HANDLE *HandleBuffer = NULL;

    //==========================================================================//
    //                  1.  Searching for a FAT partition                       //
    //==========================================================================//
    SystemTable->ConOut->OutputString(
        SystemTable->ConOut, L"[Start : LoadExt2Driver]: Searching for a FAT partition ...\r\n");

    Status = BS->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL,
                                    &HandleCount, &HandleBuffer);
    if (Status != EFI_SUCCESS) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut,
                                          L"[Error]: didn't find a FAT parition!\r\n");
        return Status;
    }

    //==========================================================================//
    //              2.   Iterating through all found partitions                 //
    //==========================================================================//
    for (UINTN i = 0; i < HandleCount; i++) {
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFS;
        EFI_FILE_PROTOCOL *DriverFile;
        EFI_FILE_PROTOCOL *Root;

        //------------------------
        // 2.1 - connecting to a FAT partition
        //@Root is a handle to this found FAT partition
        Status =
            BS->HandleProtocol(HandleBuffer[i], &gEfiSimpleFileSystemProtocolGuid, (void **)&SFS);
        if (Status != EFI_SUCCESS)
            continue;

        Status = SFS->OpenVolume(SFS, &Root);
        if (Status != EFI_SUCCESS)
            continue;
        //------------------------

        //------------------------
        // 2.2 - using root handle searching for a ext_2_x64 driver
        //      it must be in /EFI/BOOT/ directory
        Status = Root->Open(Root, &DriverFile, L"\\EFI\\BOOT\\ext2_x64.efi", EFI_FILE_MODE_READ, 0);

        if (Status == EFI_SUCCESS) {

            //-----------------------------------
            // Succesfully found ext_2_x64 driver
            //-----------------------------------

            UINTN InfoSize = 0;
            DriverFile->GetInfo(DriverFile, &gEfiFileInfoGuid, &InfoSize, NULL);

            EFI_FILE_INFO *FileInfo;
            BS->AllocatePool(EfiBootServicesData, InfoSize, (void **)&FileInfo);
            DriverFile->GetInfo(DriverFile, &gEfiFileInfoGuid, &InfoSize, FileInfo);
            UINTN FileSize = FileInfo->FileSize;
            BS->FreePool(FileInfo);

            // Rezerwujemy pamięć RAM na plik i czytamy go
            void *FileBuffer;
            BS->AllocatePool(EfiBootServicesData, FileSize, &FileBuffer);
            DriverFile->Read(DriverFile, &FileSize, FileBuffer);
            DriverFile->Close(DriverFile);

            EFI_HANDLE DriverImageHandle;
            Status =
                BS->LoadImage(FALSE, ImageHandle, NULL, FileBuffer, FileSize, &DriverImageHandle);

            if (Status == EFI_SUCCESS) {
                Status = BS->StartImage(DriverImageHandle, NULL, NULL);

                if (Status == EFI_SUCCESS) {
                    SystemTable->ConOut->OutputString(
                        SystemTable->ConOut, L"[OK] Sterownik EXT2 załadowany i uruchomiony!\r\n");

                    // -------- 4. KLUCZOWY MOMENT: ODŚWIEŻANIE DYSKÓW --------
                    // Zmuszamy UEFI, żeby nowy sterownik podpiął się pod dyski
                    UINTN BlkCount = 0;
                    EFI_HANDLE *BlkHandles = NULL;

                    Status = BS->LocateHandleBuffer(ByProtocol, &gEfiBlockIoProtocolGuid, NULL,
                                                    &BlkCount, &BlkHandles);
                    if (Status == EFI_SUCCESS) {
                        for (UINTN j = 0; j < BlkCount; j++) {
                            // Funkcja ConnectController przypina sterownik ext2 do surowej
                            // partycji
                            BS->ConnectController(BlkHandles[j], NULL, NULL, TRUE);
                        }
                        BS->FreePool(BlkHandles);
                        SystemTable->ConOut->OutputString(
                            SystemTable->ConOut, L"[OK] Dyski odswiezone. UEFI widzi ext4!\r\n");
                    }
                    // --------------------------------------------------------
                } else {
                    SystemTable->ConOut->OutputString(
                        SystemTable->ConOut, L"[BLAD] StartImage dla sterownika zwrocil blad.\r\n");
                }
            } else {
                SystemTable->ConOut->OutputString(SystemTable->ConOut,
                                                  L"[BLAD] LoadImage dla sterownika zawiodl.\r\n");
            }

            // Sprzątamy po sterowniku
            BS->FreePool(FileBuffer);
            Root->Close(Root);
            BS->FreePool(HandleBuffer);
            return EFI_SUCCESS;
        }
        Root->Close(Root);
    }

    // Jeśli pętla doszła tutaj, znaczy że na żadnej partycji nie było pliku
    // ext2_x64.efi
    SystemTable->ConOut->OutputString(
        SystemTable->ConOut,
        L"[BLAD] Nie znaleziono pliku \\EFI\\BOOT\\ext2_x64.efi na dysku.\r\n");
    BS->FreePool(HandleBuffer);
    return EFI_NOT_FOUND;
}

EFI_STATUS FindAndBootArch(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_BOOT_SERVICES *BS = SystemTable->BootServices;
    EFI_STATUS Status;

    UINTN HandleCount = 0;
    EFI_HANDLE *HandleBuffer = NULL;

    Status = BS->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL,
                                    &HandleCount, &HandleBuffer);

    if (Status != EFI_SUCCESS)
        return Status;

    for (UINTN i = 0; i < HandleCount; i++) {
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFS;
        EFI_FILE_PROTOCOL *RootDirectory;
        EFI_FILE_PROTOCOL *KernelFile;

        Status =
            BS->HandleProtocol(HandleBuffer[i], &gEfiSimpleFileSystemProtocolGuid, (void **)&SFS);
        if (Status != EFI_SUCCESS)
            continue;

        Status = SFS->OpenVolume(SFS, &RootDirectory);
        if (Status != EFI_SUCCESS)
            continue;

        Status = RootDirectory->Open(RootDirectory, &KernelFile, L"\\boot\\vmlinuz-linux",
                                     EFI_FILE_MODE_READ, 0);
        if (Status == EFI_SUCCESS) {
            SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[OK] ARCH KERNEL FOUND!\r\n");

            UINTN FileInfoSize = 0;
            KernelFile->GetInfo(KernelFile, &gEfiFileInfoGuid, &FileInfoSize, NULL);

            EFI_FILE_INFO *FileInfo = NULL;
            Status = BS->AllocatePool(EfiLoaderData, FileInfoSize, (void **)&FileInfo);
            if (Status != EFI_SUCCESS || FileInfo == NULL) {
                SystemTable->ConOut->OutputString(SystemTable->ConOut,
                                                  L"[ERROR] AllocatePool for FileInfo failed!\r\n");
                while (1) {
                }
            }

            KernelFile->GetInfo(KernelFile, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
            UINTN KernelSize = FileInfo->FileSize;
            BS->FreePool(FileInfo);

            if (KernelSize == 0) {
                SystemTable->ConOut->OutputString(SystemTable->ConOut,
                                                  L"[ERROR] KernelSize is 0 bytes!\r\n");
                while (1) {
                }
            }

            SystemTable->ConOut->OutputString(SystemTable->ConOut,
                                              L"[OK] Size retrieved. Allocating RAM...\r\n");

            void *KernelBuffer = NULL;
            Status = BS->AllocatePool(EfiLoaderData, KernelSize, &KernelBuffer);

            if (Status != EFI_SUCCESS || KernelBuffer == NULL) {
                SystemTable->ConOut->OutputString(
                    SystemTable->ConOut,
                    L"[ERROR] Out of memory for kernel (AllocatePool failed)!\r\n");
                while (1) {
                }
            }

            SystemTable->ConOut->OutputString(
                SystemTable->ConOut,
                L"[OK] RAM allocated. Reading file (this may take a while)...\r\n");

            Status = KernelFile->Read(KernelFile, &KernelSize, KernelBuffer);
            if (Status != EFI_SUCCESS) {
                SystemTable->ConOut->OutputString(SystemTable->ConOut,
                                                  L"[ERROR] KernelFile->Read failed!\r\n");
                while (1) {
                }
            }

            KernelFile->Close(KernelFile);
            RootDirectory->Close(RootDirectory);

            SystemTable->ConOut->OutputString(SystemTable->ConOut,
                                              L"[OK] File in RAM. Executing LoadImage...\r\n");

            EFI_HANDLE KernelImageHandle = NULL;
            Status = BS->LoadImage(FALSE, ImageHandle, NULL, KernelBuffer, KernelSize,
                                   &KernelImageHandle);

            if (Status != EFI_SUCCESS) {
                SystemTable->ConOut->OutputString(
                    SystemTable->ConOut,
                    L"[ERROR] LoadImage failed! Is the file an EFI application?\r\n");
                while (1) {
                }
            }

            SystemTable->ConOut->OutputString(
                SystemTable->ConOut, L"[OK] LoadImage successful! Injecting arguments...\r\n");

            EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
            Status = BS->HandleProtocol(KernelImageHandle, &gEfiLoadedImageProtocolGuid,
                                        (void **)&LoadedImage);

            if (Status == EFI_SUCCESS) {
                CHAR16 CmdLine[] = L"root=/dev/sda rw init=/bin/sh debug";
                LoadedImage->LoadOptions = CmdLine;
                LoadedImage->LoadOptionsSize = sizeof(CmdLine);
            }

            SystemTable->ConOut->OutputString(SystemTable->ConOut, L">>> START IMAGE <<<\r\n");

            Status = BS->StartImage(KernelImageHandle, NULL, NULL);

            SystemTable->ConOut->OutputString(SystemTable->ConOut,
                                              L"[ERROR] StartImage returned control to UEFI!\r\n");
            BS->FreePool(KernelBuffer);
            BS->FreePool(HandleBuffer);
            while (1) {
            }
            return Status;
        }
        RootDirectory->Close(RootDirectory);
    }

    SystemTable->ConOut->OutputString(SystemTable->ConOut,
                                      L"[ERROR] Kernel not found, stuck in loop.\r\n");
    BS->FreePool(HandleBuffer);
    return EFI_NOT_FOUND;
}
