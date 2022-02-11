#ifndef __GD32VF103_ADC_H__
#define __GD32VF103_ADC_H__

#include "gd32vf103.h"

#define ADC0			ADC_BASE
#define ADC1			(ADC_BASE + 0x400U)

#define ADC_STAT(adcx)		REG32((adcx) + 0x00U)
#define ADC_CTL0(adcx)		REG32((adcx) + 0x04U)
#define ADC_CTL1(adcx)		REG32((adcx) + 0x08U)

#define ADC_SAMPT0(adcx)	REG32((adcx) + 0x0CU)
#define ADC_SAMPT1(adcx)	REG32((adcx) + 0x10U)

// ADC inserted channel data offset register 0
#define ADC_IOFF0(adcx)		REG32((adcx) + 0x14U)
// ADC inserted channel data offset register 1
#define ADC_IOFF1(adcx)		REG32((adcx) + 0x18U)
// ADC inserted channel data offset register 2
#define ADC_IOFF2(adcx)		REG32((adcx) + 0x1CU)
// ADC inserted channel data offset register 3
#define ADC_IOFF3(adcx)		REG32((adcx) + 0x20U)

// ADC watchdog high threshold register
#define ADC_WDHT(adcx)		REG32((adcx) + 0x24U)
// ADC watchdog low threshold register
#define ADC_WDLT(adcx)		REG32((adcx) + 0x28U)

// ADC regular sequence register 0
#define ADC_RSQ0(adcx)		REG32((adcx) + 0x2CU)
// ADC regular sequence register 1
#define ADC_RSQ1(adcx)		REG32((adcx) + 0x30U)
// ADC regular sequence register 2
#define ADC_RSQ2(adcx)		REG32((adcx) + 0x34U)

// ADC inserted sequence register
#define ADC_ISQ(adcx)		REG32((adcx) + 0x38U)

// ADC inserted data register 0
#define ADC_IDATA0(adcx)	REG32((adcx) + 0x3CU)
// ADC inserted data register 1
#define ADC_IDATA1(adcx)	REG32((adcx) + 0x40U)
// ADC inserted data register 2
#define ADC_IDATA2(adcx)	REG32((adcx) + 0x44U)
// ADC inserted data register 3
#define ADC_IDATA3(adcx)	REG32((adcx) + 0x48U)

// ADC regular data register
#define ADC_RDATA(adcx)		REG32((adcx) + 0x4CU)

// ADC oversample control register
#define ADC_OVSCR(adcx)		REG32((adcx) + 0x80U)


// ADC_STAT
#define ADC_STAT_WDE		BIT(0) // analog watchdog event flag
#define ADC_STAT_EOC		BIT(1) // end of conversion
#define ADC_STAT_EOIC		BIT(2) // inserted channel end of conversion
#define ADC_STAT_STIC		BIT(3) // inserted channel start flag
#define ADC_STAT_STRC		BIT(4) // regular channel start flag


// ADC_CTL0

// analog watchdog channel select bits
#define ADC_CTL0_WDCHSEL	BITS(0, 4)

// interrupt enable for EOC
#define ADC_CTL0_EOCIE		BIT(5)

// analog watchdog interrupt enable
#define ADC_CTL0_WDEIE		BIT(6)

// interrupt enable for inserted channels
#define ADC_CTL0_EOICIE		BIT(7)

// scan mode
#define ADC_CTL0_SM		BIT(8)

// when in scan mode, analog watchdog is effective on a single channel
#define ADC_CTL0_WDSC		BIT(9)

// automatic inserted group conversion
#define ADC_CTL0_ICA		BIT(10)

// discontinuous mode on regular channels
#define ADC_CTL0_DISRC		BIT(11)

// discontinuous mode on inserted channels
#define ADC_CTL0_DISIC		BIT(12)

// discontinuous mode channel count
#define ADC_CTL0_DISNUM		BITS(13, 15)

// sync mode selection
#define ADC_CTL0_SYNCM		BITS(16, 19)

