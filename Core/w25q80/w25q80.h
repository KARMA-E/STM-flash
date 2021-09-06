#ifndef _W25Q80_H
#define _W25Q80_H

#include "stm32f1xx_hal.h"

#define W25Q_STAT_BUSY		(0x01)

#define W25Q_START_ADDR 	0x000000										// Начало памяти для хранения данных
#define W25Q_END_ADDR 		0x100000										// Конец памяти для хранения данных
#define W25Q_PAGE_SIZ 		0x100											// Размер страницы
#define W25Q_BLOCK_SIZ 		0x1000											// Размер "блока" (в документации называется сектором)
#define W25Q_TARGET_SIZ		0x10000											// Размер "target" (в документации называется блоком)
#define W25Q_MEM_SIZ		(W25Q_END_ADDR - W25Q_START_ADDR)
#define W25Q_PAGE_NUM 		(W25Q_MEM_SIZ / W25Q_PAGE_SIZ)					// Кол-во страниц
#define W25Q_BLOCK_NUM 		(W25Q_MEM_SIZ / W25Q_BLOCK_SIZ)					// Кол-во блоков


uint8_t W25Q80_init(SPI_HandleTypeDef* hspi);
uint8_t W25Q80_erase_block(uint32_t addr);
uint8_t W25Q80_write_page(uint32_t addr, uint8_t* data_buf);
uint8_t W25Q80_read_page(uint32_t addr, uint8_t* data_buf);
uint8_t W25Q80_erase_all(void);


#endif
