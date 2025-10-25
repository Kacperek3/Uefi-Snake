.POSIX:
.PHONY: all clean image run

# Pliki wejściowe i wyjściowe
SOURCE = main.c
TARGET = BOOTX64.EFI

# Cross-kompilator MinGW (tak jak w Twoim pliku)
# To jest poprawna metoda, która omija problemy z gnu-efi
CC = x86_64-w64-mingw32-gcc \
    -Wl,--subsystem,10 \
    -e efi_main 

# Twoje flagi kompilatora (bez zmian)
CFLAGS = \
    -std=c17 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -mno-red-zone \
    -ffreestanding \
    -nostdlib 

# Zmienne dla obrazu dysku (Metoda 2)
IMAGE_FILE = efi.img
IMAGE_SIZE_MB = 32
EFI_PATH = ::/EFI/BOOT

#
# GŁÓWNE CELE
#

# Domyślny cel: zbuduj program ORAZ obraz dysku
all: $(IMAGE_FILE)

# Cel do uruchomienia QEMU (zależny od 'all')
run: all
	@echo "--- Resetowanie NVRAM (my_ovmf_vars.fd) ---"
	cp /usr/share/OVMF/OVMF_VARS_4M.fd my_ovmf_vars.fd
	@echo "--- Uruchamianie QEMU ---"
	qemu-system-x86_64 \
		-enable-kvm \
		-m 1G \
		-drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd \
		-drive if=pflash,format=raw,file=my_ovmf_vars.fd \
		-hda $(IMAGE_FILE)

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

# Cel do czyszczenia
clean:
	@echo "--- Sprzątanie ---"
	rm -f $(TARGET) $(IMAGE_FILE) my_ovmf_vars.fd *.o