// analog watchdog enable on inserted channels
#define ADC_CTL0_IWDEN		BIT(22)

// analog watchdog enable on regular channels
#define ADC_CTL0_RWDEN		BIT(23)


// ADC_CTL1

// ADC converter on
#define ADC_CTL1_ADCON		BIT(0)

// continuous conversion
#define ADC_CTL1_CTN		BIT(1)

// ADC calibration
#define ADC_CTL1_CLB		BIT(2)

// reset calibration
#define ADC_CTL1_RSTCLB		BIT(3)

// direct memory access mode
#define ADC_CTL1_DMA		BIT(8)

// data alignment
#define ADC_CTL1_DAL		BIT(11)

// external trigger select for inserted channel
#define ADC_CTL1_ETSIC		BITS(12, 14)

// external trigger enable for inserted channel
#define ADC_CTL1_ETEIC		BIT(15)

// external trigger select for regular channel
#define ADC_CTL1_ETSRC		BITS(17, 19)

// external trigger conversion mode for inserted channels
#define ADC_CTL1_ETERC		BIT(20)

// start on inserted channel
#define ADC_CTL1_SWICST		BIT(21)

// start on regular channel
#define ADC_CTL1_SWRCST		BIT(22)

// channel 16 and 17 enable of ADC0
#define ADC_CTL1_TSVREN		BIT(23)


// ADC_SAMPTx x=0..1, channel n sample time selection
#define ADC_SAMPTX_SPTN		BITS(0, 2)

// ADC_IOFFx x=0..3, data offset for inserted channel x
#define ADC_IOFFX_IOFF		BITS(0, 11)

// ADC_WDHT, analog watchdog high threshold
#define ADC_WDHT_WDHT		BITS(0, 11)

// ADC_WDLT, analog watchdog low threshold
#define ADC_WDLT_WDLT		BITS(0, 11)

// ADC_RSQx x=0..2, nth conversion in regular sequence
#define ADC_RSQX_RSQN		BITS(0, 4)
// regular channel sequence length
#define ADC_RSQ0_RL		BITS(20, 23)

// ADC_ISQ, nth conversion in inserted sequence
#define ADC_ISQ_ISQN		BITS(0, 4)
// inserted sequence length
#define ADC_ISQ_IL		BITS(20, 21)

// ADC_IDATAx x=0..3, inserted data n
#define ADC_IDATAX_IDATAN	BITS(0, 15)

// ADC_RDATA, regular data
#define ADC_RDATA_RDATA		BITS(0, 15)

// ADC1 regular channel data
#define ADC_RDATA_ADC1RDTR	BITS(16, 31)

// ADC_OVSCR
#define ADC_OVSCR_OVSEN	BIT(0)		// oversampling enable
#define ADC_OVSCR_OVSR	BITS(2, 4)	// oversampling ratio
#define ADC_OVSCR_OVSS	BITS(5, 8)	// oversampling shift
#define ADC_OVSCR_TOVS	BIT(9)		// triggered oversampling
#define ADC_OVSCR_DRES	BITS(12, 13)	// ADC data resolution

// adc_stat register value
#define ADC_FLAG_WDE	ADC_STAT_WDE	// analog watchdog event flag
#define ADC_FLAG_EOC	ADC_STAT_EOC	// end of conversion
#define ADC_FLAG_EOIC	ADC_STAT_EOIC	// inserted channel end of conversion
#define ADC_FLAG_STIC	ADC_STAT_STIC	// inserted channel start flag
#define ADC_FLAG_STRC	ADC_STAT_STRC	// regular channel start flag

///////////////////////////////////////////////////////////////////////////////
// adc_ctl0 register value

// write value to ADC_CTL0_DISNUM bit field
#define CTL0_DISNUM(regval)	(BITS(13, 15) & ((uint32_t) (regval) << 13))
#define ADC_SCAN_MODE		ADC_CTL0_SM // scan mode

// inserted channel group convert automatically
#define ADC_INSERTED_CHANNEL_AUTO	ADC_CTL0_ICA

