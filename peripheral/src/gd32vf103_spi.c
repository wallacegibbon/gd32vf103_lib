#include "gd32vf103_spi.h"

// SPI parameter initialization mask
#define SPI_INIT_MASK			((uint32_t) 0x00003040U)
// I2S parameter initialization mask
#define I2S_INIT_MASK			((uint32_t) 0x0000F047U)

// I2S clock source selection, multiplication and division mask

// I2S1 clock source selection
#define I2S1_CLOCK_SEL			((uint32_t) 0x00020000U)
// I2S2 clock source selection
#define I2S2_CLOCK_SEL			((uint32_t) 0x00040000U)
// I2S clock multiplication mask
#define I2S_CLOCK_MUL_MASK		((uint32_t) 0x0000F000U)
// I2S clock division mask
#define I2S_CLOCK_DIV_MASK		((uint32_t) 0x000000F0U)

// default value of SPI_I2SPSC register
#define SPI_I2SPSC_DEFAULT_VALUE 	((uint32_t) 0x00000002U)

// PREDV1 offset in RCU_CFG1
#define RCU_CFG1_PREDV1_OFFSET		4U
// PLL2MF offset in RCU_CFG1
#define RCU_CFG1_PLL2MF_OFFSET		12U

void spi_i2s_deinit(uint32_t spi_periph) {
	switch (spi_periph) {
	case SPI0:
		rcu_periph_reset_enable(RCU_SPI0RST);
		rcu_periph_reset_disable(RCU_SPI0RST);
		break;
	case SPI1:
		rcu_periph_reset_enable(RCU_SPI1RST);
		rcu_periph_reset_disable(RCU_SPI1RST);
		break;
	case SPI2:
		rcu_periph_reset_enable(RCU_SPI2RST);
		rcu_periph_reset_disable(RCU_SPI2RST);
		break;
	default:
		break;
	}
}

void spi_struct_para_init(struct spi_param * spi_struct) {
	spi_struct->device_mode = SPI_SLAVE;
	spi_struct->trans_mode = SPI_TRANSMODE_FULLDUPLEX;
	spi_struct->frame_size = SPI_FRAMESIZE_8BIT;
	spi_struct->nss = SPI_NSS_HARD;
	spi_struct->clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
	spi_struct->prescale = SPI_PSC_2;
}

/*
 * spi_periph: SPIx(x=0,1,2)
 * spi_struct: SPI parameter initialization stuct members of the structure
 * 	device_mode:
 * 		SPI_MASTER, SPI_SLAVE
 * 	trans_mode:
 * 		SPI_TRANSMODE_FULLDUPLEX, SPI_TRANSMODE_RECEIVEONLY,
 * 		SPI_TRANSMODE_BDRECEIVE, SPI_TRANSMODE_BDTRANSMIT
 * 	frame_size:
 * 		SPI_FRAMESIZE_16BIT, SPI_FRAMESIZE_8BIT
 * 	nss:
 * 		SPI_NSS_SOFT, SPI_NSS_HARD
 * 	endian:
 * 		SPI_ENDIAN_MSB, SPI_ENDIAN_LSB
 * 	clock_polarity_phase:
 * 		SPI_CK_PL_LOW_PH_1EDGE, SPI_CK_PL_HIGH_PH_1EDGE,
 * 		SPI_CK_PL_LOW_PH_2EDGE, SPI_CK_PL_HIGH_PH_2EDGE
 * 	prescale:
 * 		SPI_PSC_n (n=2,4,8,16,32,64,128,256)
 */
void spi_init(uint32_t spi_periph, struct spi_param * spi_struct) {
	uint32_t reg = SPI_CTL0(spi_periph);

	reg &= SPI_INIT_MASK;

	reg |= spi_struct->device_mode;
	reg |= spi_struct->trans_mode;
	reg |= spi_struct->frame_size;
	reg |= spi_struct->nss;
	reg |= spi_struct->endian;
	reg |= spi_struct->clock_polarity_phase;
	reg |= spi_struct->prescale;

	SPI_CTL0(spi_periph) = reg;

	SPI_I2SCTL(spi_periph) &= ~SPI_I2SCTL_I2SSEL;
}

void spi_enable(uint32_t spi_periph) {
	SPI_CTL0(spi_periph) |= SPI_CTL0_SPIEN;
}

