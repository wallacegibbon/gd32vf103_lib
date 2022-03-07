#include "riscv_encoding.h"
#include "riscv_bits.h"
#include "n200_eclic.h"
#include "n200_timer.h"

	.macro DISABLE_MIE
	csrc CSR_MSTATUS, MSTATUS_MIE
	.endm


	.macro SAVE_CONTEXT
#ifdef __riscv_flen
	#if (__riscv_flen == 64 )
	addi sp, sp, -20*REGBYTES - 20*FPREGBYTES
	#else
	addi sp, sp, -20*REGBYTES
	#endif
#else
	#ifndef __riscv_32e
	addi sp, sp, -20*REGBYTES
	#else
	addi sp, sp, -14*REGBYTES
	#endif
#endif
	STORE ra,  0*REGBYTES(sp)
	STORE tp,  1*REGBYTES(sp)
	STORE t0,  2*REGBYTES(sp)
	STORE t1,  3*REGBYTES(sp)
	STORE t2,  4*REGBYTES(sp)
	STORE a0,  5*REGBYTES(sp)
	STORE a1,  6*REGBYTES(sp)
	STORE a2,  7*REGBYTES(sp)
	STORE a3,  8*REGBYTES(sp)
	STORE a4,  9*REGBYTES(sp)
	STORE a5, 10*REGBYTES(sp)
#ifndef __riscv_32e
	STORE a6, 14*REGBYTES(sp)
	STORE a7, 15*REGBYTES(sp)
	STORE t3, 16*REGBYTES(sp)
	STORE t4, 17*REGBYTES(sp)
	STORE t5, 18*REGBYTES(sp)
	STORE t6, 19*REGBYTES(sp)
#endif
#ifdef __riscv_flen
	#if (__riscv_flen == 64)
	FPSTORE f0 , (20*REGBYTES +  0*FPREGBYTES)(sp)
	FPSTORE f1 , (20*REGBYTES +  1*FPREGBYTES)(sp)
	FPSTORE f2 , (20*REGBYTES +  2*FPREGBYTES)(sp)
	FPSTORE f3 , (20*REGBYTES +  3*FPREGBYTES)(sp)
	FPSTORE f4 , (20*REGBYTES +  4*FPREGBYTES)(sp)
	FPSTORE f5 , (20*REGBYTES +  5*FPREGBYTES)(sp)
	FPSTORE f6 , (20*REGBYTES +  6*FPREGBYTES)(sp)
	FPSTORE f7 , (20*REGBYTES +  7*FPREGBYTES)(sp)
	FPSTORE f10, (20*REGBYTES +  8*FPREGBYTES)(sp)
	FPSTORE f11, (20*REGBYTES +  9*FPREGBYTES)(sp)
	FPSTORE f12, (20*REGBYTES + 10*FPREGBYTES)(sp)
	FPSTORE f13, (20*REGBYTES + 11*FPREGBYTES)(sp)
	FPSTORE f14, (20*REGBYTES + 12*FPREGBYTES)(sp)
	FPSTORE f15, (20*REGBYTES + 13*FPREGBYTES)(sp)
	FPSTORE f16, (20*REGBYTES + 14*FPREGBYTES)(sp)
	FPSTORE f17, (20*REGBYTES + 15*FPREGBYTES)(sp)
	FPSTORE f28, (20*REGBYTES + 16*FPREGBYTES)(sp)
	FPSTORE f29, (20*REGBYTES + 17*FPREGBYTES)(sp)
	FPSTORE f30, (20*REGBYTES + 18*FPREGBYTES)(sp)
	FPSTORE f31, (20*REGBYTES + 19*FPREGBYTES)(sp)
	#endif
#endif
	.endm


	.macro RESTORE_CONTEXT
	LOAD ra,  0*REGBYTES(sp)
	LOAD tp,  1*REGBYTES(sp)
	LOAD t0,  2*REGBYTES(sp)
	LOAD t1,  3*REGBYTES(sp)
	LOAD t2,  4*REGBYTES(sp)
	LOAD a0,  5*REGBYTES(sp)
	LOAD a1,  6*REGBYTES(sp)
	LOAD a2,  7*REGBYTES(sp)
	LOAD a3,  8*REGBYTES(sp)
	LOAD a4,  9*REGBYTES(sp)
	LOAD a5, 10*REGBYTES(sp)
