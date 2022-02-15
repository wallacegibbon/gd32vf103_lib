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
	} >FLASH

	.text : {
		. = ALIGN(4);
		*(.rodata .rodata.*)
		*(.srodata .srodata.*)
		*(.text .text.*)
		. = ALIGN(4);
		_etext = .;
	} >FLASH

	/* VMA is 0x0800xxxx here */

	.data : AT(_etext) {
		. = ALIGN(4);
		_sdata = .;
		*(.rdata);
		*(.data .data.*)
		*(.sdata .sdata.*)
		. = ALIGN(4);
		_edata = .;
	} >RAM

	/* VMA is 0x2000xxxx here */

	.bss : {
		. = ALIGN(4);
		_sbss = .;
		*(.sbss*)
		*(.bss .bss.*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} >RAM

	.stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size : {
		. = ALIGN(4);
		. = __stack_size;
		_sp = .;
	} >RAM
}
