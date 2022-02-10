#include "gd32vf103_i2c.h"

// i2cclk maximum value
#define I2CCLK_MAX			((uint32_t) 0x00000036U)

// i2cclk minimum value
#define I2CCLK_MIN			((uint32_t) 0x00000002U)

// i2c flag mask
#define I2C_FLAG_MASK			((uint32_t) 0x0000FFFFU)

// i2c address mask
#define I2C_ADDRESS_MASK		((uint32_t) 0x000003FFU)

// the second i2c address mask
#define I2C_ADDRESS2_MASK		((uint32_t) 0x000000FEU)

// bit offset of PECV in I2C_STAT1
#define STAT1_PECV_OFFSET		((uint32_t) 8U)

void i2c_deinit(uint32_t i2c_periph) {
	switch (i2c_periph) {
	case I2C0:
		rcu_periph_reset_enable(RCU_I2C0RST);
		rcu_periph_reset_disable(RCU_I2C0RST);
		break;
	case I2C1:
		rcu_periph_reset_enable(RCU_I2C1RST);
		rcu_periph_reset_disable(RCU_I2C1RST);
		break;
	default:
		break;
	}
}

/*
 * clkspeed:
 * 	I2C clock speed, supports standard mode (up to 100 kHz),
 * 	fast mode (up to 400 kHz) and fast mode plus (up to 1MHz)
 *
 * dutycyc: duty cycle in fast mode or fast mode plus
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_DTCY_2: T_low/T_high=2
 * 		I2C_DTCY_16_9: T_low/T_high=16/9
 */
void i2c_clock_config(uint32_t i2c_periph, uint32_t clkspeed, uint32_t dutycyc) {
	uint32_t pclk1 = rcu_clock_freq_get(CK_APB1);

	// I2C peripheral clock frequency
	uint32_t freq = pclk1 / 1000000U;
	if (freq >= I2CCLK_MAX)
		freq = I2CCLK_MAX;

	uint32_t temp = I2C_CTL1(i2c_periph);
	temp &= ~I2C_CTL1_I2CCLK;
	temp |= freq;

	I2C_CTL1(i2c_periph) = temp;

	uint32_t clkc;

	if (clkspeed < 100000U) {
		// the maximum SCL rise time is 1000ns in standard mode
		uint32_t risetime = pclk1 / 1000000U + 1;
		if (risetime >= I2CCLK_MAX) {
			I2C_RT(i2c_periph) = I2CCLK_MAX;
		} else if (risetime <= I2CCLK_MIN) {
			I2C_RT(i2c_periph) = I2CCLK_MIN;
		} else {
			I2C_RT(i2c_periph) = risetime;
		}
		clkc = pclk1 / (clkspeed * 2);
		if (clkc < 4)
			// the CLKC in standard mode minimum value is 4
			clkc = 4;

		I2C_CKCFG(i2c_periph) |= clkc &  I2C_CKCFG_CLKC;

	} else if (clkspeed < 400000U) {
		// the maximum SCL rise time is 300ns in fast mode
		I2C_RT(i2c_periph) = freq * 300 / 1000 + 1;
		if (I2C_DTCY_2 == dutycyc) {
			// I2C duty cycle is 2
			clkc = pclk1 / (clkspeed * 3);
			I2C_CKCFG(i2c_periph) &= ~I2C_CKCFG_DTCY;
		} else {
			// I2C duty cycle is 16/9
			clkc = pclk1 / (clkspeed * 25);
			I2C_CKCFG(i2c_periph) |= I2C_CKCFG_DTCY;
		}

		if (!(clkc & I2C_CKCFG_CLKC))
			// the CLKC in fast mode minimum value is 1
			clkc |= 1;

		I2C_CKCFG(i2c_periph) |= I2C_CKCFG_FAST;
		I2C_CKCFG(i2c_periph) |= clkc;
	} else {
		// fast mode plus, the maximum SCL rise time is 120ns
		I2C_RT(i2c_periph) = freq * 120 / 1000 + 1;
		if (I2C_DTCY_2 == dutycyc) {
			// I2C duty cycle is 2
			clkc = pclk1 / (clkspeed * 3);
			I2C_CKCFG(i2c_periph) &= ~I2C_CKCFG_DTCY;
		} else {
			// I2C duty cycle is 16/9
			clkc = pclk1 / (clkspeed * 25);
			I2C_CKCFG(i2c_periph) |= I2C_CKCFG_DTCY;
		}
		// enable fast mode
		I2C_CKCFG(i2c_periph) |= I2C_CKCFG_FAST;
		I2C_CKCFG(i2c_periph) |= clkc;
		// enable I2C fast mode plus
		I2C_FMPCFG(i2c_periph) |= I2C_FMPCFG_FMPEN;
	}
}

