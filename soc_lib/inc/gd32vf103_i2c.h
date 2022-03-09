#ifndef __GD32VF103_I2C_H
#define __GD32VF103_I2C_H

#include "gd32vf103.h"

#define I2C0				I2C_BASE
#define I2C1				(I2C_BASE + 0x00000400U)

#define I2C_CTL0(i2cx)			REG32((i2cx) + 0x00U)
#define I2C_CTL1(i2cx)			REG32((i2cx) + 0x04U)

// I2C slave address register
#define I2C_SADDR0(i2cx)		REG32((i2cx) + 0x08U)
#define I2C_SADDR1(i2cx)		REG32((i2cx) + 0x0CU)

// I2C transfer buffer register
#define I2C_DATA(i2cx)			REG32((i2cx) + 0x10U)

// I2C transfer status register
#define I2C_STAT0(i2cx)			REG32((i2cx) + 0x14U)
#define I2C_STAT1(i2cx)			REG32((i2cx) + 0x18U)

// I2C clock configure register
#define I2C_CKCFG(i2cx)			REG32((i2cx) + 0x1CU)

// I2C rise time register
#define I2C_RT(i2cx)			REG32((i2cx) + 0x20U)

// I2C fast-mode-plus configure register
#define I2C_FMPCFG(i2cx)		REG32((i2cx) + 0x90U)

// peripheral enable
#define I2C_CTL0_I2CEN			BIT(0)

// SMBus mode
#define I2C_CTL0_SMBEN			BIT(1)

// SMBus type
#define I2C_CTL0_SMBSEL			BIT(3)

// ARP enable
#define I2C_CTL0_ARPEN			BIT(4)

// PEC enable
#define I2C_CTL0_PECEN			BIT(5)

// general call enable
#define I2C_CTL0_GCEN			BIT(6)

// clock stretching disable (slave mode)
#define I2C_CTL0_SS			BIT(7)

// start generation
#define I2C_CTL0_START			BIT(8)

// stop generation
#define I2C_CTL0_STOP			BIT(9)

// acknowledge enable
#define I2C_CTL0_ACKEN			BIT(10)

// acknowledge/PEC position (for data reception)
#define I2C_CTL0_POAP			BIT(11)

// packet error checking
#define I2C_CTL0_PECTRANS		BIT(12)

// SMBus alert
#define I2C_CTL0_SALT			BIT(13)

// software reset
#define I2C_CTL0_SRESET			BIT(15)


// I2CCLK[5:0] bits (peripheral clock frequency)
#define I2C_CTL1_I2CCLK			BITS(0, 5)

// error interrupt enable
#define I2C_CTL1_ERRIE			BIT(8)

// event interrupt enable
#define I2C_CTL1_EVIE			BIT(9)

// buffer interrupt enable
#define I2C_CTL1_BUFIE			BIT(10)

// DMA requests enable
#define I2C_CTL1_DMAON			BIT(11)

// DMA last transfer
#define I2C_CTL1_DMALST			BIT(12)


// bit 0 of a 10-bit address
#define I2C_SADDR0_ADDRESS0		BIT(0)

// 7-bit address or bits 7:1 of a 10-bit address
#define I2C_SADDR0_ADDRESS		BITS(1, 7)

// highest two bits of a 10-bit address
#define I2C_SADDR0_ADDRESS_H		BITS(8, 9)

// address mode for the I2C slave
#define I2C_SADDR0_ADDFORMAT		BIT(15)


// aual-address mode switch
#define I2C_SADDR1_DUADEN		BIT(0)

// second I2C address for the slave in dual-address mode
#define I2C_SADDR1_ADDRESS2		BITS(1, 7)


// 8-bit data register
#define I2C_DATA_TRB			BITS(0, 7)


// start bit (master mode)
#define I2C_STAT0_SBSEND		BIT(0)

// address sent (master mode)/matched (slave mode)
#define I2C_STAT0_ADDSEND		BIT(1)

// byte transfer finished
#define I2C_STAT0_BTC			BIT(2)

// 10-bit header sent (master mode)
#define I2C_STAT0_ADD10SEND		BIT(3)

// stop detection (slave mode)
#define I2C_STAT0_STPDET		BIT(4)

