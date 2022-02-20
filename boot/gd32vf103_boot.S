#include "riscv_encoding.h"

	.global vtable
	.type vtable, %object
	.section .vector_table, "a", %progbits

	.extern main

	.macro IRQ handler
	.word \handler
	.weak \handler
	.set \handler, default_interrupt_handler
	.endm

vtable:
	j reset_handler
	.align 2
	.word 0
	.word 0
	IRQ eclic_msip_handler
	.word 0
	.word 0
	.word 0
	IRQ eclic_mtip_handler
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	IRQ eclic_bwei_handler
	IRQ eclic_pmovi_handler
	IRQ watchdog_IRQn_handler
	IRQ LVD_IRQn_handler
	IRQ tamper_IRQn_handler
	IRQ RTC_IRQn_handler
	IRQ FMC_IRQn_handler
	IRQ RCU_IRQn_handler
	IRQ EXTI0_IRQn_handler
	IRQ EXTI1_IRQn_handler
	IRQ EXTI2_IRQn_handler
	IRQ EXTI3_IRQn_handler
	IRQ EXTI4_IRQn_handler
	IRQ DMA0_chan0_IRQn_handler
	IRQ DMA0_chan1_IRQn_handler
	IRQ DMA0_chan2_IRQn_handler
	IRQ DMA0_chan3_IRQn_handler
	IRQ DMA0_chan4_IRQn_handler
	IRQ DMA0_chan5_IRQn_handler
	IRQ DMA0_chan6_IRQn_handler
	IRQ ADC0_1_IRQn_handler
	IRQ CAN0_TX_IRQn_handler
	IRQ CAN0_RX0_IRQn_handler
	IRQ CAN0_RX1_IRQn_handler
	IRQ CAN0_EWMC_IRQn_handler
	IRQ EXTI5_9_IRQn_handler
	IRQ TIM0_break_IRQn_handler
	IRQ TIM0_update_IRQn_handler
	IRQ TIM0_trigger_commutation_IRQn_handler
	IRQ TIM0_channel_IRQn_handler
	IRQ TIM1_IRQn_handler
	IRQ TIM2_IRQn_handler
	IRQ TIM3_IRQn_handler
	IRQ I2C0_EV_IRQn_handler
	IRQ I2C0_ER_IRQn_handler
	IRQ I2C1_EV_IRQn_handler
	IRQ I2C1_ER_IRQn_handler
	IRQ SPI0_IRQn_handler
	IRQ SPI1_IRQn_handler
	IRQ USART0_IRQn_handler
	IRQ USART1_IRQn_handler
	IRQ USART2_IRQn_handler
	IRQ EXTI10_15_IRQn_handler
	IRQ RTC_alarm_IRQn_handler
	IRQ USB_wakeup_IRQn_handler
	.word 0
	.word 0
	.word 0
	.word 0
	.word 0
	IRQ EXMC_IRQn_handler
	.word 0
	IRQ TIM4_IRQn_handler
	IRQ SPI2_IRQn_handler
	IRQ UART3_IRQn_handler
	IRQ UART4_IRQn_handler
	IRQ TIM5_IRQn_handler
	IRQ TIM6_IRQn_handler
	IRQ DMA1_chan0_IRQn_handler
	IRQ DMA1_chan1_IRQn_handler
	IRQ DMA1_chan2_IRQn_handler
	IRQ DMA1_chan3_IRQn_handler
	IRQ DMA1_chan4_IRQn_handler
	.word 0
	.word 0
	IRQ CAN1_TX_IRQn_handler
	IRQ CAN1_RX0_IRQn_handler
	IRQ CAN1_RX1_IRQn_handler
	IRQ CAN1_EWMC_IRQn_handler
	IRQ USB_IRQn_handler

	.section .text.default_interrupt_handler, "ax", %progbits
default_interrupt_handler:
	/*
	j default_interrupt_handler
	*/
	mret

	.global reset_handler
	.type reset_handler, @function
reset_handler:
	/* Disable interrupts until they are needed. */
	csrc CSR_MSTATUS, MSTATUS_MIE
	/* Move from 0x00000000 to 0x08000000 address space if necessary. */
	la a0, in_address_space
	li a1, 1
	slli a1, a1, 27
	bleu a1, a0, in_address_space
	add a0, a0, a1
	jr a0
in_address_space:
	/* Load the initial stack pointer value. */
	la sp, _sp
	/* Set the vector table's base address. */
	la a0, vtable
	csrw CSR_MTVT, a0

	/*
	Set non-vectored interrupts to use the default handler.
	(That will gracefully crash the program, so only use vectored
	interrupts for now.)
	*/
	la a0, default_interrupt_handler
	csrw CSR_MTVEC, a0

	/* initialized .data section and clear .bss section */
	la a0, _etext
	la a1, _sdata
	la a2, _edata
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:
	la a0, _sbss
	la a1, _ebss
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:
	/* Call main(0, NULL) */
	li a0, 0
	li a1, 0
	call main