void spi_disable(uint32_t spi_periph) {
	SPI_CTL0(spi_periph) &= ~SPI_CTL0_SPIEN;
}

/*
 * spi_periph: SPIx(x=1,2)
 * mode: I2S operation mode
 * 	I2S_MODE_SLAVETX: I2S slave transmit mode
 * 	I2S_MODE_SLAVERX: I2S slave receive mode
 * 	I2S_MODE_MASTERTX: I2S master transmit mode
 * 	I2S_MODE_MASTERRX: I2S master receive mode
 *
 * standard: I2S standard
 * 	I2S_STD_PHILLIPS: I2S phillips standard
 * 	I2S_STD_MSB: I2S MSB standard
 * 	I2S_STD_LSB: I2S LSB standard
 * 	I2S_STD_PCMSHORT: I2S PCM short standard
 * 	I2S_STD_PCMLONG: I2S PCM long standard
 *
 * ckpl: I2S idle state clock polarity
 * 	I2S_CKPL_LOW: I2S clock polarity low level
 * 	I2S_CKPL_HIGH: I2S clock polarity high level
 */
void i2s_init(uint32_t spi_periph, uint32_t mode, uint32_t standard,
		uint32_t ckpl) {

	uint32_t reg = SPI_I2SCTL(spi_periph);

	reg &= I2S_INIT_MASK;

	reg |= SPI_I2SCTL_I2SSEL;
	reg |= mode;
	reg |= standard;
	reg |= ckpl;

	SPI_I2SCTL(spi_periph) = reg;
}

/*
 * configure I2S prescaler
 * spi_periph: SPIx(x=1,2)
 * audiosample: I2S audio sample rate
 * 	I2S_AUDIOSAMPLE_8K: audio sample rate is 8KHz
 * 	I2S_AUDIOSAMPLE_11K: audio sample rate is 11KHz
 * 	I2S_AUDIOSAMPLE_16K: audio sample rate is 16KHz
 * 	I2S_AUDIOSAMPLE_22K: audio sample rate is 22KHz
 * 	I2S_AUDIOSAMPLE_32K: audio sample rate is 32KHz
 * 	I2S_AUDIOSAMPLE_44K: audio sample rate is 44KHz
 * 	I2S_AUDIOSAMPLE_48K: audio sample rate is 48KHz
 * 	I2S_AUDIOSAMPLE_96K: audio sample rate is 96KHz
 * 	I2S_AUDIOSAMPLE_192K: audio sample rate is 192KHz
 *
 * frameformat: I2S data length and channel length
 * 	I2S_FRAMEFORMAT_DT16B_CH16B:
 * 		I2S data length is 16 bit and channel length is 16 bit
 * 	I2S_FRAMEFORMAT_DT16B_CH32B:
 * 		I2S data length is 16 bit and channel length is 32 bit
 * 	I2S_FRAMEFORMAT_DT24B_CH32B:
 * 		I2S data length is 24 bit and channel length is 32 bit
 * 	I2S_FRAMEFORMAT_DT32B_CH32B:
 * 		I2S data length is 32 bit and channel length is 32 bit
 *
 * mckout: I2S master clock output
 * 	I2S_MCKOUT_ENABLE: I2S master clock output enable
 * 	I2S_MCKOUT_DISABLE: I2S master clock output disable
*/
void i2s_psc_config(uint32_t spi_periph, uint32_t audiosample,
		uint32_t frameformat, uint32_t mckout) {

	uint32_t clks = 0U;
	uint32_t i2sclock = 0U;

	// deinit SPI_I2SPSC register
	SPI_I2SPSC(spi_periph) = SPI_I2SPSC_DEFAULT_VALUE;

	// get the I2S clock source
	if (spi_periph == SPI1)
		clks = I2S1_CLOCK_SEL;
	else
		clks = I2S2_CLOCK_SEL;

	if (RCU_CFG1 & clks) {
		// get RCU PLL2 clock multiplication factor
		clks = (RCU_CFG1 & I2S_CLOCK_MUL_MASK) >>
			RCU_CFG1_PLL2MF_OFFSET;

		if (clks > 5 && clks < 15) {
			// multiplier is between 8 and 16
			clks += 2;
		} else {
			if (clks == 15) {
				// multiplier is 20
				clks = 20;
			}
		}

		// get the PREDV1 value
		i2sclock =
			((RCU_CFG1 & I2S_CLOCK_DIV_MASK) >>
			 RCU_CFG1_PREDV1_OFFSET) + 1;

		// calculate I2S clock based on PLL2 and PREDV1
		i2sclock = HXTAL_VALUE / i2sclock * clks * 2;
	} else {
		// get system clock
		i2sclock = rcu_clock_freq_get(CK_SYS);
	}

	// config the prescaler depending on the mclk output state,
	// the frame format and audio sample rate
	if (mckout == I2S_MCKOUT_ENABLE) {
		clks = i2sclock / 256 * 10 / audiosample;
	} else {
		if (frameformat == I2S_FRAMEFORMAT_DT16B_CH16B)
			clks = i2sclock / 32 * 10 / audiosample;
		else
			clks = i2sclock / 64 * 10 / audiosample;
	}

	// remove the floating point
	clks = (clks + 5U) / 10U;
	uint32_t i2sof = clks & 0x00000001U;
	uint32_t i2sdiv = (clks - i2sof) / 2;
	i2sof = i2sof << 8;

	// set the default values
	if (i2sdiv < 2 || i2sdiv > 255) {
		i2sdiv = 2;
		i2sof = 0;
	}

	// configure SPI_I2SPSC
	SPI_I2SPSC(spi_periph) = i2sdiv | i2sof | mckout;

	// clear SPI_I2SCTL_DTLEN and SPI_I2SCTL_CHLEN bits
	SPI_I2SCTL(spi_periph) &= ~(SPI_I2SCTL_DTLEN | SPI_I2SCTL_CHLEN);
	// configure data frame format
	SPI_I2SCTL(spi_periph) |= frameformat;
}

