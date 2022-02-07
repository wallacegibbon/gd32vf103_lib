#include "gd32vf103_adc.h"

/* discontinuous mode macro*/
#define  ADC_CHANNEL_LENGTH_SUBTRACT_ONE            ((uint8_t)1U)

/* ADC regular channel macro */
#define  ADC_REGULAR_CHANNEL_RANK_SIX               ((uint8_t)6U)
#define  ADC_REGULAR_CHANNEL_RANK_TWELVE            ((uint8_t)12U)
#define  ADC_REGULAR_CHANNEL_RANK_SIXTEEN           ((uint8_t)16U)
#define  ADC_REGULAR_CHANNEL_RANK_LENGTH            ((uint8_t)5U)

/* ADC sampling time macro */
#define  ADC_CHANNEL_SAMPLE_TEN                     ((uint8_t)10U)
#define  ADC_CHANNEL_SAMPLE_EIGHTEEN                ((uint8_t)18U)
#define  ADC_CHANNEL_SAMPLE_LENGTH                  ((uint8_t)3U)

/* ADC inserted channel macro */
#define  ADC_INSERTED_CHANNEL_RANK_LENGTH           ((uint8_t)5U)
#define  ADC_INSERTED_CHANNEL_SHIFT_LENGTH          ((uint8_t)15U)

/* ADC inserted channel offset macro */
#define  ADC_OFFSET_LENGTH                          ((uint8_t)3U)
#define  ADC_OFFSET_SHIFT_LENGTH                    ((uint8_t)4U)

/* reset ADC */
void adc_deinit(uint32_t adc_periph) {
	switch (adc_periph) {
	case ADC0:
		rcu_periph_reset_enable(RCU_ADC0RST);
		rcu_periph_reset_disable(RCU_ADC0RST);
		break;
	case ADC1:
		rcu_periph_reset_enable(RCU_ADC1RST);
		rcu_periph_reset_disable(RCU_ADC1RST);
		break;
	default:
		break;
	}
}

/*
 * configure the ADC sync mode
 *
 * ADC_MODE_FREE:
 * 	all the ADCs work independently
 *
 * ADC_DAUL_REGULAL_PARALLEL_INSERTED_PARALLEL:
 * 	ADC0 and ADC1 work in combined regular parallel + inserted parallel mode
 *
 * ADC_DAUL_REGULAL_PARALLEL_INSERTED_ROTATION:
 * 	ADC0 and ADC1 work in combined regular parallel + trigger rotation mode
 *
 * ADC_DAUL_INSERTED_PARALLEL_REGULAL_FOLLOWUP_FAST:
 * 	ADC0 and ADC1 work in combined inserted parallel + follow-up fast mode
 *
 * ADC_DAUL_INSERTED_PARALLEL_REGULAL_FOLLOWUP_SLOW:
 * 	ADC0 and ADC1 work in combined inserted parallel + follow-up slow mode
 *
 * ADC_DAUL_INSERTED_PARALLEL:
 * 	ADC0 and ADC1 work in inserted parallel mode only
 *
 * ADC_DAUL_REGULAL_PARALLEL:
 * 	ADC0 and ADC1 work in regular parallel mode only
 *
 * ADC_DAUL_REGULAL_FOLLOWUP_FAST:
 * 	ADC0 and ADC1 work in follow-up fast mode only
 *
 * ADC_DAUL_REGULAL_FOLLOWUP_SLOW:
 * 	ADC0 and ADC1 work in follow-up slow mode only
 *
 * ADC_DAUL_INSERTED_TRIGGER_ROTATION:
 * 	ADC0 and ADC1 work in trigger rotation mode only
 *
 */
void adc_mode_config(uint32_t mode) {
	ADC_CTL0(ADC0) &= ~(ADC_CTL0_SYNCM);
	ADC_CTL0(ADC0) |= mode;
}

/*
 * enable or disable ADC special function
 * adc_periph: ADCx, x=0,1
 *
 * function: the function to config
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_SCAN_MODE:
 * 			scan mode select
 *
 * 		ADC_INSERTED_CHANNEL_AUTO:
 * 			inserted channel group convert automatically
 *
 * 		ADC_CONTINUOUS_MODE:
 * 			continuous mode select
 *
 * newvalue: ENABLE or DISABLE
 */
void adc_special_function_config(uint32_t adc_periph, uint32_t function,
		enum control_status newvalue) {
	if (newvalue) {
		if (0U != (function & ADC_SCAN_MODE)) {
			// enable scan mode
			ADC_CTL0(adc_periph) |= ADC_SCAN_MODE;
		}
		if (0U != (function & ADC_INSERTED_CHANNEL_AUTO)) {
			// enable inserted channel group convert automatically
			ADC_CTL0(adc_periph) |= ADC_INSERTED_CHANNEL_AUTO;
		}
		if (0U != (function & ADC_CONTINUOUS_MODE)) {
			// enable continuous mode
			ADC_CTL1(adc_periph) |= ADC_CONTINUOUS_MODE;
		}
	} else {
		if (0U != (function & ADC_SCAN_MODE)) {
			// disable scan mode
			ADC_CTL0(adc_periph) &= ~ADC_SCAN_MODE;
		}
		if (0U != (function & ADC_INSERTED_CHANNEL_AUTO)) {
			// disable inserted channel group convert automatically
			ADC_CTL0(adc_periph) &= ~ADC_INSERTED_CHANNEL_AUTO;
		}
		if (0U != (function & ADC_CONTINUOUS_MODE)) {
			/* disable continuous mode */
			ADC_CTL1(adc_periph) &= ~ADC_CONTINUOUS_MODE;
		}
	}
}