// data register not empty (receivers)
#define I2C_STAT0_RBNE			BIT(6)

// data register empty (transmitters)
#define I2C_STAT0_TBE			BIT(7)

// bus error
#define I2C_STAT0_BERR			BIT(8)

// arbitration lost (master mode)
#define I2C_STAT0_LOSTARB		BIT(9)

// acknowledge failure
#define I2C_STAT0_AERR			BIT(10)

// overrun/underrun
#define I2C_STAT0_OUERR			BIT(11)

// PEC error in reception
#define I2C_STAT0_PECERR		BIT(12)

// timeout signal in SMBus mode
#define I2C_STAT0_SMBTO			BIT(14)

// SMBus alert status
#define I2C_STAT0_SMBALT		BIT(15)


// master/slave
#define I2C_STAT1_MASTER		BIT(0)

// bus busy
#define I2C_STAT1_I2CBSY		BIT(1)

// transmitter/receiver
#define I2C_STAT1_TR			BIT(2)

// general call address (slave mode)
#define I2C_STAT1_RXGC			BIT(4)

// SMBus device default address (slave mode)
#define I2C_STAT1_DEFSMB		BIT(5)

// SMBus host header (slave mode)
#define I2C_STAT1_HSTSMB		BIT(6)

// dual flag (slave mode)
#define I2C_STAT1_DUMODF		BIT(7)

// packet error checking value
#define I2C_STAT1_PECV			BITS(8, 15)


// clock control register in fast/standard mode (master mode)
#define I2C_CKCFG_CLKC			BITS(0, 11)

// fast mode duty cycle
#define I2C_CKCFG_DTCY			BIT(14)

// I2C speed selection in master mode
#define I2C_CKCFG_FAST			BIT(15)


// maximum rise time in fast/standard mode (Master mode)
#define I2C_RT_RISETIME			BITS(0, 5)


// fast mode plus enable bit
#define I2C_FMPCFG_FMPEN		BIT(0)


// define the I2C bit position and its register index offset
#define I2C_REGIDX_BIT(regidx, bitpos) \
	(((uint32_t) (regidx) << 6) | (uint32_t)(bitpos))

#define I2C_REG_VAL(i2cx, offset) \
	(REG32((i2cx) + (((uint32_t) (offset) & 0xFFFFU) >> 6)))

#define I2C_BIT_POS(val) \
	((uint32_t) (val) & 0x1FU)

#define I2C_REGIDX_BIT2(regidx, bitpos, regidx2, bitpos2) \
	(((uint32_t) (regidx2) << 22) | (uint32_t) ((bitpos2) << 16) | \
	 (((uint32_t) (regidx) << 6) | (uint32_t) (bitpos)))

#define I2C_REG_VAL2(i2cx, offset) \
	(REG32((i2cx) + ((uint32_t) (offset) >> 22)))

#define I2C_BIT_POS2(val) \
	(((uint32_t) (val) & 0x1F0000U) >> 16)

#define I2C_CTL1_REG_OFFSET		0x04U
#define I2C_STAT0_REG_OFFSET		0x14U
#define I2C_STAT1_REG_OFFSET		0x18U

enum i2c_flag {
	// flags in STAT0 register