void i2s_enable(uint32_t spi_periph) {
	SPI_I2SCTL(spi_periph) |= SPI_I2SCTL_I2SEN;
}

void i2s_disable(uint32_t spi_periph) {
	SPI_I2SCTL(spi_periph) &= ~SPI_I2SCTL_I2SEN;
}

void spi_nss_output_enable(uint32_t spi_periph) {
	SPI_CTL1(spi_periph) |= SPI_CTL1_NSSDRV;
}

void spi_nss_output_disable(uint32_t spi_periph) {
	SPI_CTL1(spi_periph) &= ~SPI_CTL1_NSSDRV;
}

void spi_nss_internal_high(uint32_t spi_periph) {
	SPI_CTL0(spi_periph) |= SPI_CTL0_SWNSS;
}

void spi_nss_internal_low(uint32_t spi_periph) {
	SPI_CTL0(spi_periph) &= ~SPI_CTL0_SWNSS;
}

/*
 * dma: SPI DMA mode
 * 	SPI_DMA_TRANSMIT: SPI transmit data using DMA
 * 	SPI_DMA_RECEIVE: SPI receive data using DMA
*/
void spi_dma_enable(uint32_t spi_periph, uint8_t dma) {
	if (dma == SPI_DMA_TRANSMIT)
		SPI_CTL1(spi_periph) |= SPI_CTL1_DMATEN;
	else
		SPI_CTL1(spi_periph) |= SPI_CTL1_DMAREN;
}

void spi_dma_disable(uint32_t spi_periph, uint8_t dma) {
	if (dma == SPI_DMA_TRANSMIT)
		SPI_CTL1(spi_periph) &= ~SPI_CTL1_DMATEN;
	else
		SPI_CTL1(spi_periph) &= ~SPI_CTL1_DMAREN;
}

/*
 * frame_format: SPI frame size
 * 	SPI_FRAMESIZE_16BIT: SPI frame size is 16 bits
 * 	SPI_FRAMESIZE_8BIT: SPI frame size is 8 bits
 */
void spi_i2s_data_frame_format_config(uint32_t spi_periph,
		uint16_t frame_format) {

	// clear SPI_CTL0_FF16 bit
	SPI_CTL0(spi_periph) &= ~SPI_CTL0_FF16;
	// configure SPI_CTL0_FF16 bit
	SPI_CTL0(spi_periph) |= frame_format;
}

void spi_i2s_data_transmit(uint32_t spi_periph, uint16_t data) {
	SPI_DATA(spi_periph) = data;
}

uint16_t spi_i2s_data_receive(uint32_t spi_periph) {
	return (uint16_t) SPI_DATA(spi_periph);
}