/*
 * configure ADC data alignment 
 * adc_periph: ADCx, x=0,1
 *
 * data_alignment: data alignment select
 * 	only one parameter can be selected which is shown as below:
 *	ADC_DATAALIGN_RIGHT: LSB alignment
 *	ADC_DATAALIGN_LEFT: MSB alignment
 *
 */
void adc_data_alignment_config(uint32_t adc_periph, uint32_t data_alignment) {
	if (ADC_DATAALIGN_RIGHT != data_alignment)
		// MSB alignment
		ADC_CTL1(adc_periph) |= ADC_CTL1_DAL;
	else
		// LSB alignment
		ADC_CTL1(adc_periph) &= ~((uint32_t) ADC_CTL1_DAL);
}

void adc_enable(uint32_t adc_periph) {
	if (RESET == (ADC_CTL1(adc_periph) & ADC_CTL1_ADCON))
		ADC_CTL1(adc_periph) |= (uint32_t) ADC_CTL1_ADCON;
}

void adc_disable(uint32_t adc_periph) {
	ADC_CTL1(adc_periph) &= ~((uint32_t) ADC_CTL1_ADCON);
}

/*
 * ADC calibration and reset calibration
 */
void adc_calibration_enable(uint32_t adc_periph) {
	/* reset the selected ADC1 calibration registers */
	ADC_CTL1(adc_periph) |= (uint32_t) ADC_CTL1_RSTCLB;

	/* check the RSTCLB bit state */
	while (RESET != (ADC_CTL1(adc_periph) & ADC_CTL1_RSTCLB));

	/* enable ADC calibration process */
	ADC_CTL1(adc_periph) |= ADC_CTL1_CLB;

	/* check the CLB bit state */
	while (RESET != (ADC_CTL1(adc_periph) & ADC_CTL1_CLB));
}

/*
 * enable the temperature sensor and Vrefint channel
 */
void adc_tempsensor_vrefint_enable(void) {
	/* enable the temperature sensor and Vrefint channel */
	ADC_CTL1(ADC0) |= ADC_CTL1_TSVREN;
}

/*
 * disable the temperature sensor and Vrefint channel
 */
void adc_tempsensor_vrefint_disable(void) {
	/* disable the temperature sensor and Vrefint channel */
	ADC_CTL1(ADC0) &= ~ADC_CTL1_TSVREN;
}

void adc_dma_mode_enable(uint32_t adc_periph) {
	/* enable DMA request */
	ADC_CTL1(adc_periph) |= (uint32_t) (ADC_CTL1_DMA);
}

void adc_dma_mode_disable(uint32_t adc_periph) {
	/* disable DMA request */
	ADC_CTL1(adc_periph) &= ~((uint32_t) ADC_CTL1_DMA);
}

/*
 * configure ADC discontinuous mode 
 * dc_periph: ADCx, x=0,1
 *
 * adc_channel_group: select the channel group
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_REGULAR_CHANNEL:
 * 			regular channel group
 *
 * 		ADC_INSERTED_CHANNEL:
 * 			inserted channel group
 *
 * 		ADC_CHANNEL_DISCON_DISABLE:
 * 			disable discontinuous mode of regular & inserted channel
 *
 * length: number of conversions in discontinuous mode,the number can be 1..8
 * 	for regular channel, the number has no effect for inserted channel
 */
void adc_discontinuous_mode_config(uint32_t adc_periph,
		uint8_t adc_channel_group, uint8_t length) {
	/* disable discontinuous mode of regular & inserted channel */
	ADC_CTL0(adc_periph) &= ~((uint32_t) (ADC_CTL0_DISRC | ADC_CTL0_DISIC));
	switch (adc_channel_group) {
	case ADC_REGULAR_CHANNEL:
		/* config the number of conversions in discontinuous mode */
		ADC_CTL0(adc_periph) &= ~((uint32_t) ADC_CTL0_DISNUM);
		ADC_CTL0(adc_periph) |= CTL0_DISNUM(((uint32_t) length -
					ADC_CHANNEL_LENGTH_SUBTRACT_ONE));
		/* enable regular channel group discontinuous mode */
		ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_DISRC;
		break;
	case ADC_INSERTED_CHANNEL:
		/* enable inserted channel group discontinuous mode */
		ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_DISIC;
		break;
	case ADC_CHANNEL_DISCON_DISABLE:
		/* disable discontinuous mode of regular & inserted channel */
	default:
		break;
	}
}

/*
 * configure the length of regular channel group or inserted channel group
 * adc_periph: ADCx, x=0,1
 *
 * adc_channel_group: select the channel group
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_REGULAR_CHANNEL: regular channel group
 * 		ADC_INSERTED_CHANNEL: inserted channel group
 *
 * length: the length of the channel
 * 	regular channel 1-16
 * 	inserted channel 1-4
 *
 */
