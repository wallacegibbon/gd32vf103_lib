#include "gd32vf103_usart.h"

/*
 * reset USART/UART
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 */
void usart_deinit(uint32_t usart_periph) {
	switch (usart_periph) {
	case USART0:
		rcu_periph_reset_enable(RCU_USART0RST);
		rcu_periph_reset_disable(RCU_USART0RST);
		break;
	case USART1:
		rcu_periph_reset_enable(RCU_USART1RST);
		rcu_periph_reset_disable(RCU_USART1RST);
		break;
	case USART2:
		rcu_periph_reset_enable(RCU_USART2RST);
		rcu_periph_reset_disable(RCU_USART2RST);
		break;
	case UART3:
		rcu_periph_reset_enable(RCU_UART3RST);
		rcu_periph_reset_disable(RCU_UART3RST);
		break;
	case UART4:
		rcu_periph_reset_enable(RCU_UART4RST);
		rcu_periph_reset_disable(RCU_UART4RST);
		break;
	default:
		break;
	}
}

/*
 * configure USART baud rate value
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * baudval: baud rate value
 */
void usart_baudrate_set(uint32_t usart_periph, uint32_t baudval) {
	uint32_t uclk = 0, intdiv = 0, fradiv = 0, udiv = 0;
	// get clock frequency
	switch (usart_periph) {
	case USART0:
		uclk = rcu_clock_freq_get(CK_APB2);
		break;
	case USART1:
		uclk = rcu_clock_freq_get(CK_APB1);
		break;
	case USART2:
		uclk = rcu_clock_freq_get(CK_APB1);
		break;
	case UART3:
		uclk = rcu_clock_freq_get(CK_APB1);
		break;
	case UART4:
		uclk = rcu_clock_freq_get(CK_APB1);
		break;
	default:
		break;
	}
	// oversampling by 16, configure the value of USART_BAUD
	udiv = (uclk + baudval / 2) / baudval;
	intdiv = udiv & 0x0000fff0;
	fradiv = udiv & 0x0000000f;
	USART_BAUD(usart_periph) =
		(USART_BAUD_FRADIV | USART_BAUD_INTDIV) & (intdiv | fradiv);
}

/*
 * configure USART parity
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * paritycfg: configure USART parity
 * 	USART_PM_NONE: no parity
 * 	USART_PM_ODD:  odd parity
 * 	USART_PM_EVEN: even parity 
 */
void usart_parity_config(uint32_t usart_periph, uint32_t paritycfg) {
	USART_CTL0(usart_periph) &= ~(USART_CTL0_PM | USART_CTL0_PCEN);
	USART_CTL0(usart_periph) |= paritycfg;
}

/*
 * configure USART word length
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * wlen: USART word length configure
 * 	USART_WL_8BIT: 8 bits
 * 	USART_WL_9BIT: 9 bits
 */
void usart_word_length_set(uint32_t usart_periph, uint32_t wlen) {
	USART_CTL0(usart_periph) &= ~USART_CTL0_WL;
	USART_CTL0(usart_periph) |= wlen;
}

/*
 * configure USART stop bit length
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * stblen: USART stop bit configure
 * 	USART_STB_1BIT:   1 bit
 * 	USART_STB_0_5BIT: 0.5 bit, not available for UARTx(x=3,4)
 * 	USART_STB_2BIT:   2 bits
 * 	USART_STB_1_5BIT: 1.5 bits, not available for UARTx(x=3,4)
 */
void usart_stop_bit_set(uint32_t usart_periph, uint32_t stblen) {
	USART_CTL1(usart_periph) &= ~USART_CTL1_STB;
	USART_CTL1(usart_periph) |= stblen;
}

void usart_enable(uint32_t usart_periph) {
	USART_CTL0(usart_periph) |= USART_CTL0_UEN;
}

void usart_disable(uint32_t usart_periph) {
	USART_CTL0(usart_periph) &= ~(USART_CTL0_UEN);
}

