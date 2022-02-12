#ifndef GD32VF103_DAC_H
#define GD32VF103_DAC_H

#include "gd32vf103.h"

#define DAC					DAC_BASE
#define DAC0					0
#define DAC1					1

#define DAC_CTL					REG32(DAC + 0x00U)

// DAC software trigger register
#define DAC_SWT					REG32(DAC + 0x04U)

// DAC0 12-bit right-aligned data holding register
#define DAC0_R12DH				REG32(DAC + 0x08U)
// DAC0 12-bit left-aligned data holding register
#define DAC0_L12DH				REG32(DAC + 0x0CU)
// DAC0 8-bit right-aligned data holding register
#define DAC0_R8DH				REG32(DAC + 0x10U)

// DAC1 12-bit right-aligned data holding register
#define DAC1_R12DH				REG32(DAC + 0x14U)
// DAC1 12-bit left-aligned data holding register
#define DAC1_L12DH				REG32(DAC + 0x18U)
// DAC1 8-bit right-aligned data holding register
#define DAC1_R8DH				REG32(DAC + 0x1CU)

// DAC concurrent mode 12-bit right-aligned data holding register
#define DACC_R12DH				REG32(DAC + 0x20U)
// DAC concurrent mode 12-bit left-aligned data holding register
#define DACC_L12DH				REG32(DAC + 0x24U)
// DAC concurrent mode 8-bit right-aligned data holding register
#define DACC_R8DH				REG32(DAC + 0x28U)

// DAC0 data output register
#define DAC0_DO					REG32(DAC + 0x2CU)
// DAC1 data output register
#define DAC1_DO					REG32(DAC + 0x30U)


// DAC_CTL
// DAC0 enable/disable bit
#define DAC_CTL_DEN0				BIT(0)

// DAC0 output buffer turn on/turn off bit
#define DAC_CTL_DBOFF0				BIT(1)

// DAC0 trigger enable/disable bit
#define DAC_CTL_DTEN0				BIT(2)

// DAC0 trigger source selection enable/disable bits
#define DAC_CTL_DTSEL0				BITS(3,5)

// DAC0 noise wave mode
#define DAC_CTL_DWM0				BITS(6,7)

// DAC0 noise wave bit width
#define DAC_CTL_DWBW0				BITS(8,11)

// DAC0 DMA enable/disable bit
#define DAC_CTL_DDMAEN0				BIT(12)

// DAC1 enable/disable bit 
#define DAC_CTL_DEN1				BIT(16)

// DAC1 output buffer turn on/turn off bit
#define DAC_CTL_DBOFF1				BIT(17)

// DAC1 trigger enable/disable bit
#define DAC_CTL_DTEN1				BIT(18)

// DAC1 trigger source selection enable/disable bits
#define DAC_CTL_DTSEL1				BITS(19,21)

// DAC1 noise wave mode
#define DAC_CTL_DWM1				BITS(22,23)

// DAC1 noise wave bit width
#define DAC_CTL_DWBW1				BITS(24,27)

// DAC1 DMA enable/disable bit
#define DAC_CTL_DDMAEN1				BIT(28)


// DAC_SWT
// DAC0 software trigger bit, cleared by hardware
#define DAC_SWT_SWTR0				BIT(0)
// DAC1 software trigger bit, cleared by hardware
#define DAC_SWT_SWTR1				BIT(1)


// DAC0_R12DH
// DAC0 12-bit right-aligned data bits
#define DAC0_R12DH_DAC0_DH			BITS(0,11)


// DAC0_L12DH
// DAC0 12-bit left-aligned data bits
#define DAC0_L12DH_DAC0_DH			BITS(4,15)


// DAC0_R8DH
// DAC0 8-bit right-aligned data bits
#define DAC0_R8DH_DAC0_DH			BITS(0,7)


// DAC1_R12DH
// DAC1 12-bit right-aligned data bits
#define DAC1_R12DH_DAC1_DH			BITS(0,11)


// DAC1_L12DH
// DAC1 12-bit left-aligned data bits
#define DAC1_L12DH_DAC1_DH			BITS(4,15)


// DAC1_R8DH
// DAC1 8-bit right-aligned data bits
#define DAC1_R8DH_DAC1_DH			BITS(0,7)


