#include "gd32vf103_crc.h"

#define CRC_DATA_RESET_VALUE	((uint32_t) 0xFFFFFFFFU)
#define CRC_FDATA_RESET_VALUE	((uint32_t) 0x00000000U)

/*
 * deinit CRC calculation unit
 */
void crc_deinit(void) {
	CRC_DATA = CRC_DATA_RESET_VALUE;
	CRC_FDATA = CRC_FDATA_RESET_VALUE;
	CRC_CTL = (uint32_t) CRC_CTL_RST;
}

/*
 * reset data register(CRC_DATA) to the value of 0xFFFFFFFF
 */
void crc_data_register_reset(void) {
	CRC_CTL |= (uint32_t) CRC_CTL_RST;
}

/*
 * read the value of the data register
 */
uint32_t crc_data_register_read(void) {
	uint32_t data;
	data = CRC_DATA;
	return (data);
}

/*
 * read the value of the free data register
 */
uint8_t crc_free_data_register_read(void) {
	uint8_t fdata;
	fdata = (uint8_t) CRC_FDATA;
	return (fdata);
}

/*
 * write data to the free data register
 */
void crc_free_data_register_write(uint8_t free_data) {
	CRC_FDATA = (uint32_t) free_data;
}

/*
 * calculate the CRC value of a 32-bit data
 */
uint32_t crc_single_data_calculate(uint32_t sdata) {
	CRC_DATA = sdata;
	return (CRC_DATA);
}

/*
 * calculate the CRC value of an array of 32-bit values
 */
uint32_t crc_block_data_calculate(uint32_t array[], uint32_t size) {
	for (uint32_t i = 0U; i < size; i++)
		CRC_DATA = array[i];

	return (CRC_DATA);
}
