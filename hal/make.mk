ifeq ($(ARCH), RISC_V_32)
	SUB_MODULES += hal/RISC_V
endif

ifeq ($(TARGET), esp32_c3)
	SUB_MODULES += hal/esp32c3
endif

SUB_MODULES += hal/interrupt

LOCAL_INCLUDES += include
