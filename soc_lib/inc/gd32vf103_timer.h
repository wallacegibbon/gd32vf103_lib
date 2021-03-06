#ifndef __GD32VF103_TIMER_H
#define __GD32VF103_TIMER_H

#include "gd32vf103.h"

#define TIMER0			(TIMER_BASE + 0x00012C00U)
#define TIMER1			(TIMER_BASE + 0x00000000U)
#define TIMER2			(TIMER_BASE + 0x00000400U)
#define TIMER3			(TIMER_BASE + 0x00000800U)
#define TIMER4			(TIMER_BASE + 0x00000C00U)
#define TIMER5			(TIMER_BASE + 0x00001000U)
#define TIMER6			(TIMER_BASE + 0x00001400U)

#define TIMER_CTL0(timerx)	REG32((timerx) + 0x00U)
#define TIMER_CTL1(timerx)	REG32((timerx) + 0x04U)

// TIMER slave mode configuration register
#define TIMER_SMCFG(timerx)	REG32((timerx) + 0x08U)

// TIMER DMA and interrupt enable register
#define TIMER_DMAINTEN(timerx)	REG32((timerx) + 0x0CU)

// TIMER interrupt flag register
#define TIMER_INTF(timerx)	REG32((timerx) + 0x10U)

// TIMER software event generation register
#define TIMER_SWEVG(timerx)	REG32((timerx) + 0x14U)

#define TIMER_CHCTL0(timerx)	REG32((timerx) + 0x18U)
#define TIMER_CHCTL1(timerx)	REG32((timerx) + 0x1CU)
#define TIMER_CHCTL2(timerx)	REG32((timerx) + 0x20U)
#define TIMER_CNT(timerx)	REG32((timerx) + 0x24U)

// TIMER prescaler register
#define TIMER_PSC(timerx)	REG32((timerx) + 0x28U)

// TIMER counter auto reload register
#define TIMER_CAR(timerx)	REG32((timerx) + 0x2CU)

// TIMER counter repetition register
#define TIMER_CREP(timerx)	REG32((timerx) + 0x30U)

// TIMER channel 0 capture/compare value register
#define TIMER_CH0CV(timerx)	REG32((timerx) + 0x34U)

// TIMER channel 1 capture/compare value register
#define TIMER_CH1CV(timerx)	REG32((timerx) + 0x38U)

// TIMER channel 2 capture/compare value register
#define TIMER_CH2CV(timerx)	REG32((timerx) + 0x3CU)

// TIMER channel 3 capture/compare value register
#define TIMER_CH3CV(timerx)	REG32((timerx) + 0x40U)

// TIMER channel complementary protection register
#define TIMER_CCHP(timerx)	REG32((timerx) + 0x44U)

// TIMER DMA configuration register
#define TIMER_DMACFG(timerx)	REG32((timerx) + 0x48U)

// TIMER DMA transfer buffer register
#define TIMER_DMATB(timerx)	REG32((timerx) + 0x4CU)

// TIMER counter enable
#define TIMER_CTL0_CEN		BIT(0)

// update disable
#define TIMER_CTL0_UPDIS	BIT(1)

// update source
#define TIMER_CTL0_UPS		BIT(2)

// single pulse mode
#define TIMER_CTL0_SPM		BIT(3)

// timer counter direction
#define TIMER_CTL0_DIR		BIT(4)

// center-aligned mode selection
#define TIMER_CTL0_CAM		BITS(5, 6)

// auto-reload shadow enable
#define TIMER_CTL0_ARSE		BIT(7)

// clock division
#define TIMER_CTL0_CKDIV	BITS(8, 9)


// commutation control shadow enable
#define TIMER_CTL1_CCSE		BIT(0)

// commutation control shadow register update control
#define TIMER_CTL1_CCUC		BIT(2)

// DMA request source selection
#define TIMER_CTL1_DMAS		BIT(3)

// master mode control
#define TIMER_CTL1_MMC		BITS(4, 6)

// channel 0 trigger input selection(hall mode selection)
#define TIMER_CTL1_TI0S		BIT(7)

// idle state of channel 0 output
#define TIMER_CTL1_ISO0		BIT(8)

// idle state of channel 0 complementary output
#define TIMER_CTL1_ISO0N	BIT(9)

// idle state of channel 1 output
#define TIMER_CTL1_ISO1		BIT(10)

// idle state of channel 1 complementary output
#define TIMER_CTL1_ISO1N	BIT(11)

// idle state of channel 2 output
#define TIMER_CTL1_ISO2		BIT(12)

// idle state of channel 2 complementary output
#define TIMER_CTL1_ISO2N	BIT(13)

// idle state of channel 3 output
#define TIMER_CTL1_ISO3		BIT(14)



// slave mode control
#define TIMER_SMCFG_SMC		BITS(0, 2)

// trigger selection
#define TIMER_SMCFG_TRGS	BITS(4, 6)

// master-slave mode
#define TIMER_SMCFG_MSM		BIT(7)

// external trigger filter control
#define TIMER_SMCFG_ETFC	BITS(8, 11)

// external trigger prescaler
#define TIMER_SMCFG_ETPSC	BITS(12, 13)

// part of SMC for enable external clock mode 1
#define TIMER_SMCFG_SMC1	BIT(14)

// external trigger polarity
#define TIMER_SMCFG_ETP		BIT(15)



// update interrupt enable
#define TIMER_DMAINTEN_UPIE	BIT(0)

// channel 0 capture/compare interrupt enable
#define TIMER_DMAINTEN_CH0IE	BIT(1)

// channel 1 capture/compare interrupt enable
#define TIMER_DMAINTEN_CH1IE	BIT(2)

