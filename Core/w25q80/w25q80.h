#ifndef _W25Q80_H
#define _W25Q80_H

#include "stm32f1xx_hal.h"


uint8_t W25Q80_init(SPI_HandleTypeDef* hspi);
uint8_t W25Q80_erase_sector(uint32_t addr);
uint8_t W25Q80_write_page(uint32_t addr);
uint8_t W25Q80_read_page(uint32_t addr);


