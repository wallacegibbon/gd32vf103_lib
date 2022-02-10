#ifndef GD32VF103_SPI_H
#define GD32VF103_SPI_H

#include "gd32vf103.h"

#define SPI0				(SPI_BASE + 0x0000F800U)
#define SPI1				SPI_BASE
#define SPI2				(SPI_BASE + 0x00000400U)

#define SPI_CTL0(spix)			REG32((spix) + 0x00U)
#define SPI_CTL1(spix)			REG32((spix) + 0x04U)
#define SPI_STAT(spix)			REG32((spix) + 0x08U)
#define SPI_DATA(spix)			REG32((spix) + 0x0CU)

// SPI CRC polynomial register
#define SPI_CRCPOLY(spix)		REG32((spix) + 0x10U)

// SPI receive CRC register
#define SPI_RCRC(spix)			REG32((spix) + 0x14U)
// SPI transmit CRC register
#define SPI_TCRC(spix)			REG32((spix) + 0x18U)

#define SPI_I2SCTL(spix)		REG32((spix) + 0x1CU)
// SPI I2S clock prescaler register
#define SPI_I2SPSC(spix)		REG32((spix) + 0x20U)


// clock phase selection
#define SPI_CTL0_CKPH			BIT(0)

// clock polarity selection
#define SPI_CTL0_CKPL			BIT(1)

// master mode enable
#define SPI_CTL0_MSTMOD			BIT(2)

// master clock prescaler selection
#define SPI_CTL0_PSC			BITS(3, 5)

// SPI enabl
#define SPI_CTL0_SPIEN			BIT(6)

// LSB first mode
#define SPI_CTL0_LF			BIT(7)

// NSS pin selection in NSS software mode
#define SPI_CTL0_SWNSS			BIT(8)

// NSS software mode selection
#define SPI_CTL0_SWNSSEN		BIT(9)

// receive only
#define SPI_CTL0_RO			BIT(10)

// data frame size
#define SPI_CTL0_FF16			BIT(11)

// CRC next transfer
#define SPI_CTL0_CRCNT			BIT(12)

// CRC calculation enable
#define SPI_CTL0_CRCEN			BIT(13)

// bidirectional transmit output enabl
#define SPI_CTL0_BDOEN			BIT(14)

// bidirectional enable
#define SPI_CTL0_BDEN			BIT(15)



// receive buffer dma enable
#define SPI_CTL1_DMAREN			BIT(0)
// transmit buffer dma enable
#define SPI_CTL1_DMATEN			BIT(1)

// drive NSS output
#define SPI_CTL1_NSSDRV			BIT(2)
// SPI NSS pulse mode enable
#define SPI_CTL1_NSSP			BIT(3)

// SPI TI mode enable
#define SPI_CTL1_TMOD			BIT(4)

// errors interrupt enable
#define SPI_CTL1_ERRIE			BIT(5)

// receive buffer not empty interrupt enable
#define SPI_CTL1_RBNEIE			BIT(6)

// transmit buffer empty interrupt enable
#define SPI_CTL1_TBEIE			BIT(7)

// receive buffer not empty
#define SPI_STAT_RBNE			BIT(0)
// transmit buffer empty
#define SPI_STAT_TBE			BIT(1)

// I2S channel side
#define SPI_STAT_I2SCH			BIT(2)

// I2S transmission underrun error bit
#define SPI_STAT_TXURERR		BIT(3)

// SPI CRC error bit
#define SPI_STAT_CRCERR			BIT(4)

// SPI configuration error bit
#define SPI_STAT_CONFERR		BIT(5)

// SPI reception overrun error bit
#define SPI_STAT_RXORERR		BIT(6)

// transmitting on-going bit
#define SPI_STAT_TRANS			BIT(7)

// format error bit
#define SPI_STAT_FERR			BIT(8)

// data transfer register
#define SPI_DATA_DATA			BITS(0, 15)

// CRC polynomial value
#define SPI_CRCPOLY_CRCPOLY		BITS(0, 15)

// RX CRC value
#define SPI_RCRC_RCRC			BITS(0, 15)
// TX CRC value
#define SPI_TCRC_TCRC			BITS(0, 15)

// channel length
#define SPI_I2SCTL_CHLEN		BIT(0)

// data length
#define SPI_I2SCTL_DTLEN		BITS(1, 2)

// idle state clock polarity
#define SPI_I2SCTL_CKPL			BIT(3)