// channel 2 capture/compare interrupt enable
#define TIMER_DMAINTEN_CH2IE	BIT(3)

// channel 3 capture/compare interrupt enable
#define TIMER_DMAINTEN_CH3IE	BIT(4)

// commutation interrupt request enable
#define TIMER_DMAINTEN_CMTIE	BIT(5)

// trigger interrupt enable
#define TIMER_DMAINTEN_TRGIE	BIT(6)

// break interrupt enable
#define TIMER_DMAINTEN_BRKIE	BIT(7)

// update DMA request enable
#define TIMER_DMAINTEN_UPDEN	BIT(8)

// channel 0 capture/compare DMA request enable
#define TIMER_DMAINTEN_CH0DEN	BIT(9)

// channel 1 capture/compare DMA request enable
#define TIMER_DMAINTEN_CH1DEN	BIT(10)

// channel 2 capture/compare DMA request enable
#define TIMER_DMAINTEN_CH2DEN	BIT(11)

// channel 3 capture/compare DMA request enable
#define TIMER_DMAINTEN_CH3DEN	BIT(12)

// commutation DMA request enable
#define TIMER_DMAINTEN_CMTDEN	BIT(13)

// trigger DMA request enable
#define TIMER_DMAINTEN_TRGDEN	BIT(14)



// update interrupt flag
#define TIMER_INTF_UPIF		BIT(0)

// channel 0 capture/compare interrupt flag
#define TIMER_INTF_CH0IF	BIT(1)

// channel 1 capture/compare interrupt flag
#define TIMER_INTF_CH1IF	BIT(2)

// channel 2 capture/compare interrupt flag
#define TIMER_INTF_CH2IF	BIT(3)

// channel 3 capture/compare interrupt flag
#define TIMER_INTF_CH3IF	BIT(4)

// channel commutation interrupt flag
#define TIMER_INTF_CMTIF	BIT(5)

// trigger interrupt flag
#define TIMER_INTF_TRGIF	BIT(6)

// break interrupt flag
#define TIMER_INTF_BRKIF	BIT(7)

// channel 0 over capture flag
#define TIMER_INTF_CH0OF	BIT(9)

// channel 1 over capture flag
#define TIMER_INTF_CH1OF	BIT(10)

// channel 2 over capture flag
#define TIMER_INTF_CH2OF	BIT(11)

// channel 3 over capture flag
#define TIMER_INTF_CH3OF	BIT(12)



// update event generate
#define TIMER_SWEVG_UPG		BIT(0)

// channel 0 capture or compare event generation
#define TIMER_SWEVG_CH0G	BIT(1)

// channel 1 capture or compare event generation
#define TIMER_SWEVG_CH1G	BIT(2)

// channel 2 capture or compare event generation
#define TIMER_SWEVG_CH2G	BIT(3)

// channel 3 capture or compare event generation
#define TIMER_SWEVG_CH3G	BIT(4)

// channel commutation event generation
#define TIMER_SWEVG_CMTG	BIT(5)

// trigger event generation
#define TIMER_SWEVG_TRGG	BIT(6)

// break event generation
#define TIMER_SWEVG_BRKG	BIT(7)


// output compare mode
// channel 0 mode selection
#define TIMER_CHCTL0_CH0MS	BITS(0, 1)

// channel 0 output compare fast enable
#define TIMER_CHCTL0_CH0COMFEN	BIT(2)

// channel 0 output compare shadow enable
#define TIMER_CHCTL0_CH0COMSEN	BIT(3)

// channel 0 output compare control
#define TIMER_CHCTL0_CH0COMCTL	BITS(4, 6)

// channel 0 output compare clear enable
#define TIMER_CHCTL0_CH0COMCEN	BIT(7)

// channel 1 mode selection
#define TIMER_CHCTL0_CH1MS	BITS(8, 9)

// channel 1 output compare fast enable
#define TIMER_CHCTL0_CH1COMFEN	BIT(10)

// channel 1 output compare shadow enable
#define TIMER_CHCTL0_CH1COMSEN	BIT(11)

// channel 1 output compare control
#define TIMER_CHCTL0_CH1COMCTL	BITS(12, 14)

// channel 1 output compare clear enable
#define TIMER_CHCTL0_CH1COMCEN	BIT(15)


// input capture mode
// channel 0 input capture prescaler
#define TIMER_CHCTL0_CH0CAPPSC	BITS(2, 3)

// channel 0 input capture filter control
#define TIMER_CHCTL0_CH0CAPFLT	BITS(4, 7)

// channel 1 input capture prescaler
#define TIMER_CHCTL0_CH1CAPPSC	BITS(10, 11)

// channel 1 input capture filter control
#define TIMER_CHCTL0_CH1CAPFLT	BITS(12, 15)


// output compare mode
// channel 2 mode selection
#define TIMER_CHCTL1_CH2MS	BITS(0, 1)

// channel 2 output compare fast enable
#define TIMER_CHCTL1_CH2COMFEN	BIT(2)

// channel 2 output compare shadow enable
#define TIMER_CHCTL1_CH2COMSEN	BIT(3)

// channel 2 output compare control
#define TIMER_CHCTL1_CH2COMCTL	BITS(4, 6)

// channel 2 output compare clear enable
#define TIMER_CHCTL1_CH2COMCEN	BIT(7)

// channel 3 mode selection
#define TIMER_CHCTL1_CH3MS	BITS(8, 9)

// channel 3 output compare fast enable
#define TIMER_CHCTL1_CH3COMFEN	BIT(10)

// channel 3 output compare shadow enable
#define TIMER_CHCTL1_CH3COMSEN	BIT(11)

// channel 3 output compare control
#define TIMER_CHCTL1_CH3COMCTL	BITS(12, 14)

