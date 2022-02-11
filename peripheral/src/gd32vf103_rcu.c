#include "gd32vf103_rcu.h"

#define SEL_IRC8M			((uint16_t) 0U)
#define SEL_HXTAL			((uint16_t) 1U)
#define SEL_PLL				((uint16_t) 2U)

// define startup timeout count
#define OSC_STARTUP_TIMEOUT		((uint32_t) 0xFFFFFU)
#define LXTAL_STARTUP_TIMEOUT		((uint32_t) 0x3FFFFFFU)

void rcu_deinit() {
	// enable IRC8M
	RCU_CTL |= RCU_CTL_IRC8MEN;
	rcu_osci_stab_wait(RCU_IRC8M);

	// reset CFG0 register
	RCU_CFG0 &= ~(RCU_CFG0_SCS | RCU_CFG0_AHBPSC | RCU_CFG0_APB1PSC |
			RCU_CFG0_APB2PSC | RCU_CFG0_ADCPSC | RCU_CFG0_PLLSEL |
			RCU_CFG0_PREDV0_LSB | RCU_CFG0_PLLMF |
			RCU_CFG0_USBFSPSC | RCU_CFG0_CKOUT0SEL |
			RCU_CFG0_ADCPSC_2 | RCU_CFG0_PLLMF_4);

	// reset CTL register
	RCU_CTL &= ~(RCU_CTL_HXTALEN | RCU_CTL_CKMEN | RCU_CTL_PLLEN);
	RCU_CTL &= ~RCU_CTL_HXTALBPS;
	RCU_CTL &= ~(RCU_CTL_PLL1EN | RCU_CTL_PLL2EN);

	// reset INT and CFG1 register
	RCU_INT = 0x00ff0000U;
	RCU_CFG1 &= ~(RCU_CFG1_PREDV0 | RCU_CFG1_PREDV1 | RCU_CFG1_PLL1MF |
			RCU_CFG1_PLL2MF | RCU_CFG1_PREDV0SEL |
			RCU_CFG1_I2S1SEL | RCU_CFG1_I2S2SEL);
}

void rcu_periph_clock_enable(enum rcu_periph periph) {
	RCU_REG_VAL(periph) |= BIT(RCU_BIT_POS(periph));
}

void rcu_periph_clock_disable(enum rcu_periph periph) {
	RCU_REG_VAL(periph) &= ~BIT(RCU_BIT_POS(periph));
}

void rcu_periph_clock_sleep_enable(enum rcu_periph_sleep periph) {
	RCU_REG_VAL(periph) |= BIT(RCU_BIT_POS(periph));
}

void rcu_periph_clock_sleep_disable(enum rcu_periph_sleep periph) {
	RCU_REG_VAL(periph) &= ~BIT(RCU_BIT_POS(periph));
}

void rcu_periph_reset_enable(enum rcu_periph_reset periph_reset) {
	RCU_REG_VAL(periph_reset) |= BIT(RCU_BIT_POS(periph_reset));
}

void rcu_periph_reset_disable(enum rcu_periph_reset periph_reset) {
	RCU_REG_VAL(periph_reset) &= ~BIT(RCU_BIT_POS(periph_reset));
}

void rcu_bkp_reset_enable() {
	RCU_BDCTL |= RCU_BDCTL_BKPRST;
}

void rcu_bkp_reset_disable() {
	RCU_BDCTL &= ~RCU_BDCTL_BKPRST;
}

/*
 * ck_sys: system clock source select
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_CKSYSSRC_IRC8M: select CK_IRC8M as the CK_SYS source
 * 		RCU_CKSYSSRC_HXTAL: select CK_HXTAL as the CK_SYS source
 * 		RCU_CKSYSSRC_PLL: select CK_PLL as the CK_SYS source
 */
void rcu_system_clock_source_config(uint32_t ck_sys) {
	uint32_t reg = RCU_CFG0;
	// reset the SCS bits and set according to ck_sys
	reg &= ~RCU_CFG0_SCS;
	RCU_CFG0 = reg | ck_sys;
}