// ADC sync mode
// write value to ADC_CTL0_SYNCM bit field
#define CTL0_SYNCM(regval)	(BITS(16, 19) & ((uint32_t) (regval) << 16))

// all the ADCs work independently
#define ADC_MODE_FREE		CTL0_SYNCM(0)

// ADC0 and ADC1 work in combined regular parallel + inserted parallel mode
#define ADC_DAUL_REGULAL_PARALLEL_INSERTED_PARALLEL \
	CTL0_SYNCM(1)

// ADC0 and ADC1 work in combined regular parallel + trigger rotation mode
#define ADC_DAUL_REGULAL_PARALLEL_INSERTED_ROTATION \
	CTL0_SYNCM(2)

// ADC0 and ADC1 work in combined inserted parallel + follow-up fast mode
#define ADC_DAUL_INSERTED_PARALLEL_REGULAL_FOLLOWUP_FAST \
	CTL0_SYNCM(3)

// ADC0 and ADC1 work in combined inserted parallel + follow-up slow mode
#define ADC_DAUL_INSERTED_PARALLEL_REGULAL_FOLLOWUP_SLOW \
	CTL0_SYNCM(4)

// ADC0 and ADC1 work in inserted parallel mode only
#define ADC_DAUL_INSERTED_PARALLEL \
	CTL0_SYNCM(5)

// ADC0 and ADC1 work in regular parallel mode only
#define ADC_DAUL_REGULAL_PARALLEL \
	CTL0_SYNCM(6)

// ADC0 and ADC1 work in follow-up fast mode only
#define ADC_DAUL_REGULAL_FOLLOWUP_FAST \
	CTL0_SYNCM(7)

// ADC0 and ADC1 work in follow-up slow mode only
#define ADC_DAUL_REGULAL_FOLLOWUP_SLOW \
	CTL0_SYNCM(8)

// ADC0 and ADC1 work in trigger rotation mode only
#define ADC_DAUL_INSERTED_TRIGGER_ROTATION \
	CTL0_SYNCM(9)

///////////////////////////////////////////////////////////////////////////////
// adc_ctl1 register value

#define ADC_DATAALIGN_RIGHT	((uint32_t) 0x00000000U) // LSB alignment
#define ADC_DATAALIGN_LEFT	ADC_CTL1_DAL // MSB alignment
#define ADC_CONTINUOUS_MODE	ADC_CTL1_CTN // continuous mode

// external trigger select for regular channel
// write value to ADC_CTL1_ETSRC bit field
#define CTL1_ETSRC(regval)	(BITS(17, 19) & ((uint32_t) (regval) << 17))

// for ADC0 and ADC1 regular channel
#define ADC0_1_EXTTRIG_REGULAR_T0_CH0	CTL1_ETSRC(0)
#define ADC0_1_EXTTRIG_REGULAR_T0_CH1	CTL1_ETSRC(1)
#define ADC0_1_EXTTRIG_REGULAR_T0_CH2	CTL1_ETSRC(2)
#define ADC0_1_EXTTRIG_REGULAR_T1_CH1	CTL1_ETSRC(3)
#define ADC0_1_EXTTRIG_REGULAR_T2_TRGO	CTL1_ETSRC(4)
#define ADC0_1_EXTTRIG_REGULAR_T3_CH3	CTL1_ETSRC(5)
#define ADC0_1_EXTTRIG_REGULAR_EXTI_11	CTL1_ETSRC(6)
#define ADC0_1_EXTTRIG_REGULAR_NONE	CTL1_ETSRC(7)

// external trigger mode for inserted channel
// write value to ADC_CTL1_ETSIC bit field
#define CTL1_ETSIC(regval)	(BITS(12, 14) & ((uint32_t) (regval) << 12))

