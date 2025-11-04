#ifndef EFI_H
#define EFI_H


/*
 * NOTE: void* fields in structs = not implemented!!
 */

// __has_include is clang/gcc defined; But should be in C standard C2X
#if __has_include(<uchar.h>) 
#include <uchar.h>
#endif

#include <stdint.h>
#include <stddef.h> // NULL

#ifndef _UCHAR_H
typedef uint_least16_t char16_t;
#endif

// Common UEFI Data Types: UEFI Spec 2.10 section 2.3.1
typedef uint8_t     UINT8;
typedef uint16_t    UINT16;
typedef uint32_t    UINT32;
typedef uint64_t    UINT64;
typedef uint64_t    UINTN;
typedef char16_t    CHAR16;	// UTF-16, but should use UCS-2 code points 0x0000-0xFFFF
typedef void        VOID;
typedef uint8_t BOOLEAN;

typedef UINTN       EFI_STATUS;
typedef VOID*       EFI_HANDLE;

// Taken from EDKII at
// https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Base.h
#define IN
#define OUT
#define OPTIONAL
#define CONST const
#define INTN int64_t

#define TRUE 1
#define FALSE 0

// Taken from gnu-efi at
// https://github.com/vathpela/gnu-efi/blob/master/inc/x86_64/efibind.h
#define EFIAPI __attribute__((ms_abi))  // x86_64 Microsoft calling convention

// EFI_STATUS codes: UEFI Spec 2.10 Appendix D
#define EFI_SUCCESS 0

// EFI Simple Text Input Protocol: UEFI Spec 2.10 section 12.3
// Forward declare struct for this to work and compile
typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct EFI_BOOT_SERVICES EFI_BOOT_SERVICES;



typedef struct {
    UINT16  ScanCode;
    CHAR16  UnicodeChar;
} EFI_INPUT_KEY;

typedef struct {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8  Data4[8];
} EFI_GUID;

typedef 
EFI_STATUS 
(EFIAPI *EFI_INPUT_READ_KEY) (
 IN  EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *This, 
 OUT EFI_INPUT_KEY                   *Key
);

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    void*               Reset;
    EFI_INPUT_READ_KEY  ReadKeyStroke;
    void*               WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

// EFI Simple Text Output Protocol: UEFI Spec 2.10 section 12.4
// Forward declare struct for this to work and compile
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// Text attributes: UEFI Spec 2.10 section 12.4.7
#define EFI_BLACK  0x00
#define EFI_BLUE   0x01
#define EFI_GREEN  0x02
#define EFI_CYAN   0x03
#define EFI_RED    0x04
#define EFI_YELLOW 0x0E
#define EFI_WHITE  0x0F

// Only use 0x00-0x07 for background with this macro!
#define EFI_TEXT_ATTR(Foreground,Background) \
    ((Foreground) | ((Background) << 4))

typedef 
EFI_STATUS 
(EFIAPI *EFI_TEXT_STRING) (
 IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
 IN CHAR16                          *String
);

typedef 
EFI_STATUS 
(EFIAPI *EFI_TEXT_SET_ATTRIBUTE) (
 IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
 IN UINTN                           Attribute
);

typedef 
EFI_STATUS 
(EFIAPI *EFI_TEXT_CLEAR_SCREEN) (
 IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
);

typedef 
EFI_STATUS
(EFIAPI *EFI_TEXT_SET_CURSOR_POS)(
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN UINTN                           Column,
    IN UINTN                           Row
);

typedef 
EFI_STATUS
(EFIAPI *EFI_TEXT_ENABLE_CURSOR)(
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN BOOLEAN                         Visible
);

typedef
(EFIAPI *EFI_TEXT_RESET) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN BOOLEAN                         ExtendedVerification
);


typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET                  Reset;
    EFI_TEXT_STRING                 OutputString;
    void*                           TestString;
    void*                           QueryMode;
    void*                           SetMode;
    EFI_TEXT_SET_ATTRIBUTE          SetAttribute;
    EFI_TEXT_CLEAR_SCREEN           ClearScreen;
    EFI_TEXT_SET_CURSOR_POS         SetCursorPosition;
    void*                           EnableCursor;
    void*                           Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef enum {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef 
VOID 
(EFIAPI *EFI_RESET_SYSTEM) (
   IN EFI_RESET_TYPE ResetType,      
   IN EFI_STATUS     ResetStatus,   
   IN UINTN          DataSize,     
   IN VOID           *ResetData OPTIONAL
);

// EFI Table Header: UEFI Spec 2.10 section 4.2
typedef struct {
    UINT64  Signature;
    UINT32  Revision;
    UINT32  HeaderSize;
    UINT32  CRC32;
    UINT32  Reserved;
} EFI_TABLE_HEADER;


typedef
EFI_STATUS
(EFIAPI *EFI_LOCATE_PROTOCOL) (
  IN EFI_GUID     *Protocol,
  IN VOID         *Registration OPTIONAL,
  OUT VOID        **Interface
);

typedef
EFI_STATUS
(EFIAPI *EFI_STALL)
(
    IN UINTN Microseconds
);

typedef
EFI_STATUS
(EFIAPI *EFI_CHECK_EVENT)
(
    IN VOID *Event
);

struct EFI_BOOT_SERVICES{
EFI_TABLE_HEADER Hdr;
//
//
// Task Priority Services
void * RaiseTPL; // EFI 1.0+
void * RestoreTPL; // EFI 1.0+
//
//
// Memory Services
void * AllocatePages; // EFI 1.0+
void * FreePages; // EFI 1.0+
void * GetMemoryMap; // EFI 1.0+
void * AllocatePool; // EFI 1.0+
void * FreePool; // EFI 1.0+
//
// Event & Timer Services
//
void * CreateEvent; // EFI 1.0+
void * SetTimer; // EFI 1.0+
void * WaitForEvent; // EFI 1.0+
void * SignalEvent; // EFI 1.0+
void * CloseEvent; // EFI 1.0+
EFI_CHECK_EVENT  CheckEvent; // EFI 1.0+
//
//
// Protocol Handler Services
void * InstallProtocolInterface; // EFI 1.0+
void * ReinstallProtocolInterface; // EFI 1.0+
void * UninstallProtocolInterface; // EFI 1.0+
void * HandleProtocol; // EFI 1.0+
VOID* Reserved; // EFI 1.0+
void * RegisterProtocolNotify; // EFI 1.0+
void * LocateHandle; // EFI 1.0+
void * LocateDevicePath; // EFI 1.0+
void * InstallConfigurationTable; // EFI 1.0+
//
// Image Services
//
void * LoadImage; // EFI 1.0+
void * StartImage; // EFI 1.0+
void * Exit; // EFI 1.0+
void * UnloadImage; // EFI 1.0+
void * ExitBootServices; // EFI 1.0+
//
//
// Miscellaneous Services
void * GetNextMonotonicCount; // EFI 1.0+
EFI_STALL Stall; // EFI 1.0+
void * SetWatchdogTimer; // EFI 1.0+
//
//
// DriverSupport Services
void * ConnectController; // EFI 1.1
void * DisconnectController; // EFI 1.1+
//// Open and Close Protocol Services
//
void * OpenProtocol; // EFI 1.1+
void * CloseProtocol; // EFI 1.1+
void * OpenProtocolInformation;// EFI 1.1+
//
// Library Services
//
void * ProtocolsPerHandle; // EFI 1.1+
void * LocateHandleBuffer; // EFI 1.1+
EFI_LOCATE_PROTOCOL LocateProtocol; // EFI 1.1+
void * InstallMultipleProtocolInterfaces; // EFI 1.1+
void * UninstallMultipleProtocolInterfaces; // EFI 1.1+
//
//
// 32-bit CRC Services
void * CalculateCrc32; // EFI 1.1+
//
// Miscellaneous Services
//
void * CopyMem; // EFI 1.1+
void * SetMem; // EFI 1.1+
void * CreateEventEx; // UEFI 2.0+
};



typedef struct {
    EFI_TABLE_HEADER Hdr;

    // Time Services
    void*                           GetTime; 
    void*                           SetTime; 
    void*                           GetWakeupTime; 
    void*                           SetWakeupTime; 

    // Virtual Memory Services
    void*                           SetVirtualAddressMap;
    void*                           ConvertPointer;

    // Variable Services
    void*                           GetVariable;
    void*                           GetNextVariableName;
    void*                           SetVariable;

    // Miscellaneous Services
    void*                           GetNextHighMonotonicCount;
    EFI_RESET_SYSTEM                ResetSystem;

    // UEFI 2.0 Capsule Services
    void*                           UpdateCapsule;
    void*                           QueryCapsuleCapabilities;

    // Miscellaneous UEFI 2.0 Service
    void*                           QueryVariableInfo; 
} EFI_RUNTIME_SERVICES;

// EFI System Table: UEFI Spec 2.10 section 4.3
typedef struct {
    EFI_TABLE_HEADER                Hdr;

    void*                           FirmwareVendor;
    UINT32                          FirmwareRevision;
    void*                           ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL 	*ConIn;
	void*                           ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
	void*                           StandardErrorHandle;
	void*                           StdErr;
	EFI_RUNTIME_SERVICES            *RuntimeServices;
	EFI_BOOT_SERVICES               *BootServices;
	UINTN                           NumberOfTableEntries;
	void*                           ConfigurationTable;
} EFI_SYSTEM_TABLE;


// EFI Graphics Output Protocol: UEFI Spec 2.10 section 12.9

typedef struct {
    UINT32 RedMask;
    UINT32 GreenMask;
    UINT32 BlueMask;
    UINT32 ReservedMask;
} EFI_PIXEL_BITMASK;

typedef enum {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor, // Najczęstszy format!
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct {
    UINT32                      Version;
    UINT32                      HorizontalResolution;
    UINT32                      VerticalResolution;
    EFI_GRAPHICS_PIXEL_FORMAT   PixelFormat;
    EFI_PIXEL_BITMASK           PixelInformation;
    UINT32                      PixelsPerScanLine; // Ważne! To jest "pitch"
} EFI_GRAPHICS_MODE_INFORMATION;

typedef struct {
    UINT32                               MaxMode;
    UINT32                               Mode;
    EFI_GRAPHICS_MODE_INFORMATION        *Info;
    UINTN                                SizeOfInfo;
    UINT64                               FrameBufferBase;
    UINTN                                FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef
EFI_STATUS
(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT) (
    IN VOID *This
);

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
    void* QueryMode;
    void* SetMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT     Blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE    *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

#endif // EFI_H