void adc_channel_length_config(uint32_t adc_periph, uint8_t adc_channel_group,
		uint32_t length) {
	switch (adc_channel_group) {
	case ADC_REGULAR_CHANNEL:
		/* configure the length of regular channel group */
		ADC_RSQ0(adc_periph) &= ~((uint32_t) ADC_RSQ0_RL);
		ADC_RSQ0(adc_periph) |=
			RSQ0_RL((uint32_t) (length -
						ADC_CHANNEL_LENGTH_SUBTRACT_ONE));
		break;
	case ADC_INSERTED_CHANNEL:
		/* configure the length of inserted channel group */
		ADC_ISQ(adc_periph) &= ~((uint32_t) ADC_ISQ_IL);
		ADC_ISQ(adc_periph) |=
		    ISQ_IL((uint32_t) (length -
					    ADC_CHANNEL_LENGTH_SUBTRACT_ONE));
		break;
	default:
		break;
	}
}

/*
 * configure ADC regular channel 
 * dc_periph:
 * 	ADCx, x=0,1
 *
 * rank:
 * 	the regular group sequence rank,this parameter must be between 0 to 15
 *
 * adc_channel: the selected ADC channel
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_CHANNEL_x(x=0..17)(x=16 and x=17 are only for ADC0):
 * 			ADC Channelx 
 *
 * sample_time: the sample time value
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_SAMPLETIME_1POINT5: 1.5 cycles
 * 		ADC_SAMPLETIME_7POINT5: 7.5 cycles
 * 		ADC_SAMPLETIME_13POINT5: 13.5 cycles
 * 		ADC_SAMPLETIME_28POINT5: 28.5 cycles
 * 		ADC_SAMPLETIME_41POINT5: 41.5 cycles
 * 		ADC_SAMPLETIME_55POINT5: 55.5 cycles
 * 		ADC_SAMPLETIME_71POINT5: 71.5 cycles
 * 		ADC_SAMPLETIME_239POINT5: 239.5 cycles
 */
void adc_regular_channel_config(uint32_t adc_periph, uint8_t rank,
		uint8_t adc_channel, uint32_t sample_time) {
	uint32_t rsq, sampt;

	// ADC regular sequence config
	if (rank < ADC_REGULAR_CHANNEL_RANK_SIX) {
		// the regular group sequence rank is smaller than six
		rsq = ADC_RSQ2(adc_periph);
		rsq &= ~((uint32_t) (ADC_RSQX_RSQN <<
					(ADC_REGULAR_CHANNEL_RANK_LENGTH * rank)));
		// the channel number is written to these bits to select
		// a channel as the nth conversion in the regular channel group
		rsq |= ((uint32_t) adc_channel <<
				(ADC_REGULAR_CHANNEL_RANK_LENGTH * rank));
		ADC_RSQ2(adc_periph) = rsq;
	} else if (rank < ADC_REGULAR_CHANNEL_RANK_TWELVE) {
		// the regular group sequence rank is smaller than twelve
		rsq = ADC_RSQ1(adc_periph);
		rsq &= ~((uint32_t) (ADC_RSQX_RSQN <<
					(ADC_REGULAR_CHANNEL_RANK_LENGTH *
					 (rank - ADC_REGULAR_CHANNEL_RANK_SIX))));
		// the channel number is written to these bits to select
		// a channel as the nth conversion in the regular channel group
		rsq |= ((uint32_t) adc_channel <<
				(ADC_REGULAR_CHANNEL_RANK_LENGTH *
				 (rank - ADC_REGULAR_CHANNEL_RANK_SIX)));
		ADC_RSQ1(adc_periph) = rsq;
	} else if (rank < ADC_REGULAR_CHANNEL_RANK_SIXTEEN) {
		// the regular group sequence rank is smaller than sixteen
		rsq = ADC_RSQ0(adc_periph);
		rsq &= ~((uint32_t) (ADC_RSQX_RSQN <<
					(ADC_REGULAR_CHANNEL_RANK_LENGTH *
					 (rank - ADC_REGULAR_CHANNEL_RANK_TWELVE))));
		// the channel number is written to these bits to select
		// a channel as the nth conversion in the regular channel group
		rsq |= ((uint32_t) adc_channel <<
				(ADC_REGULAR_CHANNEL_RANK_LENGTH *
				 (rank - ADC_REGULAR_CHANNEL_RANK_TWELVE)));
		ADC_RSQ0(adc_periph) = rsq;
	} else {
	}

	/* ADC sampling time config */
	if (adc_channel < ADC_CHANNEL_SAMPLE_TEN) {
		// the regular group sequence rank is smaller than ten
		sampt = ADC_SAMPT1(adc_periph);
		sampt &= ~((uint32_t) (ADC_SAMPTX_SPTN <<
					(ADC_CHANNEL_SAMPLE_LENGTH * adc_channel)));
		// channel sample time set
		sampt |= (uint32_t) (sample_time <<
				(ADC_CHANNEL_SAMPLE_LENGTH * adc_channel));
		ADC_SAMPT1(adc_periph) = sampt;
	} else if (adc_channel < ADC_CHANNEL_SAMPLE_EIGHTEEN) {
		// the regular group sequence rank is smaller than eighteen
		sampt = ADC_SAMPT0(adc_periph);
		sampt &= ~((uint32_t) (ADC_SAMPTX_SPTN <<
					(ADC_CHANNEL_SAMPLE_LENGTH *
					 (adc_channel - ADC_CHANNEL_SAMPLE_TEN))));
		// channel sample time set
		sampt |= (uint32_t) (sample_time <<
				(ADC_CHANNEL_SAMPLE_LENGTH *
				 (adc_channel - ADC_CHANNEL_SAMPLE_TEN)));
		ADC_SAMPT0(adc_periph) = sampt;
	} else {
	}
}

