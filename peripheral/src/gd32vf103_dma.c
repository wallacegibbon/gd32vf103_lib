#include "gd32vf103_dma.h"

#define DMA_WRONG_HANDLE		while(1);

void dangle_when_dma_wrong(uint32_t dma_periph, enum dma_channel channelx) {
	if (dma_periph == DMA1 && channelx > DMA_CH4)
		while (1);
}

void dma_deinit(uint32_t dma_periph, enum dma_channel channelx) {
	dangle_when_dma_wrong(dma_periph, channelx);

	// disable DMA a channel
	DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CHEN;
	// reset DMA channel registers
	DMA_CHCTL(dma_periph, channelx) = DMA_CHCTL_RESET_VALUE;
	DMA_CHCNT(dma_periph, channelx) = DMA_CHCNT_RESET_VALUE;
	DMA_CHPADDR(dma_periph, channelx) = DMA_CHPADDR_RESET_VALUE;
	DMA_CHMADDR(dma_periph, channelx) = DMA_CHMADDR_RESET_VALUE;
	DMA_INTC(dma_periph) |= DMA_FLAG_ADD(DMA_CHINTF_RESET_VALUE, channelx);
}

/* initialize the parameters of DMA struct with the default values */
void dma_struct_para_init(struct dma_param *init_struct) {
	// set the DMA struct with the default values
	init_struct->periph_addr = 0;
	init_struct->periph_width = 0;
	init_struct->periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	init_struct->memory_addr = 0;
	init_struct->memory_width = 0;
	init_struct->memory_inc = DMA_MEMORY_INCREASE_DISABLE;
	init_struct->number = 0;
	init_struct->direction = DMA_PERIPHERAL_TO_MEMORY;
	init_struct->priority = DMA_PRIORITY_LOW;
}

/*
 * initialize DMA channel
 *
 * init_struct: the data needed to initialize DMA channel
 * 	periph_addr:
 * 		peripheral base address
 * 	periph_width:
 * 		DMA_PERIPHERAL_WIDTH_8BIT, DMA_PERIPHERAL_WIDTH_16BIT,
 * 		DMA_PERIPHERAL_WIDTH_32BIT
 * 	periph_inc:
 * 		DMA_PERIPH_INCREASE_ENABLE, DMA_PERIPH_INCREASE_DISABLE
 * 	memory_addr:
 * 		memory base address
 * 	memory_width:
 * 		DMA_MEMORY_WIDTH_8BIT, DMA_MEMORY_WIDTH_16BIT,
 * 		DMA_MEMORY_WIDTH_32BIT
 * 	memory_inc:
 * 		DMA_MEMORY_INCREASE_ENABLE, DMA_MEMORY_INCREASE_DISABLE
 * 	direction:
 * 		DMA_PERIPHERAL_TO_MEMORY, DMA_MEMORY_TO_PERIPHERAL
 * 	number:
 * 		the number of remaining data to be transferred by the DMA
 * 	priority:
 * 		DMA_PRIORITY_LOW, DMA_PRIORITY_MEDIUM, DMA_PRIORITY_HIGH,
 * 		DMA_PRIORITY_ULTRA_HIGH
 */
void dma_init(uint32_t dma_periph, enum dma_channel channelx,
		struct dma_param * init_struct) {

	dangle_when_dma_wrong(dma_periph, channelx);

	// configure peripheral base address
	DMA_CHPADDR(dma_periph, channelx) = init_struct->periph_addr;

	// configure memory base address
	DMA_CHMADDR(dma_periph, channelx) = init_struct->memory_addr;

	// configure the number of remaining data to be transferred
	DMA_CHCNT(dma_periph, channelx) =
	    (init_struct->number & DMA_CHANNEL_CNT_MASK);

	// configure peripheral transfer width,memory transfer width and
	// priority
	uint32_t ctl = DMA_CHCTL(dma_periph, channelx);
	ctl &= ~(DMA_CHXCTL_PWIDTH | DMA_CHXCTL_MWIDTH | DMA_CHXCTL_PRIO);
	ctl |= init_struct->periph_width | init_struct-> memory_width |
		init_struct->priority;

	DMA_CHCTL(dma_periph, channelx) = ctl;

	// configure peripheral increasing mode
	if (init_struct->periph_inc == DMA_PERIPH_INCREASE_ENABLE)
		DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
	else
		DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_PNAGA;

	// configure memory increasing mode
	if (init_struct->memory_inc == DMA_MEMORY_INCREASE_ENABLE)
		DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MNAGA;
	else
		DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MNAGA;

	// configure the direction of data transfer
	if (init_struct->direction == DMA_PERIPHERAL_TO_MEMORY)
		DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_DIR;
	else
		DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_DIR;
}

