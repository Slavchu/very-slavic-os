# Description
This is my very first and very ugly steps to write an operating system. Generally it is my attempt to make my coursework and diploma be more interesting. If you want to see project docs please refer: [here](docs/README.md)

# Instructions
Firstly you need to init submodules
```bash
git submodule update --init --recursive
```
## Build
```bash
make TARGET=[target]
```
## Flash
```bash
make TARGET=[target] flash
```

# Supported targets
- esp32_c3

# TODO list
- core: init function
- HAL: enable/disable irq, context switch, basic GPIO functionality, set up timers and interrupts
- scheduler: I hope it will exist :3