/*
 * configure ADC inserted channel 
 * dc_periph:
 * 	ADCx, x=0,1
 *
 * rank:
 * 	the inserted group sequencer rank,this parameter must be between 0 to 3
 *
 * adc_channel: the selected ADC channel
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_CHANNEL_x(x=0..17)(x=16 and x=17 are only for ADC0):
 * 			ADC Channelx
 *
 * sample_time: The sample time value
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_SAMPLETIME_1POINT5: 1.5 cycles
 * 		ADC_SAMPLETIME_7POINT5: 7.5 cycles
 * 		ADC_SAMPLETIME_13POINT5: 13.5 cycles
 * 		ADC_SAMPLETIME_28POINT5: 28.5 cycles
 * 		ADC_SAMPLETIME_41POINT5: 41.5 cycles
 * 		ADC_SAMPLETIME_55POINT5: 55.5 cycles
 * 		ADC_SAMPLETIME_71POINT5: 71.5 cycles
 * 		ADC_SAMPLETIME_239POINT5: 239.5 cycles
 */
void adc_inserted_channel_config(uint32_t adc_periph, uint8_t rank,
		uint8_t adc_channel, uint32_t sample_time) {
	uint8_t inserted_length;
	uint32_t isq, sampt;
	/* get inserted channel group length */
	inserted_length = (uint8_t) GET_BITS(ADC_ISQ(adc_periph), 20U, 21U);
	// the channel number is written to these bits to select a channel
	// as the nth conversion in the inserted channel group
	isq = ADC_ISQ(adc_periph);
	isq &= ~((uint32_t) (ADC_ISQ_ISQN <<
				(ADC_INSERTED_CHANNEL_SHIFT_LENGTH -
				 (inserted_length - rank) * ADC_INSERTED_CHANNEL_RANK_LENGTH)));
	isq |= ((uint32_t) adc_channel <<
			(ADC_INSERTED_CHANNEL_SHIFT_LENGTH -
			 (inserted_length - rank) * ADC_INSERTED_CHANNEL_RANK_LENGTH));

	ADC_ISQ(adc_periph) = isq;

	/* ADC sampling time config */
	if (adc_channel < ADC_CHANNEL_SAMPLE_TEN) {
		/* the inserted group sequence rank is smaller than ten */
		sampt = ADC_SAMPT1(adc_periph);
		sampt &= ~((uint32_t) (ADC_SAMPTX_SPTN <<
					(ADC_CHANNEL_SAMPLE_LENGTH * adc_channel)));
		/* channel sample time set */
		sampt |= (uint32_t) sample_time <<
			(ADC_CHANNEL_SAMPLE_LENGTH * adc_channel);
		ADC_SAMPT1(adc_periph) = sampt;
	} else if (adc_channel < ADC_CHANNEL_SAMPLE_EIGHTEEN) {
		/* the inserted group sequence rank is smaller than eighteen */
		sampt = ADC_SAMPT0(adc_periph);
		sampt &= ~((uint32_t) (ADC_SAMPTX_SPTN <<
					(ADC_CHANNEL_SAMPLE_LENGTH *
					 (adc_channel - ADC_CHANNEL_SAMPLE_TEN))));
		/* channel sample time set */
		sampt |= ((uint32_t) sample_time <<
				(ADC_CHANNEL_SAMPLE_LENGTH *
				 (adc_channel - ADC_CHANNEL_SAMPLE_TEN)));
		ADC_SAMPT0(adc_periph) = sampt;
	} else {
	}
}

/*
 * configure ADC inserted channel offset 
 * dc_periph: ADCx, x=0,1
 *
 * inserted_channel: insert channel select
 * 	only one parameter can be selected
 * 		ADC_INSERTED_CHANNEL_0: inserted channel0
 * 		ADC_INSERTED_CHANNEL_1: inserted channel1
 * 		ADC_INSERTED_CHANNEL_2: inserted channel2
 * 		ADC_INSERTED_CHANNEL_3: inserted channel3
 *
 * offset: the offset data
 */
void adc_inserted_channel_offset_config(uint32_t adc_periph,
		uint8_t inserted_channel, uint16_t offset) {
	uint8_t inserted_length;
	uint32_t num = 0U;

	inserted_length = (uint8_t) GET_BITS(ADC_ISQ(adc_periph), 20U, 21U);
	num = ((uint32_t) ADC_OFFSET_LENGTH -
			((uint32_t) inserted_length -
			 (uint32_t) inserted_channel));

	if (num <= ADC_OFFSET_LENGTH) {
		/* calculate the offset of the register */
		num = num * ADC_OFFSET_SHIFT_LENGTH;
		/* config the offset of the selected channels */
		REG32((adc_periph) + 0x14U + num) =
			IOFFX_IOFF((uint32_t) offset);
	}
}

