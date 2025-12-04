.POSIX:
.PHONY: all clean image run

SOURCE = src/main.c src/platform.c src/renderer.c src/gameState.c src/menuState.c src/snakeState.c
HEADERS = src/efi.h src/renderer.h src/platform.h src/gameState.h src/menuState.h src/snakeState.h
TARGET = BOOTX64.EFI


CC = x86_64-w64-mingw32-gcc \
    -Wl,--subsystem,10 \
    -e efi_main 
#source https://learn.microsoft.com/en-us/windows/win32/debug/pe-format

# compiler flags
CFLAGS = \
    -std=c17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -mno-red-zone \
    -ffreestanding \
    -nostdlib 
# source https://www.systutorials.com/docs/linux/man/1-x86_64-w64-mingw32-gcc/

#variables to create image
IMAGE_FILE = efi.img
IMAGE_SIZE_MB = 32
EFI_PATH = ::/EFI/BOOT

#is linux or macos
OS := $(shell uname -s)

ifeq ($(OS),Darwin)
    ACCEL = -accel hvf
    OVMF_VARS_PATH = /usr/local/opt/ovmf/share/OVMF/OvmfX64/OVMF_VARS.fd
    OVMF_CODE_PATH = /usr/local/opt/ovmf/share/OVMF/OvmfX64/OVMF_CODE.fd
else ifeq ($(OS),Linux)
    ACCEL = -enable-kvm
    OVMF_VARS_PATH = /usr/share/OVMF/OVMF_VARS_4M.fd
    OVMF_CODE_PATH = /usr/share/OVMF/OVMF_CODE_4M.fd
else
    $(error "Nieobsługiwany system operacyjny: $(OS)")
endif



all: $(IMAGE_FILE)

# run the qemu with disk image
run: all
	@echo "Resetowanie NVRAM (my_ovmf_vars.fd)"
	cp $(OVMF_VARS_PATH) my_ovmf_vars.fd
	@echo "Uruchamianie QEMU"
	qemu-system-x86_64 \
    	$(ACCEL) \
    	-m 1G \
    	-drive if=pflash,format=raw,readonly=on,file=$(OVMF_CODE_PATH) \
    	-drive if=pflash,format=raw,file=my_ovmf_vars.fd \
    	-drive file=$(IMAGE_FILE),format=raw,if=ide
#source man qemu

$(IMAGE_FILE): $(TARGET)
	@echo "Tworzenie surowego obrazu dysku $(IMAGE_FILE)"
	rm -f $(IMAGE_FILE)
	dd if=/dev/zero of=$(IMAGE_FILE) bs=1M count=$(IMAGE_SIZE_MB)
	mformat -i $(IMAGE_FILE) ::
	mmd -i $(IMAGE_FILE) ::/EFI
	mmd -i $(IMAGE_FILE) $(EFI_PATH)
	mcopy -i $(IMAGE_FILE) $(TARGET) $(EFI_PATH)
	@echo "Obraz dysku gotowy"

# compile
$(TARGET): $(SOURCE) $(HEADERS)
	@echo "Kompilowanie $(SOURCE) do $(TARGET)"
	$(CC) $(CFLAGS) -o $@ $(SOURCE)

clean:
	@echo "Sprzątanie"
	rm -f $(TARGET) $(IMAGE_FILE) my_ovmf_vars.fd *.o