PICOLIBC_SRC     := $(CURDIR)/core/picolibc/picolibc
PICOLIBC_BUILD   := $(CURDIR)/$(BUILD_DIR)/picolibc_build
PICOLIBC_INSTALL := $(CURDIR)/$(BUILD_DIR)/picolibc_install
PICOLIBC_TARGET  := $(CURDIR)/$(PICOLIBC_INSTALL)/lib/libc.a
PICOLIBC_SPECS	 := $(PICOLIBC_INSTALL)/specs/picolibc.specs

DEPS += $(PICOLIBC_TARGET)

$(PICOLIBC_BUILD)/build.ninja:
	@echo ">>> Configuring Picolibc with Meson..."
	@mkdir -p $(PICOLIBC_BUILD)
	@cd $(PICOLIBC_BUILD) && meson setup $(PICOLIBC_SRC) \
		--cross-file  $(PICOLIBC_CROSS_FILE) \
		--prefix=$(PICOLIBC_INSTALL) \
		-Dspecsdir=$(PICOLIBC_INSTALL)/specs \
		-Dtests=false \
		-Dthread-local-storage=false \
		-Dpicocrt=false \
		-Dformat-default=minimal \
		-Datomic-ungetc=false \
		-Dprintf-small-ultoa=false \
		-Dio-wchar=false \
		-Dposix-console=false \
		-Dprintf-aliases=false \
		-Db_lto=true \
		-Dc_args="-ffat-lto-objects" \
		-Dio-c99-formats=false > /dev/null

$(PICOLIBC_TARGET): $(PICOLIBC_BUILD)/build.ninja
	@echo ">>> Building/Installing Picolibc with Ninja..."
	@ninja -C $(PICOLIBC_BUILD) install > /dev/null

GLOBAL_CFLAGS += -isystem $(PICOLIBC_INSTALL)/include --specs=$(PICOLIBC_SPECS)
GLOBAL_LDFLAGS += -L$(PICOLIBC_INSTALL)/lib -Wl,--start-group -lc -lgcc -Wl,--end-group --specs=$(PICOLIBC_SPECS)
