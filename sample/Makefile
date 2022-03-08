include config.prj

CROSS_COMPILER_PREFIX = /usr/local/nuclei-riscv-tools/gcc/bin/riscv-nuclei-elf-
OPENOCD_CMD = /usr/local/riscv-openocd/bin/openocd

OPENOCD_ARGS = -f interface/ftdi/sipeed-rv-debugger.cfg \
	-f target/gd32vf103.cfg

ifeq "$(USE_RV32E)" "1"
	ARCH_AND_ABI = -march=rv32emac -mabi=ilp32e
else
	ARCH_AND_ABI = -march=rv32imac -mabi=ilp32
endif

## "-ffunction-sections -fdata-sections" will make every function a section.
## this works with the "-Wl,--gc-sections" parameter of the linker.

CROSS_C_ASM_FLAGS = -W -g -Os -ffunction-sections -fdata-sections -fno-common \
	-fno-builtin \
	-I$(LIBDIR)/core -I$(LIBDIR)/chip \
	-I$(LIBDIR)/chip/src -I$(LIBDIR)/chip/inc \
	-I$(LIBDIR)/c -I$(LIBDIR)/longan -I./src \
	$(ARCH_AND_ABI) -DLONGON_NANO -DUSE_FLOAT=$(USE_FLOAT)

## "-Wl,--gc-sections" will pass "--gc-sections" to linker to make it ignore
## unused sections.

## if you want to use float point, you need to remove "-nostdlib" and add "-lm"
## which could increase the whole size.

ifeq "$(USE_FLOAT)" "1"
	FLOAT_RELATED_LD_FLAG = -lm
else
	FLOAT_RELATED_LD_FLAG = -nostdlib
endif

CROSS_LD_FLAGS = -T$(LIBDIR)/chip/gd32vf103.ld \
	-Wl,--gc-sections -Wl,--no-relax \
	-nostartfiles $(FLOAT_RELATED_LD_FLAG) $(ARCH_AND_ABI)

CROSS_CC = $(CROSS_COMPILER_PREFIX)gcc
CROSS_LD = $(CROSS_COMPILER_PREFIX)gcc
CROSS_GDB = $(CROSS_COMPILER_PREFIX)gdb
CROSS_OBJCOPY = $(CROSS_COMPILER_PREFIX)objcopy
CROSS_OBJDUMP = $(CROSS_COMPILER_PREFIX)objdump
CROSS_SIZE = $(CROSS_COMPILER_PREFIX)size

C_FILES = $(wildcard $(LIBDIR)/chip/*.c)
C_FILES += $(wildcard $(LIBDIR)/chip/src/*.c)
C_FILES += $(wildcard $(LIBDIR)/core/*.c)
C_FILES += $(wildcard $(LIBDIR)/c/*/*.c)
C_FILES += $(wildcard $(LIBDIR)/longan/*.c)
C_FILES += $(wildcard ./src/*.c)
C_OBJS = $(patsubst %.c, %.o, $(C_FILES))

ASM_FILES = $(wildcard $(LIBDIR)/chip/*.S)
ASM_FILES += $(wildcard $(LIBDIR)/chip/src/*.S)
ASM_FILES += $(wildcard $(LIBDIR)/core/*.S)
ASM_FILES += $(wildcard $(LIBDIR)/c/*/*.S)
ASM_FILES += $(wildcard $(LIBDIR)/longan/*.S)
ASM_FILES += $(wildcard ./src/*.S)
ASM_OBJS = $(patsubst %.S, %.o, $(ASM_FILES))

OBJ_FILES = $(C_OBJS) $(ASM_OBJS)

all: $(TARGET_NAME).hex

$(TARGET_NAME).hex: $(TARGET_NAME).elf
	@echo "\tGenerating hex and bin files..."
	@$(CROSS_OBJCOPY) -Obinary $< $(TARGET_NAME).bin
	@$(CROSS_OBJCOPY) -Oihex $< $(TARGET_NAME).hex
	@echo "\n\tMemory Usage:\n"
	@$(CROSS_SIZE) $<
	@echo "\n\tdone.\n"

$(TARGET_NAME).elf: $(OBJ_FILES)
	@echo "\n\tLinking..."
	@$(CROSS_LD) $(CROSS_LD_FLAGS) -o $@ $^

%.o: %.c
	@echo "\tCROSS_CC $<..."
	@$(CROSS_CC) $(CROSS_C_ASM_FLAGS) -M $< | \
		sed "s,$(@F)\s*:,$@:," > $<.d
	@$(CROSS_CC) $(CROSS_C_ASM_FLAGS) -c $< -o $@

%.o: %.S
	@echo "\tCROSS_CC $<..."
	@$(CROSS_CC) $(CROSS_C_ASM_FLAGS) -M $< | \
		sed "s,$(@F)\s*:,$@:," > $<.d
	@$(CROSS_CC) $(CROSS_C_ASM_FLAGS) -c $< -o $@

DEPENDENCY_FILES = $(patsubst %.c, %.c.d, $(C_FILES))
DEPENDENCY_FILES += $(patsubst %.S, %.S.d, $(ASM_FILES))

-include $(DEPENDENCY_FILES)

.PHONY: load openocd debug lss tags clean

load:
	@echo "\tWrite to the target machine..."
	@$(OPENOCD_CMD) $(OPENOCD_ARGS) -d1 \
		-c "program $(TARGET_NAME).bin verify reset exit"
openocd:
	@echo "\tStarting the OpenOCD server..."
	@$(OPENOCD_CMD) $(OPENOCD_ARGS)
debug:
	@echo "\tStarting GDB and connect to OpenOCD..."
	@$(CROSS_GDB) $(TARGET_NAME).elf \
		--eval-command="target extended-remote localhost:3333"
tags:
	@echo "\tGenerating ctag file..."
	@find $(LIBDIR) . -regex '.*\.[ch]' -exec realpath {} \; \
		| sort | uniq | xargs ctags
lss:
	@echo "\tGenerating the disassembled file..."
	@$(CROSS_OBJDUMP) -S -D $(TARGET_NAME).elf > $(TARGET_NAME).lss
clean:
	@echo "\tRemoving generated files..."
	@rm -f $(OBJ_FILES) $(DEPENDENCY_FILES) \
		*.elf *.hex *.bin *.lss tags