	// start condition sent out in master mode
	I2C_FLAG_SBSEND = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 0U),

	// address is sent in master mode or received and matches in slave mode
	I2C_FLAG_ADDSEND = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 1U),

	// byte transmission finishes
	I2C_FLAG_BTC = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 2U),

	// header of 10-bit address is sent in master mode
	I2C_FLAG_ADD10SEND = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 3U),

	// stop condition detected in slave mode
	I2C_FLAG_STPDET = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 4U),

	// I2C_DATA is not Empty during receiving
	I2C_FLAG_RBNE = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 6U),

	// I2C_DATA is empty during transmitting
	I2C_FLAG_TBE = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 7U),

	// a bus error occurs indication a unexpected start or stop condition
	// on I2C bus
	I2C_FLAG_BERR = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 8U),

	// arbitration lost in master mode
	I2C_FLAG_LOSTARB = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 9U),

	// acknowledge error
	I2C_FLAG_AERR = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 10U),

	// over-run or under-run situation occurs in slave mode
	I2C_FLAG_OUERR = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 11U),

	// PEC error when receiving data
	I2C_FLAG_PECERR = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 12U),

	// timeout signal in SMBus mode
	I2C_FLAG_SMBTO = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 14U),

	// SMBus alert status
	I2C_FLAG_SMBALT = I2C_REGIDX_BIT(I2C_STAT0_REG_OFFSET, 15U),

	// flags in STAT1 register

	// a flag indicating whether I2C block is in master or slave mode
	I2C_FLAG_MASTER = I2C_REGIDX_BIT(I2C_STAT1_REG_OFFSET, 0U),

	// busy flag
	I2C_FLAG_I2CBSY = I2C_REGIDX_BIT(I2C_STAT1_REG_OFFSET, 1U),

	// whether the I2C is a transmitter or a receiver
	I2C_FLAG_TR = I2C_REGIDX_BIT(I2C_STAT1_REG_OFFSET, 2U),

	// general call address (00h) received
	I2C_FLAG_RXGC = I2C_REGIDX_BIT(I2C_STAT1_REG_OFFSET, 4U),

	// default address of SMBus device
	I2C_FLAG_DEFSMB = I2C_REGIDX_BIT(I2C_STAT1_REG_OFFSET, 5U),

	// SMBus host header detected in slave mode
	I2C_FLAG_HSTSMB = I2C_REGIDX_BIT(I2C_STAT1_REG_OFFSET, 6U),

	// dual flag in slave mode indicating which address is matched in
	// dual-address mode
	I2C_FLAG_DUMODF = I2C_REGIDX_BIT(I2C_STAT1_REG_OFFSET, 7U),

};

enum i2c_interrupt_flag {
	// interrupt flags in CTL1 register

	// start condition sent out in master mode interrupt flag
	I2C_INT_FLAG_SBSEND =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 9U,
				I2C_STAT0_REG_OFFSET, 0U),

	// address is sent in master mode or received and matches in slave
	// mode interrupt flag
	I2C_INT_FLAG_ADDSEND =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 9U,
				I2C_STAT0_REG_OFFSET, 1U),

	// byte transmission finishes
	I2C_INT_FLAG_BTC =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 9U,
				I2C_STAT0_REG_OFFSET, 2U),

	// header of 10-bit address is sent in master mode interrupt flag
	I2C_INT_FLAG_ADD10SEND =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 9U,
				I2C_STAT0_REG_OFFSET, 3U),

	// stop condition detected in slave mode interrupt flag
	I2C_INT_FLAG_STPDET =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 9U,
				I2C_STAT0_REG_OFFSET, 4U),

	// I2C_DATA is not Empty during receiving interrupt flag
	I2C_INT_FLAG_RBNE =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 9U,
				I2C_STAT0_REG_OFFSET, 6U),

	// I2C_DATA is empty during transmitting interrupt flag
	I2C_INT_FLAG_TBE =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 9U,
				I2C_STAT0_REG_OFFSET, 7U),

	// a bus error occurs indication a unexpected start or stop condition
	// on I2C bus interrupt flag
	I2C_INT_FLAG_BERR =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 8U,
				I2C_STAT0_REG_OFFSET, 8U),

	// arbitration lost in master mode interrupt flag
	I2C_INT_FLAG_LOSTARB =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 8U,
				I2C_STAT0_REG_OFFSET, 9U),

	// acknowledge error interrupt flag
	I2C_INT_FLAG_AERR =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 8U,
				I2C_STAT0_REG_OFFSET, 10U),

	// over-run or under-run situation occurs in slave mode interrupt flag
	I2C_INT_FLAG_OUERR =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 8U,
				I2C_STAT0_REG_OFFSET, 11U),

	// PEC error when receiving data interrupt flag
	I2C_INT_FLAG_PECERR =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 8U,
				I2C_STAT0_REG_OFFSET, 12U),

	// timeout signal in SMBus mode interrupt flag
	I2C_INT_FLAG_SMBTO =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 8U,
				I2C_STAT0_REG_OFFSET, 14U),

	// SMBus Alert status interrupt flag
	I2C_INT_FLAG_SMBALT =
		I2C_REGIDX_BIT2(I2C_CTL1_REG_OFFSET, 8U,
				I2C_STAT0_REG_OFFSET, 15U),

};