// for ADC0 and ADC1 inserted channel
#define ADC0_1_EXTTRIG_INSERTED_T0_TRGO	CTL1_ETSIC(0)
#define ADC0_1_EXTTRIG_INSERTED_T0_CH3	CTL1_ETSIC(1)
#define ADC0_1_EXTTRIG_INSERTED_T1_TRGO	CTL1_ETSIC(2)
#define ADC0_1_EXTTRIG_INSERTED_T1_CH0	CTL1_ETSIC(3)
#define ADC0_1_EXTTRIG_INSERTED_T2_CH3	CTL1_ETSIC(4)
#define ADC0_1_EXTTRIG_INSERTED_T3_TRGO	CTL1_ETSIC(5)
#define ADC0_1_EXTTRIG_INSERTED_EXTI_15	CTL1_ETSIC(6)
#define ADC0_1_EXTTRIG_INSERTED_NONE	CTL1_ETSIC(7)

// adc_samptx register value
// write value to ADC_SAMPTX_SPT bit field
#define SAMPTX_SPT(regval)	(BITS(0, 2) & ((uint32_t) (regval) << 0))

#define ADC_SAMPLETIME_1POINT5		SAMPTX_SPT(0) // 1.5 sampling cycles
#define ADC_SAMPLETIME_7POINT5		SAMPTX_SPT(1) // 7.5 sampling cycles
#define ADC_SAMPLETIME_13POINT5		SAMPTX_SPT(2) // 13.5 sampling cycles
#define ADC_SAMPLETIME_28POINT5		SAMPTX_SPT(3) // 28.5 sampling cycles
#define ADC_SAMPLETIME_41POINT5		SAMPTX_SPT(4) // 41.5 sampling cycles
#define ADC_SAMPLETIME_55POINT5		SAMPTX_SPT(5) // 55.5 sampling cycles
#define ADC_SAMPLETIME_71POINT5		SAMPTX_SPT(6) // 71.5 sampling cycles
#define ADC_SAMPLETIME_239POINT5	SAMPTX_SPT(7) // 239.5 sampling cycles

// adc_ioffx register value
// write value to ADC_IOFFX_IOFF bit field
#define IOFFX_IOFF(regval)	(BITS(0, 11) & ((uint32_t) (regval) << 0))


// adc_wdht register value
// write value to ADC_WDHT_WDHT bit field
#define WDHT_WDHT(regval)	(BITS(0, 11) & ((uint32_t) (regval) << 0))


// adc_wdlt register value
// write value to ADC_WDLT_WDLT bit field
#define WDLT_WDLT(regval)	(BITS(0, 11) & ((uint32_t) (regval) << 0))


// adc_rsqx register value
// write value to ADC_RSQ0_RL bit field
#define RSQ0_RL(regval)		(BITS(20, 23) & ((uint32_t) (regval) << 20))

// adc_isq register value
// write value to ADC_ISQ_IL bit field
#define ISQ_IL(regval)		(BITS(20, 21) & ((uint32_t) (regval) << 20))

// ADC channel group definitions
// adc regular channel group
#define ADC_REGULAR_CHANNEL	((uint8_t) 0x01U)

// adc inserted channel group
#define ADC_INSERTED_CHANNEL	((uint8_t) 0x02U)

// both regular and inserted channel group
#define ADC_REGULAR_INSERTED_CHANNEL	((uint8_t) 0x03U)

// disable discontinuous mode of regular & inserted channel
#define ADC_CHANNEL_DISCON_DISABLE	((uint8_t) 0x04U)

#define ADC_INSERTED_CHANNEL_0	((uint8_t) 0x00U)
#define ADC_INSERTED_CHANNEL_1	((uint8_t) 0x01U)
#define ADC_INSERTED_CHANNEL_2	((uint8_t) 0x02U)
#define ADC_INSERTED_CHANNEL_3	((uint8_t) 0x03U)