/* retval: which clock is selected as CK_SYS source
 * 	RCU_SCSS_IRC8M: CK_IRC8M is selected as the CK_SYS source
 * 	RCU_SCSS_HXTAL: CK_HXTAL is selected as the CK_SYS source
 * 	RCU_SCSS_PLL: CK_PLL is selected as the CK_SYS source
 */
uint32_t rcu_system_clock_source_get() {
	return RCU_CFG0 & RCU_CFG0_SCSS;
}

/*
 * configure the AHB clock prescaler selection
 * ck_ahb: AHB clock prescaler selection
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_AHB_CKSYS_DIVx, x=1, 2, 4, 8, 16, 64, 128, 256, 512
 */
void rcu_ahb_clock_config(uint32_t ck_ahb) {
	uint32_t reg = RCU_CFG0;
	// reset the AHBPSC bits and set according to ck_ahb
	reg &= ~RCU_CFG0_AHBPSC;
	RCU_CFG0 = reg | ck_ahb;
}

/*
 * configure the APB1 clock prescaler selection
 * ck_apb1: APB1 clock prescaler selection
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_APB1_CKAHB_DIV1: select CK_AHB as CK_APB1
 * 		RCU_APB1_CKAHB_DIV2: select CK_AHB/2 as CK_APB1
 * 		RCU_APB1_CKAHB_DIV4: select CK_AHB/4 as CK_APB1
 * 		RCU_APB1_CKAHB_DIV8: select CK_AHB/8 as CK_APB1
 * 		RCU_APB1_CKAHB_DIV16: select CK_AHB/16 as CK_APB1
 */
void rcu_apb1_clock_config(uint32_t ck_apb1) {
	uint32_t reg = RCU_CFG0;
	// reset the APB1PSC and set according to ck_apb1
	reg &= ~RCU_CFG0_APB1PSC;
	RCU_CFG0 = reg | ck_apb1;
}

void rcu_apb2_clock_config(uint32_t ck_apb2) {
	uint32_t reg = RCU_CFG0;
	// reset the APB2PSC and set according to ck_apb2
	reg &= ~RCU_CFG0_APB2PSC;
	RCU_CFG0 = reg | ck_apb2;
}

/*
 * configure the CK_OUT0 clock source
 * ckout0_src: CK_OUT0 clock source selection
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_CKOUT0SRC_NONE: no clock
 * 		RCU_CKOUT0SRC_CKSYS: system clock
 * 		RCU_CKOUT0SRC_IRC8M: high speed 8M internal oscillator clock
 * 		RCU_CKOUT0SRC_HXTAL: HXTAL
 * 		RCU_CKOUT0SRC_CKPLL_DIV2: CK_PLL/2
 * 		RCU_CKOUT0SRC_CKPLL1: CK_PLL1
 * 		RCU_CKOUT0SRC_CKPLL2_DIV2: CK_PLL2/2
 * 		RCU_CKOUT0SRC_EXT1: EXT1
 * 		RCU_CKOUT0SRC_CKPLL2: PLL2
 */
void rcu_ckout0_config(uint32_t ckout0_src) {
	uint32_t reg = RCU_CFG0;
	// reset the CKOUT0SRC, set according to ckout0_src
	reg &= ~RCU_CFG0_CKOUT0SEL;
	RCU_CFG0 = reg | ckout0_src;
}

/*
 * configure the main PLL clock
 * pll_src: PLL clock source selection
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_PLLSRC_IRC8M_DIV2:
 * 			IRC8M/2 clock selected as source clock of PLL
 * 		RCU_PLLSRC_HXTAL:
 * 			HXTAL selected as source clock of PLL
 * pll_mul: PLL clock multiplication factor
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_PLL_MULx (x = 2..14, 6.5, 16..32)
 */
void rcu_pll_config(uint32_t pll_src, uint32_t pll_mul) {
	uint32_t reg = RCU_CFG0;
	// PLL clock source and multiplication factor configuration
	reg &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
	reg |= pll_src | pll_mul;
	RCU_CFG0 = reg;
}

