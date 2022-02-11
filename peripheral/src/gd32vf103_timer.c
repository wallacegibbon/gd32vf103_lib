#include "gd32vf103_timer.h"

// TIMER init parameter aligne dmode mask
#define ALIGNEDMODE_MASK	((uint32_t)0x00000060U)

// TIMER init parameter counter direction mask
#define COUNTERDIRECTION_MASK	((uint32_t)0x00000010U)

// TIMER init parameter clock division value mask
#define CLOCKDIVISION_MASK	((uint32_t)0x00000300U)

void timer_deinit(uint32_t timer_periph) {
	switch (timer_periph) {
	case TIMER0:
		rcu_periph_reset_enable(RCU_TIMER0RST);
		rcu_periph_reset_disable(RCU_TIMER0RST);
		break;
	case TIMER1:
		rcu_periph_reset_enable(RCU_TIMER1RST);
		rcu_periph_reset_disable(RCU_TIMER1RST);
		break;
	case TIMER2:
		rcu_periph_reset_enable(RCU_TIMER2RST);
		rcu_periph_reset_disable(RCU_TIMER2RST);
		break;
	case TIMER3:
		rcu_periph_reset_enable(RCU_TIMER3RST);
		rcu_periph_reset_disable(RCU_TIMER3RST);
		break;
	case TIMER4:
		rcu_periph_reset_enable(RCU_TIMER4RST);
		rcu_periph_reset_disable(RCU_TIMER4RST);
		break;
	case TIMER5:
		rcu_periph_reset_enable(RCU_TIMER5RST);
		rcu_periph_reset_disable(RCU_TIMER5RST);
		break;
	case TIMER6:
		rcu_periph_reset_enable(RCU_TIMER6RST);
		rcu_periph_reset_disable(RCU_TIMER6RST);
		break;

	default:
		break;
	}
}

void timer_struct_para_init(struct timer_param * initpara) {
	initpara->prescaler = 0;
	initpara->alignedmode = TIMER_COUNTER_EDGE;
	initpara->counterdirection = TIMER_COUNTER_UP;
	initpara->period = 65535;
	initpara->clockdivision = TIMER_CKDIV_DIV1;
	initpara->repetitioncounter = 0;
}

/*
 * initialize TIMER counter
 * initpara: init parameter struct
 * 	prescaler:
 * 		prescaler value of the counter clock, 0~65535
 * 	alignedmode:
 * 		TIMER_COUNTER_EDGE, TIMER_COUNTER_CENTER_DOWN,
 * 		TIMER_COUNTER_CENTER_UP, TIMER_COUNTER_CENTER_BOTH
 * 	counterdirection:
 * 		TIMER_COUNTER_UP, TIMER_COUNTER_DOWN
 * 	period:
 * 		counter auto reload value, 0~65535
 * 	clockdivision:
 * 		TIMER_CKDIV_DIV1, TIMER_CKDIV_DIV2, TIMER_CKDIV_DIV4
 * 	repetitioncounter:
 * 		counter repetition value, 0~255
 */
void timer_init(uint32_t timer_periph, struct timer_param * initpara) {
	TIMER_PSC(timer_periph) = initpara->prescaler;

	if ((timer_periph == TIMER0) || (timer_periph == TIMER1) ||
			(timer_periph == TIMER2) || (timer_periph == TIMER3) ||
			(timer_periph == TIMER4)) {
		TIMER_CTL0(timer_periph) &=
			~(TIMER_CTL0_DIR | TIMER_CTL0_CAM);
		TIMER_CTL0(timer_periph) |=
			initpara->alignedmode & ALIGNEDMODE_MASK;
		TIMER_CTL0(timer_periph) |=
			initpara->counterdirection & COUNTERDIRECTION_MASK;
	} else {
		TIMER_CTL0(timer_periph) &=
			~TIMER_CTL0_DIR;
		TIMER_CTL0(timer_periph) |=
			initpara->counterdirection & COUNTERDIRECTION_MASK;
	}

	// configure the autoreload value
	TIMER_CAR(timer_periph) = initpara->period;

	if ((timer_periph != TIMER5) && (timer_periph != TIMER6)) {
		// reset the CKDIV bit
		TIMER_CTL0(timer_periph) &=
			~TIMER_CTL0_CKDIV;
		TIMER_CTL0(timer_periph) |=
			initpara->clockdivision & CLOCKDIVISION_MASK;
	}

	if (timer_periph == TIMER0)
		// configure the repetition counter value
		TIMER_CREP(timer_periph) = initpara->repetitioncounter;

	// generate an update event
	TIMER_SWEVG(timer_periph) |= TIMER_SWEVG_UPG;
}

void timer_enable(uint32_t timer_periph) {
	TIMER_CTL0(timer_periph) |= TIMER_CTL0_CEN;
}

void timer_disable(uint32_t timer_periph) {
	TIMER_CTL0(timer_periph) &= ~TIMER_CTL0_CEN;
}

void timer_auto_reload_shadow_enable(uint32_t timer_periph) {
	TIMER_CTL0(timer_periph) |= TIMER_CTL0_ARSE;
}

void timer_auto_reload_shadow_disable(uint32_t timer_periph) {
	TIMER_CTL0(timer_periph) &= ~TIMER_CTL0_ARSE;
}

void timer_update_event_enable(uint32_t timer_periph) {
	TIMER_CTL0(timer_periph) &= ~TIMER_CTL0_UPDIS;
}

void timer_update_event_disable(uint32_t timer_periph) {
	TIMER_CTL0(timer_periph) |= TIMER_CTL0_UPDIS;
}

/*
 * set TIMER counter alignment mode
 * aligned:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_COUNTER_EDGE:
 * 			edge-aligned mode
 * 		TIMER_COUNTER_CENTER_DOWN:
 * 			center-aligned and counting down assert mode
 * 		TIMER_COUNTER_CENTER_UP:
 * 			center-aligned and counting up assert mode
 * 		TIMER_COUNTER_CENTER_BOTH:
 * 			center-aligned and counting up/down assert mode
 */
void timer_counter_alignment(uint32_t timer_periph, uint16_t aligned) {
	TIMER_CTL0(timer_periph) &= ~TIMER_CTL0_CAM;
	TIMER_CTL0(timer_periph) |= aligned;
}

void timer_counter_up_direction(uint32_t timer_periph) {
	TIMER_CTL0(timer_periph) &= ~TIMER_CTL0_DIR;
}

void timer_counter_down_direction(uint32_t timer_periph) {
	TIMER_CTL0(timer_periph) |= (uint32_t) TIMER_CTL0_DIR;
}

/*
 * configure TIMER prescaler
 * pscreload: prescaler reload mode
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_PSC_RELOAD_NOW:
 * 		the prescaler is loaded right now
 * 		TIMER_PSC_RELOAD_UPDATE:
 * 			the prescaler is loaded at the next update event
 */
void timer_prescaler_config(uint32_t timer_periph, uint16_t prescaler,
		uint32_t pscreload) {

	TIMER_PSC(timer_periph) = prescaler;

	if (pscreload == TIMER_PSC_RELOAD_NOW)
		TIMER_SWEVG(timer_periph) |= TIMER_SWEVG_UPG;
}

/* repetition: the counter repetition value, 0~255 */
void timer_repetition_value_config(uint32_t timer_periph, uint16_t repetition) {
	TIMER_CREP(timer_periph) = repetition;
}

void timer_autoreload_value_config(uint32_t timer_periph, uint16_t autoreload) {
	TIMER_CAR(timer_periph) = autoreload;
}

void timer_counter_value_config(uint32_t timer_periph, uint16_t counter) {
	TIMER_CNT(timer_periph) = (uint32_t) counter;
}

uint32_t timer_counter_read(uint32_t timer_periph) {
	return TIMER_CNT(timer_periph);
}

uint16_t timer_prescaler_read(uint32_t timer_periph) {
	return TIMER_PSC(timer_periph);
}