/*
 * mode:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_I2CMODE_ENABLE: I2C mode
 * 		I2C_SMBUSMODE_ENABLE: SMBus mode
 *
 * addformat: 7bits or 10bits
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_ADDFORMAT_7BITS: 7bits
 * 		I2C_ADDFORMAT_10BITS: 10bits
 * addr:
 * 	I2C address
 */
void i2c_mode_addr_config(uint32_t i2c_periph, uint32_t mode,
		uint32_t addformat, uint32_t addr) {

	uint32_t ctl = I2C_CTL0(i2c_periph);
	ctl &= ~(I2C_CTL0_SMBEN);
	ctl |= mode;
	I2C_CTL0(i2c_periph) = ctl;
	// configure address
	addr = addr & I2C_ADDRESS_MASK;
	I2C_SADDR0(i2c_periph) = addformat | addr;
}

/*
 * type:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_SMBUS_DEVICE: device
 * 		I2C_SMBUS_HOST: host
 */
void i2c_smbus_type_config(uint32_t i2c_periph, uint32_t type) {
	if (type == I2C_SMBUS_HOST)
		I2C_CTL0(i2c_periph) |= I2C_CTL0_SMBSEL;
	else
		I2C_CTL0(i2c_periph) &= ~I2C_CTL0_SMBSEL;
}

/*
 * ack:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_ACK_ENABLE: ACK will be sent
 * 		I2C_ACK_DISABLE: ACK will not be sent
 */
void i2c_ack_config(uint32_t i2c_periph, uint32_t ack) {
	if (ack == I2C_ACK_ENABLE)
		I2C_CTL0(i2c_periph) |= I2C_CTL0_ACKEN;
	else
		I2C_CTL0(i2c_periph) &= ~I2C_CTL0_ACKEN;
}

/*
 * pos:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_ACKPOS_CURRENT: whether to send ACK or not for the current
 * 		I2C_ACKPOS_NEXT: whether to send ACK or not for the next byte
 */
void i2c_ackpos_config(uint32_t i2c_periph, uint32_t pos) {
	if (pos == I2C_ACKPOS_NEXT)
		I2C_CTL0(i2c_periph) |= I2C_CTL0_POAP;
	else
		I2C_CTL0(i2c_periph) &= ~I2C_CTL0_POAP;
}

/*
 * master sends slave address
 *
 * addr: slave address  
 *
 * trandirection: transmitter or receiver
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_TRANSMITTER: transmitter
 * 		I2C_RECEIVER: receiver  
 */
void i2c_master_addressing(uint32_t i2c_periph, uint32_t addr,
		uint32_t trandirection) {

	// master is a transmitter or a receiver
	if (trandirection == I2C_TRANSMITTER)
		addr = addr & I2C_TRANSMITTER;
	else
		addr = addr | I2C_RECEIVER;

	// send slave address
	I2C_DATA(i2c_periph) = addr;
}

/* addr: the second address in dual-address mode */
void i2c_dualaddr_enable(uint32_t i2c_periph, uint32_t addr) {
	// configure address
	addr = addr & I2C_ADDRESS2_MASK;
	I2C_SADDR1(i2c_periph) = I2C_SADDR1_DUADEN | addr;
}

void i2c_dualaddr_disable(uint32_t i2c_periph) {
	I2C_SADDR1(i2c_periph) &= ~I2C_SADDR1_DUADEN;
}

void i2c_enable(uint32_t i2c_periph) {
	I2C_CTL0(i2c_periph) |= I2C_CTL0_I2CEN;
}

void i2c_disable(uint32_t i2c_periph) {
	I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_I2CEN);
}

/* generate a START condition on I2C bus */
void i2c_start_on_bus(uint32_t i2c_periph) {
	I2C_CTL0(i2c_periph) |= I2C_CTL0_START;
}

/* generate a STOP condition on I2C bus */
void i2c_stop_on_bus(uint32_t i2c_periph) {
	I2C_CTL0(i2c_periph) |= I2C_CTL0_STOP;
}

void i2c_data_transmit(uint32_t i2c_periph, uint8_t data) {
	I2C_DATA(i2c_periph) = DATA_TRANS(data);
}

