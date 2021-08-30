#ifndef _FLASH_MY_H
#define _FLASH_MY_H


#define FL_START_ADDR 	0x08010000										// Начало памяти для хранения данных
#define FL_END_ADDR 	0x08020000										// Конец памяти для хранения данных
#define FL_PAGE_SIZ 	0x400											// Размер страницы
#define FL_PAGE_NBR 	((FL_END_ADDR - FL_START_ADDR) / FL_PAGE_SIZ)	// Кол-во страниц

#include "stm32f1xx_hal.h"


void PVD_Init(void);										// Инициализация детектора напряжения

void flash_unlock(void);									// Разблокировать FLASH
void flash_lock();											// Заблокировать FLASH
uint8_t flash_ready(void);									// Проверка готовности FLASH
void flash_erase_page(uint32_t addr);						// Стирание страницы FLASH

uint32_t flash_read(uint32_t addr);							// Чтение FLASH
void flash_write(uint32_t addr,uint32_t data);				// Запись FLASH

#endif
