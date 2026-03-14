ifeq ($(TARGET), esp32_c3)
    CREATE_IMAGE_CMD = esptool.py --chip esp32c3 elf2image $(1)
		LINKER_SCRIPT := hal/ESP32/esp32c3.ld
else
    CREATE_IMAGE_CMD = @echo "No image creation defined for TARGET=$(TARGET). Skipping."
endif
