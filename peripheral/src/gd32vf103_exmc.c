#include "gd32vf103_exmc.h"

/* EXMC bank0 register reset value */
#define BANK0_SNCTL0_REGION_RESET         ((uint32_t)0x000030DAU)
#define BANK0_SNTCFG_RESET                ((uint32_t)0x0FFFFFFFU)

/* EXMC register bit offset */
#define SNCTL_NRMUX_OFFSET                ((uint32_t)1U)
#define SNCTL_WREN_OFFSET                 ((uint32_t)12U)
#define SNCTL_NRWTEN_OFFSET               ((uint32_t)13U)
#define SNCTL_ASYNCWAIT_OFFSET            ((uint32_t)15U)

#define SNTCFG_AHLD_OFFSET                ((uint32_t)4U)
#define SNTCFG_DSET_OFFSET                ((uint32_t)8U)
#define SNTCFG_BUSLAT_OFFSET              ((uint32_t)16U)

/*!
    \brief      deinitialize EXMC NOR/SRAM region
    \param[in]  norsram_region: select the region of bank0
      \arg        EXMC_BANK0_NORSRAM_REGIONx(x=0)
    \param[out] none
    \retval     none
*/
void exmc_norsram_deinit(uint32_t norsram_region) {
	/* reset the registers */
	if (EXMC_BANK0_NORSRAM_REGION0 == norsram_region) {
		EXMC_SNCTL(norsram_region) = BANK0_SNCTL0_REGION_RESET;
	}

	EXMC_SNTCFG(norsram_region) = BANK0_SNTCFG_RESET;
}

/*!
    \brief      initialize the structure struct exmc_norsram_param
    \param[in]  none
    \param[out] exmc_norsram_init_struct: the initialized structure struct exmc_norsram_param pointer
    \retval     none
*/
void exmc_norsram_struct_para_init(struct exmc_norsram_param *
				   exmc_norsram_init_struct) {
	/* configure the structure with default value */
	exmc_norsram_init_struct->norsram_region = EXMC_BANK0_NORSRAM_REGION0;
	exmc_norsram_init_struct->address_data_mux = ENABLE;
	exmc_norsram_init_struct->memory_type = EXMC_MEMORY_TYPE_SRAM;
	exmc_norsram_init_struct->databus_width = EXMC_NOR_DATABUS_WIDTH_16B;
	exmc_norsram_init_struct->nwait_polarity = EXMC_NWAIT_POLARITY_LOW;
	exmc_norsram_init_struct->memory_write = ENABLE;
	exmc_norsram_init_struct->nwait_signal = ENABLE;
	exmc_norsram_init_struct->asyn_wait = DISABLE;

	/* read/write timing configure */
	exmc_norsram_init_struct->read_write_timing->asyn_address_setuptime =
	    0xFU;
	exmc_norsram_init_struct->read_write_timing->asyn_address_holdtime =
	    0xFU;
	exmc_norsram_init_struct->read_write_timing->asyn_data_setuptime =
	    0xFFU;
	exmc_norsram_init_struct->read_write_timing->bus_latency = 0xFU;
}

/*!
    \brief      initialize EXMC NOR/SRAM region
    \param[in]  struct exmc_norsram_param: configure the EXMC NOR/SRAM parameter
                  norsram_region: EXMC_BANK0_NORSRAM_REGIONx,x=0
                  asyn_wait: ENABLE or DISABLE
                  nwait_signal: ENABLE or DISABLE
                  memory_write: ENABLE or DISABLE
                  nwait_polarity: EXMC_NWAIT_POLARITY_LOW,EXMC_NWAIT_POLARITY_HIGH
                  databus_width: EXMC_NOR_DATABUS_WIDTH_8B,EXMC_NOR_DATABUS_WIDTH_16B
                  memory_type: EXMC_MEMORY_TYPE_SRAM,EXMC_MEMORY_TYPE_PSRAM,EXMC_MEMORY_TYPE_NOR
                  address_data_mux: ENABLE
                  read_write_timing: structure struct exmc_norsram_timing_param set the time
    \param[out] none
    \retval     none
*/
void exmc_norsram_init(struct exmc_norsram_param * exmc_norsram_init_struct) {
	uint32_t snctl = 0x00000000U, sntcfg = 0x00000000U;

	/* get the register value */
	snctl = EXMC_SNCTL(exmc_norsram_init_struct->norsram_region);

	/* clear relative bits */
	snctl &=
	    ((uint32_t) ~
	     (EXMC_SNCTL_NREN | EXMC_SNCTL_NRTP | EXMC_SNCTL_NRW |
	      EXMC_SNCTL_NRWTPOL | EXMC_SNCTL_WREN | EXMC_SNCTL_NRWTEN |
	      EXMC_SNCTL_ASYNCWAIT | EXMC_SNCTL_NRMUX));

	snctl |=
	    (uint32_t) (exmc_norsram_init_struct->
			address_data_mux << SNCTL_NRMUX_OFFSET) |
	    exmc_norsram_init_struct->memory_type | exmc_norsram_init_struct->
	    databus_width | exmc_norsram_init_struct->
	    nwait_polarity | (exmc_norsram_init_struct->
			      memory_write << SNCTL_WREN_OFFSET) |
	    (exmc_norsram_init_struct->
	     nwait_signal << SNCTL_NRWTEN_OFFSET) | (exmc_norsram_init_struct->
						     asyn_wait <<
						     SNCTL_ASYNCWAIT_OFFSET);

	sntcfg =
	    (uint32_t) ((exmc_norsram_init_struct->read_write_timing->
			 asyn_address_setuptime -
			 1U) & EXMC_SNTCFG_ASET) | (((exmc_norsram_init_struct->
						      read_write_timing->
						      asyn_address_holdtime -
						      1U) << SNTCFG_AHLD_OFFSET)
						    & EXMC_SNTCFG_AHLD) |
	    (((exmc_norsram_init_struct->read_write_timing->
	       asyn_data_setuptime -
	       1U) << SNTCFG_DSET_OFFSET) & EXMC_SNTCFG_DSET) |
	    (((exmc_norsram_init_struct->read_write_timing->bus_latency -
	       1U) << SNTCFG_BUSLAT_OFFSET) & EXMC_SNTCFG_BUSLAT);

	/* nor flash access enable */
	if (EXMC_MEMORY_TYPE_NOR == exmc_norsram_init_struct->memory_type) {
		snctl |= (uint32_t) EXMC_SNCTL_NREN;
	}

	/* configure the registers */
	EXMC_SNCTL(exmc_norsram_init_struct->norsram_region) = snctl;
	EXMC_SNTCFG(exmc_norsram_init_struct->norsram_region) = sntcfg;
}

/*!
    \brief      enable EXMC NOR/PSRAM bank region
    \param[in]  norsram_region: specify the region of NOR/PSRAM bank
      \arg        EXMC_BANK0_NORSRAM_REGIONx(x=0)
    \param[out] none
    \retval     none
*/
void exmc_norsram_enable(uint32_t norsram_region) {
	EXMC_SNCTL(norsram_region) |= (uint32_t) EXMC_SNCTL_NRBKEN;
}

/*!
    \brief      disable EXMC NOR/PSRAM bank region
    \param[in]  norsram_region: specify the region of NOR/PSRAM bank
      \arg        EXMC_BANK0_NORSRAM_REGIONx(x=0)
    \param[out] none
    \retval     none
*/
void exmc_norsram_disable(uint32_t norsram_region) {
	EXMC_SNCTL(norsram_region) &= ~(uint32_t) EXMC_SNCTL_NRBKEN;
}
