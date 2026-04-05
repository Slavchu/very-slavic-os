ifeq ($(TARGET), esp32_c3)
	CROSS_COMPILE := riscv-none-elf-
	ARCH_FLAGS := -march=rv32imc_zicsr_zifencei -mabi=ilp32 -mcmodel=medany
	CFLAGS += -ffreestanding  -mrelax
	ARCH := RISC_V_32
	CLANGD_PATH := clangd/clangd_riscv
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0
else
  CFLAGS += -Os -ffunction-sections -fdata-sections
  LDFLAGS += -Os -Wl,--gc-sections
endif

CC 			:= $(CROSS_COMPILE)gcc
AS 			:= $(CROSS_COMPILE)gcc
LD 			:= $(CROSS_COMPILE)gcc
OBJCOPY := $(CROSS_COMPILE)objcopy
RM			:= rm
CP			:= cp

CFLAGS  += $(ARCH_FLAGS) -nostdlib -fno-builtin -std=c23
LDFLAGS += -nostartfiles -nostdlib
ASFLAGS += $(ARCH_FLAGS) -x assembler-with-cpp

GLOBAL_DEFINES+= -D$(ARCH)