/*
 * transfer_direction: SPI transfer direction
 * 	SPI_BIDIRECTIONAL_TRANSMIT: SPI work in transmit-only mode
 * 	SPI_BIDIRECTIONAL_RECEIVE: SPI work in receive-only mode
 */
void spi_bidirectional_transfer_config(uint32_t spi_periph,
				       uint32_t transfer_direction) {
	if (transfer_direction == SPI_BIDIRECTIONAL_TRANSMIT)
		// set the transmit-only mode
		SPI_CTL0(spi_periph) |= SPI_BIDIRECTIONAL_TRANSMIT;
	else
		// set the receive-only mode
		SPI_CTL0(spi_periph) &= SPI_BIDIRECTIONAL_RECEIVE;
}

void spi_crc_polynomial_set(uint32_t spi_periph, uint16_t crc_poly) {
	SPI_CTL0(spi_periph) |= SPI_CTL0_CRCEN;
	SPI_CRCPOLY(spi_periph) = crc_poly;
}

uint16_t spi_crc_polynomial_get(uint32_t spi_periph) {
	return (uint16_t) SPI_CRCPOLY(spi_periph);
}

void spi_crc_on(uint32_t spi_periph) {
	SPI_CTL0(spi_periph) |= SPI_CTL0_CRCEN;
}

void spi_crc_off(uint32_t spi_periph) {
	SPI_CTL0(spi_periph) &= ~SPI_CTL0_CRCEN;
}

void spi_crc_next(uint32_t spi_periph) {
	SPI_CTL0(spi_periph) |= SPI_CTL0_CRCNT;
}

/*
 * get SPI CRC send value or receive value
 *
 * crc: SPI crc value
 * 	SPI_CRC_TX: get transmit crc value
 * 	SPI_CRC_RX: get receive crc value
 */
uint16_t spi_crc_get(uint32_t spi_periph, uint8_t crc) {
	if (crc == SPI_CRC_TX)
		return (uint16_t) (SPI_TCRC(spi_periph));
	else
		return (uint16_t) (SPI_RCRC(spi_periph));
}

void spi_ti_mode_enable(uint32_t spi_periph) {
	SPI_CTL1(spi_periph) |= SPI_CTL1_TMOD;
}

void spi_ti_mode_disable(uint32_t spi_periph) {
	SPI_CTL1(spi_periph) &= ~SPI_CTL1_TMOD;
}

/* enable SPI NSS pulse mode */
void spi_nssp_mode_enable(uint32_t spi_periph) {
	SPI_CTL1(spi_periph) |= SPI_CTL1_NSSP;
}

void spi_nssp_mode_disable(uint32_t spi_periph) {
	SPI_CTL1(spi_periph) &= ~SPI_CTL1_NSSP;
}

/*
 * interrupt: SPI/I2S interrupt
 * 	SPI_I2S_INT_TBE:
 * 		transmit buffer empty interrupt
 * 	SPI_I2S_INT_RBNE:
 * 		receive buffer not empty interrupt
 * 	SPI_I2S_INT_ERR:
 * 		CRC error,configuration error,reception overrun error,
 * 		transmission underrun error and format error interrupt
*/
void spi_i2s_interrupt_enable(uint32_t spi_periph, uint8_t interrupt) {
	switch (interrupt) {
	// SPI/I2S transmit buffer empty interrupt
	case SPI_I2S_INT_TBE:
		SPI_CTL1(spi_periph) |= SPI_CTL1_TBEIE;
		break;
	// SPI/I2S receive buffer not empty interrupt
	case SPI_I2S_INT_RBNE:
		SPI_CTL1(spi_periph) |= SPI_CTL1_RBNEIE;
		break;
	// SPI/I2S error
	case SPI_I2S_INT_ERR:
		SPI_CTL1(spi_periph) |= SPI_CTL1_ERRIE;
		break;
	default:
		break;
	}
}

void spi_i2s_interrupt_disable(uint32_t spi_periph, uint8_t interrupt) {
	switch (interrupt) {
	// SPI/I2S transmit buffer empty interrupt
	case SPI_I2S_INT_TBE:
		SPI_CTL1(spi_periph) &= ~SPI_CTL1_TBEIE;
		break;
	// SPI/I2S receive buffer not empty interrupt
	case SPI_I2S_INT_RBNE:
		SPI_CTL1(spi_periph) &= ~SPI_CTL1_RBNEIE;
		break;
	// SPI/I2S error
	case SPI_I2S_INT_ERR:
		SPI_CTL1(spi_periph) &= ~SPI_CTL1_ERRIE;
		break;
	default:
		break;
	}
}