// channel 3 output compare clear enable
#define TIMER_CHCTL1_CH3COMCEN	BIT(15)


// input capture mode
// channel 2 input capture prescaler
#define TIMER_CHCTL1_CH2CAPPSC	BITS(2, 3)

// channel 2 input capture filter control
#define TIMER_CHCTL1_CH2CAPFLT	BITS(4, 7)

// channel 3 input capture prescaler
#define TIMER_CHCTL1_CH3CAPPSC	BITS(10, 11)

// channel 3 input capture filter control
#define TIMER_CHCTL1_CH3CAPFLT	BITS(12, 15)



// channel 0 capture/compare function enable
#define TIMER_CHCTL2_CH0EN	BIT(0)

// channel 0 capture/compare function polarity
#define TIMER_CHCTL2_CH0P	BIT(1)

// channel 0 complementary output enable
#define TIMER_CHCTL2_CH0NEN	BIT(2)

// channel 0 complementary output polarity
#define TIMER_CHCTL2_CH0NP	BIT(3)

// channel 1 capture/compare function enable
#define TIMER_CHCTL2_CH1EN	BIT(4)

// channel 1 capture/compare function polarity
#define TIMER_CHCTL2_CH1P	BIT(5)

// channel 1 complementary output enable
#define TIMER_CHCTL2_CH1NEN	BIT(6)

// channel 1 complementary output polarity
#define TIMER_CHCTL2_CH1NP	BIT(7)

// channel 2 capture/compare function enable
#define TIMER_CHCTL2_CH2EN	BIT(8)

// channel 2 capture/compare function polarity
#define TIMER_CHCTL2_CH2P	BIT(9)

// channel 2 complementary output enable
#define TIMER_CHCTL2_CH2NEN	BIT(10)

// channel 2 complementary output polarity
#define TIMER_CHCTL2_CH2NP	BIT(11)

// channel 3 capture/compare function enable
#define TIMER_CHCTL2_CH3EN	BIT(12)

// channel 3 capture/compare function polarity
#define TIMER_CHCTL2_CH3P	BIT(13)



// 16 bit timer counter
#define TIMER_CNT_CNT		BITS(0, 15)


// prescaler value of the counter clock
#define TIMER_PSC_PSC		BITS(0, 15)


// 16 bit counter auto reload value
#define TIMER_CAR_CARL		BITS(0, 15)


// counter repetition value
#define TIMER_CREP_CREP		BITS(0, 7)


// 16 bit capture/compare value of channel 0/1/2/3
#define TIMER_CH0CV_CH0VAL	BITS(0, 15)
#define TIMER_CH1CV_CH1VAL	BITS(0, 15)
#define TIMER_CH2CV_CH2VAL	BITS(0, 15)
#define TIMER_CH3CV_CH3VAL	BITS(0, 15)


// dead time configure
#define TIMER_CCHP_DTCFG	BITS(0, 7)

// complementary register protect control
#define TIMER_CCHP_PROT		BITS(8, 9)

// idle mode off-state configure
#define TIMER_CCHP_IOS		BIT(10)

// run mode off-state configure
#define TIMER_CCHP_ROS		BIT(11)

// break enable
#define TIMER_CCHP_BRKEN	BIT(12)

// break polarity
#define TIMER_CCHP_BRKP		BIT(13)

// output automatic enable
#define TIMER_CCHP_OAEN		BIT(14)

// primary output enable
#define TIMER_CCHP_POEN		BIT(15)



// DMA transfer access start address
#define TIMER_DMACFG_DMATA	BITS(0, 4)

// DMA transfer count
#define TIMER_DMACFG_DMATC	BITS(8, 12)



// DMA transfer buffer address
#define TIMER_DMATB_DMATB	BITS(0, 15)


struct timer_param {
	uint16_t	prescaler;
	uint16_t	alignedmode;
	uint16_t	counterdirection;
	uint32_t	period;
	uint16_t	clockdivision;
	uint8_t		repetitioncounter;
};

struct timer_break_param {
	uint16_t	runoffstate;
	uint16_t	ideloffstate;
	uint16_t	deadtime;
	uint16_t	breakpolarity;
	uint16_t	outputautostate;

	// complementary register protect control
	uint16_t	protectmode;

	uint16_t	breakstate;
};

// channel output parameter struct definitions
struct timer_oc_param {
	uint16_t	outputstate;
	uint16_t	outputnstate;
	uint16_t	ocpolarity;

	// channel complementary output polarity
	uint16_t	ocnpolarity;

	// idle state of channel output
	uint16_t	ocidlestate;

 	// idle state of channel complementary output
	uint16_t	ocnidlestate;
};

// channel input parameter struct definitions
struct timer_ic_param {
	uint16_t	icpolarity;

	// channel input mode selection
	uint16_t	icselection;

	// channel input capture prescaler
	uint16_t	icprescaler;

	// channel input capture filter control
	uint16_t	icfilter;
};

// TIMER interrupt enable or disable
// update interrupt
#define TIMER_INT_UP		TIMER_DMAINTEN_UPIE

// channel 0 interrupt
#define TIMER_INT_CH0		TIMER_DMAINTEN_CH0IE

// channel 1 interrupt
#define TIMER_INT_CH1		TIMER_DMAINTEN_CH1IE

// channel 2 interrupt
#define TIMER_INT_CH2		TIMER_DMAINTEN_CH2IE

// channel 3 interrupt
#define TIMER_INT_CH3		TIMER_DMAINTEN_CH3IE

// channel commutation interrupt flag
#define TIMER_INT_CMT		TIMER_DMAINTEN_CMTIE

// trigger interrupt
#define TIMER_INT_TRG		TIMER_DMAINTEN_TRGIE

