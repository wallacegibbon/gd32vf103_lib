#include "gd32vf103.h"

//#define __SYSTEM_CLOCK_48M_PLL_IRC8M	((uint32_t) (48000000))
//#define __SYSTEM_CLOCK_72M_PLL_IRC8M	((uint32_t) (72000000))
//#define __SYSTEM_CLOCK_108M_PLL_IRC8M	((uint32_t) (108000000))
//#define __SYSTEM_CLOCK_HXTAL		(HXTAL_VALUE)
//#define __SYSTEM_CLOCK_24M_PLL_HXTAL	((uint32_t) (24000000))
//#define __SYSTEM_CLOCK_36M_PLL_HXTAL	((uint32_t) (36000000))
//#define __SYSTEM_CLOCK_48M_PLL_HXTAL	((uint32_t) (48000000))
//#define __SYSTEM_CLOCK_56M_PLL_HXTAL	((uint32_t) (56000000))
//#define __SYSTEM_CLOCK_72M_PLL_HXTAL	((uint32_t) (72000000))
//#define __SYSTEM_CLOCK_96M_PLL_HXTAL	((uint32_t) (96000000))
#define __SYSTEM_CLOCK_108M_PLL_HXTAL	((uint32_t) (108000000))


#ifdef __SYSTEM_CLOCK_48M_PLL_IRC8M
uint32_t sys_core_clk = __SYSTEM_CLOCK_48M_PLL_IRC8M;
#elif defined(__SYSTEM_CLOCK_72M_PLL_IRC8M)
uint32_t sys_core_clk = __SYSTEM_CLOCK_72M_PLL_IRC8M;
#elif defined(__SYSTEM_CLOCK_108M_PLL_IRC8M)
uint32_t sys_core_clk = __SYSTEM_CLOCK_108M_PLL_IRC8M;
#elif defined(__SYSTEM_CLOCK_HXTAL)
uint32_t sys_core_clk = __SYSTEM_CLOCK_HXTAL;
#elif defined(__SYSTEM_CLOCK_24M_PLL_HXTAL)
uint32_t sys_core_clk = __SYSTEM_CLOCK_24M_PLL_HXTAL;
#elif defined(__SYSTEM_CLOCK_36M_PLL_HXTAL)
uint32_t sys_core_clk = __SYSTEM_CLOCK_36M_PLL_HXTAL;
#elif defined(__SYSTEM_CLOCK_48M_PLL_HXTAL)
uint32_t sys_core_clk = __SYSTEM_CLOCK_48M_PLL_HXTAL;
#elif defined(__SYSTEM_CLOCK_56M_PLL_HXTAL)
uint32_t sys_core_clk = __SYSTEM_CLOCK_56M_PLL_HXTAL;
#elif defined(__SYSTEM_CLOCK_72M_PLL_HXTAL)
uint32_t sys_core_clk = __SYSTEM_CLOCK_72M_PLL_HXTAL;
#elif defined(__SYSTEM_CLOCK_96M_PLL_HXTAL)
uint32_t sys_core_clk = __SYSTEM_CLOCK_96M_PLL_HXTAL;
#elif defined(__SYSTEM_CLOCK_108M_PLL_HXTAL)
uint32_t sys_core_clk = __SYSTEM_CLOCK_108M_PLL_HXTAL;
#else
uint32_t sys_core_clk = IRC8M_VALUE;
#endif