uint8_t i2c_data_receive(uint32_t i2c_periph) {
	return (uint8_t) DATA_RECV(I2C_DATA(i2c_periph));
}

/*
 * dmastate:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_DMA_ON: DMA mode enable
 * 		I2C_DMA_OFF: DMA mode disable
 */
void i2c_dma_enable(uint32_t i2c_periph, uint32_t dmastate) {
	// configure I2C DMA function
	uint32_t ctl = I2C_CTL1(i2c_periph);
	ctl &= ~I2C_CTL1_DMAON;
	ctl |= dmastate;
	I2C_CTL1(i2c_periph) = ctl;
}

/*
 * dmalast:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_DMALST_ON: next DMA EOT is the last transfer
 * 		I2C_DMALST_OFF: next DMA EOT is not the last transfer
 */
void i2c_dma_last_transfer_config(uint32_t i2c_periph, uint32_t dmalast) {
	// configure DMA last transfer
	uint32_t ctl = I2C_CTL1(i2c_periph);
	ctl &= ~I2C_CTL1_DMALST;
	ctl |= dmalast;
	I2C_CTL1(i2c_periph) = ctl;
}

/*
 * whether to stretch SCL low when data is not ready in slave mode 
 *
 * stretchpara:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_SCLSTRETCH_ENABLE: SCL stretching is enabled
 * 		I2C_SCLSTRETCH_DISABLE: SCL stretching is disabled
 */
void i2c_stretch_scl_low_config(uint32_t i2c_periph, uint32_t stretchpara) {
	uint32_t ctl = I2C_CTL0(i2c_periph);
	ctl &= ~I2C_CTL0_SS;
	ctl |= stretchpara;
	I2C_CTL0(i2c_periph) = ctl;
}

/*
 * whether or not to response to a general call
 * gcallpara:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_GCEN_ENABLE: slave will response to a general call
 * 		I2C_GCEN_DISABLE: slave will not response to a general call
 */
void i2c_slave_response_to_gcall_config(uint32_t i2c_periph,
		uint32_t gcallpara) {

	uint32_t ctl = I2C_CTL0(i2c_periph);
	ctl &= ~I2C_CTL0_GCEN;
	ctl |= gcallpara;
	I2C_CTL0(i2c_periph) = ctl;
}

/*
 * sreset:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_SRESET_SET: I2C is under reset
 * 		I2C_SRESET_RESET: I2C is not under reset
 */
void i2c_software_reset_config(uint32_t i2c_periph, uint32_t sreset) {
	// modify CTL0 and configure software reset I2C state
	uint32_t ctl = I2C_CTL0(i2c_periph);
	ctl &= ~I2C_CTL0_SRESET;
	ctl |= sreset;
	I2C_CTL0(i2c_periph) = ctl;
}

/*
 * pecpara:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_PEC_ENABLE: PEC calculation on
 * 		I2C_PEC_DISABLE: PEC calculation off 
 */
void i2c_pec_enable(uint32_t i2c_periph, uint32_t pecstate) {
	uint32_t ctl = I2C_CTL0(i2c_periph);
	ctl &= ~I2C_CTL0_PECEN;
	ctl |= pecstate;
	I2C_CTL0(i2c_periph) = ctl;
}

/*
 * pecpara:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_PECTRANS_ENABLE: transfer PEC
 * 		I2C_PECTRANS_DISABLE: not transfer PEC 
 */
void i2c_pec_transfer_enable(uint32_t i2c_periph, uint32_t pecpara) {
	uint32_t ctl = I2C_CTL0(i2c_periph);
	ctl &= ~I2C_CTL0_PECTRANS;
	ctl |= pecpara;
	I2C_CTL0(i2c_periph) = ctl;
}

/* get packet error checking value */
uint8_t i2c_pec_value_get(uint32_t i2c_periph) {
	return (uint8_t) ((I2C_STAT1(i2c_periph) & I2C_STAT1_PECV) >>
			  STAT1_PECV_OFFSET);
}

/*
 * I2C issue alert through SMBA pin
 * i2c_periph:
 * 	I2Cx(x=0,1)
 * smbuspara:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_SALTSEND_ENABLE: issue alert through SMBA pin
 * 		I2C_SALTSEND_DISABLE: not issue alert through SMBA pin 
 */
void i2c_smbus_issue_alert(uint32_t i2c_periph, uint32_t smbuspara) {
	uint32_t ctl = I2C_CTL0(i2c_periph);
	ctl &= ~I2C_CTL0_SALT;
	ctl |= smbuspara;
	I2C_CTL0(i2c_periph) = ctl;
}