/*
 * configure USART transmitter
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * txconfig: enable or disable USART transmitter
 * 	USART_TRANSMIT_ENABLE: enable USART transmission
 * 	USART_TRANSMIT_DISABLE: disable USART transmission
 */
void usart_transmit_config(uint32_t usart_periph, uint32_t txconfig) {
	uint32_t ctl = USART_CTL0(usart_periph);
	ctl &= ~USART_CTL0_TEN;
	ctl |= txconfig;
	// configure transfer mode
	USART_CTL0(usart_periph) = ctl;
}

/*
 * configure USART receiver
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * rxconfig: enable or disable USART receiver
 * 	USART_RECEIVE_ENABLE: enable USART reception
 * 	USART_RECEIVE_DISABLE: disable USART reception
 */
void usart_receive_config(uint32_t usart_periph, uint32_t rxconfig) {
	uint32_t ctl = USART_CTL0(usart_periph);
	ctl &= ~USART_CTL0_REN;
	ctl |= rxconfig;
	// configure receiver mode
	USART_CTL0(usart_periph) = ctl;
}

/*
 * USART transmit data function
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * data: data of transmission 
 */
void usart_data_transmit(uint32_t usart_periph, uint32_t data) {
	USART_DATA(usart_periph) = USART_DATA_DATA & data;
}

/*
 * USART receive data function
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 */
uint16_t usart_data_receive(uint32_t usart_periph) {
	return (uint16_t) (GET_BITS(USART_DATA(usart_periph), 0, 8));
}

/*
 * configure the address of the USART in wake up by address match mode
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * addr: address of USART/UART
 */
void usart_address_config(uint32_t usart_periph, uint8_t addr) {
	USART_CTL1(usart_periph) &= ~(USART_CTL1_ADDR);
	USART_CTL1(usart_periph) |= (USART_CTL1_ADDR & addr);
}

/*
 * receiver in mute mode
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 */
void usart_mute_mode_enable(uint32_t usart_periph) {
	USART_CTL0(usart_periph) |= USART_CTL0_RWU;
}

/*
 * receiver in active mode
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 */
void usart_mute_mode_disable(uint32_t usart_periph) {
	USART_CTL0(usart_periph) &= ~(USART_CTL0_RWU);
}

/*
 * configure wakeup method in mute mode
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * wmethod: two methods be used to enter or exit the mute mode
 * 	USART_WM_IDLE: idle line
 * 	USART_WM_ADDR: address mask
 */
void usart_mute_mode_wakeup_config(uint32_t usart_periph, uint32_t wmethod) {
	USART_CTL0(usart_periph) &= ~(USART_CTL0_WM);
	USART_CTL0(usart_periph) |= wmethod;
}

void usart_lin_mode_enable(uint32_t usart_periph) {
	USART_CTL1(usart_periph) |= USART_CTL1_LMEN;
}

void usart_lin_mode_disable(uint32_t usart_periph) {
	USART_CTL1(usart_periph) &= ~(USART_CTL1_LMEN);
}

/*
 * configure lin break frame length
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * lblen: lin break frame length
 * 	USART_LBLEN_10B: 10 bits
 * 	USART_LBLEN_11B: 11 bits
 */
void usart_lin_break_detection_length_config(uint32_t usart_periph,
		uint32_t lblen) {
	USART_CTL1(usart_periph) &= ~(USART_CTL1_LBLEN);
	USART_CTL1(usart_periph) |= (USART_CTL1_LBLEN & lblen);
}

void usart_send_break(uint32_t usart_periph) {
	USART_CTL0(usart_periph) |= USART_CTL0_SBKCMD;
}

void usart_halfduplex_enable(uint32_t usart_periph) {
	USART_CTL2(usart_periph) |= USART_CTL2_HDEN;
}

void usart_halfduplex_disable(uint32_t usart_periph) {
	USART_CTL2(usart_periph) &= ~(USART_CTL2_HDEN);
}