// update the sys_core_clk with current core clock retrieved from cpu registers
void sys_core_clk_update() {
	// 2 for SEL_PLL, 1 for SEL_HXTAL, 0 for SEL_IRC8M
	uint32_t scss = GET_BITS(RCU_CFG0, 2, 3);

	// (notice: it should not be 3, maybe more checks should be put here)
	if (scss == 0 || scss == 3) {
		sys_core_clk = IRC8M_VALUE;
		return;
	}

	if (scss == 1) {
		sys_core_clk = HXTAL_VALUE;
		return;
	}

	// PLL clock source selection, HXTAL or IRC8M/2
	uint32_t pllsel = RCU_CFG0 & RCU_CFG0_PLLSEL;
	uint32_t ck_src;

	if (pllsel == RCU_PLLSRC_IRC8M_DIV2) {
		// PLL clock source is IRC8M/2
		ck_src = IRC8M_VALUE / 2;
	} else {
		// PLL clock source is HXTAL
		ck_src = HXTAL_VALUE;

		uint32_t predv0sel = (RCU_CFG1 & RCU_CFG1_PREDV0SEL);
		if (predv0sel == RCU_PREDV0SRC_CKPLL1) {
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
	}

	// PLL multiplication factor
	uint32_t pllmf = GET_BITS(RCU_CFG0, 18, 21);

	if (RCU_CFG0 & RCU_CFG0_PLLMF_4)
		pllmf |= 0x10;

	if (pllmf >= 15)
		pllmf += 1;
	else
		pllmf += 2;

	sys_core_clk = ck_src * pllmf;

	if (pllmf == 15)
		// PLL source clock multiply by 6.5
		sys_core_clk = ck_src * 6U + ck_src / 2U;
}

static inline void enable_pll() {
	RCU_CTL |= RCU_CTL_PLLEN;
	// wait until PLL is stable
	while (!(RCU_CTL & RCU_CTL_PLLSTB));
}

static inline void enable_pll1() {
	RCU_CTL |= RCU_CTL_PLL1EN;
	// wait till PLL1 is ready
	while (!(RCU_CTL & RCU_CTL_PLL1STB));
}

static inline void enable_pll2() {
	RCU_CTL |= RCU_CTL_PLL2EN;
	/* wait till PLL2 is ready */
	while (!(RCU_CTL & RCU_CTL_PLL2STB));
}

static void enable_hxtal() {
	uint32_t timeout = 0;
	uint32_t stab_flag = 0;

	RCU_CTL |= RCU_CTL_HXTALEN;

	// wait until HXTAL is stable or the startup time is longer than
	// HXTAL_STARTUP_TIMEOUT
	do {
		timeout++;
		stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
	} while (!stab_flag && (HXTAL_STARTUP_TIMEOUT != timeout));

	// if it fails, the system die
	if (!(RCU_CTL & RCU_CTL_HXTALSTB))
		while (1);
}


static void enable_irc8m() {
	uint32_t timeout = 0;
	uint32_t stab_flag = 0;

	RCU_CTL |= RCU_CTL_IRC8MEN;

	// wait until IRC8M is stable or the startup time is longer than
	// IRC8M_STARTUP_TIMEOUT
	do {
		timeout++;
		stab_flag = (RCU_CTL & RCU_CTL_IRC8MSTB);
	} while (!stab_flag && (IRC8M_STARTUP_TIMEOUT != timeout));

	// if it fails, the system die
	if (!(RCU_CTL & RCU_CTL_IRC8MSTB))
		while (1);
}


static inline xhtal_as_sys_clock() {
	// select HXTAL as system clock
	RCU_CFG0 &= ~RCU_CFG0_SCS;
	RCU_CFG0 |= RCU_CKSYSSRC_HXTAL;

	// wait until HXTAL is selected as system clock
	while (!(RCU_CFG0 & RCU_SCSS_HXTAL));
}

static inline pll_as_sys_clock() {
	// select PLL as system clock
	RCU_CFG0 &= ~RCU_CFG0_SCS;
	RCU_CFG0 |= RCU_CKSYSSRC_PLL;

	// wait until PLL is selected as system clock
	while (!(RCU_CFG0 & RCU_SCSS_PLL));
}

static inline pre_init_rcu() {
	// AHB = SYSCLK
	RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
	// APB2 = AHB/1
	RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
	// APB1 = AHB/2
	RCU_CFG0 |= RCU_APB1_CKAHB_DIV2;
}

#ifdef __SYSTEM_CLOCK_HXTAL

static void system_clock_hxtal() {
	enable_hxtal();
	pre_init_rcu();
	xhtal_as_sys_clock();
}

#elif defined(__SYSTEM_CLOCK_24M_PLL_HXTAL)

static void system_clock_24m_hxtal() {
	enable_hxtal();
	pre_init_rcu();

	/* CK_PLL = (CK_PREDIV0) * 6 = 24 MHz */
	RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
	RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL6);

	if (HXTAL_VALUE == 25000000) {
		/* CK_PREDIV0 = (CK_HXTAL)/5 *8 /10 = 4 MHz */
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PLL1MF |
				RCU_CFG1_PREDV1 | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_CKPLL1 | RCU_PLL1_MUL8 |
				RCU_PREDV1_DIV5 | RCU_PREDV0_DIV10);

		enable_pll1();

	} else if (HXTAL_VALUE == 8000000) {
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV1 |
				RCU_CFG1_PLL1MF | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_HXTAL | RCU_PREDV0_DIV2);
	}

	enable_pll();
	pll_as_sys_clock();
}

#elif defined(__SYSTEM_CLOCK_36M_PLL_HXTAL)

static void system_clock_36m_hxtal() {
	enable_hxtal();
	pre_init_rcu();

	/* CK_PLL = (CK_PREDIV0) * 9 = 36 MHz */
	RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
	RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL9);

	if (HXTAL_VALUE == 25000000) {
		/* CK_PREDIV0 = (CK_HXTAL)/5 *8 /10 = 4 MHz */
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PLL1MF |
				RCU_CFG1_PREDV1 | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_CKPLL1 | RCU_PLL1_MUL8 |
				RCU_PREDV1_DIV5 | RCU_PREDV0_DIV10);

		enable_pll1();

	} else if (HXTAL_VALUE == 8000000) {
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV1 |
				RCU_CFG1_PLL1MF | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_HXTAL | RCU_PREDV0_DIV2);
	}

	enable_pll();
	pll_as_sys_clock();
}

