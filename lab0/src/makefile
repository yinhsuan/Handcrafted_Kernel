CC := aarch64-linux-gnu-gcc
LD := aarch64-linux-gnu-ld
OBJCOPY := aarch64-linux-gnu-objcopy

CFLAGS = -Wall -nostdlib

TARGET_IMG = kernel8.img
TARGET_ELF = kernel8.elf
LD_SCRIPT = linker.ld

all: $(TARGET_IMG)

.PHONY: clean

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_ELF): a.o
	$(LD) -T $(LD_SCRIPT) $< -o $@

$(TARGET_IMG): $(TARGET_ELF)
	@mkdir -p $(@D)
	$(OBJCOPY) -O binary $< $@

run:
	qemu-system-aarch64 -M raspi3b -kernel $(TARGET_IMG) -display none -d in_asm

debug:
	qemu-system-aarch64 -M raspi3b -kernel $(TARGET_IMG) -display none -S -s

clean:
	$(RM) $(TARGET_IMG) $(TARGET_ELF) *.o

