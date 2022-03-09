#ifndef __GD32VF103_DMA_H
#define __GD32VF103_DMA_H

#include "gd32vf103.h"

#define DMA0				(DMA_BASE)
#define DMA1				(DMA_BASE + 0x0400U)

// DMA interrupt flag register
#define DMA_INTF(dmax)			REG32((dmax) + 0x00U)

// DMA interrupt flag clear register
#define DMA_INTC(dmax)			REG32((dmax) + 0x04U)


// DMA channel 0 control register
#define DMA_CH0CTL(dmax)		REG32((dmax) + 0x08U)
// DMA channel 0 counter register
#define DMA_CH0CNT(dmax)		REG32((dmax) + 0x0CU)
// DMA channel 0 peripheral base address register
#define DMA_CH0PADDR(dmax)		REG32((dmax) + 0x10U)
// DMA channel 0 memory base address register
#define DMA_CH0MADDR(dmax)		REG32((dmax) + 0x14U)

#define DMA_CH1CTL(dmax)		REG32((dmax) + 0x1CU)
#define DMA_CH1CNT(dmax)		REG32((dmax) + 0x20U)
#define DMA_CH1PADDR(dmax)		REG32((dmax) + 0x24U)
#define DMA_CH1MADDR(dmax)		REG32((dmax) + 0x28U)

#define DMA_CH2CTL(dmax)		REG32((dmax) + 0x30U)
#define DMA_CH2CNT(dmax)		REG32((dmax) + 0x34U)
#define DMA_CH2PADDR(dmax)		REG32((dmax) + 0x38U)
#define DMA_CH2MADDR(dmax)		REG32((dmax) + 0x3CU)

#define DMA_CH3CTL(dmax)		REG32((dmax) + 0x44U)
#define DMA_CH3CNT(dmax)		REG32((dmax) + 0x48U)
#define DMA_CH3PADDR(dmax)		REG32((dmax) + 0x4CU)
#define DMA_CH3MADDR(dmax)		REG32((dmax) + 0x50U)


#define DMA_CH4CTL(dmax)		REG32((dmax) + 0x58U)
#define DMA_CH4CNT(dmax)		REG32((dmax) + 0x5CU)
#define DMA_CH4PADDR(dmax)		REG32((dmax) + 0x60U)
#define DMA_CH4MADDR(dmax)		REG32((dmax) + 0x64U)


#define DMA_CH5CTL(dmax)		REG32((dmax) + 0x6CU)
#define DMA_CH5CNT(dmax)		REG32((dmax) + 0x70U)
#define DMA_CH5PADDR(dmax)		REG32((dmax) + 0x74U)
#define DMA_CH5MADDR(dmax)		REG32((dmax) + 0x78U)

#define DMA_CH6CTL(dmax)		REG32((dmax) + 0x80U)
#define DMA_CH6CNT(dmax)		REG32((dmax) + 0x84U)
#define DMA_CH6PADDR(dmax)		REG32((dmax) + 0x88U)
#define DMA_CH6MADDR(dmax)		REG32((dmax) + 0x8CU)


// DMA_INTF
// global interrupt flag of channel
#define DMA_INTF_GIF			BIT(0)
// full transfer finish flag of channel
#define DMA_INTF_FTFIF			BIT(1)
// half transfer finish flag of channel
#define DMA_INTF_HTFIF			BIT(2)
// error flag of channel
#define DMA_INTF_ERRIF			BIT(3)


// DMA_INTC
// clear global interrupt flag of channel
#define DMA_INTC_GIFC			BIT(0)
// clear transfer finish flag of channel
#define DMA_INTC_FTFIFC			BIT(1)
// clear half transfer finish flag of channel
#define DMA_INTC_HTFIFC			BIT(2)
// clear error flag of channel
#define DMA_INTC_ERRIFC			BIT(3)


// DMA_CHxCTL, x=0..6
// channel enable
#define DMA_CHXCTL_CHEN			BIT(0)
// enable bit for channel full transfer finish interrupt
#define DMA_CHXCTL_FTFIE		BIT(1)
// enable bit for channel half transfer finish interrupt
#define DMA_CHXCTL_HTFIE		BIT(2)
// enable bit for channel error interrupt
#define DMA_CHXCTL_ERRIE		BIT(3)
// transfer direction
#define DMA_CHXCTL_DIR			BIT(4)
// circular mode enable
#define DMA_CHXCTL_CMEN			BIT(5)
// next address generation algorithm of peripheral
#define DMA_CHXCTL_PNAGA		BIT(6)
// next address generation algorithm of memory
#define DMA_CHXCTL_MNAGA		BIT(7)
// transfer data width of peripheral
#define DMA_CHXCTL_PWIDTH		BITS(8,9)
// transfer data width of memory
#define DMA_CHXCTL_MWIDTH		BITS(10,11)
// priority level
#define DMA_CHXCTL_PRIO			BITS(12,13)
// memory to memory mode
#define DMA_CHXCTL_M2M			BIT(14)


