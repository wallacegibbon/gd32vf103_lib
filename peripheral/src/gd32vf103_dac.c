#include "gd32vf103_dac.h"

#define DAC1_REG_OFFSET			((uint32_t) 16U)
#define DH_12BIT_OFFSET			((uint32_t) 16U)
#define DH_8BIT_OFFSET			((uint32_t) 8U)

void dac_deinit() {
	rcu_periph_reset_enable(RCU_DACRST);
	rcu_periph_reset_disable(RCU_DACRST);
}

void dac_enable(uint32_t dac_periph) {
	if (dac_periph == DAC0)
		DAC_CTL |= DAC_CTL_DEN0;
	else
		DAC_CTL |= DAC_CTL_DEN1;
}

void dac_disable(uint32_t dac_periph) {
	if (dac_periph == DAC0)
		DAC_CTL &= ~DAC_CTL_DEN0;
	else
		DAC_CTL &= ~DAC_CTL_DEN1;
}

void dac_dma_enable(uint32_t dac_periph) {
	if (dac_periph == DAC0)
		DAC_CTL |= DAC_CTL_DDMAEN0;
	else
		DAC_CTL |= DAC_CTL_DDMAEN1;
}

void dac_dma_disable(uint32_t dac_periph) {
	if (dac_periph == DAC0)
		DAC_CTL &= ~DAC_CTL_DDMAEN0;
	else
		DAC_CTL &= ~DAC_CTL_DDMAEN1;
}

void dac_output_buffer_enable(uint32_t dac_periph) {
	if (dac_periph == DAC0)
		DAC_CTL &= ~DAC_CTL_DBOFF0;
	else
		DAC_CTL &= ~DAC_CTL_DBOFF1;
}

void dac_output_buffer_disable(uint32_t dac_periph) {
	if (dac_periph == DAC0)
		DAC_CTL |= DAC_CTL_DBOFF0;
	else
		DAC_CTL |= DAC_CTL_DBOFF1;
}

uint16_t dac_output_value_get(uint32_t dac_periph) {
	if (dac_periph == DAC0)
		return (uint16_t) DAC0_DO;
	else
		return (uint16_t) DAC1_DO;
}

/*
 * set the DAC specified data holding register value
 * dac_align: data alignment
 * 	DAC_ALIGN_8B_R: data right 8 bit alignment
 * 	DAC_ALIGN_12B_R: data right 12 bit alignment
 * 	DAC_ALIGN_12B_L: data left 12 bit alignment
 *
 * data: data to be loaded
 */
void dac_data_set(uint32_t dac_periph, uint32_t dac_align, uint16_t data) {
	if (dac_periph == DAC0) {
		switch (dac_align) {
		case DAC_ALIGN_12B_R:
			DAC0_R12DH = data;
			break;
		case DAC_ALIGN_12B_L:
			DAC0_L12DH = data;
			break;
		case DAC_ALIGN_8B_R:
			DAC0_R8DH = data;
			break;
		default:
			break;
		}
	} else {
		switch (dac_align) {
		case DAC_ALIGN_12B_R:
			DAC1_R12DH = data;
			break;
		case DAC_ALIGN_12B_L:
			DAC1_L12DH = data;
			break;
		case DAC_ALIGN_8B_R:
			DAC1_R8DH = data;
			break;
		default:
			break;
		}
	}
}

void dac_trigger_enable(uint32_t dac_periph) {
	if (dac_periph == DAC0)
		DAC_CTL |= DAC_CTL_DTEN0;
	else
		DAC_CTL |= DAC_CTL_DTEN1;
}

void dac_trigger_disable(uint32_t dac_periph) {
	if (dac_periph == DAC0)
		DAC_CTL &= ~DAC_CTL_DTEN0;
	else
		DAC_CTL &= ~DAC_CTL_DTEN1;
}

/*
 * triggersource: external triggers of DAC
 * 	DAC_TRIGGER_T1_TRGO: TIMER1 TRGO
 * 	DAC_TRIGGER_T2_TRGO: TIMER2 TRGO
 * 	DAC_TRIGGER_T3_TRGO: TIMER3 TRGO
 * 	DAC_TRIGGER_T4_TRGO: TIMER4 TRGO
 * 	DAC_TRIGGER_T5_TRGO: TIMER5 TRGO
 * 	DAC_TRIGGER_T6_TRGO: TIMER6 TRGO
 * 	DAC_TRIGGER_EXTI_9: EXTI interrupt line9 event
 * 	DAC_TRIGGER_SOFTWARE: software trigger
 */
