
# ARMGNU ?= aarch64-linux-gnu
CC := aarch64-linux-gnu-gcc
LD := aarch64-linux-gnu-ld
OBJCOPY := aarch64-linux-gnu-objcopy

CFLAGS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASMFLAGS = -Iinclude

BUILD_DIR = build
SRC_DIR = src

TARGET_IMG = kernel8.img
TARGET_ELF = kernel8.elf
LD_SCRIPT = linker.ld

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	$(CC) $(ASMFLAGS) -MMD -c $< -o $@

$(TARGET_IMG): $(SRC_DIR)/$(LD_SCRIPT) $(OBJ_FILES)
	$(LD) -T $(SRC_DIR)/$(LD_SCRIPT) -o $(BUILD_DIR)/$(TARGET_ELF) $(OBJ_FILES)
	$(OBJCOPY) $(BUILD_DIR)/$(TARGET_ELF) -O binary $(TARGET_IMG)


all: $(TARGET_IMG)

run:
	qemu-system-aarch64 -M raspi3b -kernel $(TARGET_IMG) -display none -serial null -serial stdio

debug:
	qemu-system-aarch64 -M raspi3b -kernel $(TARGET_IMG) -display none -S -s

clean:
	rm -rf $(BUILD_DIR) *.img