/*
 * configure the PREDV0 division factor and clock source
 * predv0_source: PREDV0 input clock source selection
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_PREDV0SRC_HXTAL:
 * 			HXTAL selected as PREDV0 input source clock
 * 		RCU_PREDV0SRC_CKPLL1:
 * 			CK_PLL1 selected as PREDV0 input source clock
 * predv0_div: PREDV0 division factor
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_PREDV0_DIVx, x = 1..16
 */
void rcu_predv0_config(uint32_t predv0_source, uint32_t predv0_div) {
	uint32_t reg = RCU_CFG1;
	// reset PREDV0SEL and PREDV0 bits
	reg &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV0);
	// set the PREDV0SEL and PREDV0 division factor
	reg |= (predv0_source | predv0_div);
	RCU_CFG1 = reg;
}

/*
 * configure the PREDV1 division factor
 * predv1_div: PREDV1 division factor
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_PREDV1_DIVx, x = 1..16
 */
void rcu_predv1_config(uint32_t predv1_div) {
	uint32_t reg = RCU_CFG1;
	// reset the PREDV1 bits
	reg &= ~RCU_CFG1_PREDV1;
	// set the PREDV1 division factor
	reg |= predv1_div;
	RCU_CFG1 = reg;
}

/*
 * configure the PLL1 clock
 * pll_mul: PLL clock multiplication factor
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_PLL1_MULx (x = 8..16, 20)
 */
void rcu_pll1_config(uint32_t pll_mul) {
	RCU_CFG1 &= ~RCU_CFG1_PLL1MF;
	RCU_CFG1 |= pll_mul;
}

void rcu_pll2_config(uint32_t pll_mul) {
	RCU_CFG1 &= ~RCU_CFG1_PLL2MF;
	RCU_CFG1 |= pll_mul;
}

/*
 * configure the ADC prescaler factor
 * adc_psc: ADC prescaler factor
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_CKADC_CKAPB2_DIV2: ADC prescaler select CK_APB2/2
 * 		RCU_CKADC_CKAPB2_DIV4: ADC prescaler select CK_APB2/4
 * 		RCU_CKADC_CKAPB2_DIV6: ADC prescaler select CK_APB2/6
 * 		RCU_CKADC_CKAPB2_DIV8: ADC prescaler select CK_APB2/8
 * 		RCU_CKADC_CKAPB2_DIV12: ADC prescaler select CK_APB2/12
 * 		RCU_CKADC_CKAPB2_DIV16: ADC prescaler select CK_APB2/16
 */
void rcu_adc_clock_config(uint32_t adc_psc) {
	// reset the ADCPSC bits
	uint32_t reg0 = RCU_CFG0;
	reg0 &= ~(RCU_CFG0_ADCPSC_2 | RCU_CFG0_ADCPSC);

	// set the ADC prescaler factor
	switch (adc_psc) {
	case RCU_CKADC_CKAPB2_DIV2:
	case RCU_CKADC_CKAPB2_DIV4:
	case RCU_CKADC_CKAPB2_DIV6:
	case RCU_CKADC_CKAPB2_DIV8:
		reg0 |= adc_psc << 14;
		break;

	case RCU_CKADC_CKAPB2_DIV12:
	case RCU_CKADC_CKAPB2_DIV16:
		adc_psc &= ~BIT(2);
		reg0 |= adc_psc << 14 | RCU_CFG0_ADCPSC_2;
		break;

	default:
		break;
	}

	// set the register
	RCU_CFG0 = reg0;
}

/*
 * configure the USBFS prescaler factor
 * usb_psc: USB prescaler factor
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_CKUSB_CKPLL_DIV1_5: USBFS prescaler select CK_PLL/1.5
 * 		RCU_CKUSB_CKPLL_DIV1: USBFS prescaler select CK_PLL/1
 * 		RCU_CKUSB_CKPLL_DIV2_5: USBFS prescaler select CK_PLL/2.5
 * 		RCU_CKUSB_CKPLL_DIV2: USBFS prescaler select CK_PLL/2
 */
void rcu_usb_clock_config(uint32_t usb_psc) {
	uint32_t reg = RCU_CFG0;
	// configure the USBFS prescaler factor
	reg &= ~RCU_CFG0_USBFSPSC;
	RCU_CFG0 = reg | usb_psc;
}

