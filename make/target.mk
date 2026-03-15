#Creating image
ifeq ($(TARGET), esp32_c3)
    CREATE_IMAGE_CMD = esptool --chip esp32c3 elf2image --flash-mode dio --flash-freq 40m $(1)
	LINKER_SCRIPT := hal/ESP32/esp32c3.ld
else
    CREATE_IMAGE_CMD = @echo "No image creation defined for TARGET=$(TARGET). Skipping."
endif

#Flashing
ifeq ($(TARGET), esp32_c3)
    FLASH_CMD = esptool.py --chip esp32c3 --port /dev/ttyACM0 write_flash -fm dio 0x0 build/core.bin
else
    FLASH_CMD = @echo "No flash command defined for TARGET=$(TARGET). Skipping."
endif