#define ADC_CHANNEL_0		((uint8_t) 0x00U)
#define ADC_CHANNEL_1		((uint8_t) 0x01U)
#define ADC_CHANNEL_2		((uint8_t) 0x02U)
#define ADC_CHANNEL_3		((uint8_t) 0x03U)
#define ADC_CHANNEL_4		((uint8_t) 0x04U)
#define ADC_CHANNEL_5		((uint8_t) 0x05U)
#define ADC_CHANNEL_6		((uint8_t) 0x06U)
#define ADC_CHANNEL_7		((uint8_t) 0x07U)
#define ADC_CHANNEL_8		((uint8_t) 0x08U)
#define ADC_CHANNEL_9		((uint8_t) 0x09U)
#define ADC_CHANNEL_10		((uint8_t) 0x0AU)
#define ADC_CHANNEL_11		((uint8_t) 0x0BU)
#define ADC_CHANNEL_12		((uint8_t) 0x0CU)
#define ADC_CHANNEL_13		((uint8_t) 0x0DU)
#define ADC_CHANNEL_14		((uint8_t) 0x0EU)
#define ADC_CHANNEL_15		((uint8_t) 0x0FU)
#define ADC_CHANNEL_16		((uint8_t) 0x10U)
#define ADC_CHANNEL_17		((uint8_t) 0x11U)


// ADC interrupt

// analog watchdog event interrupt
#define ADC_INT_WDE		ADC_STAT_WDE
// end of group conversion interrupt
#define ADC_INT_EOC		ADC_STAT_EOC
// end of inserted group conversion interrupt
#define ADC_INT_EOIC		ADC_STAT_EOIC

// ADC interrupt flag
// analog watchdog event interrupt flag
#define ADC_INT_FLAG_WDE	ADC_STAT_WDE
// end of group conversion interrupt flag
#define ADC_INT_FLAG_EOC	ADC_STAT_EOC
// end of inserted group conversion interrupt flag
#define ADC_INT_FLAG_EOIC	ADC_STAT_EOIC

// ADC resolution definitions
#define OVSCR_DRES(regval)	(BITS(12, 13) & ((uint32_t) (regval) << 12))

#define ADC_RESOLUTION_12B	OVSCR_DRES(0)
#define ADC_RESOLUTION_10B	OVSCR_DRES(1)
#define ADC_RESOLUTION_8B	OVSCR_DRES(2)
#define ADC_RESOLUTION_6B	OVSCR_DRES(3)

// ADC oversampling mode

// all conversions for a channel are done consecutively after a trigger
#define ADC_OVERSAMPLING_ALL_CONVERT	0
// each conversion for a channel needs a trigger
#define ADC_OVERSAMPLING_ONE_CONVERT	1

// ADC oversampling shift
#define OVSCR_OVSS(regval)	(BITS(5, 8) & ((uint32_t) (regval) << 5))

#define ADC_OVERSAMPLING_SHIFT_NONE	OVSCR_OVSS(0)
#define ADC_OVERSAMPLING_SHIFT_1B	OVSCR_OVSS(1)
#define ADC_OVERSAMPLING_SHIFT_2B	OVSCR_OVSS(2)
#define ADC_OVERSAMPLING_SHIFT_3B	OVSCR_OVSS(3)
#define ADC_OVERSAMPLING_SHIFT_4B	OVSCR_OVSS(4)
#define ADC_OVERSAMPLING_SHIFT_5B	OVSCR_OVSS(5)
#define ADC_OVERSAMPLING_SHIFT_6B	OVSCR_OVSS(6)
#define ADC_OVERSAMPLING_SHIFT_7B	OVSCR_OVSS(7)
#define ADC_OVERSAMPLING_SHIFT_8B	OVSCR_OVSS(8)

// ADC oversampling ratio
#define OVSCR_OVSR(regval)	(BITS(2, 4) & ((uint32_t) (regval) << 2))

#define ADC_OVERSAMPLING_RATIO_MUL2	OVSCR_OVSR(0)
#define ADC_OVERSAMPLING_RATIO_MUL4	OVSCR_OVSR(1)
#define ADC_OVERSAMPLING_RATIO_MUL8	OVSCR_OVSR(2)
#define ADC_OVERSAMPLING_RATIO_MUL16	OVSCR_OVSR(3)
#define ADC_OVERSAMPLING_RATIO_MUL32	OVSCR_OVSR(4)
#define ADC_OVERSAMPLING_RATIO_MUL64	OVSCR_OVSR(5)
#define ADC_OVERSAMPLING_RATIO_MUL128	OVSCR_OVSR(6)
#define ADC_OVERSAMPLING_RATIO_MUL256	OVSCR_OVSR(7)


