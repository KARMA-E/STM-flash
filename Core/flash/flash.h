#ifndef _FLASH_MY_H
#define _FLASH_MY_H

#include "stm32f1xx_hal.h"

#define FL_START_ADDR 		0x08008000										// Начало памяти для хранения данных
#define FL_END_ADDR 		0x08020000										// Конец памяти для хранения данных
#define FL_PAGE_SIZ 		0x400											// Размер страницы
#define FL_MEM_SIZ			(FL_END_ADDR - FL_START_ADDR)
#define FL_PAGE_NUM 		(FL_MEM_SIZ / FL_PAGE_SIZ)	// Кол-во страниц


void 		flash_debug_print(char* x);

void 		flash_unlock(void);
void 		flash_lock(void);

uint8_t 	flash_ready(void);
void 		flash_erase_page(uint32_t addr);
uint32_t 	flash_read(uint32_t addr);
void 		flash_write(uint32_t addr,uint32_t data);

#endif