// DACC_R12DH
// DAC concurrent mode DAC0 12-bit right-aligned data bits
#define DACC_R12DH_DAC0_DH			BITS(0,11)
// DAC concurrent mode DAC1 12-bit right-aligned data bits
#define DACC_R12DH_DAC1_DH			BITS(16,27)


// DACC_L12DH
// DAC concurrent mode DAC0 12-bit left-aligned data bits
#define DACC_L12DH_DAC0_DH			BITS(4,15)

// DAC concurrent mode DAC1 12-bit left-aligned data bits
#define DACC_L12DH_DAC1_DH			BITS(20,31)


// DACC_R8DH
// DAC concurrent mode DAC0 8-bit right-aligned data bits
#define DACC_R8DH_DAC0_DH			BITS(0,7)

// DAC concurrent mode DAC1 8-bit right-aligned data bits
#define DACC_R8DH_DAC1_DH			BITS(8,15)


// DAC0_DO
// DAC0 12-bit output data bits
#define DAC0_DO_DAC0_DO				BITS(0,11)


// DAC1_DO
// DAC1 12-bit output data bits
#define DAC1_DO_DAC1_DO				BITS(0,11)


// DAC trigger source
#define CTL_DTSEL(regval) \
	(BITS(3,5) & ((uint32_t) (regval) << 3))

// TIMER5 TRGO
#define DAC_TRIGGER_T5_TRGO			CTL_DTSEL(0)
// TIMER2 TRGO
#define DAC_TRIGGER_T2_TRGO			CTL_DTSEL(1)
// TIMER6 TRGO
#define DAC_TRIGGER_T6_TRGO			CTL_DTSEL(2)
// TIMER4 TRGO
#define DAC_TRIGGER_T4_TRGO			CTL_DTSEL(3)
// TIMER1 TRGO
#define DAC_TRIGGER_T1_TRGO			CTL_DTSEL(4)
// TIMER3 TRGO
#define DAC_TRIGGER_T3_TRGO			CTL_DTSEL(5)

// EXTI interrupt line9 event
#define DAC_TRIGGER_EXTI_9			CTL_DTSEL(6)

// software trigger
#define DAC_TRIGGER_SOFTWARE			CTL_DTSEL(7)


// DAC noise wave mode
#define CTL_DWM(regval) \
	(BITS(6,7) & ((uint32_t) (regval) << 6))

// wave disable
#define DAC_WAVE_DISABLE			CTL_DWM(0)

// LFSR noise mode
#define DAC_WAVE_MODE_LFSR			CTL_DWM(1)

// triangle noise mode
#define DAC_WAVE_MODE_TRIANGLE			CTL_DWM(2)


// DAC noise wave bit width
#define DWBW(regval) \
	(BITS(8,11) & ((uint32_t) (regval) << 8))

// bit width of the wave signal is 1 - 12
#define DAC_WAVE_BIT_WIDTH_1			DWBW(0)
#define DAC_WAVE_BIT_WIDTH_2			DWBW(1)
#define DAC_WAVE_BIT_WIDTH_3			DWBW(2)
#define DAC_WAVE_BIT_WIDTH_4			DWBW(3)
#define DAC_WAVE_BIT_WIDTH_5			DWBW(4)
#define DAC_WAVE_BIT_WIDTH_6			DWBW(5)
#define DAC_WAVE_BIT_WIDTH_7			DWBW(6)
#define DAC_WAVE_BIT_WIDTH_8			DWBW(7)
#define DAC_WAVE_BIT_WIDTH_9			DWBW(8)
#define DAC_WAVE_BIT_WIDTH_10			DWBW(9)
#define DAC_WAVE_BIT_WIDTH_11			DWBW(10)
#define DAC_WAVE_BIT_WIDTH_12			DWBW(11)


// unmask LFSR bits in DAC LFSR noise mode
// unmask the LFSR bit 0, [1:0], [2:0] ... [11:0]
#define DAC_LFSR_BIT0				DAC_WAVE_BIT_WIDTH_1
#define DAC_LFSR_BITS1_0			DAC_WAVE_BIT_WIDTH_2
#define DAC_LFSR_BITS2_0			DAC_WAVE_BIT_WIDTH_3
#define DAC_LFSR_BITS3_0			DAC_WAVE_BIT_WIDTH_4
#define DAC_LFSR_BITS4_0			DAC_WAVE_BIT_WIDTH_5
#define DAC_LFSR_BITS5_0			DAC_WAVE_BIT_WIDTH_6
#define DAC_LFSR_BITS6_0			DAC_WAVE_BIT_WIDTH_7
#define DAC_LFSR_BITS7_0			DAC_WAVE_BIT_WIDTH_8
#define DAC_LFSR_BITS8_0			DAC_WAVE_BIT_WIDTH_9
#define DAC_LFSR_BITS9_0			DAC_WAVE_BIT_WIDTH_10
#define DAC_LFSR_BITS10_0			DAC_WAVE_BIT_WIDTH_11
#define DAC_LFSR_BITS11_0			DAC_WAVE_BIT_WIDTH_12