/*
 * get SPI and I2S interrupt flag status
 * interrupt: SPI/I2S interrupt flag status
 * 	SPI_I2S_INT_FLAG_TBE: transmit buffer empty interrupt flag
 * 	SPI_I2S_INT_FLAG_RBNE: receive buffer not empty interrupt flag
 * 	SPI_I2S_INT_FLAG_RXORERR: overrun interrupt flag
 * 	SPI_INT_FLAG_CONFERR: config error interrupt flag
 * 	SPI_INT_FLAG_CRCERR: CRC error interrupt flag
 * 	I2S_INT_FLAG_TXURERR: underrun error interrupt flag
 * 	SPI_I2S_INT_FLAG_FERR: format error interrupt flag
 */
enum flag_status spi_i2s_interrupt_flag_get(uint32_t spi_periph,
		uint8_t interrupt) {

	uint32_t reg1 = SPI_STAT(spi_periph);
	uint32_t reg2 = SPI_CTL1(spi_periph);

	switch (interrupt) {
	// SPI/I2S transmit buffer empty interrupt
	case SPI_I2S_INT_FLAG_TBE:
		reg1 = reg1 & SPI_STAT_TBE;
		reg2 = reg2 & SPI_CTL1_TBEIE;
		break;
	// SPI/I2S receive buffer not empty interrupt
	case SPI_I2S_INT_FLAG_RBNE:
		reg1 = reg1 & SPI_STAT_RBNE;
		reg2 = reg2 & SPI_CTL1_RBNEIE;
		break;
	// SPI/I2S overrun interrupt
	case SPI_I2S_INT_FLAG_RXORERR:
		reg1 = reg1 & SPI_STAT_RXORERR;
		reg2 = reg2 & SPI_CTL1_ERRIE;
		break;
	// SPI config error interrupt
	case SPI_INT_FLAG_CONFERR:
		reg1 = reg1 & SPI_STAT_CONFERR;
		reg2 = reg2 & SPI_CTL1_ERRIE;
		break;
	// SPI CRC error interrupt
	case SPI_INT_FLAG_CRCERR:
		reg1 = reg1 & SPI_STAT_CRCERR;
		reg2 = reg2 & SPI_CTL1_ERRIE;
		break;
	// I2S underrun error interrupt
	case I2S_INT_FLAG_TXURERR:
		reg1 = reg1 & SPI_STAT_TXURERR;
		reg2 = reg2 & SPI_CTL1_ERRIE;
		break;
	// SPI/I2S format error interrupt
	case SPI_I2S_INT_FLAG_FERR:
		reg1 = reg1 & SPI_STAT_FERR;
		reg2 = reg2 & SPI_CTL1_ERRIE;
		break;
	default:
		break;
	}

	// get SPI/I2S interrupt flag status
	if (reg1 && reg2)
		return SET;
	else
		return RESET;
}

/*
 * flag: SPI/I2S flag status
 * 	SPI_FLAG_TBE: transmit buffer empty flag
 * 	SPI_FLAG_RBNE: receive buffer not empty flag
 * 	SPI_FLAG_TRANS: transmit on-going flag
 * 	SPI_FLAG_RXORERR: receive overrun error flag
 * 	SPI_FLAG_CONFERR: mode config error flag
 * 	SPI_FLAG_CRCERR: CRC error flag
 * 	SPI_FLAG_FERR: format error interrupt flag
 * 	I2S_FLAG_TBE: transmit buffer empty flag
 * 	I2S_FLAG_RBNE: receive buffer not empty flag
 * 	I2S_FLAG_TRANS: transmit on-going flag
 * 	I2S_FLAG_RXORERR: overrun error flag
 * 	I2S_FLAG_TXURERR: underrun error flag
 * 	I2S_FLAG_CH: channel side flag
 * 	I2S_FLAG_FERR: format error interrupt flag
 */
enum flag_status spi_i2s_flag_get(uint32_t spi_periph, uint32_t flag) {
	if (SPI_STAT(spi_periph) & flag != RESET)
		return SET;
	else
		return RESET;
}

void spi_crc_error_clear(uint32_t spi_periph) {
	SPI_STAT(spi_periph) &= ~SPI_FLAG_CRCERR;
}