/*
 * arpstate:
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_ARP_ENABLE: enable ARP
 * 		I2C_ARP_DISABLE: disable ARP
 */
void i2c_smbus_arp_enable(uint32_t i2c_periph, uint32_t arpstate) {
	uint32_t ctl = I2C_CTL0(i2c_periph);
	ctl &= ~I2C_CTL0_ARPEN;
	ctl |= arpstate;
	I2C_CTL0(i2c_periph) = ctl;
}

enum flag_status i2c_flag_get(uint32_t i2c_periph, enum i2c_flag flag) {
	if ((I2C_REG_VAL(i2c_periph, flag) & BIT(I2C_BIT_POS(flag))) != RESET)
		return SET;
	else
		return RESET;
}

void i2c_flag_clear(uint32_t i2c_periph, enum i2c_flag flag) {
	uint32_t temp;
	if (flag == I2C_FLAG_ADDSEND) {
		// read I2C_STAT0 and then read I2C_STAT1 to clear ADDSEND
		temp = I2C_STAT0(i2c_periph);
		temp = I2C_STAT1(i2c_periph);
	} else {
		I2C_REG_VAL(i2c_periph, flag) &= ~BIT(I2C_BIT_POS(flag));
	}
}

void i2c_interrupt_enable(uint32_t i2c_periph, enum i2c_interrupt interrupt) {
	I2C_REG_VAL(i2c_periph, interrupt) |= BIT(I2C_BIT_POS(interrupt));
}

void i2c_interrupt_disable(uint32_t i2c_periph, enum i2c_interrupt interrupt) {
	I2C_REG_VAL(i2c_periph, interrupt) &= ~BIT(I2C_BIT_POS(interrupt));
}

enum flag_status i2c_interrupt_flag_get(uint32_t i2c_periph,
		enum i2c_interrupt_flag int_flag) {

	uint32_t intenable = 0U, flag = 0U, bufie;

	// check BUFIE
	bufie = I2C_CTL1(i2c_periph) & I2C_CTL1_BUFIE;

	// get the interrupt enable bit status
	intenable = I2C_REG_VAL(i2c_periph, int_flag) &
		BIT(I2C_BIT_POS(int_flag));

	// get the corresponding flag bit status
	flag = I2C_REG_VAL2(i2c_periph, int_flag) &
		BIT(I2C_BIT_POS2(int_flag));

	if (int_flag == I2C_INT_FLAG_RBNE || int_flag == I2C_INT_FLAG_TBE) {
		if (intenable && bufie)
			intenable = 1;
		else
			intenable = 0;
	}

	if (flag && intenable)
		return SET;
	else
		return RESET;
}

/*
 * int_flag: I2C interrupt flags, refer to enum i2c_interrupt_flag
 * 	only one parameter can be selected which is shown as below:
 * 		I2C_INT_FLAG_ADDSEND:
 * 			address is sent in master mode or received and
 * 			matches in slave mode interrupt flag
 * 		I2C_INT_FLAG_BERR:
 * 			a bus error occurs indication a unexpected start
 * 			or stop condition on I2C bus interrupt flag
 * 		I2C_INT_FLAG_LOSTARB:
 * 			arbitration lost in master mode interrupt flag
 * 		I2C_INT_FLAG_AERR:
 * 			acknowledge error interrupt flag
 * 		I2C_INT_FLAG_OUERR:
 * 			over-run or under-run situation occurs in slave mode
 * 			interrupt flag
 * 		I2C_INT_FLAG_PECERR:
 * 			PEC error when receiving data interrupt flag
 * 		I2C_INT_FLAG_SMBTO:
 * 			timeout signal in SMBus mode interrupt flag
 * 		I2C_INT_FLAG_SMBALT:
 * 			SMBus Alert status interrupt flag
 */
void i2c_interrupt_flag_clear(uint32_t i2c_periph,
		enum i2c_interrupt_flag int_flag) {

	uint32_t temp;
	if (int_flag == I2C_INT_FLAG_ADDSEND) {
		// read I2C_STAT0 and then read I2C_STAT1 to clear ADDSEND
		temp = I2C_STAT0(i2c_periph);
		temp = I2C_STAT1(i2c_periph);
	} else {
		I2C_REG_VAL2(i2c_periph, int_flag) &=
			~BIT(I2C_BIT_POS2(int_flag));
	}
}
