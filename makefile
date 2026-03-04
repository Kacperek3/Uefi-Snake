.POSIX:
.PHONY: all clean image run

SOURCE = src/main.c src/platform.c src/renderer.c src/gameState.c src/menuState.c src/snakeState.c src/bootArch.c src/optionsManager.c
HEADERS = src/renderer.h src/platform.h src/gameState.h src/menuState.h src/snakeState.h src/bootArch.h src/optionsManager.h src/archLogo.h
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
    -nostdlib \
		-Iuefi_inc \
		-Iuefi_inc/x86_64 \
		-Iuefi_inc/protocol

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
    OVMF_VARS_PATH = /usr/share/OVMF/x64/OVMF_VARS.4m.fd
    OVMF_CODE_PATH = /usr/share/OVMF/x64/OVMF_CODE.4m.fd
else
    $(error "Nieobsługiwany system operacyjny: $(OS)")
endif



all: $(IMAGE_FILE)

# run the qemu with disk image
run: all
	@echo "Reset NVRAM (my_ovmf_vars.fd)"
	cp $(OVMF_VARS_PATH) my_ovmf_vars.fd
	@echo "Running QEMU"
	qemu-system-x86_64 \
    	$(ACCEL) \
			-display sdl \
    	-m 1G \
    	-drive if=pflash,format=raw,readonly=on,file=$(OVMF_CODE_PATH) \
    	-drive if=pflash,format=raw,file=my_ovmf_vars.fd \
    	-drive file=$(IMAGE_FILE),format=raw,if=ide \
			-drive file=arch_root.img,format=raw,media=disk 
#source man qemu

$(IMAGE_FILE): $(TARGET)
	@echo "Creating raw disk image $(IMAGE_FILE)"
	rm -f $(IMAGE_FILE)
	dd if=/dev/zero of=$(IMAGE_FILE) bs=1M count=$(IMAGE_SIZE_MB)
	mformat -i $(IMAGE_FILE) ::
	mmd -i $(IMAGE_FILE) ::/EFI
	mmd -i $(IMAGE_FILE) $(EFI_PATH)
	mcopy -i $(IMAGE_FILE) $(TARGET) $(EFI_PATH)
	mcopy -i $(IMAGE_FILE) ext2_x64.efi $(EFI_PATH)
	@echo "Disk Image is ready"
	@echo "------------------------------"
	@echo "Creating Arch linux partition"
	dd if=/dev/zero of=arch_root.img bs=1M count=64 status=none
	mkfs.ext4 -F -q arch_root.img
	
	mkdir -p /tmp/arch_mnt
	sudo mount -o loop arch_root.img /tmp/arch_mnt
	sudo mkdir -p /tmp/arch_mnt/boot
	sudo cp vmlinuz-linux /tmp/arch_mnt/boot/
	sudo umount /tmp/arch_mnt
	rmdir /tmp/arch_mnt
	@echo "Succesfully create Arch linux partition with kernel"

	
	

# compile
$(TARGET): $(SOURCE) $(HEADERS)
	@echo "Compile $(SOURCE) in to $(TARGET)"
	$(CC) $(CFLAGS) -o $@ $(SOURCE)

clean:
	@echo "Cleaning"
	rm -f $(TARGET) $(IMAGE_FILE) arch_root.img my_ovmf_vars.fd *.o