/*
 * enable CK pin in synchronous mode
 * usart_periph: USARTx(x=0,1,2)
 */
void usart_synchronous_clock_enable(uint32_t usart_periph) {
	USART_CTL1(usart_periph) |= USART_CTL1_CKEN;
}

/*
 * disable CK pin in synchronous mode
 * usart_periph: USARTx(x=0,1,2)
 */
void usart_synchronous_clock_disable(uint32_t usart_periph) {
	USART_CTL1(usart_periph) &= ~(USART_CTL1_CKEN);
}

/*
 * configure USART synchronous mode parameters
 * usart_periph: USARTx(x=0,1,2)
 * clen: CK length
 * 	USART_CLEN_NONE:
 * 		there are 7 CK pulses for an 8 bit frame and 8 CK pulses for
 * 		a 9 bit frame
 * 	USART_CLEN_EN:
 * 		there are 8 CK pulses for an 8 bit frame and 9 CK pulses for
 * 		a 9 bit frame
 *
 * cph: clock phase
 * 	USART_CPH_1CK:
 * 		first clock transition is the first data capture edge
 * 	USART_CPH_2CK:
 * 		second clock transition is the first data capture edge
 *
 * cpl: clock polarity
 * 	USART_CPL_LOW:  steady low value on CK pin
 * 	USART_CPL_HIGH: steady high value on CK pin
 *
 */
void usart_synchronous_clock_config(uint32_t usart_periph, uint32_t clen,
		uint32_t cph, uint32_t cpl) {
	// read USART_CTL1 register
	uint32_t ctl = USART_CTL1(usart_periph);
	ctl &= ~(USART_CTL1_CLEN | USART_CTL1_CPH | USART_CTL1_CPL);
	// set CK length, CK phase, CK polarity
	ctl |= (USART_CTL1_CLEN & clen) | (USART_CTL1_CPH & cph) |
		(USART_CTL1_CPL & cpl);

	USART_CTL1(usart_periph) = ctl;
}

void usart_guard_time_config(uint32_t usart_periph, uint32_t gaut) {
	USART_GP(usart_periph) &= ~(USART_GP_GUAT);
	USART_GP(usart_periph) |= (USART_GP_GUAT & ((gaut) << 8));
}

void usart_smartcard_mode_enable(uint32_t usart_periph) {
	USART_CTL2(usart_periph) |= USART_CTL2_SCEN;
}

void usart_smartcard_mode_disable(uint32_t usart_periph) {
	USART_CTL2(usart_periph) &= ~(USART_CTL2_SCEN);
}

void usart_smartcard_mode_nack_enable(uint32_t usart_periph) {
	USART_CTL2(usart_periph) |= USART_CTL2_NKEN;
}

void usart_smartcard_mode_nack_disable(uint32_t usart_periph) {
	USART_CTL2(usart_periph) &= ~(USART_CTL2_NKEN);
}

void usart_irda_mode_enable(uint32_t usart_periph) {
	USART_CTL2(usart_periph) |= USART_CTL2_IREN;
}

void usart_irda_mode_disable(uint32_t usart_periph) {
	USART_CTL2(usart_periph) &= ~(USART_CTL2_IREN);
}

/*
 * configure the peripheral clock prescaler in USART IrDA low-power mode
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * psc: 0x00-0xFF
 */
void usart_prescaler_config(uint32_t usart_periph, uint8_t psc) {
	USART_GP(usart_periph) &= ~(USART_GP_PSC);
	USART_GP(usart_periph) |= psc;
}

/*
 * configure IrDA low-power
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
 * irlp: IrDA low-power or normal
 * 	USART_IRLP_LOW: low-power
 * 	USART_IRLP_NORMAL: normal
 */
void usart_irda_lowpower_config(uint32_t usart_periph, uint32_t irlp) {
	USART_CTL2(usart_periph) &= ~(USART_CTL2_IRLP);
	USART_CTL2(usart_periph) |= (USART_CTL2_IRLP & irlp);
}

