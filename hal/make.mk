GLOBAL_INCLUDES += $(MOD_DIR)/hal

ifeq ($(ARCH), RISC)
	SUB_MODULES += hal/RISC_V
endif
