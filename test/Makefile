TARGET = simple_test

TOOL_PREFIX = /usr/local/xpack-riscv-none-embed-gcc-10.2.0-1.2/bin/riscv-none-embed-

ARCH = rv32imac
CPU =

CFLAGS = -W -g -Os -ffunction-sections -fdata-sections -fno-common \
	-I../peripheral -I../peripheral/src -I../peripheral/inc \
	-I../riscv/drivers -I../riscv/stubs \
	-I./src -I./inc \
	-DLONGON_NANO
	-march=$(ARCH) \
	#-mcpu=$(CPU)

CC = $(TOOL_PREFIX)gcc

LDFLAGS = -T../riscv/env/gd32vf103.lds -nostartfiles -Wl,--gc-sections \
	  -march=$(ARCH) \
	  #-mcpu=$(CPU)

LD = $(TOOL_PREFIX)gcc

OBJCOPY = $(TOOL_PREFIX)objcopy
OBJDUMP = $(TOOL_PREFIX)objdump
SIZE = $(TOOL_PREFIX)size

#ASM_FILE = $(wildcard ../riscv/env/*.s)
#ASM_OBJ = $(patsubst %.s, %.o, $(ASM_FILE))
ASM_FILE = ../riscv/env/start.s
ASM_OBJ = $(patsubst %.s, %.o, $(ASM_FILE))

C_FILE = $(wildcard ../peripheral/*.c)
C_FILE += $(wildcard ../peripheral/src/*.c)
C_FILE += $(wildcard ../riscv/drivers/*.c)
C_FILE += $(wildcard ../riscv/stubs/*.c)
C_FILE += $(wildcard ../riscv/env/*.c)
C_FILE += $(wildcard ./src/*.c)

C_OBJ = $(patsubst %.c, %.o, $(C_FILE))

OBJFILE = $(C_OBJ)
OBJFILE += $(STARTUP_OBJ)

all: $(TARGET).hex
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -Obinary $< $(TARGET).bin
	$(OBJCOPY) -Oihex $< $(TARGET).hex
	$(SIZE) $^

$(TARGET).elf: $(OBJFILE)
	$(LD) $(LDFLAGS) -o $@ $^

$(ASM_OBJ): $(ASM_FILE)
	$(CC) $(CFLAGS) -o $@ $^

start.o: $(ASM_FILE)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c %.h
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

