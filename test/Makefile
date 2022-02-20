TARGET = simple_test

#COMPILER_PREFIX = /usr/local/xpack-riscv-none-embed-gcc-10.2.0-1.2/bin/riscv-none-embed-
COMPILER_PREFIX = /usr/local/nuclei-riscv-tools/gcc/bin/riscv-nuclei-elf-
#COMPILER_PREFIX = riscv64-unknown-elf-

#OPENOCD_CMD = /usr/local/nuclei-riscv-tools/openocd/2022.01/bin/openocd
OPENOCD_CMD = /usr/local/riscv-openocd/bin/openocd

OPENOCD_ARGS = -f interface/ftdi/sipeed-rv-debugger.cfg \
	-f target/gd32vf103.cfg

## "-ffunction-sections -fdata-sections" will make every function a section.
## this works with the "-Wl,--gc-sections" parameter of the linker.

CFLAGS = -W -g -Os -ffunction-sections -fdata-sections -fno-common \
	-I../riscv -I../peripheral -I../peripheral/src -I../peripheral/inc \
	-I./src \
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

C_FILES = $(wildcard ../peripheral/*.c)
C_FILES += $(wildcard ../peripheral/src/*.c)
C_FILES += $(wildcard ../riscv/*.c)
C_FILES += $(wildcard ./src/*.c)

C_OBJS = $(patsubst %.c, %.o, $(C_FILES))

STARTUP_OBJS = ../boot/start.o ../boot/entry.o

OBJFILES = $(C_OBJS) $(STARTUP_OBJS)

all: $(TARGET).hex
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -Obinary $< $(TARGET).bin
	$(OBJCOPY) -Oihex $< $(TARGET).hex
	$(SIZE) --format=sysv --radix=16 $<
	$(SIZE) $<

$(TARGET).elf: $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $^

../boot/start.o: ../boot/start.S
	$(CC) $(CFLAGS) -c $< -o $@

../boot/entry.o: ../boot/entry.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(warning $<)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: load openocd debug lss clean

load: $(TARGET).bin
	$(OPENOCD_CMD) $(OPENOCD_ARGS) -c "program $< verify reset exit" -d1

openocd:
	$(OPENOCD_CMD) $(OPENOCD_ARGS)

debug:
	$(GDB)

lss: $(TARGET).elf
	$(OBJDUMP) -S -D $< > $(TARGET).lss

clean:
	rm -f *.elf *.hex *.bin *.lss
	rm -f $(OBJFILES)