/*
 * spmode:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_SP_MODE_SINGLE: single pulse mode
 * 		TIMER_SP_MODE_REPETITIVE: repetitive pulse mode
 */
void timer_single_pulse_mode_config(uint32_t timer_periph, uint32_t spmode) {
	if (spmode == TIMER_SP_MODE_SINGLE) {
		TIMER_CTL0(timer_periph) |= TIMER_CTL0_SPM;
	} else if (spmode == TIMER_SP_MODE_REPETITIVE) {
		TIMER_CTL0(timer_periph) &= ~TIMER_CTL0_SPM;
	} else {
		// illegal parameters
	}
}

/*
 * update: only one parameter can be selected which is shown as below:
 * 	TIMER_UPDATE_SRC_GLOBAL:
 * 		update generate by setting of UPG bit or the counter
 * 		overflow/underflow, or the slave mode controller trigger
 * 	TIMER_UPDATE_SRC_REGULAR:
 * 		update generate only by counter overflow/underflow
 */
void timer_update_source_config(uint32_t timer_periph, uint32_t update) {
	if (update == TIMER_UPDATE_SRC_REGULAR) {
		TIMER_CTL0(timer_periph) |= TIMER_CTL0_UPS;
	} else if (update == TIMER_UPDATE_SRC_GLOBAL) {
		TIMER_CTL0(timer_periph) &= ~TIMER_CTL0_UPS;
	} else {
		// illegal parameters
	}
}

/*
 * enable the TIMER DMA
 * dma: specify which DMA to enable
 * 	one or more parameters can be selected which are shown as below:
 * 		TIMER_DMA_UPD:
 * 			update DMA enable, TIMERx(x=0..6)
 * 		TIMER_DMA_CH0D:
 * 			channel 0 DMA enable, TIMERx(x=0..4)
 * 		TIMER_DMA_CH1D:
 * 			channel 1 DMA enable, TIMERx(x=0..4)
 * 		TIMER_DMA_CH2D:
 * 			channel 2 DMA enable, TIMERx(x=0..4)
 * 		TIMER_DMA_CH3D:
 * 			channel 3 DMA enable, TIMERx(x=0..4)
 * 		TIMER_DMA_CMTD:
 * 			channel commutation DMA request enable, TIMERx(x=0)
 * 		TIMER_DMA_TRGD:
 * 			trigger DMA enable, TIMERx(x=0..4)
 */
void timer_dma_enable(uint32_t timer_periph, uint16_t dma) {
	TIMER_DMAINTEN(timer_periph) |= dma;
}

void timer_dma_disable(uint32_t timer_periph, uint16_t dma) {
	TIMER_DMAINTEN(timer_periph) &= ~dma;
}

/*
 * channel DMA request source selection
 * dma_request: channel DMA request source selection
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_DMAREQUEST_CHANNELEVENT:
 * 			DMA request of channel n is sent when channel n event
 * 			occurs
 * 		TIMER_DMAREQUEST_UPDATEEVENT:
 * 			DMA request of channel n is sent when update event
 * 			occurs 
 */
void timer_channel_dma_request_source_select(uint32_t timer_periph,
		uint32_t dma_request) {

	if (dma_request == TIMER_DMAREQUEST_UPDATEEVENT) {
		TIMER_CTL1(timer_periph) |= TIMER_CTL1_DMAS;
	} else if (dma_request == TIMER_DMAREQUEST_CHANNELEVENT) {
		TIMER_CTL1(timer_periph) &= ~TIMER_CTL1_DMAS;
	} else {
		// illegal parameters
	}
}

/*
 * configure the TIMER DMA transfer
 * dma_baseaddr:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_DMACFG_DMATA_CTL0:
 * 			DMA transfer address is TIMER_CTL0, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_CTL1:
 * 			DMA transfer address is TIMER_CTL1, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_SMCFG:
 * 			DMA transfer address is TIMER_SMCFG, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_DMAINTEN:
 * 			DMA transfer address is TIMER_DMAINTEN, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_INTF:
 * 			DMA transfer address is TIMER_INTF, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_SWEVG:
 * 			DMA transfer address is TIMER_SWEVG, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_CHCTL0:
 * 			DMA transfer address is TIMER_CHCTL0, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_CHCTL1:
 * 			DMA transfer address is TIMER_CHCTL1, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_CHCTL2:
 * 			DMA transfer address is TIMER_CHCTL2, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_CNT:
 * 			DMA transfer address is TIMER_CNT, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_PSC:
 * 			DMA transfer address is TIMER_PSC, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_CAR:
 * 			DMA transfer address is TIMER_CAR, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_CREP:
 * 			DMA transfer address is TIMER_CREP, TIMERx(x=0)
 * 		TIMER_DMACFG_DMATA_CH0CV:
 * 			DMA transfer address is TIMER_CH0CV, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_CH1CV:
 * 			DMA transfer address is TIMER_CH1CV, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_CH2CV:
 * 			DMA transfer address is TIMER_CH2CV, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_CH3CV:
 * 			DMA transfer address is TIMER_CH3CV, TIMERx(x=0..4)
 * 		TIMER_DMACFG_DMATA_CCHP:
 * 			DMA transfer address is TIMER_CCHP, TIMERx(x=0) 
 * 		TIMER_DMACFG_DMATA_DMACFG:
 * 			DMA transfer address is TIMER_DMACFG, TIMERx(x=0..4) *
 * dma_lenth:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_DMACFG_DMATC_xTRANSFER(x=1..6): DMA transfer x time
 */
void timer_dma_transfer_config(uint32_t timer_periph, uint32_t dma_baseaddr,
		uint32_t dma_lenth) {

	TIMER_DMACFG(timer_periph) &=
		~(TIMER_DMACFG_DMATA | TIMER_DMACFG_DMATC);

	TIMER_DMACFG(timer_periph) |=
		dma_baseaddr | dma_lenth;
}

/*
 * software generate events
 * event: the timer software event generation sources
 * 	one or more parameters can be selected which are shown as below:
 * 		TIMER_EVENT_SRC_UPG:
 * 			update event generation, TIMERx(x=0..6)
 * 		TIMER_EVENT_SRC_CH0G:
 * 			channel 0 capture or compare event generation,
 * 			TIMERx(x=0..4)
 * 		TIMER_EVENT_SRC_CH1G:
 * 			channel 1 capture or compare event generation,
 * 			TIMERx(x=0..4)
 * 		TIMER_EVENT_SRC_CH2G:
 * 			channel 2 capture or compare event generation,
 * 			TIMERx(x=0..4)
 * 		TIMER_EVENT_SRC_CH3G:
 * 			channel 3 capture or compare event generation,
 * 			TIMERx(x=0..4)
 * 		TIMER_EVENT_SRC_CMTG:
 * 			channel commutation event generation, TIMERx(x=0)
 * 		TIMER_EVENT_SRC_TRGG:
 * 			trigger event generation, TIMERx(x=0..4)
 * 		TIMER_EVENT_SRC_BRKG:
 * 			break event generation, TIMERx(x=0)
 */
void timer_event_software_generate(uint32_t timer_periph, uint16_t event) {
	TIMER_SWEVG(timer_periph) |= event;
}

void timer_break_struct_para_init(struct timer_break_param * breakpara) {
	breakpara->runoffstate = TIMER_ROS_STATE_DISABLE;
	breakpara->ideloffstate = TIMER_IOS_STATE_DISABLE;
	breakpara->deadtime = 0;
	breakpara->breakpolarity = TIMER_BREAK_POLARITY_LOW;
	breakpara->outputautostate = TIMER_OUTAUTO_DISABLE;
	breakpara->protectmode = TIMER_CCHP_PROT_OFF;
	breakpara->breakstate = TIMER_BREAK_DISABLE;
}

