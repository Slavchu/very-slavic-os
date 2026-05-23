MAP_FILE ?= $(BUILD_DIR)/core.map

ifeq ($(TARGET), esp32_c3)
	CROSS_COMPILE := riscv-none-elf-
	ARCH_FLAGS := -march=rv32imc_zicsr_zifencei -mabi=ilp32 -mcmodel=medlow
	CFLAGS += -ffreestanding  -mrelax
	ARCH := RISC_V_32
	CLANGD_PATH := clangd/clangd_riscv
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0
else
  CFLAGS += -Os -ffunction-sections -fdata-sections
  LDFLAGS += -Os -Wl,--gc-sections -Wl,--strip-debug
endif

CC 			:= $(CROSS_COMPILE)gcc
AS 			:= $(CROSS_COMPILE)gcc
LD 			:= $(CROSS_COMPILE)gcc
OBJCOPY := $(CROSS_COMPILE)objcopy
RM			:= rm
CP			:= cp

CFLAGS  += $(ARCH_FLAGS) -fno-builtin -std=c23 -Wall -Werror -fmerge-all-constants -fdata-sections -flto
LDFLAGS += -nostartfiles -Wl,-Map=$(MAP_FILE),--cref -Wl,--gc-sections -flto
ASFLAGS += $(ARCH_FLAGS) -x assembler-with-cpp

GLOBAL_DEFINES+= -D$(ARCH)
