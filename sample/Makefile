TARGET = sample
LIBDIR = ..
USE_FLOAT = 1
#USE_FLOAT = 0

COMPILER_PREFIX = /usr/local/nuclei-riscv-tools/gcc/bin/riscv-nuclei-elf-
OPENOCD_CMD = /usr/local/riscv-openocd/bin/openocd

OPENOCD_ARGS = -f interface/ftdi/sipeed-rv-debugger.cfg \
	-f target/gd32vf103.cfg

## "-ffunction-sections -fdata-sections" will make every function a section.
## this works with the "-Wl,--gc-sections" parameter of the linker.

C_ASM_FLAGS = -W -g -Os -ffunction-sections -fdata-sections -fno-common \
	-fno-builtin \
	-I$(LIBDIR)/riscv -I$(LIBDIR)/peripheral \
	-I$(LIBDIR)/peripheral/src -I$(LIBDIR)/peripheral/inc \
	-I$(LIBDIR)/libc -I./src \
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

LD_FLAGS = -T$(LIBDIR)/ldscript/gd32vf103.ld -Wl,--gc-sections -Wl,--no-relax \
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

ASM_FILES = $(wildcard $(LIBDIR)/boot/*.S)
ASM_FILES += $(wildcard $(LIBDIR)/riscv/*.S)
ASM_FILES += $(wildcard ./src/*.S)
ASM_OBJS = $(patsubst %.S, %.o, $(ASM_FILES))

OBJ_FILES = $(C_OBJS) $(ASM_OBJS)

all: $(TARGET).hex
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -Obinary $< $(TARGET).bin
	$(OBJCOPY) -Oihex $< $(TARGET).hex
	$(SIZE) --format=sysv --radix=16 $<
	$(SIZE) $<

$(TARGET).elf: $(OBJ_FILES)
	$(LD) $(LD_FLAGS) -o $@ $^

%.o: %.c
	$(warning $<)
	$(CC) $(C_ASM_FLAGS) -c $< -o $@

%.o: %.S
	$(warning $<)
	$(CC) $(C_ASM_FLAGS) -c $< -o $@

.PHONY: load openocd debug lss tags clean

load:
	$(OPENOCD_CMD) $(OPENOCD_ARGS) -d1 \
		-c "program $(TARGET).bin verify reset exit"
openocd:
	$(OPENOCD_CMD) $(OPENOCD_ARGS)
debug:
	$(GDB) $(TARGET).elf \
		--eval-command="target extended-remote localhost:3333"
lss:
	$(OBJDUMP) -S -D $(TARGET).elf > $(TARGET).lss
tags:
	find $(LIBDIR) . -regex '.*\.[ch]' | xargs ctags
clean:
	rm -f $(OBJ_FILES) *.elf *.hex *.bin *.lss tags