/*
 * configure TIMER break function 
 * reakpara: TIMER break parameter struct
 * 	runoffstate:
 * 		TIMER_ROS_STATE_ENABLE, TIMER_ROS_STATE_DISABLE
 * 	ideloffstate:
 * 		TIMER_IOS_STATE_ENABLE, TIMER_IOS_STATE_DISABLE
 * 	deadtime:
 * 		0~255
 * 	breakpolarity:
 * 		TIMER_BREAK_POLARITY_LOW, TIMER_BREAK_POLARITY_HIGH
 * 	outputautostate:
 * 		TIMER_OUTAUTO_ENABLE, TIMER_OUTAUTO_DISABLE
 * 	protectmode:
 * 		TIMER_CCHP_PROT_OFF, TIMER_CCHP_PROT_0, TIMER_CCHP_PROT_1,
 * 		TIMER_CCHP_PROT_2
 * 	breakstate:
 * 		TIMER_BREAK_ENABLE, TIMER_BREAK_DISABLE
 */
void timer_break_config(uint32_t timer_periph,
		struct timer_break_param * breakpara) {

	TIMER_CCHP(timer_periph) =
		breakpara->runoffstate | breakpara->ideloffstate |
		breakpara->deadtime | breakpara->breakpolarity |
		breakpara->outputautostate | breakpara->protectmode |
		breakpara->breakstate;
}

void timer_break_enable(uint32_t timer_periph) {
	TIMER_CCHP(timer_periph) |= TIMER_CCHP_BRKEN;
}

void timer_break_disable(uint32_t timer_periph) {
	TIMER_CCHP(timer_periph) &= ~TIMER_CCHP_BRKEN;
}

void timer_automatic_output_enable(uint32_t timer_periph) {
	TIMER_CCHP(timer_periph) |= TIMER_CCHP_OAEN;
}

void timer_automatic_output_disable(uint32_t timer_periph) {
	TIMER_CCHP(timer_periph) &= ~TIMER_CCHP_OAEN;
}

void timer_primary_output_config(uint32_t timer_periph,
		enum control_status newvalue) {

	if (newvalue == ENABLE)
		TIMER_CCHP(timer_periph) |= TIMER_CCHP_POEN;
	else
		TIMER_CCHP(timer_periph) &= ~TIMER_CCHP_POEN;
}

void timer_channel_control_shadow_config(uint32_t timer_periph,
		enum control_status newvalue) {

	if (newvalue == ENABLE)
		TIMER_CTL1(timer_periph) |= TIMER_CTL1_CCSE;
	else
		TIMER_CTL1(timer_periph) &= ~TIMER_CTL1_CCSE;
}

/*
 * configure TIMER channel control shadow register update control
 * ccuctl: channel control shadow register update control
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_UPDATECTL_CCU:
 * 			the shadow registers update by when CMTG bit is set
 * 		TIMER_UPDATECTL_CCUTRI:
 * 			the shadow registers update by when CMTG bit is set or
 * 			an rising edge of TRGI occurs 
 */
void timer_channel_control_shadow_update_config(uint32_t timer_periph,
		uint32_t ccuctl) {

	if (ccuctl == TIMER_UPDATECTL_CCU) {
		TIMER_CTL1(timer_periph) &= (~(uint32_t) TIMER_CTL1_CCUC);
	} else if (ccuctl == TIMER_UPDATECTL_CCUTRI) {
		TIMER_CTL1(timer_periph) |= (uint32_t) TIMER_CTL1_CCUC;
	} else {
		// illegal parameters
	}
}

void timer_channel_output_struct_para_init(struct timer_oc_param * ocpara) {
	ocpara->outputstate = TIMER_CCX_DISABLE;
	ocpara->outputnstate = TIMER_CCXN_DISABLE;
	ocpara->ocpolarity = TIMER_OC_POLARITY_HIGH;
	ocpara->ocnpolarity = TIMER_OCN_POLARITY_HIGH;
	ocpara->ocidlestate = TIMER_OC_IDLE_STATE_LOW;
	ocpara->ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
}

/*
 * channel:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_CH_0: TIMER channel 0(TIMERx(x=0..4))
 * 		TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
 * 		TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
 * 		TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
 *
 * ocpara: TIMER channeln output parameter struct
 * 	outputstate: TIMER_CCX_ENABLE, TIMER_CCX_DISABLE
 * 	outputnstate: TIMER_CCXN_ENABLE, TIMER_CCXN_DISABLE
 * 	ocpolarity: TIMER_OC_POLARITY_HIGH, TIMER_OC_POLARITY_LOW
 * 	ocnpolarity: TIMER_OCN_POLARITY_HIGH, TIMER_OCN_POLARITY_LOW
 * 	ocidlestate: TIMER_OC_IDLE_STATE_LOW, TIMER_OC_IDLE_STATE_HIGH
 * 	ocnidlestate: TIMER_OCN_IDLE_STATE_LOW, TIMER_OCN_IDLE_STATE_HIGH
 */
void timer_channel_output_config(uint32_t timer_periph, uint16_t channel,
		struct timer_oc_param * ocpara) {

	switch (channel) {
	case TIMER_CH_0:
		// reset the CH0EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0EN;
		// set the CH0EN bit
		TIMER_CHCTL2(timer_periph) |= ocpara->outputstate;
		// reset the CH0P bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0P;
		// set the CH0P bit
		TIMER_CHCTL2(timer_periph) |= ocpara->ocpolarity;

		if (TIMER0 == timer_periph) {
			// reset the CH0NEN bit
			TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0NEN;
			// set the CH0NEN bit
			TIMER_CHCTL2(timer_periph) |= ocpara->outputnstate;
			// reset the CH0NP bit
			TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0NP;
			// set the CH0NP bit
			TIMER_CHCTL2(timer_periph) |= ocpara->ocnpolarity;
			// reset the ISO0 bit
			TIMER_CTL1(timer_periph) &= ~TIMER_CTL1_ISO0;
			// set the ISO0 bit
			TIMER_CTL1(timer_periph) |= ocpara->ocidlestate;
			// reset the ISO0N bit
			TIMER_CTL1(timer_periph) &= ~TIMER_CTL1_ISO0N;
			// set the ISO0N bit
			TIMER_CTL1(timer_periph) |= ocpara->ocnidlestate;
		}
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0MS;
		break;
	case TIMER_CH_1:
		// reset the CH1EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH1EN;
		// set the CH1EN bit
		TIMER_CHCTL2(timer_periph) |= ocpara->outputstate << 4;
		// reset the CH1P bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH1P;
		// set the CH1P bit
		TIMER_CHCTL2(timer_periph) |= ocpara->ocpolarity << 4;

		if (TIMER0 == timer_periph) {
			// reset the CH1NEN bit
			TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH1NEN;
			// set the CH1NEN bit
			TIMER_CHCTL2(timer_periph) |=
				(ocpara->outputnstate) << 4;
			// reset the CH1NP bit
			TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH1NP;
			// set the CH1NP bit
			TIMER_CHCTL2(timer_periph) |=
				(ocpara->ocnpolarity) << 4;
			// reset the ISO1 bit
			TIMER_CTL1(timer_periph) &= ~TIMER_CTL1_ISO1;
			// set the ISO1 bit
			TIMER_CTL1(timer_periph) |= (ocpara->ocidlestate) << 2;
			// reset the ISO1N bit
			TIMER_CTL1(timer_periph) &= ~TIMER_CTL1_ISO1N;
			// set the ISO1N bit
			TIMER_CTL1(timer_periph) |= ocpara->ocnidlestate << 2;
		}
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1MS;
		break;
	case TIMER_CH_2:
		// reset the CH2EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH2EN;
		// set the CH2EN bit
		TIMER_CHCTL2(timer_periph) |= ocpara->outputstate << 8;
		// reset the CH2P bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH2P;
		// set the CH2P bit
		TIMER_CHCTL2(timer_periph) |= ocpara->ocpolarity << 8;

		if (timer_periph == TIMER0) {
			// reset the CH2NEN bit
			TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH2NEN;
			// set the CH2NEN bit
			TIMER_CHCTL2(timer_periph) |=
				ocpara->outputnstate << 8;
			// reset the CH2NP bit
			TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH2NP;
			// set the CH2NP bit
			TIMER_CHCTL2(timer_periph) |=
				ocpara->ocnpolarity << 8;
			// reset the ISO2 bit
			TIMER_CTL1(timer_periph) &= ~TIMER_CTL1_ISO2;
			// set the ISO2 bit
			TIMER_CTL1(timer_periph) |= ocpara->ocidlestate << 4;
			// reset the ISO2N bit
			TIMER_CTL1(timer_periph) &= ~TIMER_CTL1_ISO2N;
			// set the ISO2N bit
			TIMER_CTL1(timer_periph) |= ocpara->ocnidlestate << 4;
		}
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH2MS;
		break;
	case TIMER_CH_3:
		// reset the CH3EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH3EN;
		// set the CH3EN bit
		TIMER_CHCTL2(timer_periph) |= ocpara->outputstate << 12;
		// reset the CH3P bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH3P;
		// set the CH3P bit
		TIMER_CHCTL2(timer_periph) |= ocpara->ocpolarity << 12;

		if (timer_periph == TIMER0) {
			// reset the ISO3 bit
			TIMER_CTL1(timer_periph) &= ~TIMER_CTL1_ISO3;
			// set the ISO3 bit
			TIMER_CTL1(timer_periph) |= ocpara->ocidlestate << 6;
		}
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH3MS;
		break;
	default:
		break;
	}
}

