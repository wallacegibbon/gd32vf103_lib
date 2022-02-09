#include "gd32vf103_crc.h"

#define CRC_DATA_RESET_VALUE	((uint32_t) 0xFFFFFFFFU)
#define CRC_FDATA_RESET_VALUE	((uint32_t) 0x00000000U)

void crc_deinit(void) {
	CRC_DATA = CRC_DATA_RESET_VALUE;
	CRC_FDATA = CRC_FDATA_RESET_VALUE;
	CRC_CTL = (uint32_t) CRC_CTL_RST;
}

/*
 * reset data register(CRC_DATA) to the value of 0xFFFFFFFF
 */
void crc_data_register_reset(void) {
	CRC_CTL |= CRC_CTL_RST;
}

uint32_t crc_data_register_read(void) {
	return CRC_DATA;
}

uint8_t crc_free_data_register_read(void) {
	return CRC_FDATA;
}

void crc_free_data_register_write(uint8_t free_data) {
	CRC_FDATA = free_data;
}

uint32_t crc_single_data_calculate(uint32_t sdata) {
	CRC_DATA = sdata;
	return CRC_DATA;
}

uint32_t crc_block_data_calculate(uint32_t array[], uint32_t size) {
	for (uint32_t i = 0U; i < size; i++)
		CRC_DATA = array[i];

	return CRC_DATA;
}