// I2S standard selection
#define SPI_I2SCTL_I2SSTD		BITS(4, 5)

// PCM frame synchronization mode
#define SPI_I2SCTL_PCMSMOD		BIT(7)

// I2S operation mode
#define SPI_I2SCTL_I2SOPMOD		BITS(8, 9)

// I2S enable
#define SPI_I2SCTL_I2SEN		BIT(10)

// I2S mode selection
#define SPI_I2SCTL_I2SSEL		BIT(11)

// dividing factor for the prescaler
#define SPI_I2SPSC_DIV			BITS(0, 7)

// odd factor for the prescaler
#define SPI_I2SPSC_OF			BIT(8)

// I2S MCK output enable
#define SPI_I2SPSC_MCKOEN		BIT(9)


struct spi_param {   
	// SPI master or slave
	uint32_t device_mode;

	uint32_t trans_mode;

	// SPI frame size
	uint32_t frame_size;

	// SPI NSS control by handware or software
	uint32_t nss;

	uint32_t endian;
	uint32_t clock_polarity_phase;
	uint32_t prescale;
};

// SPI mode definitions
#define SPI_MASTER			(SPI_CTL0_MSTMOD | SPI_CTL0_SWNSS)
#define SPI_SLAVE			((uint32_t) 0x00000000U)

// SPI work in transmit-only mode
#define SPI_BIDIRECTIONAL_TRANSMIT	SPI_CTL0_BDOEN
// SPI work in receive-only mode
#define SPI_BIDIRECTIONAL_RECEIVE	(~SPI_CTL0_BDOEN)

// SPI transmit type
// SPI receive and send data at fullduplex communication
#define SPI_TRANSMODE_FULLDUPLEX	((uint32_t) 0x00000000U)
// SPI only receive data
#define SPI_TRANSMODE_RECEIVEONLY	SPI_CTL0_RO
// bidirectional receive data
#define SPI_TRANSMODE_BDRECEIVE		SPI_CTL0_BDEN
// bidirectional transmit data
#define SPI_TRANSMODE_BDTRANSMIT	(SPI_CTL0_BDEN | SPI_CTL0_BDOEN)

// SPI frame size
#define SPI_FRAMESIZE_16BIT		SPI_CTL0_FF16
#define SPI_FRAMESIZE_8BIT            	((uint32_t) 0x00000000U)

// SPI NSS control by software
#define SPI_NSS_SOFT			SPI_CTL0_SWNSSEN
// SPI NSS control by hardware
#define SPI_NSS_HARD                    ((uint32_t) 0x00000000U)

// SPI transmit way is big endian: transmit MSB first
#define SPI_ENDIAN_MSB			((uint32_t) 0x00000000U)
// SPI transmit way is little endian: transmit LSB first
#define SPI_ENDIAN_LSB			SPI_CTL0_LF


// SPI clock polarity is low level and phase is first edge
#define SPI_CK_PL_LOW_PH_1EDGE		((uint32_t) 0x00000000U)
// SPI clock polarity is high level and phase is first edge
#define SPI_CK_PL_HIGH_PH_1EDGE		SPI_CTL0_CKPL

// SPI clock polarity is low level and phase is second edge
#define SPI_CK_PL_LOW_PH_2EDGE		SPI_CTL0_CKPH
// SPI clock polarity is high level and phase is second edge
#define SPI_CK_PL_HIGH_PH_2EDGE		(SPI_CTL0_CKPL | SPI_CTL0_CKPH)

// SPI clock prescale factor
#define CTL0_PSC(regval) \
	(BITS(3, 5) & ((uint32_t) (regval) << 3))

#define SPI_PSC_2			CTL0_PSC(0)
#define SPI_PSC_4			CTL0_PSC(1)
#define SPI_PSC_8			CTL0_PSC(2)
#define SPI_PSC_16			CTL0_PSC(3)
#define SPI_PSC_32			CTL0_PSC(4)
#define SPI_PSC_64			CTL0_PSC(5)
#define SPI_PSC_128			CTL0_PSC(6)
#define SPI_PSC_256			CTL0_PSC(7)