// DMA_CHxCNT, x=0..6
// transfer counter
#define DMA_CHXCNT_CNT			BITS(0,15)


// DMA_CHxPADDR, x=0..6
// peripheral base address
#define DMA_CHXPADDR_PADDR		BITS(0,31)


// DMA_CHxMADDR, x=0..6
// memory base address
#define DMA_CHXMADDR_MADDR		BITS(0,31)


enum dma_channel {
	DMA_CH0 = 0,		/* DMA Channel0 */
	DMA_CH1,		/* DMA Channel1 */
	DMA_CH2,		/* DMA Channel2 */
	DMA_CH3,		/* DMA Channel3 */
	DMA_CH4,		/* DMA Channel4 */
	DMA_CH5,		/* DMA Channel5 */
	DMA_CH6			/* DMA Channel6 */
};

struct dma_param {
	uint32_t periph_addr;	/* peripheral base address */
	uint32_t periph_width;	/* transfer data size of peripheral */
	uint32_t memory_addr;	/* memory base address */
	uint32_t memory_width;	/* transfer data size of memory */
	uint32_t number;	/* channel transfer number */
	uint32_t priority;	/* channel priority level */
	uint8_t periph_inc;	/* peripheral increasing mode */
	uint8_t memory_inc;	/* memory increasing mode */
	uint8_t direction;	/* channel data transfer direction */

};

// DMA channel flag shift
#define DMA_FLAG_ADD(flag, shift) \
	((flag) << ((shift) * 4))

// DMA_register address
// the address of DMA channel CHXCTL register
#define DMA_CHCTL(dma, channel) \
	REG32(((dma) + 0x08) + 0x14 * (uint32_t) (channel))	

// the address of DMA channel CHXCNT register
#define DMA_CHCNT(dma, channel) \
	REG32(((dma) + 0x0C) + 0x14 * (uint32_t) (channel))

// the address of DMA channel CHXPADDR register
#define DMA_CHPADDR(dma, channel) \
	REG32(((dma) + 0x10) + 0x14 * (uint32_t) (channel))

// the address of DMA channel CHXMADDR register
#define DMA_CHMADDR(dma, channel) \
	REG32(((dma) + 0x14) + 0x14 * (uint32_t) (channel))	

// the reset value of DMA channel CHXCTL register
#define DMA_CHCTL_RESET_VALUE		((uint32_t) 0x00000000U)

// the reset value of DMA channel CHXCNT register
#define DMA_CHCNT_RESET_VALUE		((uint32_t) 0x00000000U)

// the reset value of DMA channel CHXPADDR register
#define DMA_CHPADDR_RESET_VALUE		((uint32_t) 0x00000000U)

// the reset value of DMA channel CHXMADDR register
#define DMA_CHMADDR_RESET_VALUE		((uint32_t) 0x00000000U)

// clear DMA channel DMA_INTF register
#define DMA_CHINTF_RESET_VALUE \
	(DMA_INTF_GIF | DMA_INTF_FTFIF | DMA_INTF_HTFIF | DMA_INTF_ERRIF)

// DMA_INTF register

// global interrupt flag of channel
#define DMA_INT_FLAG_G			DMA_INTF_GIF

// full transfer finish interrupt flag of channel
#define DMA_INT_FLAG_FTF		DMA_INTF_FTFIF

// half transfer finish interrupt flag of channel
#define DMA_INT_FLAG_HTF		DMA_INTF_HTFIF

// error interrupt flag of channel
#define DMA_INT_FLAG_ERR		DMA_INTF_ERRIF


// global interrupt flag of channel
#define DMA_FLAG_G			DMA_INTF_GIF

// full transfer finish flag of channel
#define DMA_FLAG_FTF			DMA_INTF_FTFIF

// half transfer finish flag of channel
#define DMA_FLAG_HTF			DMA_INTF_HTFIF

// error flag of channel
#define DMA_FLAG_ERR			DMA_INTF_ERRIF


// DMA_CHxCTL register
// enable bit for channel full transfer finish interrupt
#define DMA_INT_FTF			DMA_CHXCTL_FTFIE

// enable bit for channel half transfer finish interrupt
#define DMA_INT_HTF			DMA_CHXCTL_HTFIE

// enable bit for channel error interrupt
#define DMA_INT_ERR			DMA_CHXCTL_ERRIE


/* transfer direction */
// read from peripheral and write to memory
#define DMA_PERIPHERAL_TO_MEMORY	((uint8_t) 0x00U)

// read from memory and write to peripheral
#define DMA_MEMORY_TO_PERIPHERAL	((uint8_t) 0x01U)


// peripheral increasing mode
// next address of peripheral is fixed address mode
#define DMA_PERIPH_INCREASE_DISABLE	((uint8_t) 0x00U)

// next address of peripheral is increasing address mode
#define DMA_PERIPH_INCREASE_ENABLE	((uint8_t) 0x01U)


// memory increasing mode
// next address of memory is fixed address mode
#define DMA_MEMORY_INCREASE_DISABLE	((uint8_t) 0x00U)