/*
 * configure the RTC clock source selection
 * rtc_clock_source: RTC clock source selection
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_RTCSRC_NONE: no clock
 * 		RCU_RTCSRC_LXTAL: CK_LXTAL as RTC source clock
 * 		RCU_RTCSRC_IRC40K: CK_IRC40K as RTC source clock
 * 		RCU_RTCSRC_HXTAL_DIV_128: CK_HXTAL/128 as RTC source clock
 */
void rcu_rtc_clock_config(uint32_t rtc_clock_source) {
	uint32_t reg = RCU_BDCTL;
	// reset the RTCSRC bits and set according to rtc_clock_source
	reg &= ~RCU_BDCTL_RTCSRC;
	RCU_BDCTL = reg | rtc_clock_source;
}

/*
 * configure the I2S1 clock source selection
 * i2s_clock_source: I2S1 clock source selection
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_I2S1SRC_CKSYS: System clock selected as I2S1 source clock
 * 		RCU_I2S1SRC_CKPLL2_MUL2: CK_PLL2x2 selected as I2S1 source clock
 */
void rcu_i2s1_clock_config(uint32_t i2s_clock_source) {
	uint32_t reg = RCU_CFG1;
	// reset the I2S1SEL bit and set according to i2s_clock_source
	reg &= ~RCU_CFG1_I2S1SEL;
	RCU_CFG1 = reg | i2s_clock_source;
}

/*
 * configure the I2S2 clock source selection
 * i2s_clock_source: I2S2 clock source selection
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_I2S2SRC_CKSYS: system clock selected as I2S2 source clock
 * 		RCU_I2S2SRC_CKPLL2_MUL2: CK_PLL2x2 selected as I2S2 source clock
 */
void rcu_i2s2_clock_config(uint32_t i2s_clock_source) {
	uint32_t reg = RCU_CFG1;
	// reset the I2S2SEL bit and set according to i2s_clock_source
	reg &= ~RCU_CFG1_I2S2SEL;
	RCU_CFG1 = reg | i2s_clock_source;
}

enum flag_status rcu_flag_get(enum rcu_flag flag) {
	if (RCU_REG_VAL(flag) & BIT(RCU_BIT_POS(flag)) != RESET)
		return SET;
	else
		return RESET;
}

void rcu_all_reset_flag_clear() {
	RCU_RSTSCK |= RCU_RSTSCK_RSTFC;
}

/* get the clock stabilization interrupt and ckm flags */
enum flag_status rcu_interrupt_flag_get(enum rcu_int_flag int_flag) {
	if (RCU_REG_VAL(int_flag) & BIT(RCU_BIT_POS(int_flag)) != RESET)
		return SET;
	else
		return RESET;
}

void rcu_interrupt_flag_clear(enum rcu_int_flag_clear int_flag_clear) {
	RCU_REG_VAL(int_flag_clear) |= BIT(RCU_BIT_POS(int_flag_clear));
}

/* enable the stabilization interrupt */
void rcu_interrupt_enable(enum rcu_int stab_int) {
	RCU_REG_VAL(stab_int) |= BIT(RCU_BIT_POS(stab_int));
}

/* disable the stabilization interrupt */
void rcu_interrupt_disable(enum rcu_int stab_int) {
	RCU_REG_VAL(stab_int) &= ~BIT(RCU_BIT_POS(stab_int));
}

static enum err_status osci_wait(enum rcu_flag flag, uint32_t timeout) {
	uint32_t stb_cnt = 0;
	enum flag_status osci_stat = RESET;

	while ((osci_stat == RESET) && (stb_cnt != timeout)) {
		osci_stat = rcu_flag_get(flag);
		stb_cnt++;
	}

	if (rcu_flag_get(flag) != RESET)
		return SUCCESS;
	else
		return ERROR;
}

/*
 * wait for oscillator stabilization flags is SET or oscillator startup
 * is timeout
 */