void dma_circulation_enable(uint32_t dma_periph, enum dma_channel channelx) {
	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CMEN;
}

void dma_circulation_disable(uint32_t dma_periph, enum dma_channel channelx) {
	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CMEN;
}

void dma_memory_to_memory_enable(uint32_t dma_periph,
		enum dma_channel channelx) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_M2M;
}

void dma_memory_to_memory_disable(uint32_t dma_periph,
		enum dma_channel channelx) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_M2M;
}

void dma_channel_enable(uint32_t dma_periph, enum dma_channel channelx) {
	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CHEN;
}

void dma_channel_disable(uint32_t dma_periph, enum dma_channel channelx) {
	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CHEN;
}

void dma_periph_address_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t address) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHPADDR(dma_periph, channelx) = address;
}

void dma_memory_address_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t address) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHMADDR(dma_periph, channelx) = address;
}

void dma_transfer_number_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t number) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCNT(dma_periph, channelx) = (number & DMA_CHANNEL_CNT_MASK);
}

/* get the number of remaining data to be transferred by the DMA */
uint32_t dma_transfer_number_get(uint32_t dma_periph,
		enum dma_channel channelx) {

	dangle_when_dma_wrong(dma_periph, channelx);
	return DMA_CHCNT(dma_periph, channelx);
}

/*
 * priority: priority Level of this channel
 * 	DMA_PRIORITY_LOW: low priority
 * 	DMA_PRIORITY_MEDIUM: medium priority
 * 	DMA_PRIORITY_HIGH: high priority
 * 	DMA_PRIORITY_ULTRA_HIGH: ultra high priority
 */
void dma_priority_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t priority) {

	dangle_when_dma_wrong(dma_periph, channelx);

	// acquire DMA_CHxCTL register
	uint32_t ctl = DMA_CHCTL(dma_periph, channelx);
	// assign regiser
	ctl &= ~DMA_CHXCTL_PRIO;
	ctl |= priority;
	DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*
 * configure transfer data size of memory
 *
 * mwidth: transfer data width of memory
 * 	DMA_MEMORY_WIDTH_8BIT: transfer data width of memory is 8-bit
 * 	DMA_MEMORY_WIDTH_16BIT: transfer data width of memory is 16-bit
 * 	DMA_MEMORY_WIDTH_32BIT: transfer data width of memory is 32-bit
 */
void dma_memory_width_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t mwidth) {

	dangle_when_dma_wrong(dma_periph, channelx);

	// acquire DMA_CHxCTL register
	uint32_t ctl = DMA_CHCTL(dma_periph, channelx);
	// assign regiser
	ctl &= ~DMA_CHXCTL_MWIDTH;
	ctl |= mwidth;
	DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*
 * configure transfer data size of peripheral
 * pwidth: transfer data width of peripheral
 * 	DMA_PERIPHERAL_WIDTH_8BIT:
 * 		transfer data width of peripheral is 8-bit
 * 	DMA_PERIPHERAL_WIDTH_16BIT:
 * 		transfer data width of peripheral is 16-bit
 * 	DMA_PERIPHERAL_WIDTH_32BIT:
 * 		transfer data width of peripheral is 32-bit
 */
void dma_periph_width_config(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t pwidth) {

	dangle_when_dma_wrong(dma_periph, channelx);

	// acquire DMA_CHxCTL register
	uint32_t ctl = DMA_CHCTL(dma_periph, channelx);
	// assign regiser
	ctl &= ~DMA_CHXCTL_PWIDTH;
	ctl |= pwidth;
	DMA_CHCTL(dma_periph, channelx) = ctl;
}

/* enable next address increasement algorithm of memory */
void dma_memory_increase_enable(uint32_t dma_periph,
		enum dma_channel channelx) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MNAGA;
}

/* disable next address increasement algorithm of memory */
void dma_memory_increase_disable(uint32_t dma_periph,
		enum dma_channel channelx) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MNAGA;
}

