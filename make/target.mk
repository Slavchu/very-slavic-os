#Creating image
ifeq ($(TARGET), esp32_c3)
		CREATE_IMAGE_CMD = esptool --chip esp32c3 elf2image --flash-mode dio --flash-freq 40m $(1)
		LDSCRIPT_DIR := board/esp32_c3/ld
		GLOBAL_DEFINES += -DLOG_PRINTF_FUNC=ets_printf
		PICOLIBC_CROSS_FILE += $(CURDIR)/board/esp32_c3/picolibc_cross_file.txt
else
    CREATE_IMAGE_CMD = @echo "No image creation defined for TARGET=$(TARGET). Skipping."
endif

#Flashing
ifeq ($(TARGET), esp32_c3)
    FLASH_CMD = esptool --chip esp32c3 --port /dev/ttyACM0 write-flash -fm dio 0x0 build/core.bin
else
    FLASH_CMD = @echo "No flash command defined for TARGET=$(TARGET). Skipping."
endif

ALL_LDSCRIPTS = $(wildcard $(LDSCRIPT_DIR)/*.ld)
LDFLAGS += -L $(LDSCRIPT_DIR) \
						$(addprefix -T ,$(notdir $(ALL_LDSCRIPTS)))

GLOBAL_DEFINES += -DTARGET_$(TARGET)