// initialization config

void adc_deinit(uint32_t adc_periph);

void adc_mode_config(uint32_t mode);

void adc_special_function_config(uint32_t adc_periph, uint32_t function,
		enum control_status newvalue);

void adc_data_alignment_config(uint32_t adc_periph, uint32_t data_alignment);

void adc_enable(uint32_t adc_periph);
void adc_disable(uint32_t adc_periph);

void adc_calibration_enable(uint32_t adc_periph);

void adc_tempsensor_vrefint_enable(void);
void adc_tempsensor_vrefint_disable(void);


// DMA config

void adc_dma_mode_enable(uint32_t adc_periph);
void adc_dma_mode_disable(uint32_t adc_periph);


// regular group and inserted group config

void adc_discontinuous_mode_config(uint32_t adc_periph,
		uint8_t adc_channel_group, uint8_t length);

// configure the length of regular channel group or inserted channel group
void adc_channel_length_config(uint32_t adc_periph, uint8_t adc_channel_group,
		uint32_t length);

void adc_regular_channel_config(uint32_t adc_periph, uint8_t rank,
		uint8_t adc_channel, uint32_t sample_time);

void adc_inserted_channel_config(uint32_t adc_periph, uint8_t rank,
		uint8_t adc_channel, uint32_t sample_time);

void adc_inserted_channel_offset_config(uint32_t adc_periph,
		uint8_t inserted_channel, uint16_t offset);

void adc_external_trigger_source_config(uint32_t adc_periph,
		uint8_t adc_channel_group, uint32_t external_trigger_source);

void adc_external_trigger_config(uint32_t adc_periph,
		uint8_t adc_channel_group, enum control_status newvalue);

void adc_software_trigger_enable(uint32_t adc_periph,
		uint8_t adc_channel_group);


// get channel data

// read ADC regular group data register
uint16_t adc_regular_data_read(uint32_t adc_periph);

// read ADC inserted group data register
uint16_t adc_inserted_data_read(uint32_t adc_periph, uint8_t inserted_channel);

// read the last ADC0 and ADC1 conversion result data in sync mode
uint32_t adc_sync_mode_convert_value_read(void);

// watchdog config
void adc_watchdog_single_channel_enable(uint32_t adc_periph,
		uint8_t adc_channel);

void adc_watchdog_group_channel_enable(uint32_t adc_periph,
		uint8_t adc_channel_group);

void adc_watchdog_disable(uint32_t adc_periph);

void adc_watchdog_threshold_config(uint32_t adc_periph,
		uint16_t low_threshold, uint16_t high_threshold);


// interrupt & flag functions
enum flag_status adc_flag_get(uint32_t adc_periph, uint32_t adc_flag);

void adc_flag_clear(uint32_t adc_periph, uint32_t adc_flag);

enum flag_status adc_regular_software_startconv_flag_get(uint32_t adc_periph);

enum flag_status adc_inserted_software_startconv_flag_get(uint32_t adc_periph);

enum flag_status adc_interrupt_flag_get(uint32_t adc_periph,
		uint32_t adc_interrupt);

void adc_interrupt_flag_clear(uint32_t adc_periph, uint32_t adc_interrupt);
void adc_interrupt_enable(uint32_t adc_periph, uint32_t adc_interrupt);
void adc_interrupt_disable(uint32_t adc_periph, uint32_t adc_interrupt);


// ADC resolution & oversample
void adc_resolution_config(uint32_t adc_periph, uint32_t resolution);

void adc_oversample_mode_config(uint32_t adc_periph, uint8_t mode,
		uint16_t shift, uint8_t ratio);

void adc_oversample_mode_enable(uint32_t adc_periph);
void adc_oversample_mode_disable(uint32_t adc_periph);

#endif