/* enable next address increasement algorithm of peripheral */
void dma_periph_increase_enable(uint32_t dma_periph,
		enum dma_channel channelx) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
}

/* disable next address increasement algorithm of peripheral */
void dma_periph_increase_disable(uint32_t dma_periph,
		enum dma_channel channelx) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_PNAGA;
}

/*
 * direction: specify the direction of data transfer
 * 	DMA_PERIPHERAL_TO_MEMORY: read from peripheral and write to memory
 * 	DMA_MEMORY_TO_PERIPHERAL: read from memory and write to peripheral
 */
void dma_transfer_direction_config(uint32_t dma_periph,
		enum dma_channel channelx, uint8_t direction) {

	dangle_when_dma_wrong(dma_periph, channelx);

	if (direction == DMA_PERIPHERAL_TO_MEMORY)
		DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_DIR;
	else
		DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_DIR;
}

/*
 * check DMA flag is set or not
 * flag: specify get which flag
 * 	DMA_FLAG_G: global interrupt flag of channel
 * 	DMA_FLAG_FTF: full transfer finish flag of channel
 * 	DMA_FLAG_HTF: half transfer finish flag of channel
 * 	DMA_FLAG_ERR: error flag of channel
 */
enum flag_status dma_flag_get(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t flag) {

	dangle_when_dma_wrong(dma_periph, channelx);

	if ((DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx)) != RESET)
		return SET;
	else
		return RESET;
}

/* clear DMA a channel flag
 * flag: specify get which flag
 * 	DMA_FLAG_G: global interrupt flag of channel
 * 	DMA_FLAG_FTF: full transfer finish flag of channel
 * 	DMA_FLAG_HTF: half transfer finish flag of channel
 * 	DMA_FLAG_ERR: error flag of channel
 */
void dma_flag_clear(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t flag) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_INTC(dma_periph) |= DMA_FLAG_ADD(flag, channelx);
}

/*
 * flag: specify get which flag
 * 	DMA_INT_FLAG_FTF: full transfer finish interrupt flag of channel
 * 	DMA_INT_FLAG_HTF: half transfer finish interrupt flag of channel
 * 	DMA_INT_FLAG_ERR: error interrupt flag of channel
 */
enum flag_status dma_interrupt_flag_get(uint32_t dma_periph,
		enum dma_channel channelx, uint32_t flag) {

	uint32_t interrupt_enable = 0, interrupt_flag = 0;

	switch (flag) {
	case DMA_INT_FLAG_FTF:
		// check whether the full transfer finish interrupt flag is
		// set and enabled
		interrupt_flag =
			DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx);
		interrupt_enable =
			DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_FTFIE;
		break;
	case DMA_INT_FLAG_HTF:
		// check whether the half transfer finish interrupt flag is
		// set and enabled
		interrupt_flag =
			DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx);
		interrupt_enable =
			DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_HTFIE;
		break;
	case DMA_INT_FLAG_ERR:
		// check whether the error interrupt flag is set and enabled
		interrupt_flag =
			DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx);
		interrupt_enable =
			DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_ERRIE;
		break;
	default:
		while (1);
	}

	// when the interrupt flag is set and enabled, return SET
	if (interrupt_flag && interrupt_enable)
		return SET;
	else
		return RESET;
}

/*
 * flag: specify get which flag
 * 	DMA_INT_FLAG_G: global interrupt flag of channel
 * 	DMA_INT_FLAG_FTF: full transfer finish interrupt flag of channel
 * 	DMA_INT_FLAG_HTF: half transfer finish interrupt flag of channel
 * 	DMA_INT_FLAG_ERR: error interrupt flag of channel
 */
void dma_interrupt_flag_clear(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t flag) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_INTC(dma_periph) |= DMA_FLAG_ADD(flag, channelx);
}

/*
 * source: specify which interrupt to enbale
 * 	DMA_INT_FTF: channel full transfer finish interrupt
 * 	DMA_INT_HTF: channel half transfer finish interrupt
 * 	DMA_INT_ERR: channel error interrupt
 */
void dma_interrupt_enable(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t source) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) |= source;
}

void dma_interrupt_disable(uint32_t dma_periph, enum dma_channel channelx,
		uint32_t source) {

	dangle_when_dma_wrong(dma_periph, channelx);
	DMA_CHCTL(dma_periph, channelx) &= ~source;
}