/*
 * configure ADC external trigger source
 * adc_periph: ADCx, x=0,1
 *
 * adc_channel_group: select the channel group
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_REGULAR_CHANNEL: regular channel group
 * 		ADC_INSERTED_CHANNEL: inserted channel group
 *
 * external_trigger_source: regular or inserted group trigger source
 * 	only one parameter can be selected
 * 	for regular channel:
 * 		ADC0_1_EXTTRIG_REGULAR_T0_CH0: TIMER0 CH0 event select
 * 		ADC0_1_EXTTRIG_REGULAR_T0_CH1: TIMER0 CH1 event select
 * 		ADC0_1_EXTTRIG_REGULAR_T0_CH2: TIMER0 CH2 event select
 * 		ADC0_1_EXTTRIG_REGULAR_T1_CH1: TIMER1 CH1 event select
 * 		ADC0_1_EXTTRIG_REGULAR_T2_TRGO: TIMER2 TRGO event select
 * 		ADC0_1_EXTTRIG_REGULAR_T3_CH3: TIMER3 CH3 event select
 * 		ADC0_1_EXTTRIG_REGULAR_EXTI_11: external interrupt line 11
 * 		ADC0_1_EXTTRIG_REGULAR_NONE: software trigger
 *
 * 	for inserted channel:
 * 		ADC0_1_EXTTRIG_INSERTED_T0_TRGO: TIMER0 TRGO event select
 * 		ADC0_1_EXTTRIG_INSERTED_T0_CH3: TIMER0 CH3 event select
 * 		ADC0_1_EXTTRIG_INSERTED_T1_TRGO: TIMER1 TRGO event select
 * 		ADC0_1_EXTTRIG_INSERTED_T1_CH0: TIMER1 CH0 event select
 * 		ADC0_1_EXTTRIG_INSERTED_T2_CH3: TIMER2 CH3 event select
 * 		ADC0_1_EXTTRIG_INSERTED_T3_TRGO: TIMER3 TRGO event select
 * 		ADC0_1_EXTTRIG_INSERTED_EXTI_15: external interrupt line 15
 * 		ADC0_1_EXTTRIG_INSERTED_NONE: software trigger
 *
 */
void adc_external_trigger_source_config(uint32_t adc_periph,
		uint8_t adc_channel_group, uint32_t external_trigger_source) {
	switch (adc_channel_group) {
	case ADC_REGULAR_CHANNEL:
		/* configure ADC regular group external trigger source */
		ADC_CTL1(adc_periph) &= ~((uint32_t) ADC_CTL1_ETSRC);
		ADC_CTL1(adc_periph) |= (uint32_t) external_trigger_source;
		break;
	case ADC_INSERTED_CHANNEL:
		/* configure ADC inserted group external trigger source */
		ADC_CTL1(adc_periph) &= ~((uint32_t) ADC_CTL1_ETSIC);
		ADC_CTL1(adc_periph) |= (uint32_t) external_trigger_source;
		break;
	default:
		break;
	}
}

/*
 * configure ADC external trigger 
 * dc_periph: ADCx, x=0,1
 *
 * adc_channel_group: select the channel group
 * 	one or more parameters can be selected which are shown as below:
 * 		ADC_REGULAR_CHANNEL: regular channel group
 * 		ADC_INSERTED_CHANNEL: inserted channel group
 *
 * newvalue: ENABLE or DISABLE
 *
 */
void adc_external_trigger_config(uint32_t adc_periph,
		uint8_t adc_channel_group, enum control_status newvalue) {
	if (newvalue) {
		if (0U != (adc_channel_group & ADC_REGULAR_CHANNEL)) {
			// enable ADC regular channel group external trigger
			ADC_CTL1(adc_periph) |= ADC_CTL1_ETERC;
		}
		if (0U != (adc_channel_group & ADC_INSERTED_CHANNEL)) {
			// enable ADC inserted channel group external trigger
			ADC_CTL1(adc_periph) |= ADC_CTL1_ETEIC;
		}
	} else {
		if (0U != (adc_channel_group & ADC_REGULAR_CHANNEL)) {
			// disable ADC regular channel group external trigger
			ADC_CTL1(adc_periph) &= ~ADC_CTL1_ETERC;
		}
		if (0U != (adc_channel_group & ADC_INSERTED_CHANNEL)) {
			// disable ADC regular channel group external trigger
			ADC_CTL1(adc_periph) &= ~ADC_CTL1_ETEIC;
		}
	}
}

/*
 * enable ADC software trigger 
 * dc_periph: ADCx, x=0,1
 * adc_channel_group: select the channel group
 * 	one or more parameters can be selected which are shown as below:
 * 		ADC_REGULAR_CHANNEL: regular channel group
 * 		ADC_INSERTED_CHANNEL: inserted channel group
 *
 */
void adc_software_trigger_enable(uint32_t adc_periph, uint8_t adc_channel_group) {
	if (0U != (adc_channel_group & ADC_REGULAR_CHANNEL))
		// enable ADC regular channel group software trigger
		ADC_CTL1(adc_periph) |= ADC_CTL1_SWRCST;

	if (0U != (adc_channel_group & ADC_INSERTED_CHANNEL))
		// enable ADC inserted channel group software trigger
		ADC_CTL1(adc_periph) |= ADC_CTL1_SWICST;
}