/*
 * configure TIMER channel output compare mode
 * channel:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_CH_0: TIMER channel 0(TIMERx(x=0..4))
 * 		TIMER_CH_1: TIMER channel 1(TIMERx(x=0..4))
 * 		TIMER_CH_2: TIMER channel 2(TIMERx(x=0..4))
 * 		TIMER_CH_3: TIMER channel 3(TIMERx(x=0..4))
 * ocmode: channel output compare mode
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_OC_MODE_TIMING: timing mode
 * 		TIMER_OC_MODE_ACTIVE: active mode
 * 		TIMER_OC_MODE_INACTIVE: inactive mode
 * 		TIMER_OC_MODE_TOGGLE: toggle mode
 * 		TIMER_OC_MODE_LOW: force low mode
 * 		TIMER_OC_MODE_HIGH: force high mode
 * 		TIMER_OC_MODE_PWM0: PWM mode 0
 * 		TIMER_OC_MODE_PWM1: PWM mode 1
 */
void timer_channel_output_mode_config(uint32_t timer_periph, uint16_t channel,
		uint16_t ocmode) {

	switch (channel) {
	case TIMER_CH_0:
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0COMCTL;
		TIMER_CHCTL0(timer_periph) |= ocmode;
		break;
	case TIMER_CH_1:
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1COMCTL;
		TIMER_CHCTL0(timer_periph) |= ocmode << 8;
		break;
	case TIMER_CH_2:
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH2COMCTL;
		TIMER_CHCTL1(timer_periph) |= ocmode;
		break;
	case TIMER_CH_3:
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH3COMCTL;
		TIMER_CHCTL1(timer_periph) |= ocmode << 8;
		break;
	default:
		break;
	}
}

void timer_channel_output_pulse_value_config(uint32_t timer_periph,
		uint16_t channel, uint32_t pulse) {

	switch (channel) {
	case TIMER_CH_0:
		TIMER_CH0CV(timer_periph) = (uint32_t) pulse;
		break;
	case TIMER_CH_1:
		TIMER_CH1CV(timer_periph) = (uint32_t) pulse;
		break;
	case TIMER_CH_2:
		TIMER_CH2CV(timer_periph) = (uint32_t) pulse;
		break;
	case TIMER_CH_3:
		TIMER_CH3CV(timer_periph) = (uint32_t) pulse;
		break;
	default:
		break;
	}
}

/*
 * ocshadow: channel output shadow state
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_OC_SHADOW_ENABLE: channel output shadow state enable
 * 		TIMER_OC_SHADOW_DISABLE: channel output shadow state disable
 */
void timer_channel_output_shadow_config(uint32_t timer_periph, uint16_t channel,
		uint16_t ocshadow) {

	switch (channel) {
	case TIMER_CH_0:
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0COMSEN;
		TIMER_CHCTL0(timer_periph) |= ocshadow;
		break;
	case TIMER_CH_1:
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1COMSEN;
		TIMER_CHCTL0(timer_periph) |= ocshadow << 8;
		break;
	case TIMER_CH_2:
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH2COMSEN;
		TIMER_CHCTL1(timer_periph) |= ocshadow;
		break;
	case TIMER_CH_3:
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH3COMSEN;
		TIMER_CHCTL1(timer_periph) |= ocshadow << 8;
		break;
	default:
		break;
	}
}

/*
 * ocfast: channel output fast function
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_OC_FAST_ENABLE: channel output fast function enable
 * 		TIMER_OC_FAST_DISABLE: channel output fast function disable
 */
void timer_channel_output_fast_config(uint32_t timer_periph, uint16_t channel,
		uint16_t ocfast) {

	switch (channel) {
	case TIMER_CH_0:
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0COMFEN;
		TIMER_CHCTL0(timer_periph) |= ocfast;
		break;
	case TIMER_CH_1:
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1COMFEN;
		TIMER_CHCTL0(timer_periph) |= ocfast << 8;
		break;
	case TIMER_CH_2:
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH2COMFEN;
		TIMER_CHCTL1(timer_periph) |= ocfast;
		break;
	case TIMER_CH_3:
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH3COMFEN;
		TIMER_CHCTL1(timer_periph) |= ocfast << 8;
		break;
	default:
		break;
	}
}

/*
 * occlear: channel output clear function
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_OC_CLEAR_ENABLE: channel output clear function enable
 * 		TIMER_OC_CLEAR_DISABLE: channel output clear function disable
 */
void timer_channel_output_clear_config(uint32_t timer_periph, uint16_t channel,
		uint16_t occlear) {

	switch (channel) {
	case TIMER_CH_0:
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0COMCEN;
		TIMER_CHCTL0(timer_periph) |= occlear;
		break;
	case TIMER_CH_1:
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1COMCEN;
		TIMER_CHCTL0(timer_periph) |= occlear << 8;
		break;
	case TIMER_CH_2:
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH2COMCEN;
		TIMER_CHCTL1(timer_periph) |= occlear;
		break;
	case TIMER_CH_3:
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH3COMCEN;
		TIMER_CHCTL1(timer_periph) |= occlear << 8;
		break;
	default:
		break;
	}
}

/*
 * ocpolarity: channel output polarity
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_OC_POLARITY_HIGH: channel output polarity is high
 * 		TIMER_OC_POLARITY_LOW: channel output polarity is low
 */
void timer_channel_output_polarity_config(uint32_t timer_periph,
		uint16_t channel, uint16_t ocpolarity) {

	switch (channel) {
	case TIMER_CH_0:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0P;
		TIMER_CHCTL2(timer_periph) |= ocpolarity;
		break;
	case TIMER_CH_1:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH1P;
		TIMER_CHCTL2(timer_periph) |= ocpolarity << 4;
		break;
	case TIMER_CH_2:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH2P;
		TIMER_CHCTL2(timer_periph) |= ocpolarity << 8;
		break;
	case TIMER_CH_3:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH3P;
		TIMER_CHCTL2(timer_periph) |= ocpolarity << 12;
		break;
	default:
		break;
	}
}

/*
 * configure TIMER channel complementary output polarity 
 * imer_periph: TIMER0
 *
 * channel: TIMER0 only has channel 0 1 2
 *
 * ocnpolarity: channel complementary output polarity
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_OCN_POLARITY_HIGH:
 * 			channel complementary output polarity is high
 * 		TIMER_OCN_POLARITY_LOW:
 * 			channel complementary output polarity is low
 */