enum i2c_interrupt {
	// error interrupt enable
	I2C_INT_ERR = I2C_REGIDX_BIT(I2C_CTL1_REG_OFFSET, 8U),

	// event interrupt enable
	I2C_INT_EV = I2C_REGIDX_BIT(I2C_CTL1_REG_OFFSET, 9U),

	// buffer interrupt enable
	I2C_INT_BUF = I2C_REGIDX_BIT(I2C_CTL1_REG_OFFSET, 10U),
};


// SMBus/I2C mode switch and SMBus type selection
// I2C mode
#define I2C_I2CMODE_ENABLE		((uint32_t) 0x00000000U)

// SMBus mode
#define I2C_SMBUSMODE_ENABLE		I2C_CTL0_SMBEN


// SMBus/I2C mode switch and SMBus type selection
// SMBus mode device type
#define I2C_SMBUS_DEVICE      		((uint32_t) 0x00000000U)
// SMBus mode host type
#define I2C_SMBUS_HOST			I2C_CTL0_SMBSEL


// I2C transfer direction
// receiver
#define I2C_RECEIVER			((uint32_t) 0x00000001U)
// transmitter
#define I2C_TRANSMITTER			((uint32_t) 0xFFFFFFFEU)


// whether or not to send an ACK
// ACK will be not sent
#define I2C_ACK_DISABLE			((uint32_t) 0x00000000U)
// ACK will be sent
#define I2C_ACK_ENABLE			((uint32_t) 0x00000001U)


// I2C POAP position
// ACKEN bit decides whether or not to send ACK for the next byte
#define I2C_ACKPOS_NEXT			((uint32_t) 0x00000000U)
// ACKEN bit decides whether or not to send ACK or not for the current byte
#define I2C_ACKPOS_CURRENT		((uint32_t) 0x00000001U)


// I2C dual-address mode switch
// dual-address mode disabled
#define I2C_DUADEN_DISABLE		((uint32_t) 0x00000000U)
// dual-address mode enabled
#define I2C_DUADEN_ENABLE		((uint32_t) 0x00000001U)


// whether or not to stretch SCL low
#define I2C_SCLSTRETCH_ENABLE		((uint32_t) 0x00000000U)
#define I2C_SCLSTRETCH_DISABLE		I2C_CTL0_SS


// whether or not to response to a general call

// slave will response to a general call
#define I2C_GCEN_ENABLE			I2C_CTL0_GCEN

// slave will not response to a general call
#define I2C_GCEN_DISABLE		((uint32_t) 0x00000000U)


// software reset I2C
// I2C is under reset
#define I2C_SRESET_SET			I2C_CTL0_SRESET

// I2C is not under reset
#define I2C_SRESET_RESET		((uint32_t) 0x00000000U)


// DMA mode enabled
#define I2C_DMA_ON			I2C_CTL1_DMAON
// DMA mode disabled
#define I2C_DMA_OFF			((uint32_t) 0x00000000U)


// flag indicating DMA last transfer
// next DMA EOT is the last transfer
#define I2C_DMALST_ON			I2C_CTL1_DMALST

// next DMA EOT is not the last transfer
#define I2C_DMALST_OFF			((uint32_t) 0x00000000U)


// PEC calculation on
#define I2C_PEC_ENABLE			I2C_CTL0_PECEN
// PEC calculation off
#define I2C_PEC_DISABLE			((uint32_t) 0x00000000U)


// transfer PEC
#define I2C_PECTRANS_ENABLE		I2C_CTL0_PECTRANS
// not transfer PEC value
#define I2C_PECTRANS_DISABLE		((uint32_t) 0x00000000U)


// I2C SMBus configure
// issue alert through SMBA pin
#define I2C_SALTSEND_ENABLE		I2C_CTL0_SALT
// not issue alert through SMBA
#define I2C_SALTSEND_DISABLE		((uint32_t) 0x00000000U)


// ARP protocol in SMBus switch
// ARP enable
#define I2C_ARP_ENABLE			I2C_CTL0_ARPEN
// ARP disable
#define I2C_ARP_DISABLE			((uint32_t) 0x00000000U)