/*
 * read ADC regular group data register
 * adc_periph: ADCx, x=0,1
*/
uint16_t adc_regular_data_read(uint32_t adc_periph) {
	return (uint16_t) (ADC_RDATA(adc_periph));
}

/*
 * read ADC inserted group data register
 * adc_periph: ADCx, x=0,1
 *
 * inserted_channel: insert channel select
 * 	only one parameter can be selected
 * 		ADC_INSERTED_CHANNEL_0: inserted Channel0
 * 		ADC_INSERTED_CHANNEL_1: inserted channel1
 * 		ADC_INSERTED_CHANNEL_2: inserted Channel2
 * 		ADC_INSERTED_CHANNEL_3: inserted Channel3
 *
 */
uint16_t adc_inserted_data_read(uint32_t adc_periph, uint8_t inserted_channel) {
	uint32_t idata;
	/* read the data of the selected channel */
	switch (inserted_channel) {
	case ADC_INSERTED_CHANNEL_0:
		idata = ADC_IDATA0(adc_periph);
		break;
	case ADC_INSERTED_CHANNEL_1:
		idata = ADC_IDATA1(adc_periph);
		break;
	case ADC_INSERTED_CHANNEL_2:
		idata = ADC_IDATA2(adc_periph);
		break;
	case ADC_INSERTED_CHANNEL_3:
		idata = ADC_IDATA3(adc_periph);
		break;
	default:
		idata = 0U;
		break;
	}
	return (uint16_t) idata;
}

/*
 * read the last ADC0 and ADC1 conversion result data in sync mode
 */
uint32_t adc_sync_mode_convert_value_read(void) {
	return ADC_RDATA(ADC0);
}

/*
 * configure ADC analog watchdog single channel 
 * dc_periph: ADCx, x=0,1
 * adc_channel: the selected ADC channel
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_CHANNEL_x:
 * 			ADC Channelx(x=0..17)(x=16 and x=17 are only for ADC0)
 *
 */
void adc_watchdog_single_channel_enable(uint32_t adc_periph,
		uint8_t adc_channel) {
	ADC_CTL0(adc_periph) &= (uint32_t) ~(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN |
					ADC_CTL0_WDSC | ADC_CTL0_WDCHSEL);
	// analog watchdog channel select
	ADC_CTL0(adc_periph) |= (uint32_t) adc_channel;
	ADC_CTL0(adc_periph) |= (uint32_t) (ADC_CTL0_RWDEN | ADC_CTL0_IWDEN |
						ADC_CTL0_WDSC);
}

/*
 * configure ADC analog watchdog group channel 
 * dc_periph: ADCx, x=0,1
 *
 * adc_channel_group: the channel group use analog watchdog
 * 	only one parameter can be selected which is shown as below: 
 * 		ADC_REGULAR_CHANNEL: regular channel group
 * 		ADC_INSERTED_CHANNEL: inserted channel group
 * 		ADC_REGULAR_INSERTED_CHANNEL: both regular and inserted group
 *
 */
void adc_watchdog_group_channel_enable(uint32_t adc_periph,
		uint8_t adc_channel_group) {
	ADC_CTL0(adc_periph) &=
	    (uint32_t) ~ (ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC);
	/* select the group */
	switch (adc_channel_group) {
	case ADC_REGULAR_CHANNEL:
		/* regular channel analog watchdog enable */
		ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_RWDEN;
		break;
	case ADC_INSERTED_CHANNEL:
		/* inserted channel analog watchdog enable */
		ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_IWDEN;
		break;
	case ADC_REGULAR_INSERTED_CHANNEL:
		/* regular and inserted channel analog watchdog enable */
		ADC_CTL0(adc_periph) |= (uint32_t) (ADC_CTL0_RWDEN |
							ADC_CTL0_IWDEN);
		break;
	default:
		break;
	}
}

/*
 * disable ADC analog watchdog 
 * adc_periph: ADCx, x=0,1
 */
void adc_watchdog_disable(uint32_t adc_periph) {
	ADC_CTL0(adc_periph) &=
		(uint32_t) ~ (ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC |
				ADC_CTL0_WDCHSEL);
}

/*
 * configure ADC analog watchdog threshold 
 * adc_periph: ADCx, x=0,1
 *
 * low_threshold: analog watchdog low threshold, 0..4095 
 *
 * high_threshold: analog watchdog high threshold, 0..4095
 *
 */
void adc_watchdog_threshold_config(uint32_t adc_periph, uint16_t low_threshold,
		uint16_t high_threshold) {
	ADC_WDLT(adc_periph) = (uint32_t) WDLT_WDLT(low_threshold);
	ADC_WDHT(adc_periph) = (uint32_t) WDHT_WDHT(high_threshold);
}

/*
 * get the ADC flag bits
 * adc_periph: ADCx, x=0,1
 * adc_flag: the adc flag bits
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_FLAG_WDE: analog watchdog event flag
 * 		ADC_FLAG_EOC: end of group conversion flag
 * 		ADC_FLAG_EOIC: end of inserted group conversion flag
 * 		ADC_FLAG_STIC: start flag of inserted channel group
 * 		ADC_FLAG_STRC: start flag of regular channel group
 *
 */