void timer_channel_complementary_output_polarity_config(uint32_t timer_periph,
		uint16_t channel, uint16_t ocnpolarity) {

	switch (channel) {
	case TIMER_CH_0:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0NP;
		TIMER_CHCTL2(timer_periph) |= ocnpolarity;
		break;
	case TIMER_CH_1:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH1NP;
		TIMER_CHCTL2(timer_periph) |= ocnpolarity << 4;
		break;
	case TIMER_CH_2:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH2NP;
		TIMER_CHCTL2(timer_periph) |= ocnpolarity << 8;
		break;
	default:
		break;
	}
}

/*
 * state: TIMER channel enable state
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_CCX_ENABLE: channel enable
 * 		TIMER_CCX_DISABLE: channel disable 
 */
void timer_channel_output_state_config(uint32_t timer_periph, uint16_t channel,
		uint32_t state) {

	switch (channel) {
	case TIMER_CH_0:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0EN;
		TIMER_CHCTL2(timer_periph) |= state;
		break;
	case TIMER_CH_1:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH1EN;
		TIMER_CHCTL2(timer_periph) |= state << 4;
		break;
	case TIMER_CH_2:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH2EN;
		TIMER_CHCTL2(timer_periph) |= state << 8;
		break;
	case TIMER_CH_3:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH3EN;
		TIMER_CHCTL2(timer_periph) |= state << 12;
		break;
	default:
		break;
	}
}

/*
 * configure TIMER channel complementary output enable state
 *
 * channel: TIMER0 only has channel 0 1 2
 *
 * ocnstate: TIMER channel complementary output enable state
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_CCXN_ENABLE: channel complementary enable
 * 		TIMER_CCXN_DISABLE: channel complementary disable 
 */
void timer_channel_complementary_output_state_config(uint32_t timer_periph,
		uint16_t channel, uint16_t ocnstate) {

	switch (channel) {
	case TIMER_CH_0:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0NEN;
		TIMER_CHCTL2(timer_periph) |= ocnstate;
		break;
	case TIMER_CH_1:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH1NEN;
		TIMER_CHCTL2(timer_periph) |= ocnstate << 4;
		break;
	case TIMER_CH_2:
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH2NEN;
		TIMER_CHCTL2(timer_periph) |= ocnstate << 8;
		break;
	default:
		break;
	}
}

void timer_channel_input_struct_para_init(struct timer_ic_param *icpara) {
	icpara->icpolarity = TIMER_IC_POLARITY_RISING;
	icpara->icselection = TIMER_IC_SELECTION_DIRECTTI;
	icpara->icprescaler = TIMER_IC_PSC_DIV1;
	icpara->icfilter = 0U;
}

/*
 * icpara: TIMER channel intput parameter struct
 * 	icpolarity:
 * 		TIMER_IC_POLARITY_RISING, TIMER_IC_POLARITY_FALLING,
 * 		TIMER_IC_POLARITY_BOTH_EDGE(only for TIMER1~TIMER8)
 * 	icselection:
 * 		TIMER_IC_SELECTION_DIRECTTI, TIMER_IC_SELECTION_INDIRECTTI,
 * 		TIMER_IC_SELECTION_ITS
 * 	icprescaler:
 * 		TIMER_IC_PSC_DIV1, TIMER_IC_PSC_DIV2, TIMER_IC_PSC_DIV4,
 * 		TIMER_IC_PSC_DIV8
 * 	icfilter: 0~15
 */
void timer_input_capture_config(uint32_t timer_periph, uint16_t channel,
		struct timer_ic_param *icpara) {

	switch (channel) {
	case TIMER_CH_0:
		// reset the CH0EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0EN;
		// reset the CH0P and CH0NP bits
		TIMER_CHCTL2(timer_periph) &=
			~(TIMER_CHCTL2_CH0P | TIMER_CHCTL2_CH0NP);
		TIMER_CHCTL2(timer_periph) |= icpara->icpolarity;
		// reset the CH0MS bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0MS;
		TIMER_CHCTL0(timer_periph) |= icpara->icselection;
		// reset the CH0CAPFLT bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0CAPFLT;
		TIMER_CHCTL0(timer_periph) |= icpara->icfilter << 4;

		// set the CH0EN bit
		TIMER_CHCTL2(timer_periph) |=  TIMER_CHCTL2_CH0EN;
		break;

	case TIMER_CH_1:
		// reset the CH1EN bit
		TIMER_CHCTL2(timer_periph) &= (~ TIMER_CHCTL2_CH1EN);

		// reset the CH1P and CH1NP bits
		TIMER_CHCTL2(timer_periph) &=
			~(TIMER_CHCTL2_CH1P | TIMER_CHCTL2_CH1NP);
		TIMER_CHCTL2(timer_periph) |= icpara->icpolarity << 4;
		// reset the CH1MS bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1MS;
		TIMER_CHCTL0(timer_periph) |= icpara->icselection << 8;
		// reset the CH1CAPFLT bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1CAPFLT;
		TIMER_CHCTL0(timer_periph) |= icpara->icfilter << 12;

		// set the CH1EN bit
		TIMER_CHCTL2(timer_periph) |=  TIMER_CHCTL2_CH1EN;
		break;
	case TIMER_CH_2:
		// reset the CH2EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH2EN;

		// reset the CH2P and CH2NP bits
		TIMER_CHCTL2(timer_periph) &=
			~(TIMER_CHCTL2_CH2P | TIMER_CHCTL2_CH2NP);
		TIMER_CHCTL2(timer_periph) |= icpara->icpolarity << 8;

		// reset the CH2MS bit
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH2MS;
		TIMER_CHCTL1(timer_periph) |= icpara->icselection;

		// reset the CH2CAPFLT bit
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH2CAPFLT;
		TIMER_CHCTL1(timer_periph) |= icpara->icfilter << 4;

		// set the CH2EN bit
		TIMER_CHCTL2(timer_periph) |= TIMER_CHCTL2_CH2EN;
		break;
	case TIMER_CH_3:
		// reset the CH3EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH3EN;

		// reset the CH3P bits
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH3P;
		TIMER_CHCTL2(timer_periph) |= icpara->icpolarity << 12;

		// reset the CH3MS bit
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH3MS;
		TIMER_CHCTL1(timer_periph) |= icpara->icselection << 8;

		// reset the CH3CAPFLT bit
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH3CAPFLT;
		TIMER_CHCTL1(timer_periph) |= icpara->icfilter << 12;

		// set the CH3EN bit
		TIMER_CHCTL2(timer_periph) |=  TIMER_CHCTL2_CH3EN;
		break;
	default:
		break;
	}
	// configure TIMER channel input capture prescaler value
	timer_channel_input_capture_prescaler_config(
			timer_periph, channel, icpara->icprescaler);
}

/*
 * prescaler: channel input capture prescaler value
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_IC_PSC_DIV1: no prescaler
 * 		TIMER_IC_PSC_DIV2: divided by 2
 * 		TIMER_IC_PSC_DIV4: divided by 4
 * 		TIMER_IC_PSC_DIV8: divided by 8
 */
void timer_channel_input_capture_prescaler_config(uint32_t timer_periph,
		uint16_t channel, uint16_t prescaler) {

	switch (channel) {
	case TIMER_CH_0:
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0CAPPSC;
		TIMER_CHCTL0(timer_periph) |= prescaler;
		break;
	case TIMER_CH_1:
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1CAPPSC;
		TIMER_CHCTL0(timer_periph) |= prescaler << 8;
		break;
	case TIMER_CH_2:
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH2CAPPSC;
		TIMER_CHCTL1(timer_periph) |=  prescaler;
		break;
	case TIMER_CH_3:
		TIMER_CHCTL1(timer_periph) &= ~TIMER_CHCTL1_CH3CAPPSC;
		TIMER_CHCTL1(timer_periph) |= prescaler << 8;
		break;
	default:
		break;
	}
}