// transmit I2C data
#define DATA_TRANS(regval) \
	(BITS(0, 7) & ((uint32_t) (regval) << 0))

// receive I2C data
#define DATA_RECV(regval) \
	GET_BITS((uint32_t) (regval), 0, 7)

// I2C duty cycle in fast mode
// I2C fast mode Tlow/Thigh = 2
#define I2C_DTCY_2			((uint32_t) 0x00000000U)

// I2C fast mode Tlow/Thigh = 16/9
#define I2C_DTCY_16_9			I2C_CKCFG_DTCY


// address mode for the I2C slave
// address:7 bits
#define I2C_ADDFORMAT_7BITS		((uint32_t) 0x00000000U)
// address:10 bits
#define I2C_ADDFORMAT_10BITS		I2C_SADDR0_ADDFORMAT


void i2c_deinit(uint32_t i2c_periph);

void i2c_clock_config(uint32_t i2c_periph, uint32_t clkspeed, uint32_t dutycyc);

void i2c_mode_addr_config(uint32_t i2c_periph, uint32_t mode,
		uint32_t addformat, uint32_t addr);

void i2c_smbus_type_config(uint32_t i2c_periph, uint32_t type);

// whether or not to send an ACK
void i2c_ack_config(uint32_t i2c_periph, uint32_t ack);

// configure I2C POAP position
void i2c_ackpos_config(uint32_t i2c_periph, uint32_t pos);
// master sends slave address
//
void i2c_master_addressing(uint32_t i2c_periph, uint32_t addr,
		uint32_t trandirection);

void i2c_dualaddr_enable(uint32_t i2c_periph, uint32_t dualaddr);
void i2c_dualaddr_disable(uint32_t i2c_periph);

void i2c_enable(uint32_t i2c_periph);
void i2c_disable(uint32_t i2c_periph);

// generate a START condition on I2C bus
void i2c_start_on_bus(uint32_t i2c_periph);

// generate a STOP condition on I2C bus
void i2c_stop_on_bus(uint32_t i2c_periph);

// I2C transmit data function
void i2c_data_transmit(uint32_t i2c_periph, uint8_t data);

// I2C receive data function
uint8_t i2c_data_receive(uint32_t i2c_periph);

// enable I2C DMA mode
void i2c_dma_enable(uint32_t i2c_periph, uint32_t dmastate);

// configure whether next DMA EOT is DMA last transfer or not
void i2c_dma_last_transfer_config(uint32_t i2c_periph, uint32_t dmalast);

// whether to stretch SCL low when data is not ready in slave mode
void i2c_stretch_scl_low_config(uint32_t i2c_periph, uint32_t stretchpara);

// whether or not to response to a general call
void i2c_slave_response_to_gcall_config(uint32_t i2c_periph,
		uint32_t gcallpara);

void i2c_software_reset_config(uint32_t i2c_periph, uint32_t sreset);

// I2C PEC calculation on or off
void i2c_pec_enable(uint32_t i2c_periph, uint32_t pecstate);

// I2C whether to transfer PEC value
void i2c_pec_transfer_enable(uint32_t i2c_periph, uint32_t pecpara);

// packet error checking value
uint8_t i2c_pec_value_get(uint32_t i2c_periph);

// I2C issue alert through SMBA pin
void i2c_smbus_issue_alert(uint32_t i2c_periph, uint32_t smbuspara);

// I2C ARP protocol in SMBus switch
void i2c_smbus_arp_enable(uint32_t i2c_periph, uint32_t arpstate);

// check I2C flag is set or not
enum flag_status i2c_flag_get(uint32_t i2c_periph, enum i2c_flag flag);

void i2c_flag_clear(uint32_t i2c_periph, enum i2c_flag flag);

void i2c_interrupt_enable(uint32_t i2c_periph, enum i2c_interrupt interrupt);
void i2c_interrupt_disable(uint32_t i2c_periph, enum i2c_interrupt interrupt);

// check I2C interrupt flag
enum flag_status i2c_interrupt_flag_get(uint32_t i2c_periph,
		enum i2c_interrupt_flag int_flag);

// clear I2C interrupt flag
void i2c_interrupt_flag_clear(uint32_t i2c_periph,
		enum i2c_interrupt_flag int_flag);

#endif