// break interrupt
#define TIMER_INT_BRK		TIMER_DMAINTEN_BRKIE


// TIMER interrupt flag
// update interrupt
#define TIMER_INT_FLAG_UP  	TIMER_INT_UP

// channel 0/1/2/3 interrupt
#define TIMER_INT_FLAG_CH0	TIMER_INT_CH0
#define TIMER_INT_FLAG_CH1	TIMER_INT_CH1
#define TIMER_INT_FLAG_CH2	TIMER_INT_CH2
#define TIMER_INT_FLAG_CH3	TIMER_INT_CH3

// channel commutation interrupt flag
#define TIMER_INT_FLAG_CMT	TIMER_INT_CMT

// trigger interrupt
#define TIMER_INT_FLAG_TRG	TIMER_INT_TRG

#define TIMER_INT_FLAG_BRK	TIMER_INT_BRK

// TIMER flag
// update flag
#define TIMER_FLAG_UP		TIMER_INTF_UPIF

// channel 0/1/2/3 flag
#define TIMER_FLAG_CH0		TIMER_INTF_CH0IF
#define TIMER_FLAG_CH1		TIMER_INTF_CH1IF
#define TIMER_FLAG_CH2		TIMER_INTF_CH2IF
#define TIMER_FLAG_CH3		TIMER_INTF_CH3IF

// channel control update flag
#define TIMER_FLAG_CMT		TIMER_INTF_CMTIF

// trigger flag
#define TIMER_FLAG_TRG		TIMER_INTF_TRGIF

// break flag
#define TIMER_FLAG_BRK		TIMER_INTF_BRKIF

// channel 0/1/2/3 overcapture flag
#define TIMER_FLAG_CH0O		TIMER_INTF_CH0OF
#define TIMER_FLAG_CH1O		TIMER_INTF_CH1OF
#define TIMER_FLAG_CH2O		TIMER_INTF_CH2OF
#define TIMER_FLAG_CH3O		TIMER_INTF_CH3OF

// TIMER DMA source enable

// update DMA enable
#define TIMER_DMA_UPD		((uint16_t) TIMER_DMAINTEN_UPDEN)

// channel 0 DMA enable
#define TIMER_DMA_CH0D		((uint16_t) TIMER_DMAINTEN_CH0DEN)
// channel 1 DMA enable
#define TIMER_DMA_CH1D		((uint16_t) TIMER_DMAINTEN_CH1DEN)
#define TIMER_DMA_CH2D		((uint16_t) TIMER_DMAINTEN_CH2DEN)
#define TIMER_DMA_CH3D		((uint16_t) TIMER_DMAINTEN_CH3DEN)

// commutation DMA request enable
#define TIMER_DMA_CMTD		((uint16_t) TIMER_DMAINTEN_CMTDEN)
// trigger DMA enable
#define TIMER_DMA_TRGD		((uint16_t) TIMER_DMAINTEN_TRGDEN)


// channel DMA request source selection

// DMA request of channel n is sent when update event occurs
#define TIMER_DMAREQUEST_UPDATEEVENT	TIMER_CTL1_DMAS

// DMA request of channel n is sent when channel n event occurs
#define TIMER_DMAREQUEST_CHANNELEVENT	((uint32_t) 0x00000000U)


// DMA access base address
#define DMACFG_DMATA(regval)	(BITS(0, 4) & ((uint32_t) (regval) << 0U))

// DMA transfer address is TIMER_CTL0
#define TIMER_DMACFG_DMATA_CTL0		DMACFG_DMATA(0)
// DMA transfer address is TIMER_CTL1
#define TIMER_DMACFG_DMATA_CTL1		DMACFG_DMATA(1)
// DMA transfer address is TIMER_SMCFG
#define TIMER_DMACFG_DMATA_SMCFG	DMACFG_DMATA(2)
// ...
#define TIMER_DMACFG_DMATA_DMAINTEN	DMACFG_DMATA(3)
#define TIMER_DMACFG_DMATA_INTF		DMACFG_DMATA(4)
#define TIMER_DMACFG_DMATA_SWEVG	DMACFG_DMATA(5)
#define TIMER_DMACFG_DMATA_CHCTL0	DMACFG_DMATA(6)
#define TIMER_DMACFG_DMATA_CHCTL1	DMACFG_DMATA(7)
#define TIMER_DMACFG_DMATA_CHCTL2	DMACFG_DMATA(8)
#define TIMER_DMACFG_DMATA_CNT		DMACFG_DMATA(9)
#define TIMER_DMACFG_DMATA_PSC		DMACFG_DMATA(10)
#define TIMER_DMACFG_DMATA_CAR		DMACFG_DMATA(11)
#define TIMER_DMACFG_DMATA_CREP		DMACFG_DMATA(12)
#define TIMER_DMACFG_DMATA_CH0CV	DMACFG_DMATA(13)
#define TIMER_DMACFG_DMATA_CH1CV	DMACFG_DMATA(14)
#define TIMER_DMACFG_DMATA_CH2CV	DMACFG_DMATA(15)
#define TIMER_DMACFG_DMATA_CH3CV	DMACFG_DMATA(16)
#define TIMER_DMACFG_DMATA_CCHP		DMACFG_DMATA(17)
#define TIMER_DMACFG_DMATA_DMACFG	DMACFG_DMATA(18)

// DMA access burst length
#define DMACFG_DMATC(regval)	(BITS(8, 12) & ((uint32_t) (regval) << 8U))