// I2S audio sample rate
#define I2S_AUDIOSAMPLE_8K		((uint32_t) 8000U)
#define I2S_AUDIOSAMPLE_11K		((uint32_t) 11025U)
#define I2S_AUDIOSAMPLE_16K		((uint32_t) 16000U)
#define I2S_AUDIOSAMPLE_22K		((uint32_t) 22050U)
#define I2S_AUDIOSAMPLE_32K		((uint32_t) 32000U)
#define I2S_AUDIOSAMPLE_44K		((uint32_t) 44100U)
#define I2S_AUDIOSAMPLE_48K		((uint32_t) 48000U)
#define I2S_AUDIOSAMPLE_96K		((uint32_t) 96000U)
#define I2S_AUDIOSAMPLE_192K		((uint32_t) 192000U)

// I2S frame format
#define I2SCTL_DTLEN(regval) \
	(BITS(1, 2) & ((uint32_t) (regval) << 1))

// I2S data length is 16 bit and channel length is 16 bit
#define I2S_FRAMEFORMAT_DT16B_CH16B	I2SCTL_DTLEN(0)
// I2S data length is 16 bit and channel length is 32 bit
#define I2S_FRAMEFORMAT_DT16B_CH32B	(I2SCTL_DTLEN(0) | SPI_I2SCTL_CHLEN)
// I2S data length is 24 bit and channel length is 32 bit
#define I2S_FRAMEFORMAT_DT24B_CH32B	(I2SCTL_DTLEN(1) | SPI_I2SCTL_CHLEN)
// I2S data length is 32 bit and channel length is 32 bit
#define I2S_FRAMEFORMAT_DT32B_CH32B	(I2SCTL_DTLEN(2) | SPI_I2SCTL_CHLEN)

// I2S master clock output
#define I2S_MCKOUT_DISABLE		((uint32_t) 0x00000000U)
#define I2S_MCKOUT_ENABLE		SPI_I2SPSC_MCKOEN

// I2S operation mode
#define I2SCTL_I2SOPMOD(regval) \
	(BITS(8, 9) & ((uint32_t) (regval) << 8))

// I2S slave transmit mode
#define I2S_MODE_SLAVETX		I2SCTL_I2SOPMOD(0)
// I2S slave receive mode
#define I2S_MODE_SLAVERX		I2SCTL_I2SOPMOD(1)
// I2S master transmit mode
#define I2S_MODE_MASTERTX		I2SCTL_I2SOPMOD(2)
// I2S master receive mode
#define I2S_MODE_MASTERRX		I2SCTL_I2SOPMOD(3)

// I2S standard
#define I2SCTL_I2SSTD(regval) \
	(BITS(4, 5) & ((uint32_t) (regval) << 4))

// I2S phillips standard
#define I2S_STD_PHILLIPS		I2SCTL_I2SSTD(0)
#define I2S_STD_MSB			I2SCTL_I2SSTD(1)
#define I2S_STD_LSB			I2SCTL_I2SSTD(2)
// I2S PCM short standard
#define I2S_STD_PCMSHORT		I2SCTL_I2SSTD(3)
// I2S PCM long standard
#define I2S_STD_PCMLONG			(I2SCTL_I2SSTD(3) | SPI_I2SCTL_PCMSMOD)

// I2S clock polarity
#define I2S_CKPL_LOW			((uint32_t) 0x00000000U)
#define I2S_CKPL_HIGH			SPI_I2SCTL_CKPL

/* SPI DMA constants definitions */                                    
#define SPI_DMA_TRANSMIT		((uint8_t) 0x00U)
#define SPI_DMA_RECEIVE			((uint8_t) 0x01U)

// SPI CRC constants definitions
#define SPI_CRC_TX			((uint8_t) 0x00U)
#define SPI_CRC_RX			((uint8_t) 0x01U)

// SPI/I2S interrupt enable/disable constants definitions
// transmit buffer empty interrupt
#define SPI_I2S_INT_TBE			((uint8_t) 0x00U)
// receive buffer not empty interrupt
#define SPI_I2S_INT_RBNE		((uint8_t) 0x01U)
// error interrupt
#define SPI_I2S_INT_ERR			((uint8_t) 0x02U)

// SPI/I2S interrupt flag constants definitions
// transmit buffer empty interrupt flag
#define SPI_I2S_INT_FLAG_TBE		((uint8_t) 0x00U)
// receive buffer not empty interrupt flag
#define SPI_I2S_INT_FLAG_RBNE		((uint8_t) 0x01U)
// overrun interrupt flag
#define SPI_I2S_INT_FLAG_RXORERR	((uint8_t) 0x02U)
// config error interrupt flag
#define SPI_INT_FLAG_CONFERR		((uint8_t) 0x03U)
// CRC error interrupt flag
#define SPI_INT_FLAG_CRCERR		((uint8_t) 0x04U)
// underrun error interrupt flag
#define I2S_INT_FLAG_TXURERR		((uint8_t) 0x05U)
// format error interrupt flag
#define SPI_I2S_INT_FLAG_FERR		((uint8_t) 0x06U)

