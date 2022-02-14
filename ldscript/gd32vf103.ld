OUTPUT_ARCH( "riscv" )

ENTRY( reset_handler )

MEMORY {
	/* for GD32VF103C4, flash: 16K, ram: 6K */
	/* for GD32VF103C6, flash: 32K, ram: 10K */
	/* for GD32VF103C8, flash: 64K, ram: 20K */
	/* for GD32VF103CB, flash: 128K, ram: 32K */
	flash(rx) : ORIGIN = 0x08000000, LENGTH = 128K
	ram(rwx)  : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS {
	/* for GD32VF103C4 and GD32VF103C6, stack size should be 1K */
	__stack_size = DEFINED(__stack_size) ? __stack_size : 2K;

	.vector_table : {
		KEEP(*(SORT_NONE(.vector_table)))
	} >flash AT>flash

	.text : {
		*(.rodata .rodata.*)
		*(.srodata .srodata.*)
		*(.text .text.*)
	} >flash AT>flash

	. = ALIGN(4);
	PROVIDE(__etext = .);
	PROVIDE(_etext = .);
	PROVIDE(etext = .);

	.lalign : {
		. = ALIGN(4);
		PROVIDE(_data_lma = .);
	} >flash AT>flash

	.dalign : {
		. = ALIGN(4);
		PROVIDE(_data = .);
	} >ram AT>flash

	.data : {
		_sdata = .;
		*(.rdata);
		*(.data .data.*)
		*(.sdata .sdata.*)
		_edata = .;
	} >ram

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
	} >ram AT>ram

	. = ALIGN(8);
	PROVIDE(_end = .);
	PROVIDE(end = .);

	.stack ORIGIN(ram) + LENGTH(ram) - __stack_size : {
		PROVIDE(_heap_end = .);
		. = __stack_size;
		PROVIDE(_sp = .);
	} >ram AT>ram
}