// DMA transfer 1-18 time
#define TIMER_DMACFG_DMATC_1TRANSFER	DMACFG_DMATC(0)
#define TIMER_DMACFG_DMATC_2TRANSFER	DMACFG_DMATC(1)
#define TIMER_DMACFG_DMATC_3TRANSFER	DMACFG_DMATC(2)
#define TIMER_DMACFG_DMATC_4TRANSFER	DMACFG_DMATC(3)
#define TIMER_DMACFG_DMATC_5TRANSFER	DMACFG_DMATC(4)
#define TIMER_DMACFG_DMATC_6TRANSFER	DMACFG_DMATC(5)
#define TIMER_DMACFG_DMATC_7TRANSFER	DMACFG_DMATC(6)
#define TIMER_DMACFG_DMATC_8TRANSFER	DMACFG_DMATC(7)
#define TIMER_DMACFG_DMATC_9TRANSFER	DMACFG_DMATC(8)
#define TIMER_DMACFG_DMATC_10TRANSFER	DMACFG_DMATC(9)
#define TIMER_DMACFG_DMATC_11TRANSFER	DMACFG_DMATC(10)
#define TIMER_DMACFG_DMATC_12TRANSFER	DMACFG_DMATC(11)
#define TIMER_DMACFG_DMATC_13TRANSFER	DMACFG_DMATC(12)
#define TIMER_DMACFG_DMATC_14TRANSFER	DMACFG_DMATC(13)
#define TIMER_DMACFG_DMATC_15TRANSFER	DMACFG_DMATC(14)
#define TIMER_DMACFG_DMATC_16TRANSFER	DMACFG_DMATC(15)
#define TIMER_DMACFG_DMATC_17TRANSFER	DMACFG_DMATC(16)
#define TIMER_DMACFG_DMATC_18TRANSFER	DMACFG_DMATC(17)

// TIMER software event generation source

// update event generation
#define TIMER_EVENT_SRC_UPG	((uint16_t) 0x0001U)
// channel 0 capture or compare event generation
#define TIMER_EVENT_SRC_CH0G	((uint16_t) 0x0002U)
// ...
#define TIMER_EVENT_SRC_CH1G	((uint16_t) 0x0004U)
#define TIMER_EVENT_SRC_CH2G	((uint16_t) 0x0008U)
#define TIMER_EVENT_SRC_CH3G	((uint16_t) 0x0010U)
// channel commutation event generation
#define TIMER_EVENT_SRC_CMTG	((uint16_t) 0x0020U)
// trigger event generation
#define TIMER_EVENT_SRC_TRGG	((uint16_t) 0x0040U)
// break event generation
#define TIMER_EVENT_SRC_BRKG	((uint16_t) 0x0080U)

// center-aligned mode selection
#define CTL0_CAM(regval) \
	((uint16_t) (BITS(5, 6) & ((uint32_t) (regval) << 5U)))

// edge-aligned mode
#define TIMER_COUNTER_EDGE	CTL0_CAM(0)

// center-aligned and counting down assert mode
#define TIMER_COUNTER_CENTER_DOWN	CTL0_CAM(1)

// center-aligned and counting up assert mode
#define TIMER_COUNTER_CENTER_UP		CTL0_CAM(2)

// center-aligned and counting up/down assert mode
#define TIMER_COUNTER_CENTER_BOTH	CTL0_CAM(3)

/* TIMER prescaler reload mode */

// the prescaler is loaded right now
#define TIMER_PSC_RELOAD_NOW	TIMER_SWEVG_UPG

// the prescaler is loaded at the next update event
#define TIMER_PSC_RELOAD_UPDATE	((uint32_t) 0x00000000U)

// count direction

// counter up direction
#define TIMER_COUNTER_UP	((uint16_t) 0x0000U)
// counter down direction
#define TIMER_COUNTER_DOWN	((uint16_t) TIMER_CTL0_DIR)

// specify division ratio between TIMER clock and dead-time and sampling clock
#define CTL0_CKDIV(regval) \
	((uint16_t)(BITS(8, 9) & ((uint32_t)(regval) << 8U)))

// clock division value is 1,fDTS=fTIMER_CK
#define TIMER_CKDIV_DIV1	CTL0_CKDIV(0)
// clock division value is 2,fDTS= fTIMER_CK/2
#define TIMER_CKDIV_DIV2	CTL0_CKDIV(1)
// clock division value is 4, fDTS= fTIMER_CK/4
#define TIMER_CKDIV_DIV4	CTL0_CKDIV(2)

// single pulse mode
#define TIMER_SP_MODE_SINGLE	TIMER_CTL0_SPM
// repetitive pulse mode
#define TIMER_SP_MODE_REPETITIVE	((uint32_t) 0x00000000U)

// update source

// update generate only by counter overflow/underflow
#define TIMER_UPDATE_SRC_REGULAR	TIMER_CTL0_UPS
// update generate by setting of UPG bit or the counter overflow/underflow,
// or the slave mode controller trigger
#define TIMER_UPDATE_SRC_GLOBAL	((uint32_t) 0x00000000U)

// run mode off-state configure
// when POEN bit is set, the channel output signals(CHx_O/CHx_ON) are enabled,
// with relationship to CHxEN/CHxNEN bits
#define TIMER_ROS_STATE_ENABLE	((uint16_t) TIMER_CCHP_ROS)
// when POEN bit is set, the channel output signals(CHx_O/CHx_ON) are disabled
#define TIMER_ROS_STATE_DISABLE	((uint16_t) 0x0000U)



// idle mode off-state configure

// when POEN bit is reset, he channel output signals(CHx_O/CHx_ON) are enabled,
// with relationship to CHxEN/CHxNEN bits
#define TIMER_IOS_STATE_ENABLE	((uint16_t) TIMER_CCHP_IOS)

// when POEN bit is reset, the channel output signals(CHx_O/CHx_ON) are disabled
#define TIMER_IOS_STATE_DISABLE	((uint16_t) 0x0000U)