// SPI/I2S flag definitions
// receive buffer not empty flag
#define SPI_FLAG_RBNE			SPI_STAT_RBNE
// transmit buffer empty flag
#define SPI_FLAG_TBE			SPI_STAT_TBE
// CRC error flag
#define SPI_FLAG_CRCERR			SPI_STAT_CRCERR
// mode config error flag
#define SPI_FLAG_CONFERR		SPI_STAT_CONFERR
// receive overrun error flag
#define SPI_FLAG_RXORERR		SPI_STAT_RXORERR
// transmit on-going flag
#define SPI_FLAG_TRANS			SPI_STAT_TRANS
// format error interrupt flag
#define SPI_FLAG_FERR			SPI_STAT_FERR
// receive buffer not empty flag
#define I2S_FLAG_RBNE			SPI_STAT_RBNE
// transmit buffer empty flag
#define I2S_FLAG_TBE			SPI_STAT_TBE
// channel side flag
#define I2S_FLAG_CH			SPI_STAT_I2SCH
// underrun error flag
#define I2S_FLAG_TXURERR		SPI_STAT_TXURERR
// overrun error flag
#define I2S_FLAG_RXORERR		SPI_STAT_RXORERR
// transmit on-going flag
#define I2S_FLAG_TRANS			SPI_STAT_TRANS
// format error interrupt flag
#define I2S_FLAG_FERR			SPI_STAT_FERR

void spi_i2s_deinit(uint32_t spi_periph);
void spi_struct_para_init(struct spi_param* spi_struct);
void spi_init(uint32_t spi_periph, struct spi_param* spi_struct);
void spi_enable(uint32_t spi_periph);
void spi_disable(uint32_t spi_periph);

void i2s_init(uint32_t spi_periph, uint32_t mode, uint32_t standard,
		uint32_t ckpl);

void i2s_psc_config(uint32_t spi_periph, uint32_t audiosample,
		uint32_t frameformat, uint32_t mckout);

void i2s_enable(uint32_t spi_periph);
void i2s_disable(uint32_t spi_periph);

// NSS functions
void spi_nss_output_enable(uint32_t spi_periph);
void spi_nss_output_disable(uint32_t spi_periph);
// SPI NSS pin high level in software mode
void spi_nss_internal_high(uint32_t spi_periph);
// SPI NSS pin low level in software mode
void spi_nss_internal_low(uint32_t spi_periph);

// DMA communication
void spi_dma_enable(uint32_t spi_periph, uint8_t dma);
void spi_dma_disable(uint32_t spi_periph, uint8_t dma);

// normal mode communication
void spi_i2s_data_frame_format_config(uint32_t spi_periph,
		uint16_t frame_format);

void spi_i2s_data_transmit(uint32_t spi_periph, uint16_t data);

uint16_t spi_i2s_data_receive(uint32_t spi_periph);

void spi_bidirectional_transfer_config(uint32_t spi_periph,
		uint32_t transfer_direction);

// SPI CRC functions
void spi_crc_polynomial_set(uint32_t spi_periph, uint16_t crc_poly);
uint16_t spi_crc_polynomial_get(uint32_t spi_periph);
void spi_crc_on(uint32_t spi_periph);
void spi_crc_off(uint32_t spi_periph);
void spi_crc_next(uint32_t spi_periph);
uint16_t spi_crc_get(uint32_t spi_periph, uint8_t crc);

// SPI TI mode functions
void spi_ti_mode_enable(uint32_t spi_periph);
void spi_ti_mode_disable(uint32_t spi_periph);

// SPI NSS pulse mode functions
void spi_nssp_mode_enable(uint32_t spi_periph);
void spi_nssp_mode_disable(uint32_t spi_periph);

// flag and interrupt functions
void spi_i2s_interrupt_enable(uint32_t spi_periph, uint8_t interrupt);
void spi_i2s_interrupt_disable(uint32_t spi_periph, uint8_t interrupt);
enum flag_status spi_i2s_interrupt_flag_get(uint32_t spi_periph,
		uint8_t interrupt);

enum flag_status spi_i2s_flag_get(uint32_t spi_periph, uint32_t flag);

void spi_crc_error_clear(uint32_t spi_periph);

#endif