/*
 * configure hardware flow control RTS
 * usart_periph: USARTx(x=0,1,2)
 * rtsconfig: enable or disable RTS
 * 	USART_RTS_ENABLE:  enable RTS
 * 	USART_RTS_DISABLE: disable RTS
 */
void usart_hardware_flow_rts_config(uint32_t usart_periph, uint32_t rtsconfig) {
	uint32_t ctl = USART_CTL2(usart_periph);
	ctl &= ~USART_CTL2_RTSEN;
	ctl |= rtsconfig;
	// configure RTS
	USART_CTL2(usart_periph) = ctl;
}

/*
 * configure hardware flow control CTS
 * usart_periph: USARTx(x=0,1,2)
 * ctsconfig: enable or disable CTS
 * 	USART_CTS_ENABLE:  enable CTS
 * 	USART_CTS_DISABLE: disable CTS
 */
void usart_hardware_flow_cts_config(uint32_t usart_periph, uint32_t ctsconfig) {
	uint32_t ctl = USART_CTL2(usart_periph);
	ctl &= ~USART_CTL2_CTSEN;
	ctl |= ctsconfig;
	// configure CTS
	USART_CTL2(usart_periph) = ctl;
}

/*
 * configure USART DMA reception
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3)
 * dmacmd: enable or disable DMA for reception
 * 	USART_DENR_ENABLE:  DMA enable for reception
 * 	USART_DENR_DISABLE: DMA disable for reception
 */
void usart_dma_receive_config(uint32_t usart_periph, uint32_t dmacmd) {
	uint32_t ctl = USART_CTL2(usart_periph);
	ctl &= ~USART_CTL2_DENR;
	ctl |= dmacmd;
	// configure DMA reception
	USART_CTL2(usart_periph) = ctl;
}

/*
 * configure USART DMA transmission
 * usart_periph: USARTx(x=0,1,2)/UARTx(x=3)
 * dmacmd: enable or disable DMA for transmission
 * 	USART_DENT_ENABLE:  DMA enable for transmission
 * 	USART_DENT_DISABLE: DMA disable for transmission
 */
void usart_dma_transmit_config(uint32_t usart_periph, uint32_t dmacmd) {
	uint32_t ctl = USART_CTL2(usart_periph);
	ctl &= ~USART_CTL2_DENT;
	ctl |= dmacmd;
	// configure DMA transmission
	USART_CTL2(usart_periph) = ctl;
}

/*
 * get flag in STAT register
 * flag: USART flags, refer to enum usart_flag
 * 	USART_FLAG_CTS: CTS change flag
 * 	USART_FLAG_LBD: LIN break detected flag
 * 	USART_FLAG_TBE: transmit data buffer empty
 * 	USART_FLAG_TC: transmission complete
 * 	USART_FLAG_RBNE: read data buffer not empty
 * 	USART_FLAG_IDLE: IDLE frame detected flag
 * 	USART_FLAG_ORERR: overrun error
 * 	USART_FLAG_NERR: noise error flag
 * 	USART_FLAG_FERR: frame error flag
 * 	USART_FLAG_PERR: parity error flag 
 */
enum flag_status usart_flag_get(uint32_t usart_periph, enum usart_flag flag) {
	if (RESET != (USART_REG_VAL(usart_periph, flag) &
				BIT(USART_BIT_POS(flag))))
		return SET;
	else
		return RESET;
}

/*
 * clear flag in STAT register
 * flag: USART flags, refer to enum usart_flag
 * 	USART_FLAG_CTS: CTS change flag
 * 	USART_FLAG_LBD: LIN break detected flag
 * 	USART_FLAG_TC: transmission complete
 * 	USART_FLAG_RBNE: read data buffer not empty
 */
void usart_flag_clear(uint32_t usart_periph, enum usart_flag flag) {
	USART_REG_VAL(usart_periph, flag) &= ~BIT(USART_BIT_POS(flag));
}

