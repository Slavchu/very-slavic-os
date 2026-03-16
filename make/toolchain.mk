ifeq ($(TARGET), esp32_c3)
	CROSS_COMPILE := riscv-none-elf-
	ARCH_FLAGS := -march=rv32imc_zicsr -mabi=ilp32 -mcmodel=medany
	CFLAGS += -ffreestanding  -mrelax
	ARCH := RISC_V
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0
endif

CC 			:= $(CROSS_COMPILE)gcc
AS 			:= $(CROSS_COMPILE)gcc
LD 			:= $(CROSS_COMPILE)gcc
OBJCOPY := $(CROSS_COMPILE)objcopy
RM			:= rm

CFLAGS  += $(ARCH_FLAGS) -nostdlib -fno-builtin
LDFLAGS += -nostartfiles -nostdlib -mno-relax
ASFLAGS += $(ARCH_FLAGS) -x assembler-with-cpp
