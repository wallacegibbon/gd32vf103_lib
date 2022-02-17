TARGET = simple_test

#COMPILER_PREFIX = /usr/local/xpack-riscv-none-embed-gcc-10.2.0-1.2/bin/riscv-none-embed-
COMPILER_PREFIX = /usr/local/nuclei-riscv-tools/gcc/bin/riscv-nuclei-elf-
#COMPILER_PREFIX = riscv64-unknown-elf-

#OPENOCD_CMD = /usr/local/nuclei-riscv-tools/openocd/2022.01/bin/openocd
OPENOCD_CMD = /usr/local/riscv-openocd/bin/openocd

## "-ffunction-sections -fdata-sections" will make every function a section.
## this works with the "-Wl,--gc-sections" parameter of the linker.

CFLAGS = -W -g -Os -ffunction-sections -fdata-sections -fno-common \
	-I../peripheral -I../peripheral/src -I../peripheral/inc \
	-I../riscv/drivers -I../riscv/stubs -I./src -I./inc \
	-march=rv32imac -mabi=ilp32 -DLONGON_NANO

## "-Wl,--gc-sections" will pass "--gc-sections" to linker to make it
## ignore unused sections.

LDFLAGS = -T../ldscript/gd32vf103.ld -Wl,--gc-sections -Wl,--no-relax \
	-nostartfiles -nostdlib \
	-march=rv32imac -mabi=ilp32 -DLONGON_NANO

CC = $(COMPILER_PREFIX)gcc
LD = $(COMPILER_PREFIX)gcc
GDB = $(COMPILER_PREFIX)gdb
OBJCOPY = $(COMPILER_PREFIX)objcopy
OBJDUMP = $(COMPILER_PREFIX)objdump
SIZE = $(COMPILER_PREFIX)size

STARTUP_FILE = ../riscv/env/gd32vf103_boot.S
STARTUP_OBJ = $(patsubst %.S, %.o, $(STARTUP_FILE))

C_FILES = $(wildcard ../peripheral/*.c)
C_FILES += $(wildcard ../peripheral/src/*.c)
C_FILES += $(wildcard ../riscv/drivers/*.c)
C_FILES += $(wildcard ../riscv/stubs/*.c)
C_FILES += $(wildcard ../riscv/env/*.c)
C_FILES += $(wildcard ./src/*.c)

C_OBJ = $(patsubst %.c, %.o, $(C_FILES))

OBJFILE = $(C_OBJ) $(STARTUP_OBJ)

all: $(TARGET).hex
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -Obinary $< $(TARGET).bin
	$(OBJCOPY) -Oihex $< $(TARGET).hex
	$(SIZE) --format=sysv --radix=16 $<

$(TARGET).elf: $(OBJFILE)
	$(LD) $(LDFLAGS) -o $@ $^

$(STARTUP_OBJ): $(STARTUP_FILE)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(warning $<)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean lss load

RM = rm -f

clean:
	$(RM) *.elf *.hex *.bin *.lss
	$(RM) $(OBJFILE)

lss: $(TARGET).elf
	$(OBJDUMP) -S -D $< > $(TARGET).lss

openocd:
	$(OPENOCD_CMD) \
		-f interface/ftdi/sipeed-rv-debugger.cfg \
		-f target/gd32vf103.cfg

debug:
	$(GDB)

load: $(TARGET).hex
	echo "load program" $< "to target machine"