/*
 * interrupt
 * 	USART_INT_PERR: parity error interrupt
 * 	USART_INT_TBE: transmitter buffer empty interrupt
 * 	USART_INT_TC: transmission complete interrupt
 * 	USART_INT_RBNE:
 * 		read data buffer not empty interrupt and overrun error
 * 		interrupt
 * 	USART_INT_IDLE: IDLE line detected interrupt
 * 	USART_INT_LBD: LIN break detected interrupt
 * 	USART_INT_ERR: error interrupt
 * 	USART_INT_CTS: CTS interrupt
 */
void usart_interrupt_enable(uint32_t usart_periph, uint32_t interrupt) {
	USART_REG_VAL(usart_periph, interrupt) |= BIT(USART_BIT_POS(interrupt));
}

/*
 * interrupt:
 * 	USART_INT_PERR: parity error interrupt
 * 	USART_INT_TBE: transmitter buffer empty interrupt
 * 	USART_INT_TC: transmission complete interrupt
 * 	USART_INT_RBNE:
 * 		read data buffer not empty interrupt and overrun error
 * 		interrupt
 * 	USART_INT_IDLE: IDLE line detected interrupt
 * 	USART_INT_LBD: LIN break detected interrupt
 * 	USART_INT_ERR: error interrupt
 * 	USART_INT_CTS: CTS interrupt
 */
void usart_interrupt_disable(uint32_t usart_periph, uint32_t interrupt) {
	USART_REG_VAL(usart_periph, interrupt) &=
		~BIT(USART_BIT_POS(interrupt));
}

/*
 * int_flag
 * 	USART_INT_FLAG_PERR:
 * 		parity error interrupt and flag
 * 	USART_INT_FLAG_TBE:
 * 		transmitter buffer empty interrupt and flag
 * 	USART_INT_FLAG_TC:
 * 		transmission complete interrupt and flag
 * 	USART_INT_FLAG_RBNE:
 * 		read data buffer not empty interrupt and flag
 * 	USART_INT_FLAG_RBNE_ORERR:
 * 		read data buffer not empty interrupt and overrun error flag
 * 	USART_INT_FLAG_IDLE:
 * 		IDLE line detected interrupt and flag
 * 	USART_INT_FLAG_LBD:
 * 		LIN break detected interrupt and flag
 * 	USART_INT_FLAG_CTS:
 * 		CTS interrupt and flag
 * 	USART_INT_FLAG_ERR_ORERR:
 * 		error interrupt and overrun error
 * 	USART_INT_FLAG_ERR_NERR:
 * 		error interrupt and noise error flag
 * 	USART_INT_FLAG_ERR_FERR:
 * 		error interrupt and frame error flag
 */
enum flag_status usart_interrupt_flag_get(uint32_t usart_periph,
		uint32_t int_flag) {
	uint32_t intenable = 0, flag = 0;

	// get the interrupt enable bit status
	intenable = (USART_REG_VAL(usart_periph, int_flag) &
			BIT(USART_BIT_POS(int_flag)));

	// get the corresponding flag bit status
	flag = (USART_REG_VAL2(usart_periph, int_flag) &
			BIT(USART_BIT_POS2(int_flag)));

	if (flag && intenable)
		return SET;
	else
		return RESET;
}

/*
 * clear USART interrupt flag in STAT register
 * int_flag: USART interrupt flag
 * 	USART_INT_FLAG_CTS: CTS change flag
 * 	USART_INT_FLAG_LBD: LIN break detected flag
 * 	USART_INT_FLAG_TC: transmission complete
 * 	USART_INT_FLAG_RBNE: read data buffer not empty
 */
void usart_interrupt_flag_clear(uint32_t usart_periph, uint32_t int_flag) {
	USART_REG_VAL2(usart_periph, int_flag) &=
		~BIT(USART_BIT_POS2(int_flag));
}