#define TIMER_BREAK_POLARITY_LOW	((uint16_t) 0x0000U)
#define TIMER_BREAK_POLARITY_HIGH	((uint16_t) TIMER_CCHP_BRKP)

#define TIMER_OUTAUTO_ENABLE	((uint16_t) TIMER_CCHP_OAEN)
#define TIMER_OUTAUTO_DISABLE	((uint16_t) 0x0000U)

/* complementary register protect control */
#define CCHP_PROT(regval) \
	((uint16_t) (BITS(8, 9) & ((uint32_t)(regval) << 8U)))

#define TIMER_CCHP_PROT_OFF	CCHP_PROT(0)
#define TIMER_CCHP_PROT_0	CCHP_PROT(1)
#define TIMER_CCHP_PROT_1	CCHP_PROT(2)
#define TIMER_CCHP_PROT_2	CCHP_PROT(3)

#define TIMER_BREAK_ENABLE	((uint16_t) TIMER_CCHP_BRKEN)
#define TIMER_BREAK_DISABLE	((uint16_t) 0x0000U)

// TIMER channel n(n=0,1,2,3)
#define TIMER_CH_0		((uint16_t) 0x0000U)
#define TIMER_CH_1		((uint16_t) 0x0001U)
#define TIMER_CH_2		((uint16_t) 0x0002U)
#define TIMER_CH_3		((uint16_t) 0x0003U)

#define TIMER_CCX_ENABLE	((uint16_t) 0x0001U)
#define TIMER_CCX_DISABLE	((uint16_t) 0x0000U)

// channel complementary enable
#define TIMER_CCXN_ENABLE	((uint16_t) 0x0004U)
// channel complementary disable
#define TIMER_CCXN_DISABLE	((uint16_t) 0x0000U)

#define TIMER_OC_POLARITY_HIGH	((uint16_t) 0x0000U)
#define TIMER_OC_POLARITY_LOW	((uint16_t) 0x0002U)

// channel complementary output polarity is high
#define TIMER_OCN_POLARITY_HIGH	((uint16_t) 0x0000U)
// channel complementary output polarity is low
#define TIMER_OCN_POLARITY_LOW	((uint16_t) 0x0008U)

// idle state of channel output is high
#define TIMER_OC_IDLE_STATE_HIGH	((uint16_t) 0x0100)
// idle state of channel output is low
#define TIMER_OC_IDLE_STATE_LOW	((uint16_t) 0x0000)

// idle state of channel complementary output is high
#define TIMER_OCN_IDLE_STATE_HIGH	((uint16_t) 0x0200U)
// idle state of channel complementary output is low
#define TIMER_OCN_IDLE_STATE_LOW	((uint16_t) 0x0000U)

// channel output compare mode
#define TIMER_OC_MODE_TIMING	((uint16_t) 0x0000U)
#define TIMER_OC_MODE_ACTIVE	((uint16_t) 0x0010U)
#define TIMER_OC_MODE_INACTIVE	((uint16_t) 0x0020U)
#define TIMER_OC_MODE_TOGGLE	((uint16_t) 0x0030U)
#define TIMER_OC_MODE_LOW	((uint16_t) 0x0040U) // force low mode
#define TIMER_OC_MODE_HIGH	((uint16_t) 0x0050U) // force high mode
#define TIMER_OC_MODE_PWM0	((uint16_t) 0x0060U)
#define TIMER_OC_MODE_PWM1	((uint16_t) 0x0070U)

// channel output compare shadow
#define TIMER_OC_SHADOW_ENABLE	((uint16_t) 0x0008U)
#define TIMER_OC_SHADOW_DISABLE	((uint16_t) 0x0000U)

// channel output compare fast function
#define TIMER_OC_FAST_ENABLE	((uint16_t) 0x0004)
#define TIMER_OC_FAST_DISABLE	((uint16_t) 0x0000)

// channel output compare clear function
#define TIMER_OC_CLEAR_ENABLE	((uint16_t) 0x0080U)
#define TIMER_OC_CLEAR_DISABLE	((uint16_t) 0x0000U)

// channel control shadow register update control
// the shadow registers update by when CMTG bit is set
#define TIMER_UPDATECTL_CCU	((uint32_t) 0x00000000U)
// the shadow registers update by when CMTG bit is set or an rising edge of
// TRGI occurs
#define TIMER_UPDATECTL_CCUTRI	TIMER_CTL1_CCUC

// channel input capture polarity
#define TIMER_IC_POLARITY_RISING	((uint16_t) 0x0000U) // rising edge
#define TIMER_IC_POLARITY_FALLING	((uint16_t) 0x0002U) // falling edge
#define TIMER_IC_POLARITY_BOTH_EDGE	((uint16_t) 0x000AU) // both edge

// TIMER input capture selection
// channel n is configured as input and icy is mapped on CIy
#define TIMER_IC_SELECTION_DIRECTTI	((uint16_t) 0x0001U)
// channel n is configured as input and icy is mapped on opposite input
#define TIMER_IC_SELECTION_INDIRECTTI	((uint16_t) 0x0002U)
// channel n is configured as input and icy is mapped on ITS
#define TIMER_IC_SELECTION_ITS	((uint16_t) 0x0003U)

// channel input capture prescaler
#define TIMER_IC_PSC_DIV1	((uint16_t) 0x0000U) // no prescaler
#define TIMER_IC_PSC_DIV2	((uint16_t) 0x0004U) // divided by 2
#define TIMER_IC_PSC_DIV4	((uint16_t) 0x0008U) // divided by 4
#define TIMER_IC_PSC_DIV8	((uint16_t) 0x000CU) // divided by 8