void dac_trigger_source_config(uint32_t dac_periph, uint32_t triggersource) {
	if (dac_periph == DAC0) {
		DAC_CTL &= ~DAC_CTL_DTSEL0;
		DAC_CTL |= triggersource;
	} else {
		DAC_CTL &= ~DAC_CTL_DTSEL1;
		DAC_CTL |= triggersource << DAC1_REG_OFFSET;
	}
}

void dac_software_trigger_enable(uint32_t dac_periph) {
	if (dac_periph == DAC0)
		DAC_SWT |= DAC_SWT_SWTR0;
	else
		DAC_SWT |= DAC_SWT_SWTR1;
}

void dac_software_trigger_disable(uint32_t dac_periph) {
	if (dac_periph == DAC0)
		DAC_SWT &= ~DAC_SWT_SWTR0;
	else
		DAC_SWT &= ~DAC_SWT_SWTR1;
}

/*
 * wave_mode: noise wave mode
 * 	DAC_WAVE_DISABLE: wave disable
 * 	DAC_WAVE_MODE_LFSR: LFSR noise mode
 * 	DAC_WAVE_MODE_TRIANGLE: triangle noise mode
 */
void dac_wave_mode_config(uint32_t dac_periph, uint32_t wave_mode) {
	if (dac_periph == DAC0) {
		DAC_CTL &= ~DAC_CTL_DWM0;
		DAC_CTL |= wave_mode;
	} else {
		DAC_CTL &= ~DAC_CTL_DWM1;
		DAC_CTL |= (wave_mode << DAC1_REG_OFFSET);
	}
}

/*
 * bit_width: noise wave bit width
 * 	DAC_WAVE_BIT_WIDTH_1: bit width of the wave signal is 1
 * 	DAC_WAVE_BIT_WIDTH_2: bit width of the wave signal is 2
 * 	DAC_WAVE_BIT_WIDTH_3: bit width of the wave signal is 3
 * 	DAC_WAVE_BIT_WIDTH_4: bit width of the wave signal is 4
 * 	DAC_WAVE_BIT_WIDTH_5: bit width of the wave signal is 5
 * 	DAC_WAVE_BIT_WIDTH_6: bit width of the wave signal is 6
 * 	DAC_WAVE_BIT_WIDTH_7: bit width of the wave signal is 7
 * 	DAC_WAVE_BIT_WIDTH_8: bit width of the wave signal is 8
 * 	DAC_WAVE_BIT_WIDTH_9: bit width of the wave signal is 9
 * 	DAC_WAVE_BIT_WIDTH_10: bit width of the wave signal is 10
 * 	DAC_WAVE_BIT_WIDTH_11: bit width of the wave signal is 11
 * 	DAC_WAVE_BIT_WIDTH_12: bit width of the wave signal is 12
 */
void dac_wave_bit_width_config(uint32_t dac_periph, uint32_t bit_width) {
	if (dac_periph == DAC0) {
		DAC_CTL &= ~DAC_CTL_DWBW0;
		DAC_CTL |= bit_width;
	} else {
		DAC_CTL &= ~DAC_CTL_DWBW1;
		DAC_CTL |= (bit_width << DAC1_REG_OFFSET);
	}
}

/*
 * unmask_bits: unmask LFSR bits in DAC LFSR noise mode
 * 	DAC_LFSR_BIT0: unmask the LFSR bit0
 * 	DAC_LFSR_BITS1_0: unmask the LFSR bits[1:0]
 * 	DAC_LFSR_BITS2_0: unmask the LFSR bits[2:0]
 * 	DAC_LFSR_BITS3_0: unmask the LFSR bits[3:0]
 * 	DAC_LFSR_BITS4_0: unmask the LFSR bits[4:0]
 * 	DAC_LFSR_BITS5_0: unmask the LFSR bits[5:0]
 * 	DAC_LFSR_BITS6_0: unmask the LFSR bits[6:0]
 * 	DAC_LFSR_BITS7_0: unmask the LFSR bits[7:0]
 * 	DAC_LFSR_BITS8_0: unmask the LFSR bits[8:0]
 * 	DAC_LFSR_BITS9_0: unmask the LFSR bits[9:0]
 * 	DAC_LFSR_BITS10_0: unmask the LFSR bits[10:0]
 * 	DAC_LFSR_BITS11_0: unmask the LFSR bits[11:0]
 */
