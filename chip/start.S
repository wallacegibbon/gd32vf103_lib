#include "riscv_encoding.h"

	.extern system_init
	.extern main

	.global vtable
	.type vtable, @object
	.section .vector_table, "a", @progbits

	.macro IRQ handler
	.word \handler
	.weak \handler
	.set \handler, default_interrupt_handler
	.endm

vtable:
	j _start
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

	.section .text.default_interrupt_handler, "ax", @progbits
default_interrupt_handler:
	j default_interrupt_handler
	/*
	mret
	*/

	.global _start
	.type _start, @function
_start:
	/* disable interrupts until they are needed. */
	csrc CSR_MSTATUS, MSTATUS_MIE

	/* move from 0x00000000 to 0x08000000 address space if necessary. */

	la a0, _start_1

	/* compare with 2^29 (0x20000000) */
	li a1, 1
	slli a1, a1, 29
	bleu a1, a0, _start_1

	/* compare with 2^27 (0x08000000) */
	srli a1, a1, 2
	bleu a1, a0, _start_1

	la a0, _start_1
	add a0, a0, a1
	jr a0
_start_1:
	/* load the initial stack pointer value. */
	la sp, _sp

	/* set the vector table's base address. */
	la t0, vtable
	csrw CSR_MTVT, t0

	/* init mtvt2 and enable it */
	la t0, irq_entry
	csrw CSR_MTVT2, t0
	csrs CSR_MTVT2, 1

	/* init csr mtvec for trap and nmi base addr */
	la t0, trap_entry
	csrw CSR_MTVEC, t0

	/* set the nmi base to share with mtvec */
	li t0, 0x200
	csrs CSR_MMISC_CTL, t0

#ifdef __riscv_flen
	/* enable fpu */
	li t0, MSTATUS_FS
	csrs mstatus, t0
	csrw fcsr, x0
#endif

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
	call disable_mcycle_minstret
	call system_init
	/* Call main(0, NULL) */
	li a0, 0
	li a1, 0
	call main
1:
	j 1b

	/* it can save power when the cycle/instret is disabled */
	.global disable_mcycle_minstret
	.type disable_mcycle_minstret, @function
disable_mcycle_minstret:
	csrsi CSR_MCOUNTINHIBIT, 5
	ret

	.global enable_mcycle_minstret
	.type enable_mcycle_minstret, @function
enable_mcycle_minstret:
	csrci CSR_MCOUNTINHIBIT, 5
	ret

