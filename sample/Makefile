TARGET = simple_test
LIBDIR = ..
#USE_FLOAT = 1
USE_FLOAT = 0

COMPILER_PREFIX = /usr/local/nuclei-riscv-tools/gcc/bin/riscv-nuclei-elf-
OPENOCD_CMD = /usr/local/riscv-openocd/bin/openocd

OPENOCD_ARGS = -f interface/ftdi/sipeed-rv-debugger.cfg \
	-f target/gd32vf103.cfg

## "-ffunction-sections -fdata-sections" will make every function a section.
## this works with the "-Wl,--gc-sections" parameter of the linker.

CFLAGS = -W -g -Os -ffunction-sections -fdata-sections -fno-common \
	-fno-builtin \
	-I$(LIBDIR)/riscv -I$(LIBDIR)/peripheral \
	-I$(LIBDIR)/peripheral/src -I$(LIBDIR)/peripheral/inc \
	-I./src \
	-march=rv32imac -mabi=ilp32 -DLONGON_NANO -DUSE_FLOAT=$(USE_FLOAT)

## "-Wl,--gc-sections" will pass "--gc-sections" to linker to make it ignore
## unused sections.

## if you want to use float point, you need to remove "-nostdlib" and add "-lm"
## which could increase the whole size.

ifeq ($(USE_FLOAT), 1)
	FLOAT_RELATED_LD_FLAG = -lm
else
	FLOAT_RELATED_LD_FLAG = -nostdlib
endif

LDFLAGS = -T$(LIBDIR)/ldscript/gd32vf103.ld -Wl,--gc-sections -Wl,--no-relax \
	-nostartfiles $(FLOAT_RELATED_LD_FLAG) \
	-march=rv32imac -mabi=ilp32 -DLONGON_NANO

CC = $(COMPILER_PREFIX)gcc
LD = $(COMPILER_PREFIX)gcc
GDB = $(COMPILER_PREFIX)gdb
OBJCOPY = $(COMPILER_PREFIX)objcopy
OBJDUMP = $(COMPILER_PREFIX)objdump
SIZE = $(COMPILER_PREFIX)size

C_FILES = $(wildcard $(LIBDIR)/peripheral/*.c)
C_FILES += $(wildcard $(LIBDIR)/peripheral/src/*.c)
C_FILES += $(wildcard $(LIBDIR)/riscv/*.c)
C_FILES += $(wildcard $(LIBDIR)/libc/*/*.c)
C_FILES += $(wildcard ./src/*.c)

C_OBJS = $(patsubst %.c, %.o, $(C_FILES))

STARTUP_OBJS = $(LIBDIR)/boot/start.o $(LIBDIR)/boot/entry.o

OBJFILES = $(C_OBJS) $(STARTUP_OBJS)

all: $(TARGET).hex
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -Obinary $< $(TARGET).bin
	$(OBJCOPY) -Oihex $< $(TARGET).hex
	$(SIZE) --format=sysv --radix=16 $<
	$(SIZE) $<

$(TARGET).elf: $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $^

$(LIBDIR)/boot/start.o: $(LIBDIR)/boot/start.S
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBDIR)/boot/entry.o: $(LIBDIR)/boot/entry.S
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