enum flag_status adc_flag_get(uint32_t adc_periph, uint32_t adc_flag) {
	enum flag_status reval = RESET;
	if (ADC_STAT(adc_periph) & adc_flag)
		reval = SET;

	return reval;
}

/*
 * clear the ADC flag bits
 * adc_periph: ADCx, x=0,1
 *
 * adc_flag: the adc flag bits
 * 	one or more parameters can be selected which are shown as below:
 * 		ADC_FLAG_WDE: analog watchdog event flag
 * 		ADC_FLAG_EOC: end of group conversion flag
 * 		ADC_FLAG_EOIC: end of inserted group conversion flag
 * 		ADC_FLAG_STIC: start flag of inserted channel group
 * 		ADC_FLAG_STRC: start flag of regular channel group
 *
 */
void adc_flag_clear(uint32_t adc_periph, uint32_t adc_flag) {
	ADC_STAT(adc_periph) &= ~((uint32_t) adc_flag);
}

/*
 * get the bit state of ADCx software start conversion
 * adc_periph: ADCx, x=0,1
 *
 */
enum flag_status adc_regular_software_startconv_flag_get(uint32_t adc_periph) {
	enum flag_status reval = RESET;
	if ((uint32_t) RESET != (ADC_CTL1(adc_periph) & ADC_CTL1_SWRCST))
		reval = SET;

	return reval;
}

/*
 * get the bit state of ADCx software inserted channel start conversion
 * adc_periph: ADCx, x=0,1
 *
 */
enum flag_status adc_inserted_software_startconv_flag_get(uint32_t adc_periph) {
	enum flag_status reval = RESET;
	if ((uint32_t) RESET != (ADC_CTL1(adc_periph) & ADC_CTL1_SWICST))
		reval = SET;

	return reval;
}

/*
 * get the ADC interrupt bits
 * adc_periph: ADCx, x=0,1
 *
 * adc_interrupt: the adc interrupt bits
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_INT_FLAG_WDE: analog watchdog interrupt
 * 		ADC_INT_FLAG_EOC: end of group conversion interrupt
 * 		ADC_INT_FLAG_EOIC: end of inserted group conversion interrupt
 *
 */
enum flag_status adc_interrupt_flag_get(uint32_t adc_periph,
		uint32_t adc_interrupt) {
	enum flag_status interrupt_flag = RESET;
	uint32_t state;

	switch (adc_interrupt) {
	case ADC_INT_FLAG_WDE:
		// get the ADC analog watchdog interrupt bits
		state = ADC_STAT(adc_periph) & ADC_STAT_WDE;
		if ((ADC_CTL0(adc_periph) & ADC_CTL0_WDEIE) && state)
			interrupt_flag = SET;
		break;
	case ADC_INT_FLAG_EOC:
		// get the ADC end of group conversion interrupt bits
		state = ADC_STAT(adc_periph) & ADC_STAT_EOC;
		if ((ADC_CTL0(adc_periph) & ADC_CTL0_EOCIE) && state)
			interrupt_flag = SET;
		break;
	case ADC_INT_FLAG_EOIC:
		// get the ADC end of inserted group conversion interrupt bits
		state = ADC_STAT(adc_periph) & ADC_STAT_EOIC;
		if ((ADC_CTL0(adc_periph) & ADC_CTL0_EOICIE) && state)
			interrupt_flag = SET;
		break;
	default:
		break;
	}
	return interrupt_flag;
}

/*
 * clear the ADC flag
 * adc_periph: ADCx, x=0,1
 *
 * adc_interrupt: the adc status flag
 * 	one or more parameters can be selected which are shown as below:
 * 		ADC_INT_FLAG_WDE: analog watchdog interrupt
 * 		ADC_INT_FLAG_EOC: end of group conversion interrupt
 * 		ADC_INT_FLAG_EOIC: end of inserted group conversion interrupt
 *
 */
void adc_interrupt_flag_clear(uint32_t adc_periph, uint32_t adc_interrupt) {
	ADC_STAT(adc_periph) &= ~((uint32_t) adc_interrupt);
}

/*
 * enable ADC interrupt
 * adc_periph: ADCx, x=0,1
 *
 * adc_interrupt: the adc interrupt
 * 	one or more parameters can be selected which are shown as below:
 * 		ADC_INT_WDE: analog watchdog interrupt flag
 * 		ADC_INT_EOC: end of group conversion interrupt flag
 * 		ADC_INT_EOIC: end of inserted group conversion interrupt flag
 *
 */
void adc_interrupt_enable(uint32_t adc_periph, uint32_t adc_interrupt) {
	/* enable ADC analog watchdog interrupt */
	if (0U != (adc_interrupt & ADC_INT_WDE))
		ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_WDEIE;

	/* enable ADC end of group conversion interrupt */
	if (0U != (adc_interrupt & ADC_INT_EOC))
		ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_EOCIE;

	/* enable ADC end of inserted group conversion interrupt */
	if (0U != (adc_interrupt & ADC_INT_EOIC))
		ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_EOICIE;
}

