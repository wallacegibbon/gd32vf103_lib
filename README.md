## Introduction

This is a library for GD32VF103xxxx RISC-V microcontrollers. It depends on the GNU toolchains (GCC, Binutils) for RV32IMAC.


## Debugger Adaptor

```sh
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules

sudo udevadm control --reload-rules
sudo udevadm trigger
```

```sh
sudo usermod -a -G dialout wallace
sudo usermod -a -G plugdev wallace
```

```sh
sudo cp misc/sipeed-rv-debugger.cfg /usr/local/nuclei-riscv-tools/openocd/2022.01/scripts/interface/ftdi/
```


## Extra

> This library is modified from [the official firmware library](https://www.gigadevice.com/products/microcontrollers/gd32/risc-v/) and [this template](https://github.com/WRansohoff/GD32VF103_templates).

There is a sample Makefile in the `test` directory.