// trigger selection
#define SMCFG_TRGSEL(regval)	(BITS(4, 6) & ((uint32_t) (regval) << 4U))

#define TIMER_SMCFG_TRGSEL_ITI0		SMCFG_TRGSEL(0) // internal trigger 0
#define TIMER_SMCFG_TRGSEL_ITI1		SMCFG_TRGSEL(1) // internal trigger 1
#define TIMER_SMCFG_TRGSEL_ITI2		SMCFG_TRGSEL(2)
#define TIMER_SMCFG_TRGSEL_ITI3		SMCFG_TRGSEL(3)
#define TIMER_SMCFG_TRGSEL_CI0F_ED	SMCFG_TRGSEL(4) // TI0 Edge Detector

// filtered TIMER input 0
#define TIMER_SMCFG_TRGSEL_CI0FE0	SMCFG_TRGSEL(5)
// filtered TIMER input 1
#define TIMER_SMCFG_TRGSEL_CI1FE1	SMCFG_TRGSEL(6)
// filtered external trigger input
#define TIMER_SMCFG_TRGSEL_ETIFP	SMCFG_TRGSEL(7)

// master mode control
#define CTL1_MMC(regval)	(BITS(4, 6) & ((uint32_t) (regval) << 4U))

// the UPG bit as trigger output
#define TIMER_TRI_OUT_SRC_RESET		CTL1_MMC(0)

// the counter enable signal TIMER_CTL0_CEN as trigger output
#define TIMER_TRI_OUT_SRC_ENABLE	CTL1_MMC(1)

// update event as trigger output
#define TIMER_TRI_OUT_SRC_UPDATE 	CTL1_MMC(2)

// a capture or a compare match occurred in channel 0 as trigger output TRGO
#define TIMER_TRI_OUT_SRC_CH0		CTL1_MMC(3)

#define TIMER_TRI_OUT_SRC_O0CPRE	CTL1_MMC(4) // O0CPRE as trigger output
#define TIMER_TRI_OUT_SRC_O1CPRE	CTL1_MMC(5)
#define TIMER_TRI_OUT_SRC_O2CPRE	CTL1_MMC(6)
#define TIMER_TRI_OUT_SRC_O3CPRE	CTL1_MMC(7)

// slave mode control
#define SMCFG_SMC(regval)	(BITS(0, 2) & ((uint32_t) (regval) << 0U))

#define TIMER_SLAVE_MODE_DISABLE	SMCFG_SMC(0)
#define TIMER_ENCODER_MODE0		SMCFG_SMC(1)
#define TIMER_ENCODER_MODE1		SMCFG_SMC(2)
#define TIMER_ENCODER_MODE2		SMCFG_SMC(3)
#define TIMER_SLAVE_MODE_RESTART	SMCFG_SMC(4)
#define TIMER_SLAVE_MODE_PAUSE		SMCFG_SMC(5)
#define TIMER_SLAVE_MODE_EVENT		SMCFG_SMC(6)
#define TIMER_SLAVE_MODE_EXTERNAL0	SMCFG_SMC(7) // external clock mode 0

// master slave mode selection
#define TIMER_MASTER_SLAVE_MODE_ENABLE	TIMER_SMCFG_MSM
#define TIMER_MASTER_SLAVE_MODE_DISABLE	((uint32_t) 0x00000000U)

// external trigger prescaler
#define SMCFG_ETPSC(regval)	(BITS(12, 13) & ((uint32_t) (regval) << 12U))

#define TIMER_EXT_TRI_PSC_OFF		SMCFG_ETPSC(0)
#define TIMER_EXT_TRI_PSC_DIV2		SMCFG_ETPSC(1)
#define TIMER_EXT_TRI_PSC_DIV4		SMCFG_ETPSC(2)
#define TIMER_EXT_TRI_PSC_DIV8		SMCFG_ETPSC(3)

// external trigger polarity
// active low or falling edge active
#define TIMER_ETP_FALLING	TIMER_SMCFG_ETP

// active high or rising edge active
#define TIMER_ETP_RISING	((uint32_t) 0x00000000U)


// channel 0 trigger input selection

// TIMER hall sensor mode enable
#define TIMER_HALLINTERFACE_ENABLE	TIMER_CTL1_TI0S
// TIMER hall sensor mode disable
#define TIMER_HALLINTERFACE_DISABLE	((uint32_t) 0x00000000U)

// TIMERx(x=0..4) write CHxVAL register selection
#define TIMER_CHVSEL_ENABLE	((uint16_t) TIMER_CFG_OUTSEL)
#define TIMER_CHVSEL_DISABLE	((uint16_t) 0x0000U)

// TIMER timebase
void timer_deinit(uint32_t timer_periph);

void timer_struct_para_init(struct timer_param* initpara);

void timer_init(uint32_t timer_periph, struct timer_param* initpara);

void timer_enable(uint32_t timer_periph);
void timer_disable(uint32_t timer_periph);

void timer_auto_reload_shadow_enable(uint32_t timer_periph);
void timer_auto_reload_shadow_disable(uint32_t timer_periph);

void timer_update_event_enable(uint32_t timer_periph);
void timer_update_event_disable(uint32_t timer_periph);

void timer_counter_alignment(uint32_t timer_periph, uint16_t aligned);

void timer_counter_up_direction(uint32_t timer_periph);
void timer_counter_down_direction(uint32_t timer_periph);

void timer_prescaler_config(uint32_t timer_periph, uint16_t prescaler,
		uint32_t pscreload);

void timer_repetition_value_config(uint32_t timer_periph, uint16_t repetition);

void timer_autoreload_value_config(uint32_t timer_periph, uint16_t autoreload);

