#include "gd32vf103_crc.h"

#define CRC_DATA_RESET_VALUE      ((uint32_t)0xFFFFFFFFU)
#define CRC_FDATA_RESET_VALUE     ((uint32_t)0x00000000U)

/*!
    \brief      deinit CRC calculation unit
    \param[in]  none
    \param[out] none
    \retval     none
*/
void crc_deinit(void) {
	CRC_DATA = CRC_DATA_RESET_VALUE;
	CRC_FDATA = CRC_FDATA_RESET_VALUE;
	CRC_CTL = (uint32_t) CRC_CTL_RST;
}

/*!
    \brief      reset data register(CRC_DATA) to the value of 0xFFFFFFFF
    \param[in]  none
    \param[out] none
    \retval     none
*/
void crc_data_register_reset(void) {
	CRC_CTL |= (uint32_t) CRC_CTL_RST;
}

/*!
    \brief      read the value of the data register
    \param[in]  none
    \param[out] none
    \retval     32-bit value of the data register
*/
uint32_t crc_data_register_read(void) {
	uint32_t data;
	data = CRC_DATA;
	return (data);
}

/*!
    \brief      read the value of the free data register
    \param[in]  none
    \param[out] none
    \retval     8-bit value of the free data register
*/
uint8_t crc_free_data_register_read(void) {
	uint8_t fdata;
	fdata = (uint8_t) CRC_FDATA;
	return (fdata);
}

/*!
    \brief      write data to the free data register
    \param[in]  free_data: specified 8-bit data
    \param[out] none
    \retval     none
*/
void crc_free_data_register_write(uint8_t free_data) {
	CRC_FDATA = (uint32_t) free_data;
}

/*!
    \brief      calculate the CRC value of a 32-bit data
    \param[in]  sdata: specified 32-bit data
    \param[out] none
    \retval     32-bit value calculated by CRC
*/
uint32_t crc_single_data_calculate(uint32_t sdata) {
	CRC_DATA = sdata;
	return (CRC_DATA);
}

/*!
    \brief      calculate the CRC value of an array of 32-bit values
    \param[in]  array: pointer to an array of 32-bit values
    \param[in]  size: size of the array
    \param[out] none
    \retval     32-bit value calculated by CRC
*/
uint32_t crc_block_data_calculate(uint32_t array[], uint32_t size) {
	uint32_t index;
	for (index = 0U; index < size; index++) {
		CRC_DATA = array[index];
	}
	return (CRC_DATA);
}