#elif defined(__SYSTEM_CLOCK_48M_PLL_HXTAL)

static void system_clock_48m_hxtal() {
	enable_hxtal();
	pre_init_rcu();

	/* CK_PLL = (CK_PREDIV0) * 12 = 48 MHz */
	RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
	RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL12);

	if (HXTAL_VALUE == 25000000) {

		/* CK_PREDIV0 = (CK_HXTAL)/5 *8 /10 = 4 MHz */
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PLL1MF |
				RCU_CFG1_PREDV1 | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_CKPLL1 | RCU_PLL1_MUL8 |
				RCU_PREDV1_DIV5 | RCU_PREDV0_DIV10);

		enable_pll1();

	} else if (HXTAL_VALUE == 8000000) {
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV1 |
				RCU_CFG1_PLL1MF | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_HXTAL | RCU_PREDV0_DIV2);
	}

	enable_pll();
	pll_as_sys_clock();
}

#elif defined(__SYSTEM_CLOCK_56M_PLL_HXTAL)

static void system_clock_56m_hxtal() {
	enable_hxtal();
	pre_init_rcu();

	/* CK_PLL = (CK_PREDIV0) * 14 = 56 MHz */
	RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
	RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL14);

	if (HXTAL_VALUE == 25000000) {

		/* CK_PREDIV0 = (CK_HXTAL)/5 *8 /10 = 4 MHz */
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PLL1MF |
				RCU_CFG1_PREDV1 | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_CKPLL1 | RCU_PLL1_MUL8 |
				RCU_PREDV1_DIV5 | RCU_PREDV0_DIV10);
		enable_pll1();

	} else if (HXTAL_VALUE == 8000000) {
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV1 |
				RCU_CFG1_PLL1MF | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_HXTAL | RCU_PREDV0_DIV2);
	}

	enable_pll();
	pll_as_sys_clock();
}

#elif defined(__SYSTEM_CLOCK_72M_PLL_HXTAL)

static void system_clock_72m_hxtal() {
	enable_hxtal();
	pre_init_rcu();

	/* CK_PLL = (CK_PREDIV0) * 18 = 72 MHz */
	RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
	RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL18);

	if (HXTAL_VALUE == 25000000) {

		/* CK_PREDIV0 = (CK_HXTAL)/5 *8 /10 = 4 MHz */
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PLL1MF |
				RCU_CFG1_PREDV1 | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_CKPLL1 | RCU_PLL1_MUL8 |
				RCU_PREDV1_DIV5 | RCU_PREDV0_DIV10);

		enable_pll1();

	} else if (HXTAL_VALUE == 8000000) {
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV1 |
				RCU_CFG1_PLL1MF | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_HXTAL | RCU_PREDV0_DIV2);
	}

	enable_pll();
	pll_as_sys_clock();
}

#elif defined(__SYSTEM_CLOCK_96M_PLL_HXTAL)

static void system_clock_96m_hxtal() {
	enable_hxtal();
	pre_init_rcu();

	/* CK_PLL = (CK_PREDIV0) * 24 = 96 MHz */
	RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
	RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL24);

	if (HXTAL_VALUE == 25000000) {
		/* CK_PREDIV0 = (CK_HXTAL)/5 *8 /10 = 4 MHz */
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PLL1MF |
				RCU_CFG1_PREDV1 | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_CKPLL1 | RCU_PLL1_MUL8 |
				RCU_PREDV1_DIV5 | RCU_PREDV0_DIV10);

		enable_pll1();

	} else if (HXTAL_VALUE == 8000000) {
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV1 |
				RCU_CFG1_PLL1MF | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_HXTAL | RCU_PREDV0_DIV2);
	}

	enable_pll();
	pll_as_sys_clock();
}

#elif defined(__SYSTEM_CLOCK_108M_PLL_HXTAL)

static void system_clock_108m_hxtal() {
	enable_hxtal();
	pre_init_rcu();

	/* CK_PLL = (CK_PREDIV0) * 27 = 108 MHz */
	RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
	RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_PLL_MUL27);

	if (HXTAL_VALUE == 25000000) {
		/* CK_PREDIV0 = (CK_HXTAL)/5 *8 /10 = 4 MHz */
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV1 |
				RCU_CFG1_PLL1MF | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_CKPLL1 | RCU_PREDV1_DIV5 |
				RCU_PLL1_MUL8 | RCU_PREDV0_DIV10);

		enable_pll1();
		enable_pll2();

	} else if (HXTAL_VALUE == 8000000) {
		RCU_CFG1 &= ~(RCU_CFG1_PREDV0SEL | RCU_CFG1_PREDV1 |
				RCU_CFG1_PLL1MF | RCU_CFG1_PREDV0);
		RCU_CFG1 |= (RCU_PREDV0SRC_HXTAL | RCU_PREDV0_DIV2 |
				RCU_PREDV1_DIV2 | RCU_PLL1_MUL20 |
				RCU_PLL2_MUL20);

		enable_pll1();
		enable_pll2();

	}

	enable_pll();
	pll_as_sys_clock();
}

