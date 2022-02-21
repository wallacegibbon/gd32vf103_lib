TARGET = simple_test
PRJDIR = ../

COMPILER_PREFIX = /usr/local/nuclei-riscv-tools/gcc/bin/riscv-nuclei-elf-
OPENOCD_CMD = /usr/local/riscv-openocd/bin/openocd

OPENOCD_ARGS = -f interface/ftdi/sipeed-rv-debugger.cfg \
	-f target/gd32vf103.cfg

## "-ffunction-sections -fdata-sections" will make every function a section.
## this works with the "-Wl,--gc-sections" parameter of the linker.

CFLAGS = -W -g -Os -ffunction-sections -fdata-sections -fno-common \
	-I$(PRJDIR)/riscv -I$(PRJDIR)/peripheral \
	-I$(PRJDIR)/peripheral/src -I$(PRJDIR)/peripheral/inc \
	-I./src \
	-march=rv32imac -mabi=ilp32 -DLONGON_NANO

## "-Wl,--gc-sections" will pass "--gc-sections" to linker to make it
## ignore unused sections.

LDFLAGS = -T$(PRJDIR)/ldscript/gd32vf103.ld -Wl,--gc-sections -Wl,--no-relax \
	-nostartfiles -nostdlib \
	-march=rv32imac -mabi=ilp32 -DLONGON_NANO

CC = $(COMPILER_PREFIX)gcc
LD = $(COMPILER_PREFIX)gcc
GDB = $(COMPILER_PREFIX)gdb
OBJCOPY = $(COMPILER_PREFIX)objcopy
OBJDUMP = $(COMPILER_PREFIX)objdump
SIZE = $(COMPILER_PREFIX)size

C_FILES = $(wildcard $(PRJDIR)/peripheral/*.c)
C_FILES += $(wildcard $(PRJDIR)/peripheral/src/*.c)
C_FILES += $(wildcard $(PRJDIR)/riscv/*.c)
C_FILES += $(wildcard ./src/*.c)

C_OBJS = $(patsubst %.c, %.o, $(C_FILES))

STARTUP_OBJS = $(PRJDIR)/boot/start.o $(PRJDIR)/boot/entry.o

OBJFILES = $(C_OBJS) $(STARTUP_OBJS)

all: $(TARGET).hex
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -Obinary $< $(TARGET).bin
	$(OBJCOPY) -Oihex $< $(TARGET).hex
	$(SIZE) --format=sysv --radix=16 $<
	$(SIZE) $<

$(TARGET).elf: $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $^

$(PRJDIR)/boot/start.o: $(PRJDIR)/boot/start.S
	$(CC) $(CFLAGS) -c $< -o $@

$(PRJDIR)/boot/entry.o: $(PRJDIR)/boot/entry.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(warning $<)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: load openocd debug lss clean

load:
	$(OPENOCD_CMD) $(OPENOCD_ARGS) -d1 \
		-c "program $(TARGET).bin verify reset exit"

openocd:
	$(OPENOCD_CMD) $(OPENOCD_ARGS)

debug:
	$(GDB)

lss:
	$(OBJDUMP) -S -D $(TARGET).elf > $(TARGET).lss

clean:
	rm -f *.elf *.hex *.bin *.lss $(OBJFILES)

