## Introduction

This is a library for GD32VF103xxxx RISC-V microcontrollers. It depends on the GNU toolchains (GCC, Binutils) for (at least) RV32I.


## C library

A simple (and incomplete) C library is included in this repository.


## Debugger Adaptor

The `RV-Debugger-Lite` is the recommended one (for it's tiny size and cheap price).

Configurations before using the debugger:

```sh
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules

# or
sudo cp misc/99-platformio-udev.rules /etc/udev/rules.d/

## then restart udev

sudo udevadm control --reload-rules
sudo udevadm trigger
```

```sh
sudo usermod -a -G dialout wallace
sudo usermod -a -G plugdev wallace
```


## OpenOCD

Use this [riscv openocd](https://github.com/riscv/riscv-openocd). And copy the sipeed-debugger configuration to the openocd script/interface directory.

```sh
sudo cp misc/sipeed-rv-debugger.cfg /usr/local/riscv-openocd/share/openocd/scripts/interface/ftdi/
```


## Extra

> This library is modified from [the official firmware library](https://www.gigadevice.com/products/microcontrollers/gd32/risc-v/) and [this template](https://github.com/WRansohoff/GD32VF103_templates).

There is a sample Makefile in the `sample` directory.

