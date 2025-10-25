.POSIX:
.PHONY: all clean image run

# Pliki wejściowe i wyjściowe
SOURCE = main.c
TARGET = BOOTX64.EFI

# Cross-kompilator MinGW (bez zmian)
CC = x86_64-w64-mingw32-gcc \
    -Wl,--subsystem,10 \
    -e efi_main 

# Flagi kompilatora (bez zmian)
CFLAGS = \
    -std=c17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -mno-red-zone \
    -ffreestanding \
    -nostdlib 

# Zmienne dla obrazu dysku (bez zmian)
IMAGE_FILE = efi.img
IMAGE_SIZE_MB = 32
EFI_PATH = ::/EFI/BOOT

# --- DETEKCJA SYSTEMU OPERACYJNEGO ---
# Wykrywa system: "Darwin" (macOS) lub "Linux"
OS := $(shell uname -s)

# Ustaw zmienne specyficzne dla platformy
ifeq ($(OS),Darwin)
    # Ustawienia dla macOS
    ACCEL = -accel hvf
    OVMF_VARS_PATH = /usr/local/opt/ovmf/share/OVMF/OvmfX64/OVMF_VARS.fd
    OVMF_CODE_PATH = /usr/local/opt/ovmf/share/OVMF/OvmfX64/OVMF_CODE.fd
else ifeq ($(OS),Linux)
    # Ustawienia dla Linux (Ubuntu)
    ACCEL = -enable-kvm
    OVMF_VARS_PATH = /usr/share/OVMF/OVMF_VARS_4M.fd
    OVMF_CODE_PATH = /usr/share/OVMF/OVMF_CODE_4M.fd
else
    $(error "Nieobsługiwany system operacyjny: $(OS)")
endif
# --- KONIEC DETEKCJI ---


#
# GŁÓWNE CELE
#

# Domyślny cel: zbuduj program ORAZ obraz dysku
all: $(IMAGE_FILE)

# Cel do uruchomienia QEMU (teraz uniwersalny)
run: all
	@echo "--- Resetowanie NVRAM (my_ovmf_vars.fd) ---"
	cp $(OVMF_VARS_PATH) my_ovmf_vars.fd
	@echo "--- Uruchamianie QEMU ---"
	qemu-system-x86_64 \
    	$(ACCEL) \
    	-m 1G \
    	-drive if=pflash,format=raw,readonly=on,file=$(OVMF_CODE_PATH) \
    	-drive if=pflash,format=raw,file=my_ovmf_vars.fd \
    	-drive file=$(IMAGE_FILE),format=raw,if=ide

#
# REGUŁY BUDOWANIA
#

# Reguła budowania obrazu dysku
$(IMAGE_FILE): $(TARGET)
	@echo "--- Tworzenie surowego obrazu dysku $(IMAGE_FILE) (Metoda 2) ---"
	rm -f $(IMAGE_FILE)
	dd if=/dev/zero of=$(IMAGE_FILE) bs=1M count=$(IMAGE_SIZE_MB)
	mformat -i $(IMAGE_FILE) ::
	mmd -i $(IMAGE_FILE) ::/EFI
	mmd -i $(IMAGE_FILE) $(EFI_PATH)
	mcopy -i $(IMAGE_FILE) $(TARGET) $(EFI_PATH)
	@echo "--- Obraz dysku gotowy ---"

# Reguła kompilacji programu .EFI
$(TARGET): $(SOURCE) efi.h
	@echo "--- Kompilowanie $(SOURCE) do $(TARGET) ---"
	$(CC) $(CFLAGS) -o $@ $<

clean:
	@echo "--- Sprzątanie ---"
	rm -f $(TARGET) $(IMAGE_FILE) my_ovmf_vars.fd *.o