void timer_counter_value_config(uint32_t timer_periph, uint16_t counter);

uint32_t timer_counter_read(uint32_t timer_periph);

uint16_t timer_prescaler_read(uint32_t timer_periph);

void timer_single_pulse_mode_config(uint32_t timer_periph, uint32_t spmode);

void timer_update_source_config(uint32_t timer_periph, uint32_t update);

// TIMER DMA and event
void timer_dma_enable(uint32_t timer_periph, uint16_t dma);
void timer_dma_disable(uint32_t timer_periph, uint16_t dma);

void timer_channel_dma_request_source_select(uint32_t timer_periph,
		uint32_t dma_request);

void timer_dma_transfer_config(uint32_t timer_periph, uint32_t dma_baseaddr,
		uint32_t dma_lenth);

void timer_event_software_generate(uint32_t timer_periph, uint16_t event);

// TIMER channel complementary protection
void timer_break_struct_para_init(struct timer_break_param* breakpara);

void timer_break_config(uint32_t timer_periph,
		struct timer_break_param* breakpara);

void timer_break_enable(uint32_t timer_periph);
void timer_break_disable(uint32_t timer_periph);

void timer_automatic_output_enable(uint32_t timer_periph);
void timer_automatic_output_disable(uint32_t timer_periph);

void timer_primary_output_config(uint32_t timer_periph,
		enum control_status newvalue);

// enable or disable channel capture/compare control shadow register
void timer_channel_control_shadow_config(uint32_t timer_periph,
		enum control_status newvalue);

void timer_channel_control_shadow_update_config(uint32_t timer_periph,
		uint32_t ccuctl);

// TIMER channel output
void timer_channel_output_struct_para_init(struct timer_oc_param* ocpara);
void timer_channel_output_config(uint32_t timer_periph, uint16_t channel,
		struct timer_oc_param* ocpara);

void timer_channel_output_mode_config(uint32_t timer_periph, uint16_t channel,
		uint16_t ocmode);

void timer_channel_output_pulse_value_config(uint32_t timer_periph,
		uint16_t channel, uint32_t pulse);

void timer_channel_output_shadow_config(uint32_t timer_periph,
		uint16_t channel, uint16_t ocshadow);

void timer_channel_output_fast_config(uint32_t timer_periph, uint16_t channel,
		uint16_t ocfast);

void timer_channel_output_clear_config(uint32_t timer_periph,
		uint16_t channel, uint16_t occlear);

void timer_channel_output_polarity_config(uint32_t timer_periph,
		uint16_t channel, uint16_t ocpolarity);

void timer_channel_complementary_output_polarity_config(uint32_t timer_periph,
		uint16_t channel, uint16_t ocnpolarity);

// configure TIMER channel enable state
void timer_channel_output_state_config(uint32_t timer_periph, uint16_t channel,
		uint32_t state);

// configure TIMER channel complementary output enable state
void timer_channel_complementary_output_state_config(uint32_t timer_periph,
		uint16_t channel, uint16_t ocnstate);

// TIMER channel input
void timer_channel_input_struct_para_init(struct timer_ic_param* icpara);

void timer_input_capture_config(uint32_t timer_periph, uint16_t channel,
		struct timer_ic_param* icpara);

void timer_channel_input_capture_prescaler_config(uint32_t timer_periph,
		uint16_t channel, uint16_t prescaler);

uint32_t timer_channel_capture_value_register_read(uint32_t timer_periph,
		uint16_t channel);

void timer_input_pwm_capture_config(uint32_t timer_periph, uint16_t channel,
		struct timer_ic_param* icpwm);

// configure TIMER hall sensor mode
void timer_hall_mode_config(uint32_t timer_periph, uint32_t hallmode);

// TIMER master and slave mode
void timer_input_trigger_source_select(uint32_t timer_periph,
		uint32_t intrigger);

void timer_master_output_trigger_source_select(uint32_t timer_periph,
		uint32_t outrigger);

void timer_slave_mode_select(uint32_t timer_periph, uint32_t slavemode);

void timer_master_slave_mode_config(uint32_t timer_periph,
		uint32_t masterslave);

void timer_external_trigger_config(uint32_t timer_periph, uint32_t extprescaler,
		uint32_t extpolarity, uint32_t extfilter);

void timer_quadrature_decoder_mode_config(uint32_t timer_periph,
		uint32_t decomode, uint16_t ic0polarity, uint16_t ic1polarity);

void timer_internal_clock_config(uint32_t timer_periph);

void timer_internal_trigger_as_external_clock_config(uint32_t timer_periph,
		uint32_t intrigger);

void timer_external_trigger_as_external_clock_config(uint32_t timer_periph,
		uint32_t extrigger, uint16_t extpolarity, uint32_t extfilter);

void timer_external_clock_mode0_config(uint32_t timer_periph,
		uint32_t extprescaler, uint32_t extpolarity,
		uint32_t extfilter);

void timer_external_clock_mode1_config(uint32_t timer_periph,
		uint32_t extprescaler, uint32_t extpolarity,
		uint32_t extfilter);

void timer_external_clock_mode1_disable(uint32_t timer_periph);

// TIMER interrupt and flag
void timer_interrupt_enable(uint32_t timer_periph, uint32_t interrupt);
void timer_interrupt_disable(uint32_t timer_periph, uint32_t interrupt);

enum flag_status timer_interrupt_flag_get(uint32_t timer_periph,
		uint32_t interrupt);

void timer_interrupt_flag_clear(uint32_t timer_periph, uint32_t interrupt);

enum flag_status timer_flag_get(uint32_t timer_periph, uint32_t flag);

void timer_flag_clear(uint32_t timer_periph, uint32_t flag);

#endif