uint32_t timer_channel_capture_value_register_read(uint32_t timer_periph,
		uint16_t channel) {
	switch (channel) {
	case TIMER_CH_0:
		// read TIMER channel 0 capture compare register value
		return TIMER_CH0CV(timer_periph);
	case TIMER_CH_1:
		return TIMER_CH1CV(timer_periph);
	case TIMER_CH_2:
		return TIMER_CH2CV(timer_periph);
	case TIMER_CH_3:
		return TIMER_CH3CV(timer_periph);
	default:
		return 0;
	}
}

/*
 * channel:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_CH_0: TIMER channel 0
 * 		TIMER_CH_1: TIMER channel 1
 *
 * icpwm: TIMER channel intput pwm parameter struct
 * 	icpolarity: TIMER_IC_POLARITY_RISING, TIMER_IC_POLARITY_FALLING
 * 	icselection: TIMER_IC_SELECTION_DIRECTTI, TIMER_IC_SELECTION_INDIRECTTI
 * 	icprescaler: TIMER_IC_PSC_DIV1, TIMER_IC_PSC_DIV2, TIMER_IC_PSC_DIV4,
 * 			TIMER_IC_PSC_DIV8
 * 	icfilter: 0~15
 */
void timer_input_pwm_capture_config(uint32_t timer_periph, uint16_t channel,
		struct timer_ic_param * icpwm) {
	uint16_t icpolarity = 0;
	uint16_t icselection = 0;

	// Set channel input polarity
	if (icpwm->icpolarity == TIMER_IC_POLARITY_RISING)
		icpolarity = TIMER_IC_POLARITY_FALLING;
	else
		icpolarity = TIMER_IC_POLARITY_RISING;

	// Set channel input mode selection
	if (icpwm->icselection == TIMER_IC_SELECTION_DIRECTTI)
		icselection = TIMER_IC_SELECTION_INDIRECTTI;
	else
		icselection = TIMER_IC_SELECTION_DIRECTTI;

	if (TIMER_CH_0 == channel) {
		// reset the CH0EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0EN;
		// reset the CH0P and CH0NP bits
		TIMER_CHCTL2(timer_periph) &=
			~(TIMER_CHCTL2_CH0P | TIMER_CHCTL2_CH0NP);
		// set the CH0P and CH0NP bits
		TIMER_CHCTL2(timer_periph) |= icpwm->icpolarity;
		// reset the CH0MS bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0MS;
		// set the CH0MS bit
		TIMER_CHCTL0(timer_periph) |= icpwm->icselection;
		// reset the CH0CAPFLT bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0CAPFLT;
		// set the CH0CAPFLT bit
		TIMER_CHCTL0(timer_periph) |= icpwm->icfilter << 4;
		// set the CH0EN bit
		TIMER_CHCTL2(timer_periph) |= TIMER_CHCTL2_CH0EN;

		timer_channel_input_capture_prescaler_config(
				timer_periph, TIMER_CH_0, icpwm->icprescaler);

		// reset the CH1EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH1EN;
		// reset the CH1P and CH1NP bits
		TIMER_CHCTL2(timer_periph) &=
			~(TIMER_CHCTL2_CH1P | TIMER_CHCTL2_CH1NP);
		// set the CH1P and CH1NP bits
		TIMER_CHCTL2(timer_periph) |= icpolarity << 4;
		// reset the CH1MS bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1MS;
		// set the CH1MS bit
		TIMER_CHCTL0(timer_periph) |= icselection << 8;
		// reset the CH1CAPFLT bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1CAPFLT;
		// set the CH1CAPFLT bit
		TIMER_CHCTL0(timer_periph) |= icpwm->icfilter << 12;
		// set the CH1EN bit
		TIMER_CHCTL2(timer_periph) |= TIMER_CHCTL2_CH1EN;

		timer_channel_input_capture_prescaler_config(
				timer_periph, TIMER_CH_1, icpwm->icprescaler);
	} else {
		// reset the CH1EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH1EN;
		// reset the CH1P and CH1NP bits
		TIMER_CHCTL2(timer_periph) &=
		    ~(TIMER_CHCTL2_CH1P | TIMER_CHCTL2_CH1NP);
		// set the CH1P and CH1NP bits
		TIMER_CHCTL2(timer_periph) |= icpwm->icpolarity << 4;
		// reset the CH1MS bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1MS;
		// set the CH1MS bit
		TIMER_CHCTL0(timer_periph) |= icpwm->icselection << 8;
		// reset the CH1CAPFLT bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1CAPFLT;
		// set the CH1CAPFLT bit
		TIMER_CHCTL0(timer_periph) |= icpwm->icfilter << 12;
		// set the CH1EN bit
		TIMER_CHCTL2(timer_periph) |= TIMER_CHCTL2_CH1EN;

		timer_channel_input_capture_prescaler_config(
				timer_periph, TIMER_CH_1, icpwm->icprescaler);

		// reset the CH0EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0EN;
		// reset the CH0P and CH0NP bits
		TIMER_CHCTL2(timer_periph) &=
		    ~(TIMER_CHCTL2_CH0P | TIMER_CHCTL2_CH0NP);
		// set the CH0P and CH0NP bits
		TIMER_CHCTL2(timer_periph) |= icpolarity;
		// reset the CH0MS bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0MS;
		// set the CH0MS bit
		TIMER_CHCTL0(timer_periph) |= icselection;
		// reset the CH0CAPFLT bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0CAPFLT;
		// set the CH0CAPFLT bit
		TIMER_CHCTL0(timer_periph) |= icpwm->icfilter << 4;
		TIMER_CHCTL2(timer_periph) |= TIMER_CHCTL2_CH0EN;

		timer_channel_input_capture_prescaler_config(
				timer_periph, TIMER_CH_0, icpwm->icprescaler);
	}
}

/*
 * hallmode:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_HALLINTERFACE_ENABLE: TIMER hall sensor mode enable
 * 		TIMER_HALLINTERFACE_DISABLE: TIMER hall sensor mode disable
 */
void timer_hall_mode_config(uint32_t timer_periph, uint32_t hallmode) {
	if (hallmode == TIMER_HALLINTERFACE_ENABLE) {
		TIMER_CTL1(timer_periph) |= TIMER_CTL1_TI0S;
	} else if (hallmode == TIMER_HALLINTERFACE_DISABLE) {
		TIMER_CTL1(timer_periph) &= ~TIMER_CTL1_TI0S;
	} else {
		// illegal parameters
	}
}

/*
 * intrigger:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_SMCFG_TRGSEL_ITI0:
 * 			internal trigger 0(TIMERx(x=0..4))
 * 		TIMER_SMCFG_TRGSEL_ITI1:
 * 			internal trigger 1(TIMERx(x=0..4))
 * 		TIMER_SMCFG_TRGSEL_ITI2:
 * 			internal trigger 2(TIMERx(x=0..4))
 * 		TIMER_SMCFG_TRGSEL_ITI3:
 * 			internal trigger 3(TIMERx(x=0..4))
 * 		TIMER_SMCFG_TRGSEL_CI0F_ED:
 * 			TI0 edge detector(TIMERx(x=0..4))
 * 		TIMER_SMCFG_TRGSEL_CI0FE0:
 * 			filtered TIMER input 0(TIMERx(x=0..4))
 * 		TIMER_SMCFG_TRGSEL_CI1FE1:
 * 			filtered TIMER input 1(TIMERx(x=0..4))
 * 		TIMER_SMCFG_TRGSEL_ETIFP:
 * 			filtered external trigger input(TIMERx(x=0..4))
 */
void timer_input_trigger_source_select(uint32_t timer_periph,
		uint32_t intrigger) {

	TIMER_SMCFG(timer_periph) &= ~TIMER_SMCFG_TRGS;
	TIMER_SMCFG(timer_periph) |= intrigger;
}