enum err_status rcu_osci_stab_wait(enum rcu_osci_type osci) {
	switch (osci) {
	case RCU_HXTAL:
		return osci_wait(RCU_FLAG_HXTALSTB, HXTAL_STARTUP_TIMEOUT);
	case RCU_LXTAL:
		return osci_wait(RCU_FLAG_LXTALSTB, LXTAL_STARTUP_TIMEOUT);
	case RCU_IRC8M:
		return osci_wait(RCU_FLAG_IRC8MSTB, IRC8M_STARTUP_TIMEOUT);
	case RCU_IRC40K:
		return osci_wait(RCU_FLAG_IRC40KSTB, OSC_STARTUP_TIMEOUT);
	case RCU_PLL_CK:
		return osci_wait(RCU_FLAG_PLLSTB, OSC_STARTUP_TIMEOUT);
	case RCU_PLL1_CK:
		return osci_wait(RCU_FLAG_PLL1STB, OSC_STARTUP_TIMEOUT);
	case RCU_PLL2_CK:
		return osci_wait(RCU_FLAG_PLL2STB, OSC_STARTUP_TIMEOUT);
	default:
		return ERROR;
	}
}

void rcu_osci_on(enum rcu_osci_type osci) {
	RCU_REG_VAL(osci) |= BIT(RCU_BIT_POS(osci));
}

void rcu_osci_off(enum rcu_osci_type osci) {
	RCU_REG_VAL(osci) &= ~BIT(RCU_BIT_POS(osci));
}

void rcu_osci_bypass_mode_enable(enum rcu_osci_type osci) {
	switch (osci) {
	case RCU_HXTAL:
		uint32_t reg = RCU_CTL;
		RCU_CTL &= ~RCU_CTL_HXTALEN;
		RCU_CTL = reg | RCU_CTL_HXTALBPS;
		break;
	case RCU_LXTAL:
		uint32_t reg = RCU_BDCTL;
		RCU_BDCTL &= ~RCU_BDCTL_LXTALEN;
		RCU_BDCTL = reg | RCU_BDCTL_LXTALBPS;
		break;
	case RCU_IRC8M:
	case RCU_IRC40K:
	case RCU_PLL_CK:
	case RCU_PLL1_CK:
	case RCU_PLL2_CK:
		break;
	default:
		break;
	}
}

void rcu_osci_bypass_mode_disable(enum rcu_osci_type osci) {
	switch (osci) {
	case RCU_HXTAL:
		uint32_t reg = RCU_CTL;
		RCU_CTL &= ~RCU_CTL_HXTALEN;
		RCU_CTL = reg & ~RCU_CTL_HXTALBPS;
		break;
	case RCU_LXTAL:
		uint32_t reg = RCU_BDCTL;
		RCU_BDCTL &= ~RCU_BDCTL_LXTALEN;
		RCU_BDCTL = reg & ~RCU_BDCTL_LXTALBPS;
		break;
	case RCU_IRC8M:
	case RCU_IRC40K:
	case RCU_PLL_CK:
	case RCU_PLL1_CK:
	case RCU_PLL2_CK:
		break;
	default:
		break;
	}
}

void rcu_hxtal_clock_monitor_enable() {
	RCU_CTL |= RCU_CTL_CKMEN;
}

void rcu_hxtal_clock_monitor_disable() {
	RCU_CTL &= ~RCU_CTL_CKMEN;
}

/* irc8m_adjval: IRC8M adjust value, must be between 0 and 0x1F */
void rcu_irc8m_adjust_value_set(uint32_t irc8m_adjval) {
	uint32_t reg = RCU_CTL;
	// reset the IRC8MADJ bits and set according to irc8m_adjval
	reg &= ~RCU_CTL_IRC8MADJ;
	RCU_CTL = reg | ((irc8m_adjval & 0x1F) << 3);
}

/*
 * dsvol: deep sleep mode voltage
 * 	only one parameter can be selected which is shown as below:
 * 		RCU_DEEPSLEEP_V_1_2: the core voltage is 1.2V
 * 		RCU_DEEPSLEEP_V_1_1: the core voltage is 1.1V
 * 		RCU_DEEPSLEEP_V_1_0: the core voltage is 1.0V
 * 		RCU_DEEPSLEEP_V_0_9: the core voltage is 0.9V
 */