#elif defined(__SYSTEM_CLOCK_48M_PLL_IRC8M)

static void system_clock_48m_irc8m() {
	enable_irc8m();
	pre_init_rcu();

	/* CK_PLL = (CK_IRC8M/2) * 12 = 48 MHz */
	RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
	RCU_CFG0 |= RCU_PLL_MUL12;

	enable_pll();
	pll_as_sys_clock();
}

#elif defined(__SYSTEM_CLOCK_72M_PLL_IRC8M)

static void system_clock_72m_irc8m() {
	enable_irc8m();
	pre_init_rcu();

	/* CK_PLL = (CK_IRC8M/2) * 18 = 72 MHz */
	RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
	RCU_CFG0 |= RCU_PLL_MUL18;

	enable_pll();
	pll_as_sys_clock();
}

#elif defined(__SYSTEM_CLOCK_108M_PLL_IRC8M)

static void system_clock_108m_irc8m() {
	enable_irc8m();
	pre_init_rcu();

	/* CK_PLL = (CK_IRC8M/2) * 27 = 108 MHz */
	RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
	RCU_CFG0 |= RCU_PLL_MUL27;

	enable_pll();
	pll_as_sys_clock();
}

#endif

static inline void system_clock_config() {
#ifdef __SYSTEM_CLOCK_HXTAL
	system_clock_hxtal();
#elif defined(__SYSTEM_CLOCK_24M_PLL_HXTAL)
	system_clock_24m_hxtal();
#elif defined(__SYSTEM_CLOCK_36M_PLL_HXTAL)
	system_clock_36m_hxtal();
#elif defined(__SYSTEM_CLOCK_48M_PLL_HXTAL)
	system_clock_48m_hxtal();
#elif defined(__SYSTEM_CLOCK_56M_PLL_HXTAL)
	system_clock_56m_hxtal();
#elif defined(__SYSTEM_CLOCK_72M_PLL_HXTAL)
	system_clock_72m_hxtal();
#elif defined(__SYSTEM_CLOCK_96M_PLL_HXTAL)
	system_clock_96m_hxtal();
#elif defined(__SYSTEM_CLOCK_108M_PLL_HXTAL)
	system_clock_108m_hxtal();

#elif defined(__SYSTEM_CLOCK_48M_PLL_IRC8M)
	system_clock_48m_irc8m();
#elif defined(__SYSTEM_CLOCK_72M_PLL_IRC8M)
	system_clock_72m_irc8m();
#elif defined(__SYSTEM_CLOCK_108M_PLL_IRC8M)
	system_clock_108m_irc8m();
#endif
}

void system_init() {
	// reset the RCC clock configuration to the default reset state

	// enable IRC8M
	RCU_CTL |= RCU_CTL_IRC8MEN;

	// reset SCS, AHBPSC, APB1PSC, APB2PSC, ADCPSC, CKOUT0SEL bits
	RCU_CFG0 &= ~(RCU_CFG0_SCS | RCU_CFG0_AHBPSC | RCU_CFG0_APB1PSC |
			RCU_CFG0_APB2PSC | RCU_CFG0_ADCPSC |
			RCU_CFG0_ADCPSC_2 | RCU_CFG0_CKOUT0SEL);

	// reset HXTALEN, CKMEN, PLLEN bits
	RCU_CTL &= ~(RCU_CTL_HXTALEN | RCU_CTL_CKMEN | RCU_CTL_PLLEN);

	// Reset HXTALBPS bit
	RCU_CTL &= ~(RCU_CTL_HXTALBPS);

	// reset PLLSEL, PREDV0_LSB, PLLMF, USBFSPSC bits
	RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PREDV0_LSB | RCU_CFG0_PLLMF |
			RCU_CFG0_USBFSPSC | RCU_CFG0_PLLMF_4);
	RCU_CFG1 = 0x00000000U;

	// Reset HXTALEN, CKMEN, PLLEN, PLL1EN and PLL2EN bits
	RCU_CTL &= ~(RCU_CTL_PLLEN | RCU_CTL_PLL1EN | RCU_CTL_PLL2EN |
			RCU_CTL_CKMEN | RCU_CTL_HXTALEN);

	// disable all interrupts
	RCU_INT = 0x00FF0000U;

	// Configure the System clock source, PLL Multiplier,
	// AHB/APBx prescalers and Flash settings
	system_clock_config();
}