void dac_lfsr_noise_config(uint32_t dac_periph, uint32_t unmask_bits) {
	if (dac_periph == DAC0) {
		DAC_CTL &= ~DAC_CTL_DWBW0;
		DAC_CTL |= unmask_bits;
	} else {
		DAC_CTL &= ~DAC_CTL_DWBW1;
		DAC_CTL |= (unmask_bits << DAC1_REG_OFFSET);
	}
}

/*
 * amplitude: triangle amplitude in DAC triangle noise mode
 * 	DAC_TRIANGLE_AMPLITUDE_1: triangle amplitude is 1
 * 	DAC_TRIANGLE_AMPLITUDE_3: triangle amplitude is 3
 * 	DAC_TRIANGLE_AMPLITUDE_7: triangle amplitude is 7
 * 	DAC_TRIANGLE_AMPLITUDE_15: triangle amplitude is 15
 * 	DAC_TRIANGLE_AMPLITUDE_31: triangle amplitude is 31
 * 	DAC_TRIANGLE_AMPLITUDE_63: triangle amplitude is 63
 * 	DAC_TRIANGLE_AMPLITUDE_127: triangle amplitude is 127
 * 	DAC_TRIANGLE_AMPLITUDE_255: triangle amplitude is 255
 * 	DAC_TRIANGLE_AMPLITUDE_511: triangle amplitude is 511
 * 	DAC_TRIANGLE_AMPLITUDE_1023: triangle amplitude is 1023
 * 	DAC_TRIANGLE_AMPLITUDE_2047: triangle amplitude is 2047
 * 	DAC_TRIANGLE_AMPLITUDE_4095: triangle amplitude is 4095
 */
void dac_triangle_noise_config(uint32_t dac_periph, uint32_t amplitude) {
	if (dac_periph == DAC0) {
		DAC_CTL &= ~DAC_CTL_DWBW0;
		DAC_CTL |= amplitude;
	} else {
		DAC_CTL &= ~DAC_CTL_DWBW1;
		DAC_CTL |= (amplitude << DAC1_REG_OFFSET);
	}
}

void dac_concurrent_enable() {
	DAC_CTL |= DAC_CTL_DEN0 | DAC_CTL_DEN1;
}

void dac_concurrent_disable() {
	DAC_CTL &= ~(DAC_CTL_DEN0 | DAC_CTL_DEN1);
}

void dac_concurrent_software_trigger_enable() {
	DAC_SWT |= DAC_SWT_SWTR0 | DAC_SWT_SWTR1;
}

void dac_concurrent_software_trigger_disable() {
	DAC_SWT &= ~(DAC_SWT_SWTR0 | DAC_SWT_SWTR1);
}

void dac_concurrent_output_buffer_enable() {
	DAC_CTL &= DAC_CTL_DBOFF0 | DAC_CTL_DBOFF1;
}

void dac_concurrent_output_buffer_disable() {
	DAC_CTL |= DAC_CTL_DBOFF0 | DAC_CTL_DBOFF1;
}

/*
 * set DAC concurrent mode data holding register value
 * dac_align: data alignment
 * 	DAC_ALIGN_8B_R: data right 8b alignment
 * 	DAC_ALIGN_12B_R: data right 12b alignment
 * 	DAC_ALIGN_12B_L: data left 12b alignment
 *
 * data0: data to be loaded
 * data1: data to be loaded
 */
void dac_concurrent_data_set(uint32_t dac_align, uint16_t data0,
		uint16_t data1) {

	switch (dac_align) {
	case DAC_ALIGN_12B_R:
		DACC_R12DH = (data1 << DH_12BIT_OFFSET) | data0;
		break;
	case DAC_ALIGN_12B_L:
		DACC_L12DH = (data1 << DH_12BIT_OFFSET) | data0;
		break;
	case DAC_ALIGN_8B_R:
		DACC_R8DH = (data1 << DH_8BIT_OFFSET) | data0;
		break;
	default:
		break;
	}
}
