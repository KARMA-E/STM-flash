#ifndef _FLASH_MY_H
#define _FLASH_MY_H

#include "stm32f1xx_hal.h"

#define FL_START_ADDR 		0x08008000										// Начало памяти для хранения данных
#define FL_END_ADDR 		0x08020000										// Конец памяти для хранения данных
#define FL_END_ADDR_REAL 	0x08020000
#define FL_PAGE_SIZ 		0x400											// Размер страницы
#define FL_MEM_SIZ			(FL_END_ADDR - FL_START_ADDR)
#define FL_PAGE_NUM 		(FL_MEM_SIZ / FL_PAGE_SIZ)	// Кол-во страниц


void 		flash_debug_print(char* x);


void 		flash_unlock(void);									// Разблокировать FLASH
void 		flash_lock();										// Заблокировать FLASH
uint8_t 	flash_ready(void);									// Проверка готовности FLASH
void 		flash_erase_page(uint32_t addr);					// Стирание страницы FLASH

uint32_t 	flash_read(uint32_t addr);							// Чтение FLASH
void 		flash_write(uint32_t addr,uint32_t data);			// Запись FLASH

#endif
