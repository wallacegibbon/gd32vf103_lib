TARGET = simple_test

TOOL_PREFIX = /usr/local/xpack-riscv-none-embed-gcc-10.2.0-1.2/bin/riscv-none-embed-
#TOOL_PREFIX = /usr/local/nuclei-riscv-tools/gcc/bin/riscv-nuclei-elf-

ARCH = rv32imac
#ARCH = rv64g

## "-ffunction-sections -fdata-sections" will make every function a section.
## this works with the "-Wl,--gc-sections" parameter of the linker.

CFLAGS = -W -g -Os -ffunction-sections -fdata-sections -fno-common \
	-I../peripheral -I../peripheral/src -I../peripheral/inc \
	-I../riscv/drivers -I../riscv/stubs -I./src -I./inc \
	-march=$(ARCH) -DLONGON_NANO

## "-Wl,--gc-sections" will pass "--gc-sections" to linker to make it
## ignore unused sections.

LDFLAGS = -T../ldscript/gd32vf103.ld -Wl,--gc-sections -Wl,--no-relax \
	-nostartfiles -nostdlib \
	-march=$(ARCH) -DLONGON_NANO

CC = $(TOOL_PREFIX)gcc
LD = $(TOOL_PREFIX)gcc
OBJCOPY = $(TOOL_PREFIX)objcopy
OBJDUMP = $(TOOL_PREFIX)objdump
SIZE = $(TOOL_PREFIX)size

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

load: $(TARGET).hex
	echo "load program" $< "to target machine"