/*
 * disable ADC interrupt
 * adc_periph: ADCx, x=0,1
 *
 * adc_interrupt: the adc interrupt flag
 * 	one or more parameters can be selected which are shown as below:
 * 		ADC_INT_WDE: analog watchdog interrupt flag
 * 		ADC_INT_EOC: end of group conversion interrupt flag
 * 		ADC_INT_EOIC: end of inserted group conversion interrupt flag
 *
 */
void adc_interrupt_disable(uint32_t adc_periph, uint32_t adc_interrupt) {
	/* disable ADC analog watchdog interrupt */
	if (0U != (adc_interrupt & ADC_INT_WDE))
		ADC_CTL0(adc_periph) &= ~(uint32_t) ADC_CTL0_WDEIE;

	/* disable ADC end of group conversion interrupt */
	if (0U != (adc_interrupt & ADC_INT_EOC))
		ADC_CTL0(adc_periph) &= ~(uint32_t) ADC_CTL0_EOCIE;

	/* disable ADC end of inserted group conversion interrupt */
	if (0U != (adc_interrupt & ADC_INT_EOIC))
		ADC_CTL0(adc_periph) &= ~(uint32_t) ADC_CTL0_EOICIE;
}

/*
 * adc resolution config
 * adc_periph: ADCx, x=0,1
 * resolution: ADC resolution
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_RESOLUTION_12B: 12-bit ADC resolution
 * 		ADC_RESOLUTION_10B: 10-bit ADC resolution
 * 		ADC_RESOLUTION_8B: 8-bit ADC resolution
 * 		ADC_RESOLUTION_6B: 6-bit ADC resolution
 *
 */
void adc_resolution_config(uint32_t adc_periph, uint32_t resolution) {
	ADC_OVSCR(adc_periph) &= ~((uint32_t) ADC_OVSCR_DRES);
	ADC_OVSCR(adc_periph) |= (uint32_t) resolution;
}

/*
 * adc oversample mode config
 * adc_periph: ADCx, x=0,1
 *
 * mode: ADC oversampling mode
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_OVERSAMPLING_ALL_CONVERT:
 * 			all oversampled conversions for a channel are done
 * 			consecutively after a trigger
 *
 * 		ADC_OVERSAMPLING_ONE_CONVERT:
 * 			each oversampled conversion for a channel needs a
 * 			trigger
 *
 * shift: ADC oversampling shift
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_OVERSAMPLING_SHIFT_NONE: no oversampling shift
 * 		ADC_OVERSAMPLING_SHIFT_1B: 1-bit oversampling shift
 * 		ADC_OVERSAMPLING_SHIFT_2B: 2-bit oversampling shift
 * 		ADC_OVERSAMPLING_SHIFT_3B: 3-bit oversampling shift
 * 		ADC_OVERSAMPLING_SHIFT_4B: 3-bit oversampling shift
 * 		ADC_OVERSAMPLING_SHIFT_5B: 5-bit oversampling shift
 * 		ADC_OVERSAMPLING_SHIFT_6B: 6-bit oversampling shift
 * 		ADC_OVERSAMPLING_SHIFT_7B: 7-bit oversampling shift
 * 		ADC_OVERSAMPLING_SHIFT_8B: 8-bit oversampling shift
 *
 * ratio: ADC oversampling ratio
 * 	only one parameter can be selected which is shown as below:
 * 		ADC_OVERSAMPLING_RATIO_MUL2: oversampling ratio X2
 * 		ADC_OVERSAMPLING_RATIO_MUL4: oversampling ratio X4
 * 		ADC_OVERSAMPLING_RATIO_MUL8: oversampling ratio X8
 * 		ADC_OVERSAMPLING_RATIO_MUL16: oversampling ratio X16
 * 		ADC_OVERSAMPLING_RATIO_MUL32: oversampling ratio X32
 * 		ADC_OVERSAMPLING_RATIO_MUL64: oversampling ratio X64
 * 		ADC_OVERSAMPLING_RATIO_MUL128: oversampling ratio X128
 * 	        ADC_OVERSAMPLING_RATIO_MUL256: oversampling ratio X256
 *
 */
void adc_oversample_mode_config(uint32_t adc_periph, uint8_t mode,
		uint16_t shift, uint8_t ratio) {
	if (mode)
		ADC_OVSCR(adc_periph) |= (uint32_t) ADC_OVSCR_TOVS;
	else
		ADC_OVSCR(adc_periph) &= ~((uint32_t) ADC_OVSCR_TOVS);

	/* config the shift and ratio */
	ADC_OVSCR(adc_periph) &= ~((uint32_t) (ADC_OVSCR_OVSR |
						ADC_OVSCR_OVSS));
	ADC_OVSCR(adc_periph) |= ((uint32_t) shift | (uint32_t) ratio);
}

/*
 * enable ADC oversample mode
 * adc_periph: ADCx, x=0,1
*/
void adc_oversample_mode_enable(uint32_t adc_periph) {
	ADC_OVSCR(adc_periph) |= ADC_OVSCR_OVSEN;
}

/*
 * disable ADC oversample mode
 * adc_periph: ADCx, x=0,1
*/
void adc_oversample_mode_disable(uint32_t adc_periph) {
	ADC_OVSCR(adc_periph) &= ~((uint32_t) ADC_OVSCR_OVSEN);
}
