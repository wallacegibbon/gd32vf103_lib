OUTPUT_ARCH( "riscv" )

ENTRY( reset_handler )

MEMORY {
	/* for GD32VF103C4, FLASH: 16K, RAM: 6K */
	/* for GD32VF103C6, FLASH: 32K, RAM: 10K */
	/* for GD32VF103C8, FLASH: 64K, RAM: 20K */
	/* for GD32VF103CB, FLASH: 128K, RAM: 32K */
	FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 128K
	RAM(rwx)  : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS {
	/* for GD32VF103C4 and GD32VF103C6, stack size should be 1K */
	__stack_size = DEFINED(__stack_size) ? __stack_size : 2K;

	.vector_table : {
		KEEP(*(SORT_NONE(.vector_table)))
	} >FLASH AT>FLASH

	.text : {
		*(.rodata .rodata.*)
		*(.srodata .srodata.*)
		*(.text .text.*)
	} >FLASH AT>FLASH

	/* VMA is 0x0800xxxx here */
	. = ALIGN(4);
	PROVIDE(__etext = .);
	PROVIDE(_etext = .);
	PROVIDE(etext = .);

	.lalign : {
		. = ALIGN(4);
		PROVIDE(_data_lma = .);
	} >FLASH AT>FLASH

	.dalign : {
		. = ALIGN(4);
		PROVIDE(_data = .);
	} >RAM AT>FLASH

	.data : {
		_sdata = .;
		*(.rdata);
		*(.data .data.*)
		*(.sdata .sdata.*)
		_edata = .;
	} >RAM AT>FLASH

	/* VMA is 0x2000xxxx here */
	. = ALIGN(4);
	PROVIDE(_edata = .);
	PROVIDE(edata = .);

	PROVIDE(_fbss = .);
	PROVIDE(__bss_start = .);

	.bss : {
		_sbss = .;
		*(.sbss*)
		*(.bss .bss.*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} >RAM AT>RAM

	. = ALIGN(8);
	PROVIDE(_end = .);
	PROVIDE(end = .);

	.stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size : {
		PROVIDE(_heap_end = .);
		. = __stack_size;
		PROVIDE(_sp = .);
	} >RAM AT>RAM
}
