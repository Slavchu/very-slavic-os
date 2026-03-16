# Makefile

# 1. Error checking
ifneq ($(MAKECMDGOALS),clean)
  ifndef TARGET
    $(error TARGET is not defined! Please run: make TARGET=esp32_c3)
  endif
endif

# 2. Variables
BUILD_DIR := build
MODULES   := core hal
ECHO      := @echo
DEBUG			?= 0

# 3. Includes
include make/toolchain.mk
include make/target.mk

# 4. Module Loader System
LOADED_MODULES  := 
SRCS            :=
GLOBAL_INCLUDES :=
GLOBAL_DEFINES  :=

define LOAD_MODULE
  ifeq ($$(filter $(1),$$(LOADED_MODULES)),)
    LOADED_MODULES += $(1)

    SRC            :=
    LOCAL_INCLUDES :=
    LOCAL_DEFINES  :=
    SUB_MODULES    :=
    
    MOD_DIR        := $(1)

    include $(1)/make.mk

    SRCS            += $$(addprefix $(1)/, $$(SRC))
    GLOBAL_INCLUDES += $$(addprefix -I$(1)/, $$(LOCAL_INCLUDES))
    GLOBAL_DEFINES  += $$(LOCAL_DEFINES)

    $$(foreach sub,$$(SUB_MODULES),$$(eval $$(call LOAD_MODULE,$$(sub))))
  endif
endef

# 5. Execute loader
$(foreach mod,$(MODULES),$(eval $(call LOAD_MODULE,$(mod))))

# 6. Object files mapping
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
FINAL_CFLAGS := $(CFLAGS) $(GLOBAL_INCLUDES) $(GLOBAL_DEFINES)

# 7. Build Rules
.PHONY: all clean

all: $(BUILD_DIR)/core.bin

flash: all
	$(ECHO) flashing TARGET=$(TARGET)
	$(call FLASH_CMD,$<)

# Generating binary
$(BUILD_DIR)/core.bin: $(BUILD_DIR)/core.elf
	$(ECHO) ">>> Creating image: $@"
	$(call CREATE_IMAGE_CMD,$<)

# Linker
$(BUILD_DIR)/core.elf: $(OBJS)
	$(ECHO) ">>> Linking: $@"
	$(LD) $(LDFLAGS) $(OBJS) -o $@

# C compiling
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(ECHO) "CC $<"
	$(CC) $(FINAL_CFLAGS) -c $< -o $@

# Assembly compiling
$(BUILD_DIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(ECHO) "AS $<"
	$(CC) $(ASFLAGS) $(FINAL_CFLAGS) -c $< -o $@

clean:
	$(RM) -rf $(BUILD_DIR)
