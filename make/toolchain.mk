ifeq ($(TARGET), esp31_c3)
	CROSS_COMPILE := riscv-none-elf-gcc
	ARCH_FLAGS := -march=rv32imc -mabi=ilp32
	CFLAGS += -ffreestanding
	ARCH := RISC_V
endif

CC 			?= $(CROSS_COMPILE)gcc
AS 			?= $(CROSS_COMPILE)gcc
LD 			?= $(CROSS_COMPILE)gcc
OBJCOPY ?= $(CROSS_COMPILE)objcopy
RM			?= rm

CFLAGS  += $(ARCH_FLAGS) -nostdlib -fno-builtin
ASFLAGS += $(ARCH_FLAGS) -x assembler-with-cpp
