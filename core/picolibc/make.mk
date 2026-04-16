PICOLIBC_SRC     := $(CURDIR)/core/picolibc/picolibc
PICOLIBC_BUILD   := $(CURDIR)/$(BUILD_DIR)/picolibc_build
PICOLIBC_INSTALL := $(CURDIR)/$(BUILD_DIR)/picolibc_install
PICOLIBC_TARGET  := $(CURDIR)/$(PICOLIBC_INSTALL)/lib/libc.a

DEPS += $(PICOLIBC_TARGET)

$(PICOLIBC_TARGET):
	@echo ">>> Configuring Picolibc with Meson..."
	@mkdir -p $(PICOLIBC_BUILD)
	@cd $(PICOLIBC_BUILD) && meson setup $(PICOLIBC_SRC) \
		--cross-file  $(PICOLIBC_CROSS_FILE) \
		--prefix=$(PICOLIBC_INSTALL) \
		-Dspecsdir=$(PICOLIBC_INSTALL)/specs \
		-Dtests=false \
		-Dthread-local-storage=true \
		-Dpicocrt=false \
		-Dformat-default=minimal \
		-Datomic-ungetc=false \
		-Dio-c99-formats=false
	@echo ">>> Building Picolibc with Ninja..."
	@ninja -C $(PICOLIBC_BUILD) install

GLOBAL_CFLAGS += -isystem $(PICOLIBC_INSTALL)/include
GLOBAL_LDFLAGS += -L$(PICOLIBC_INSTALL)/lib -Wl,--start-group -lc -lgcc -Wl,--end-group