// next address of memory is increasing address mode
#define DMA_MEMORY_INCREASE_ENABLE	((uint8_t) 0x01U)


// transfer data size of peripheral
#define CHCTL_PWIDTH(regval) \
	(BITS(8, 9) & ((uint32_t) (regval) << 8))

// transfer data size of peripheral is 8-bit
#define DMA_PERIPHERAL_WIDTH_8BIT	CHCTL_PWIDTH(0U)

// transfer data size of peripheral is 16-bit
#define DMA_PERIPHERAL_WIDTH_16BIT	CHCTL_PWIDTH(1U)

// transfer data size of peripheral is 32-bit
#define DMA_PERIPHERAL_WIDTH_32BIT	CHCTL_PWIDTH(2U)


// transfer data size of memory
#define CHCTL_MWIDTH(regval) \
	(BITS(10, 11) & ((uint32_t) (regval) << 10))

// transfer data size of memory is 8-bit
#define DMA_MEMORY_WIDTH_8BIT		CHCTL_MWIDTH(0U)

// transfer data size of memory is 16-bit
#define DMA_MEMORY_WIDTH_16BIT		CHCTL_MWIDTH(1U)

// transfer data size of memory is 32-bit
#define DMA_MEMORY_WIDTH_32BIT		CHCTL_MWIDTH(2U)


// channel priority level
// DMA channel priority level
#define CHCTL_PRIO(regval) \
	(BITS(12, 13) & ((uint32_t) (regval) << 12))

// low priority
#define DMA_PRIORITY_LOW		CHCTL_PRIO(0U)
// medium priority
#define DMA_PRIORITY_MEDIUM		CHCTL_PRIO(1U)
// high priority
#define DMA_PRIORITY_HIGH		CHCTL_PRIO(2U)
// ultra high priority
#define DMA_PRIORITY_ULTRA_HIGH		CHCTL_PRIO(3U)


// memory to memory mode
// disable memory to memory mode
#define DMA_MEMORY_TO_MEMORY_DISABLE	((uint32_t) 0x00000000U)

// enable memory to memory mode
#define DMA_MEMORY_TO_MEMORY_ENABLE	((uint32_t) 0x00000001U)


// DMA_CHxCNT register
// transfer counter
// transfer counter mask
#define DMA_CHANNEL_CNT_MASK		DMA_CHXCNT_CNT


// DMA deinitialization and initialization functions
void dma_deinit(uint32_t dma_periph, enum dma_channel channelx);

// initialize the parameters of DMA struct with the default values
void dma_struct_para_init(struct dma_param * init_struct);

// initialize DMA channel
void dma_init(uint32_t dma_periph, enum dma_channel channelx,
		struct dma_param * init_struct);

void dma_circulation_enable(uint32_t dma_periph, enum dma_channel channelx);
void dma_circulation_disable(uint32_t dma_periph, enum dma_channel channelx);

void dma_memory_to_memory_enable(uint32_t dma_periph,
		enum dma_channel channelx);
void dma_memory_to_memory_disable(uint32_t dma_periph,
		enum dma_channel channelx);

void dma_channel_enable(uint32_t dma_periph, enum dma_channel channelx);
void dma_channel_disable(uint32_t dma_periph, enum dma_channel channelx);

// DMA configuration functions
// set DMA peripheral base address
void dma_periph_address_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t address);

// set DMA memory base address
void dma_memory_address_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t address);

// set the number of remaining data to be transferred by the DMA
void dma_transfer_number_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t number);

// get the number of remaining data to be transferred by the DMA
uint32_t dma_transfer_number_get(uint32_t dma_periph,
		enum dma_channel channelx);

// configure priority level of DMA channel
void dma_priority_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t priority);

// configure transfer data size of memory
void dma_memory_width_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t mwidth);

// configure transfer data size of peripheral
void dma_periph_width_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t pwidth);

// enable next address increasement algorithm of memory
void dma_memory_increase_enable(uint32_t dma_periph,
		enum dma_channel channelx);

// disable next address increasement algorithm of memory
void dma_memory_increase_disable(uint32_t dma_periph,
		enum dma_channel channelx);

// enable next address increasement algorithm of peripheral
void dma_periph_increase_enable(uint32_t dma_periph,
		enum dma_channel channelx);

// disable next address increasement algorithm of peripheral
void dma_periph_increase_disable(uint32_t dma_periph,
		enum dma_channel channelx);

// configure the direction of data transfer on the channel
void dma_transfer_direction_config(uint32_t dma_periph,
		enum dma_channel channelx, uint8_t direction);

// flag and interrupt functions
enum flag_status dma_flag_get(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t flag);
void dma_flag_clear(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t flag);

enum flag_status dma_interrupt_flag_get(uint32_t dma_periph,
		enum dma_channel channelx, uint32_t flag);

void dma_interrupt_flag_clear(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t flag);

void dma_interrupt_enable(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t source);
void dma_interrupt_disable(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t source);

#endif