/*
 * timer_periph: TIMERx(x=0..6)
 * outrigger:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_TRI_OUT_SRC_RESET:
 * 			the UPG bit as trigger output(TIMERx(x=0..6))
 * 		TIMER_TRI_OUT_SRC_ENABLE:
 * 			the counter enable signal TIMER_CTL0_CEN as trigger
 * 			output(TIMERx(x=0..6))
 * 		TIMER_TRI_OUT_SRC_UPDATE:
 * 			update event as trigger output(TIMERx(x=0..6))
 * 		TIMER_TRI_OUT_SRC_CH0:
 * 			a capture or a compare match occurred in channel 0 as
 * 			trigger output TRGO(TIMERx(x=0..4))
 * 		TIMER_TRI_OUT_SRC_O0CPRE:
 * 			O0CPRE as trigger output(TIMERx(x=0..4))
 * 		TIMER_TRI_OUT_SRC_O1CPRE:
 * 			O1CPRE as trigger output(TIMERx(x=0..4))
 * 		TIMER_TRI_OUT_SRC_O2CPRE:
 * 			O2CPRE as trigger output(TIMERx(x=0..4))
 * 		TIMER_TRI_OUT_SRC_O3CPRE:
 * 			O3CPRE as trigger output(TIMERx(x=0..4))
 */
void timer_master_output_trigger_source_select(uint32_t timer_periph,
		uint32_t outrigger) {

	TIMER_CTL1(timer_periph) &= ~TIMER_CTL1_MMC;
	TIMER_CTL1(timer_periph) |= outrigger;
}

/*
 * slavemode:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_SLAVE_MODE_DISABLE: slave mode disable
 * 		TIMER_ENCODER_MODE0: encoder mode 0
 * 		TIMER_ENCODER_MODE1: encoder mode 1
 * 		TIMER_ENCODER_MODE2: encoder mode 2
 * 		TIMER_SLAVE_MODE_RESTART: restart mode
 * 		TIMER_SLAVE_MODE_PAUSE: pause mode
 * 		TIMER_SLAVE_MODE_EVENT: event mode
 * 		TIMER_SLAVE_MODE_EXTERNAL0: external clock mode 0
 */

void timer_slave_mode_select(uint32_t timer_periph, uint32_t slavemode) {
	TIMER_SMCFG(timer_periph) &= ~TIMER_SMCFG_SMC;
	TIMER_SMCFG(timer_periph) |= slavemode;
}

/*
 * masterslave:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_MASTER_SLAVE_MODE_ENABLE: master slave mode enable
 * 		TIMER_MASTER_SLAVE_MODE_DISABLE: master slave mode disable
 */
void timer_master_slave_mode_config(uint32_t timer_periph,
		uint32_t masterslave) {

	if (masterslave == TIMER_MASTER_SLAVE_MODE_ENABLE) {
		TIMER_SMCFG(timer_periph) |= TIMER_SMCFG_MSM;
	} else if (masterslave == TIMER_MASTER_SLAVE_MODE_DISABLE) {
		TIMER_SMCFG(timer_periph) &= ~TIMER_SMCFG_MSM;
	} else {
		// illegal parameters
	}
}

/*
 * configure TIMER external trigger input
 * timer_periph: TIMERx(x=0..4)
 * extprescaler:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_EXT_TRI_PSC_OFF: no divided
 * 		TIMER_EXT_TRI_PSC_DIV2: divided by 2
 * 		TIMER_EXT_TRI_PSC_DIV4: divided by 4
 * 		TIMER_EXT_TRI_PSC_DIV8: divided by 8
 * extpolarity:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_ETP_FALLING: active low or falling edge active
 * 		TIMER_ETP_RISING: active high or rising edge active
 * extfilter:
 * 	a value between 0 and 15
 */
void timer_external_trigger_config(uint32_t timer_periph,
		uint32_t extprescaler, uint32_t extpolarity,
		uint32_t extfilter) {

	TIMER_SMCFG(timer_periph) &=
		~(TIMER_SMCFG_ETP | TIMER_SMCFG_ETPSC | TIMER_SMCFG_ETFC);

	TIMER_SMCFG(timer_periph) |= extprescaler | extpolarity;
	TIMER_SMCFG(timer_periph) |= extfilter << 8;
}

/*
 * decomode:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_ENCODER_MODE0:
 * 			counter counts on CI0FE0 edge depending on CI1FE1
 * 			level
 * 		TIMER_ENCODER_MODE1:
 * 			counter counts on CI1FE1 edge depending on CI0FE0 level
 * 		TIMER_ENCODER_MODE2:
 * 			counter counts on both CI0FE0 and CI1FE1 edges
 * 			depending on the level of the other input
 * ic0polarity:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_IC_POLARITY_RISING: capture rising edge
 * 		TIMER_IC_POLARITY_FALLING: capture falling edge
 *
 * ic1polarity:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_IC_POLARITY_RISING: capture rising edge
 * 		TIMER_IC_POLARITY_FALLING: capture falling edge
 */
void timer_quadrature_decoder_mode_config(uint32_t timer_periph,
		uint32_t decomode, uint16_t ic0polarity, uint16_t ic1polarity) {

	// configure the quadrature decoder mode
	TIMER_SMCFG(timer_periph) &= ~TIMER_SMCFG_SMC;
	TIMER_SMCFG(timer_periph) |= decomode;
	// configure input capture selection
	TIMER_CHCTL0(timer_periph) &=
		~TIMER_CHCTL0_CH0MS & ~TIMER_CHCTL0_CH1MS;
	TIMER_CHCTL0(timer_periph) |=
		TIMER_IC_SELECTION_DIRECTTI |
		(TIMER_IC_SELECTION_DIRECTTI << 8);
	// configure channel input capture polarity
	TIMER_CHCTL2(timer_periph) &=
	    ~(TIMER_CHCTL2_CH0P | TIMER_CHCTL2_CH0NP);
	TIMER_CHCTL2(timer_periph) &=
	    ~(TIMER_CHCTL2_CH1P | TIMER_CHCTL2_CH1NP);
	TIMER_CHCTL2(timer_periph) |=
	    ic0polarity | (ic1polarity << 4);
}

void timer_internal_clock_config(uint32_t timer_periph) {
	TIMER_SMCFG(timer_periph) &= ~(uint32_t) TIMER_SMCFG_SMC;
}

/*
 * intrigger:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_SMCFG_TRGSEL_ITI0: internal trigger 0
 * 		TIMER_SMCFG_TRGSEL_ITI1: internal trigger 1
 * 		TIMER_SMCFG_TRGSEL_ITI2: internal trigger 2
 * 		TIMER_SMCFG_TRGSEL_ITI3: internal trigger 3
 */
void timer_internal_trigger_as_external_clock_config(uint32_t timer_periph,
		uint32_t intrigger) {
	timer_input_trigger_source_select(timer_periph, intrigger);
	TIMER_SMCFG(timer_periph) &= ~TIMER_SMCFG_SMC;
	TIMER_SMCFG(timer_periph) |= TIMER_SLAVE_MODE_EXTERNAL0;
}

/*
 * extrigger:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_SMCFG_TRGSEL_CI0F_ED: TI0 edge detector
 * 		TIMER_SMCFG_TRGSEL_CI0FE0: filtered TIMER input 0
 * 		TIMER_SMCFG_TRGSEL_CI1FE1: filtered TIMER input 1
 *
 * extpolarity:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_IC_POLARITY_RISING: active low or falling edge active
 * 		TIMER_IC_POLARITY_FALLING: active high or rising edge active
 */
