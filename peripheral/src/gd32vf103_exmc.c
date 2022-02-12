#include "gd32vf103_exmc.h"

// EXMC bank0 register reset value
#define BANK0_SNCTL0_REGION_RESET	((uint32_t) 0x000030DAU)
#define BANK0_SNTCFG_RESET		((uint32_t) 0x0FFFFFFFU)

// EXMC register bit offset
#define SNCTL_NRMUX_OFFSET		((uint32_t) 1U)
#define SNCTL_WREN_OFFSET		((uint32_t) 12U)
#define SNCTL_NRWTEN_OFFSET		((uint32_t) 13U)
#define SNCTL_ASYNCWAIT_OFFSET		((uint32_t) 15U)

#define SNTCFG_AHLD_OFFSET		((uint32_t) 4U)
#define SNTCFG_DSET_OFFSET		((uint32_t) 8U)
#define SNTCFG_BUSLAT_OFFSET		((uint32_t) 16U)

/*
 * deinitialize EXMC NOR/SRAM region
 * norsram_region: select the region of bank0
 * 	EXMC_BANK0_NORSRAM_REGIONx(x=0)
 */
void exmc_norsram_deinit(uint32_t norsram_region) {
	// reset the registers
	if (norsram_region == EXMC_BANK0_NORSRAM_REGION0)
		EXMC_SNCTL(norsram_region) = BANK0_SNCTL0_REGION_RESET;
	EXMC_SNTCFG(norsram_region) = BANK0_SNTCFG_RESET;
}

void exmc_norsram_struct_para_init(
		struct exmc_norsram_param *exmc_norsram_init_struct) {

	// configure the structure with default value
	exmc_norsram_init_struct->norsram_region = EXMC_BANK0_NORSRAM_REGION0;
	exmc_norsram_init_struct->address_data_mux = ENABLE;
	exmc_norsram_init_struct->memory_type = EXMC_MEMORY_TYPE_SRAM;
	exmc_norsram_init_struct->databus_width = EXMC_NOR_DATABUS_WIDTH_16B;
	exmc_norsram_init_struct->nwait_polarity = EXMC_NWAIT_POLARITY_LOW;
	exmc_norsram_init_struct->memory_write = ENABLE;
	exmc_norsram_init_struct->nwait_signal = ENABLE;
	exmc_norsram_init_struct->asyn_wait = DISABLE;

	// read/write timing configure
	exmc_norsram_init_struct->read_write_timing->asyn_address_setuptime =
		0xFU;
	exmc_norsram_init_struct->read_write_timing->asyn_address_holdtime =
		0xFU;
	exmc_norsram_init_struct->read_write_timing->asyn_data_setuptime =
		0xFFU;
	exmc_norsram_init_struct->read_write_timing->bus_latency =
		0xFU;
}

/*
 * initialize EXMC NOR/SRAM region
 * struct exmc_norsram_param: configure the EXMC NOR/SRAM parameter
 * 	norsram_region:
 * 		EXMC_BANK0_NORSRAM_REGIONx,x=0
 * 	asyn_wait:
 * 		ENABLE or DISABLE
 * 	nwait_signal:
 * 		ENABLE or DISABLE
 * 	memory_write:
 * 		ENABLE or DISABLE
 * 	nwait_polarity:
 * 		EXMC_NWAIT_POLARITY_LOW,EXMC_NWAIT_POLARITY_HIGH
 * 	databus_width:
 * 		EXMC_NOR_DATABUS_WIDTH_8B,EXMC_NOR_DATABUS_WIDTH_16B
 * 	memory_type:
 * 		EXMC_MEMORY_TYPE_SRAM, EXMC_MEMORY_TYPE_PSRAM,
 * 		EXMC_MEMORY_TYPE_NOR
 * 	address_data_mux:
 * 		ENABLE
 * 	read_write_timing:
 * 		structure struct exmc_norsram_timing_param set the time
*/
void exmc_norsram_init(struct exmc_norsram_param *exmc_norsram_init_struct) {
	// get the register value
	uint32_t snctl = EXMC_SNCTL(exmc_norsram_init_struct->norsram_region);

	// clear relative bits
	snctl &= ~(EXMC_SNCTL_NREN | EXMC_SNCTL_NRTP | EXMC_SNCTL_NRW |
			EXMC_SNCTL_NRWTPOL | EXMC_SNCTL_WREN |
			EXMC_SNCTL_NRWTEN | EXMC_SNCTL_ASYNCWAIT |
			EXMC_SNCTL_NRMUX);

	snctl |=
		(exmc_norsram_init_struct->address_data_mux <<
		 SNCTL_NRMUX_OFFSET) |
		exmc_norsram_init_struct->memory_type |
		exmc_norsram_init_struct->databus_width |
		exmc_norsram_init_struct->nwait_polarity |
		(exmc_norsram_init_struct->memory_write <<
		 SNCTL_WREN_OFFSET) |
		(exmc_norsram_init_struct->nwait_signal <<
		 SNCTL_NRWTEN_OFFSET) |
		(exmc_norsram_init_struct->asyn_wait <<
		 SNCTL_ASYNCWAIT_OFFSET);

	uint32_t sntcfg =
	    ((exmc_norsram_init_struct->read_write_timing->
	      asyn_address_setuptime - 1) &
	     EXMC_SNTCFG_ASET) |
	    (((exmc_norsram_init_struct->read_write_timing->
	       asyn_address_holdtime - 1) << SNTCFG_AHLD_OFFSET) &
	     EXMC_SNTCFG_AHLD) |
	    (((exmc_norsram_init_struct->read_write_timing->
	       asyn_data_setuptime - 1) << SNTCFG_DSET_OFFSET) &
	     EXMC_SNTCFG_DSET) |
	    (((exmc_norsram_init_struct->read_write_timing->bus_latency - 1) <<
	      SNTCFG_BUSLAT_OFFSET) &
	     EXMC_SNTCFG_BUSLAT);

	// nor flash access enable
	if (exmc_norsram_init_struct->memory_type == EXMC_MEMORY_TYPE_NOR)
		snctl |= EXMC_SNCTL_NREN;

	// configure the registers
	EXMC_SNCTL(exmc_norsram_init_struct->norsram_region) = snctl;
	EXMC_SNTCFG(exmc_norsram_init_struct->norsram_region) = sntcfg;
}

void exmc_norsram_enable(uint32_t norsram_region) {
	EXMC_SNCTL(norsram_region) |= EXMC_SNCTL_NRBKEN;
}

void exmc_norsram_disable(uint32_t norsram_region) {
	EXMC_SNCTL(norsram_region) &= ~EXMC_SNCTL_NRBKEN;
}