#ifndef __riscv_32e
	LOAD a6, 14*REGBYTES(sp)
	LOAD a7, 15*REGBYTES(sp)
	LOAD t3, 16*REGBYTES(sp)
	LOAD t4, 17*REGBYTES(sp)
	LOAD t5, 18*REGBYTES(sp)
	LOAD t6, 19*REGBYTES(sp)
#endif
#ifdef __riscv_flen
	#if (__riscv_flen == 64)
	/* Restore fp caller registers */
	FPLOAD f0 , (20*REGBYTES +  0*FPREGBYTES)(sp)
	FPLOAD f1 , (20*REGBYTES +  1*FPREGBYTES)(sp)
	FPLOAD f2 , (20*REGBYTES +  2*FPREGBYTES)(sp)
	FPLOAD f3 , (20*REGBYTES +  3*FPREGBYTES)(sp)
	FPLOAD f4 , (20*REGBYTES +  4*FPREGBYTES)(sp)
	FPLOAD f5 , (20*REGBYTES +  5*FPREGBYTES)(sp)
	FPLOAD f6 , (20*REGBYTES +  6*FPREGBYTES)(sp)
	FPLOAD f7 , (20*REGBYTES +  7*FPREGBYTES)(sp)
	FPLOAD f10, (20*REGBYTES +  8*FPREGBYTES)(sp)
	FPLOAD f11, (20*REGBYTES +  9*FPREGBYTES)(sp)
	FPLOAD f12, (20*REGBYTES + 10*FPREGBYTES)(sp)
	FPLOAD f13, (20*REGBYTES + 11*FPREGBYTES)(sp)
	FPLOAD f14, (20*REGBYTES + 12*FPREGBYTES)(sp)
	FPLOAD f15, (20*REGBYTES + 13*FPREGBYTES)(sp)
	FPLOAD f16, (20*REGBYTES + 14*FPREGBYTES)(sp)
	FPLOAD f17, (20*REGBYTES + 15*FPREGBYTES)(sp)
	FPLOAD f28, (20*REGBYTES + 16*FPREGBYTES)(sp)
	FPLOAD f29, (20*REGBYTES + 17*FPREGBYTES)(sp)
	FPLOAD f30, (20*REGBYTES + 18*FPREGBYTES)(sp)
	FPLOAD f31, (20*REGBYTES + 19*FPREGBYTES)(sp)
	#endif
#endif
#ifdef __riscv_flen
	#if(__riscv_flen == 64 )
	addi sp, sp, 20*REGBYTES + 20*FPREGBYTES
	#else
	addi sp, sp, 20*REGBYTES
	#endif
#else
	#ifndef __riscv_32e
	addi sp, sp, 20*REGBYTES
	#else
	addi sp, sp, 14*REGBYTES
	#endif
#endif
	.endm


	.macro SAVE_CSR_CONTEXT
	csrrwi x0, CSR_PUSHMCAUSE, 11
	csrrwi x0, CSR_PUSHMEPC, 12
	csrrwi x0, CSR_PUSHMSUBM, 13
	.endm


	.macro RESTORE_CSR_CONTEXT
	LOAD t0, 13*REGBYTES(sp)
	csrw CSR_MSUBM, t0
	LOAD t0, 12*REGBYTES(sp)
	csrw CSR_MEPC, t0
	LOAD t0, 11*REGBYTES(sp)
	csrw CSR_MCAUSE, t0
	.endm


	.section .text.trap
	/* In CLIC mode, the trap entry must be 64bytes aligned */
	.align 6
	.global exception_entry
	.weak exception_entry
exception_entry:
	SAVE_CONTEXT
	SAVE_CSR_CONTEXT

	csrr a0, mcause
	mv a1, sp
	call core_exception_handler

	RESTORE_CSR_CONTEXT
	RESTORE_CONTEXT

	mret


	.section .text.irq
	.align 2
	.global irq_entry
	.weak irq_entry
irq_entry:
	SAVE_CONTEXT
	SAVE_CSR_CONTEXT

	/* invoke the handler (with tail-chaining) */
	csrrw ra, CSR_JALMNXTI, ra

	DISABLE_MIE

	RESTORE_CSR_CONTEXT
	RESTORE_CONTEXT

	mret

