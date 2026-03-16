# File structure
```bash
├── board           # Board specific files(perhaps will be extended)
│   └── esp32_c3
│       └── ld      # Linker scripts
├── core            # All the core functionality
│   ├── main.c
│   ├── make.mk
│   ├── scheduler
│   ├── source.mk
│   ├── task
│   └── utils
│       ├── include
│       │   └── bitops.h
│       └── make.mk
├── docs
│   └── README.md # <------- We are here
├── hal # HAL functionality
│   ├── esp32c3 # HAL implementation module
│   ├── include # HAL declarations
│   │   └── hal
│   │       └── watchdog.h
│   ├── make.mk
│   └── RISC_V  # ARCH specific module
│       ├── make.mk
│       ├── riscv_entry.s  # Arch specific entrypoint
│       └── source.mk
├── LICENSE
├── make # buildsystem specific fules
│   ├── target.mk
│   └── toolchain.mk
├── Makefile
└── README.md
```