void timer_external_trigger_as_external_clock_config(uint32_t timer_periph,
		uint32_t extrigger, uint16_t extpolarity, uint32_t extfilter) {
	if (extrigger == TIMER_SMCFG_TRGSEL_CI1FE1) {
		// reset the CH1EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH1EN;
		// reset the CH1NP bit
		TIMER_CHCTL2(timer_periph) &=
			~(TIMER_CHCTL2_CH1P | TIMER_CHCTL2_CH1NP);
		// set the CH1NP bit
		TIMER_CHCTL2(timer_periph) |= extpolarity << 4;
		// reset the CH1MS bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1MS;
		// set the CH1MS bit
		TIMER_CHCTL0(timer_periph) |= TIMER_IC_SELECTION_DIRECTTI << 8;
		// reset the CH1CAPFLT bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH1CAPFLT;
		// set the CH1CAPFLT bit
		TIMER_CHCTL0(timer_periph) |= extfilter << 12;
		// set the CH1EN bit
		TIMER_CHCTL2(timer_periph) |= TIMER_CHCTL2_CH1EN;
	} else {
		// reset the CH0EN bit
		TIMER_CHCTL2(timer_periph) &= ~TIMER_CHCTL2_CH0EN;
		// reset the CH0P and CH0NP bits
		TIMER_CHCTL2(timer_periph) &=
			~(TIMER_CHCTL2_CH0P | TIMER_CHCTL2_CH0NP);
		// set the CH0P and CH0NP bits
		TIMER_CHCTL2(timer_periph) |= extpolarity;
		// reset the CH0MS bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0MS;
		// set the CH0MS bit
		TIMER_CHCTL0(timer_periph) |= TIMER_IC_SELECTION_DIRECTTI;
		// reset the CH0CAPFLT bit
		TIMER_CHCTL0(timer_periph) &= ~TIMER_CHCTL0_CH0CAPFLT;
		// reset the CH0CAPFLT bit
		TIMER_CHCTL0(timer_periph) |= extfilter << 4;
		// set the CH0EN bit
		TIMER_CHCTL2(timer_periph) |= TIMER_CHCTL2_CH0EN;
	}
	// select TIMER input trigger source
	timer_input_trigger_source_select(timer_periph, extrigger);
	// reset the SMC bit
	TIMER_SMCFG(timer_periph) &= ~TIMER_SMCFG_SMC;
	// set the SMC bit
	TIMER_SMCFG(timer_periph) |= TIMER_SLAVE_MODE_EXTERNAL0;
}

/*
 * configure TIMER the external clock mode0
 * extprescaler:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_EXT_TRI_PSC_OFF: no divided
 * 		TIMER_EXT_TRI_PSC_DIV2: divided by 2
 * 		TIMER_EXT_TRI_PSC_DIV4: divided by 4
 * 		TIMER_EXT_TRI_PSC_DIV8: divided by 8
 *
 * extpolarity:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_ETP_FALLING: active low or falling edge active
 * 		TIMER_ETP_RISING: active high or rising edge active
 *
 * extfilter: a value between 0 and 15
 */
void timer_external_clock_mode0_config(uint32_t timer_periph,
		uint32_t extprescaler, uint32_t extpolarity,
		uint32_t extfilter) {
	// configure TIMER external trigger input
	timer_external_trigger_config(timer_periph, extprescaler, extpolarity,
			extfilter);

	// reset the SMC bit,TRGS bit
	TIMER_SMCFG(timer_periph) &= ~(TIMER_SMCFG_SMC | TIMER_SMCFG_TRGS);
	// set the SMC bit,TRGS bit
	TIMER_SMCFG(timer_periph) |=
		TIMER_SLAVE_MODE_EXTERNAL0 | TIMER_SMCFG_TRGSEL_ETIFP;
}

/*
 * extprescaler:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_EXT_TRI_PSC_OFF: no divided
 * 		TIMER_EXT_TRI_PSC_DIV2: divided by 2
 * 		TIMER_EXT_TRI_PSC_DIV4: divided by 4
 * 		TIMER_EXT_TRI_PSC_DIV8: divided by 8
 *
 * extpolarity:
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_ETP_FALLING: active low or falling edge active
 * 		TIMER_ETP_RISING: active high or rising edge active
 *
 * extfilter: a value between 0 and 15
 */
void timer_external_clock_mode1_config(uint32_t timer_periph,
		uint32_t extprescaler, uint32_t extpolarity,
		uint32_t extfilter) {

	// configure TIMER external trigger input
	timer_external_trigger_config(timer_periph, extprescaler, extpolarity,
			extfilter);

	TIMER_SMCFG(timer_periph) |= TIMER_SMCFG_SMC1;
}

void timer_external_clock_mode1_disable(uint32_t timer_periph) {
	TIMER_SMCFG(timer_periph) &= ~TIMER_SMCFG_SMC1;
}

/*
 * interrupt: specify which interrupt to enable
 * 	one or more parameters can be selected which are shown as below:
 * 		TIMER_INT_UP: update interrupt enable, TIMERx(x=0..6)
 * 		TIMER_INT_CH0: channel 0 interrupt enable, TIMERx(x=0..4)
 * 		TIMER_INT_CH1: channel 1 interrupt enable, TIMERx(x=0..4)
 * 		TIMER_INT_CH2: channel 2 interrupt enable, TIMERx(x=0..4)
 * 		TIMER_INT_CH3: channel 3 interrupt enable, TIMERx(x=0..4)
 * 		TIMER_INT_CMT: commutation interrupt enable, TIMERx(x=0)
 * 		TIMER_INT_TRG: trigger interrupt enable, TIMERx(x=0..4)
 * 		TIMER_INT_BRK: break interrupt enable, TIMERx(x=0)
 */
void timer_interrupt_enable(uint32_t timer_periph, uint32_t interrupt) {
	TIMER_DMAINTEN(timer_periph) |= interrupt;
}

void timer_interrupt_disable(uint32_t timer_periph, uint32_t interrupt) {
	TIMER_DMAINTEN(timer_periph) &= ~interrupt;
}

enum flag_status timer_interrupt_flag_get(uint32_t timer_periph,
		uint32_t interrupt) {
	if ((TIMER_INTF(timer_periph) & interrupt) != RESET &&
			(TIMER_DMAINTEN(timer_periph) & interrupt) != RESET)
		return SET;
	else
		return RESET;
}

void timer_interrupt_flag_clear(uint32_t timer_periph, uint32_t interrupt) {
	TIMER_INTF(timer_periph) = ~interrupt;
}

/*
 * flag: the timer interrupt flags
 * 	only one parameter can be selected which is shown as below:
 * 		TIMER_FLAG_UP: update flag, TIMERx(x=0..6)
 * 		TIMER_FLAG_CH0: channel 0 flag, TIMERx(x=0..4)
 * 		TIMER_FLAG_CH1: channel 1 flag, TIMERx(x=0..4)
 * 		TIMER_FLAG_CH2: channel 2 flag, TIMERx(x=0..4)
 * 		TIMER_FLAG_CH3: channel 3 flag, TIMERx(x=0..4)
 * 		TIMER_FLAG_CMT: channel commutation flag, TIMERx(x=0)
 * 		TIMER_FLAG_TRG: trigger flag, TIMERx(x=0..4)
 * 		TIMER_FLAG_BRK: break flag, TIMERx(x=0)
 * 		TIMER_FLAG_CH0O: channel 0 overcapture flag, TIMERx(x=0..4)
 * 		TIMER_FLAG_CH1O: channel 1 overcapture flag, TIMERx(x=0..4)
 * 		TIMER_FLAG_CH2O: channel 2 overcapture flag, TIMERx(x=0..4)
 * 		TIMER_FLAG_CH3O: channel 3 overcapture flag, TIMERx(x=0..4)
 */
enum flag_status timer_flag_get(uint32_t timer_periph, uint32_t flag) {
	if ((TIMER_INTF(timer_periph) & flag) != RESET)
		return SET;
	else
		return RESET;
}

void timer_flag_clear(uint32_t timer_periph, uint32_t flag) {
	TIMER_INTF(timer_periph) = ~flag;
}
