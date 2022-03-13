include config.prj

CROSS_COMPILER_PREFIX = /usr/local/nuclei-riscv-tools/gcc/bin/riscv-nuclei-elf-
OPENOCD_CMD = /usr/local/riscv-openocd/bin/openocd

OPENOCD_ARGS = -f interface/ftdi/sipeed-rv-debugger.cfg \
	-f target/gd32vf103.cfg

ifeq "$(CONFIG_USE_RV32E)" "1"
	ARCH_AND_ABI = -march=rv32emac -mabi=ilp32e
else
	ARCH_AND_ABI = -march=rv32imac -mabi=ilp32
endif

## "-ffunction-sections -fdata-sections" will make every function a section.
## this works with the "-Wl,--gc-sections" parameter of the linker.

CROSS_C_ASM_FLAGS = -W -g -Os -ffunction-sections -fdata-sections -fno-common \
	-fno-builtin \
	-I$(LIBDIR)/soc_lib -I$(LIBDIR)/soc_lib/src -I$(LIBDIR)/soc_lib/inc \
	-I$(LIBDIR)/core_lib \
	-I$(LIBDIR)/c_lib \
	-I$(LIBDIR)/board_longan_lib \
	-I./src \
	$(ARCH_AND_ABI) \
	-DCONFIG_USE_FLOAT=$(CONFIG_USE_FLOAT)

ifeq "$(CONFIG_IS_LONGAN)" "1"
	CROSS_C_ASM_FLAGS += -DLONGON_NANO
endif

## "-Wl,--gc-sections" will pass "--gc-sections" to linker to make it ignore
## unused sections.

## if you want to use float point, you need to remove "-nostdlib" and add "-lm"
## which could increase the whole size.

ifeq "$(CONFIG_USE_FLOAT)" "1"
	FLOAT_RELATED_LD_FLAG = -lm
else
	FLOAT_RELATED_LD_FLAG = -nostdlib
endif

CROSS_LD_FLAGS = -T$(LIBDIR)/soc_lib/gd32vf103.ld \
	-Wl,--gc-sections -Wl,--no-relax \
	-nostartfiles $(FLOAT_RELATED_LD_FLAG) $(ARCH_AND_ABI)

CROSS_CC = $(CROSS_COMPILER_PREFIX)gcc
CROSS_LD = $(CROSS_COMPILER_PREFIX)gcc
CROSS_GDB = $(CROSS_COMPILER_PREFIX)gdb
CROSS_OBJCOPY = $(CROSS_COMPILER_PREFIX)objcopy
CROSS_OBJDUMP = $(CROSS_COMPILER_PREFIX)objdump
CROSS_SIZE = $(CROSS_COMPILER_PREFIX)size

ASM_FILES = $(wildcard $(LIBDIR)/*_lib/*.S)
ASM_FILES += $(wildcard $(LIBDIR)/*_lib/*/*.S)
ASM_FILES += $(wildcard ./src/*.S)

C_FILES = $(wildcard $(LIBDIR)/*_lib/*.c)
C_FILES += $(wildcard $(LIBDIR)/*_lib/*/*.c)
C_FILES += $(wildcard ./src/*.c)

ASM_OBJ_FILES = $(patsubst %.S, %.o, $(ASM_FILES))
C_OBJ_FILES = $(patsubst %.c, %.o, $(C_FILES))

OBJ_FILES = $(ASM_OBJ_FILES) $(C_OBJ_FILES)

TARGET_BINARY_FILES = \
	$(TARGET_NAME).elf $(TARGET_NAME).hex $(TARGET_NAME).bin

all: $(TARGET_BINARY_FILES)

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
	@echo "\tCompiling $<..."
	@$(CROSS_CC) $(CROSS_C_ASM_FLAGS) -MM $< \
		| sed "s,$(@F)\s*:,$@:," > $<.d
	@$(CROSS_CC) $(CROSS_C_ASM_FLAGS) -c $< -o $@

%.o: %.S
	@echo "\tCompiling $<..."
	@$(CROSS_CC) $(CROSS_C_ASM_FLAGS) -MM $< \
		| sed "s,$(@F)\s*:,$@:," > $<.d
	@$(CROSS_CC) $(CROSS_C_ASM_FLAGS) -c $< -o $@

DEPENDENCY_FILES = $(patsubst %.S, %.S.d, $(ASM_FILES))
DEPENDENCY_FILES += $(patsubst %.c, %.c.d, $(C_FILES))

-include $(DEPENDENCY_FILES)

.PHONY: load openocd debug lss tags clean

load:
	@echo "\tLoad program to the target machine..."
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
	@echo "\tGenerating disassembled file..."
	@$(CROSS_OBJDUMP) -S -D $(TARGET_NAME).elf > $(TARGET_NAME).lss
clean:
	@echo "\tRemoving generated files..."
	@rm -f $(TARGET_BINARY_FILES) $(OBJ_FILES) $(DEPENDENCY_FILES) \
		$(TARGET_NAME).lss tags

