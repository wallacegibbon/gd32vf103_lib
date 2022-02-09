#ifndef GD32VF103_CRC_H
#define GD32VF103_CRC_H

#include "gd32vf103.h"

// CRC definitions
#define CRC			CRC_BASE

// registers definitions
#define CRC_DATA		REG32(CRC + 0x00U) // CRC data register
#define CRC_FDATA		REG32(CRC + 0x04U) // CRC free data register
#define CRC_CTL			REG32(CRC + 0x08U) // CRC control register

#define CRC_DATA_DATA		BITS(0, 31) // CRC calculation result bits
#define CRC_FDATA_FDATA		BITS(0, 7) // CRC free data bits
#define CRC_CTL_RST		BIT(0) // CRC reset CRC_DATA register bit

void crc_deinit(void);

// reset data register(CRC_DATA) to the value of 0xFFFFFFFF
void crc_data_register_reset(void);
uint32_t crc_data_register_read(void);

uint8_t crc_free_data_register_read(void);
void crc_free_data_register_write(uint8_t free_data);

uint32_t crc_single_data_calculate(uint32_t sdata);
uint32_t crc_block_data_calculate(uint32_t array[], uint32_t size);

#endif
