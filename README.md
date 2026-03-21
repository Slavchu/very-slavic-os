# Description
This is my very first and very ugly steps to write an operating system. Generally it is my attempt to make my coursework and diploma be more interesting. If you want to see project docs please refer: [here](docs/README.md) </br>
All the contributions are welcomed.

# Instructions
## Build
Firstly you need to init submodules
```bash
git submodule update --init --recursive
```
To build the project you need to type:
```bash
make TARGET=[target]
```
To auto generate helpers for clangd it is recommended to install __bear__ utility. You can build the very slavic os by writing
```bash
make TARGET=[target] clangd
bear -- make TARGET=[target] flash
```
It will automatically create for you .compile-commands.json file.
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
- add auto download of toolchains