// DAC data alignment
#define DATA_ALIGN(regval) \
	(BITS(0,1) & ((uint32_t) (regval) << 0))

// data right 12b alignment
#define DAC_ALIGN_12B_R				DATA_ALIGN(0)
// data left  12b alignment
#define DAC_ALIGN_12B_L				DATA_ALIGN(1)
// data right  8b alignment
#define DAC_ALIGN_8B_R				DATA_ALIGN(2)

// triangle amplitude in DAC triangle noise mode
// triangle amplitude is 1, 3, 7, ... 4095
#define DAC_TRIANGLE_AMPLITUDE_1		DAC_WAVE_BIT_WIDTH_1
#define DAC_TRIANGLE_AMPLITUDE_3		DAC_WAVE_BIT_WIDTH_2
#define DAC_TRIANGLE_AMPLITUDE_7		DAC_WAVE_BIT_WIDTH_3
#define DAC_TRIANGLE_AMPLITUDE_15		DAC_WAVE_BIT_WIDTH_4
#define DAC_TRIANGLE_AMPLITUDE_31		DAC_WAVE_BIT_WIDTH_5
#define DAC_TRIANGLE_AMPLITUDE_63		DAC_WAVE_BIT_WIDTH_6
#define DAC_TRIANGLE_AMPLITUDE_127		DAC_WAVE_BIT_WIDTH_7
#define DAC_TRIANGLE_AMPLITUDE_255		DAC_WAVE_BIT_WIDTH_8
#define DAC_TRIANGLE_AMPLITUDE_511		DAC_WAVE_BIT_WIDTH_9
#define DAC_TRIANGLE_AMPLITUDE_1023		DAC_WAVE_BIT_WIDTH_10
#define DAC_TRIANGLE_AMPLITUDE_2047		DAC_WAVE_BIT_WIDTH_11
#define DAC_TRIANGLE_AMPLITUDE_4095		DAC_WAVE_BIT_WIDTH_12


// initialization functions
void dac_deinit();

void dac_enable(uint32_t dac_periph);
void dac_disable(uint32_t dac_periph);

void dac_dma_enable(uint32_t dac_periph);
void dac_dma_disable(uint32_t dac_periph); 

void dac_output_buffer_enable(uint32_t dac_periph);
void dac_output_buffer_disable(uint32_t dac_periph);

uint16_t dac_output_value_get(uint32_t dac_periph);

void dac_data_set(uint32_t dac_periph, uint32_t dac_align, uint16_t data);

// DAC trigger configuration
void dac_trigger_enable(uint32_t dac_periph);
void dac_trigger_disable(uint32_t dac_periph);

void dac_trigger_source_config(uint32_t dac_periph, uint32_t triggersource);

void dac_software_trigger_enable(uint32_t dac_periph);
void dac_software_trigger_disable(uint32_t dac_periph);

// DAC wave mode configuration
// configure DAC wave mode
void dac_wave_mode_config(uint32_t dac_periph, uint32_t wave_mode);

// configure DAC wave bit width
void dac_wave_bit_width_config(uint32_t dac_periph, uint32_t bit_width);

// configure DAC LFSR noise mode
void dac_lfsr_noise_config(uint32_t dac_periph, uint32_t unmask_bits);

// configure DAC triangle noise mode
void dac_triangle_noise_config(uint32_t dac_periph, uint32_t amplitude);


// DAC concurrent mode configuration
void dac_concurrent_enable();
void dac_concurrent_disable();

void dac_concurrent_software_trigger_enable();
void dac_concurrent_software_trigger_disable();

void dac_concurrent_output_buffer_enable();
void dac_concurrent_output_buffer_disable();

// set DAC concurrent mode data holding register value
void dac_concurrent_data_set(uint32_t dac_align, uint16_t data0,
		uint16_t data1); 

#endif