void rcu_deepsleep_voltage_set(uint32_t dsvol) {
	dsvol &= RCU_DSV_DSLPVS;
	RCU_DSV = dsvol;
}

/*
 * get the system clock, bus and peripheral clock frequency.
 * returns the clock frequency of system, AHB, APB1, APB2
 */
uint32_t rcu_clock_freq_get(enum rcu_clock_freq clock) {
	uint32_t cksys_freq, ahb_freq, apb1_freq, apb2_freq;
	uint32_t ck_freq = 0;

	// exponent of AHB, APB1 and APB2 clock divider
	uint8_t ahb_exp[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
	uint8_t apb1_exp[8] = {0, 0, 0, 0, 1, 2, 3, 4};
	uint8_t apb2_exp[8] = {0, 0, 0, 0, 1, 2, 3, 4};

	uint32_t sws = GET_BITS(RCU_CFG0, 2, 3);
	switch (sws) {
	case SEL_IRC8M:
		cksys_freq = IRC8M_VALUE;
		break;
	case SEL_HXTAL:
		cksys_freq = HXTAL_VALUE;
		break;
	case SEL_PLL:
		// PLL clock source selection, HXTAL or IRC8M/2
		uint32_t pllsel = (RCU_CFG0 & RCU_CFG0_PLLSEL);

		uint32_t ck_src;
		if (RCU_PLLSRC_HXTAL == pllsel) {
			// PLL clock source is HXTAL
			ck_src = HXTAL_VALUE;

			uint32_t predv0sel = (RCU_CFG1 & RCU_CFG1_PREDV0SEL);
			// source clock use PLL1
			if (RCU_PREDV0SRC_CKPLL1 == predv0sel) {
				uint32_t predv1 =
					((RCU_CFG1 & RCU_CFG1_PREDV1) >> 4) + 1;
				uint32_t pll1mf =
					((RCU_CFG1 & RCU_CFG1_PLL1MF) >> 8) + 2;

				if (pll1mf == 17)
					pll1mf = 20;

				ck_src = (ck_src / predv1) * pll1mf;
			}
			uint32_t predv0 = (RCU_CFG1 & RCU_CFG1_PREDV0) + 1;
			ck_src /= predv0;
		} else {
			// PLL clock source is IRC8M/2
			ck_src = IRC8M_VALUE / 2;
		}

		// PLL multiplication factor
		uint32_t pllmf = GET_BITS(RCU_CFG0, 18, 21);
		if (RCU_CFG0 & RCU_CFG0_PLLMF_4)
			pllmf |= 0x10;

		if (pllmf < 15)
			pllmf += 2U;
		else
			pllmf += 1U;

		cksys_freq = ck_src * pllmf;

		if (pllmf == 15)
			// PLL source clock multiply by 6.5
			cksys_freq = ck_src * 6U + ck_src / 2U;

		break;
	// IRC8M is selected as CK_SYS
	default:
		cksys_freq = IRC8M_VALUE;
		break;
	}

	// calculate AHB clock frequency
	uint32_t idx = GET_BITS(RCU_CFG0, 4, 7);
	uint32_t clk_exp = ahb_exp[idx];
	ahb_freq = cksys_freq >> clk_exp;

	// calculate APB1 clock frequency
	idx = GET_BITS(RCU_CFG0, 8, 10);
	clk_exp = apb1_exp[idx];
	apb1_freq = ahb_freq >> clk_exp;

	// calculate APB2 clock frequency
	idx = GET_BITS(RCU_CFG0, 11, 13);
	clk_exp = apb2_exp[idx];
	apb2_freq = ahb_freq >> clk_exp;

	// return the clocks frequency
	switch (clock) {
	case CK_SYS:
		ck_freq = cksys_freq;
		break;
	case CK_AHB:
		ck_freq = ahb_freq;
		break;
	case CK_APB1:
		ck_freq = apb1_freq;
		break;
	case CK_APB2:
		ck_freq = apb2_freq;
		break;
	default:
		break;
	}
	return ck_freq;
}
