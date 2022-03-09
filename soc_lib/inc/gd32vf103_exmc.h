#ifndef __GD32VF103_EXMC_H
#define __GD32VF103_EXMC_H

#include "gd32vf103.h"

// EXMC register base address
#define EXMC				(EXMC_BASE)

// NOR/PSRAM

// EXMC SRAM/NOR flash control register 0
#define EXMC_SNCTL0			REG32(EXMC + 0x00U)

// EXMC SRAM/NOR flash timing configuration register 0
#define EXMC_SNTCFG0			REG32(EXMC + 0x04U)

// EXMC SRAM/NOR flash write timing configuration register 0
#define EXMC_SNWTCFG0			REG32(EXMC + 0x104U)


// NOR/PSRAM
// EXMC_SNCTLx, x=0

// NOR bank enable
#define EXMC_SNCTL_NRBKEN		BIT(0)

// NOR bank memory address/data multiplexing
#define EXMC_SNCTL_NRMUX		BIT(1)

// NOR bank memory type
#define EXMC_SNCTL_NRTP			BITS(2,3)

// NOR bank memory data bus width
#define EXMC_SNCTL_NRW			BITS(4,5)

// NOR flash access enable
#define EXMC_SNCTL_NREN			BIT(6)

// NWAIT signal polarity
#define EXMC_SNCTL_NRWTPOL		BIT(9)

// write enable
#define EXMC_SNCTL_WREN			BIT(12)

// NWAIT signal enable
#define EXMC_SNCTL_NRWTEN		BIT(13)

// asynchronous wait
#define EXMC_SNCTL_ASYNCWAIT		BIT(15)


// EXMC_SNTCFGx, x=0

// address setup time
#define EXMC_SNTCFG_ASET		BITS(0,3)

// address hold time
#define EXMC_SNTCFG_AHLD		BITS(4,7)

// data setup time
#define EXMC_SNTCFG_DSET		BITS(8,15)

// bus latency
#define EXMC_SNTCFG_BUSLAT		BITS(16,19)


// EXMC NOR/SRAM timing initialize struct
struct exmc_norsram_timing_param {
	// configure the bus latency
	uint32_t bus_latency;

	// configure the data setup time,asynchronous access mode valid
	uint32_t asyn_data_setuptime;

	// configure the address hold time,asynchronous access mode valid
	uint32_t asyn_address_holdtime;

	// configure the data setup time,asynchronous access mode valid
	uint32_t asyn_address_setuptime;
};

// EXMC NOR/SRAM initialize struct
struct exmc_norsram_param {
	// select the region of EXMC NOR/SRAM bank
	uint32_t norsram_region;

	// enable or disable the asynchronous wait function
	uint32_t asyn_wait;

	// enable or disable the NWAIT signal
	uint32_t nwait_signal;

	// enable or disable the write operation
	uint32_t memory_write;

	// specifies the polarity of NWAIT signal from memory
	uint32_t nwait_polarity;

	// specifies the databus width of external memory
	uint32_t databus_width;

	// specifies the type of external memory
	uint32_t memory_type;

	// specifies whether the data bus and address bus are multiplexed
	uint32_t address_data_mux;

	// timing parameters for read and write
	struct exmc_norsram_timing_param *read_write_timing;
};

// EXMC register address
// EXMC SRAM/NOR flash control register
#define EXMC_SNCTL(region)		REG32(EXMC + 0x08U * (region))

// EXMC SRAM/NOR flash timing configuration register
#define EXMC_SNTCFG(region)		REG32(EXMC + 0x04U + 0x08U * (region))


// NOR bank memory data bus width
#define SNCTL_NRW(regval) \
	(BITS(4, 5) & ((uint32_t) (regval) << 4))

// NOR data width 8 bits
#define EXMC_NOR_DATABUS_WIDTH_8B	SNCTL_NRW(0)
// NOR data width 16 bits
#define EXMC_NOR_DATABUS_WIDTH_16B	SNCTL_NRW(1)


// NOR bank memory type
#define SNCTL_NRTP(regval) \
	(BITS(2, 3) & ((uint32_t) (regval) << 2))

// SRAM,ROM
#define EXMC_MEMORY_TYPE_SRAM		SNCTL_NRTP(0)
// PSRAM,CRAM
#define EXMC_MEMORY_TYPE_PSRAM		SNCTL_NRTP(1)
// NOR flash
#define EXMC_MEMORY_TYPE_NOR		SNCTL_NRTP(2)


// EXMC NOR/SRAM bank region definition
// bank0 NOR/SRAM region0
#define EXMC_BANK0_NORSRAM_REGION0	((uint32_t) 0x00000000U)

// EXMC NWAIT signal polarity configuration
// low level is active of NWAIT
#define EXMC_NWAIT_POLARITY_LOW		((uint32_t) 0x00000000U)
// high level is active of NWAIT
#define EXMC_NWAIT_POLARITY_HIGH	((uint32_t) 0x00000200U)


// deinitialize EXMC NOR/SRAM region
void exmc_norsram_deinit(uint32_t norsram_region);

// struct exmc_norsram_param parameter initialize
void exmc_norsram_struct_para_init(
		struct exmc_norsram_param *exmc_norsram_init_struct);

// initialize EXMC NOR/SRAM region
void exmc_norsram_init(struct exmc_norsram_param* exmc_norsram_init_struct);

// EXMC NOR/SRAM bank enable
void exmc_norsram_enable(uint32_t norsram_region);

// EXMC NOR/SRAM bank disable
void exmc_norsram_disable(uint32_t norsram_region);

#endif
