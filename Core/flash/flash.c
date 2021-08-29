#include "flash.h"


// Разблокировать
void flash_unlock(void)
{
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}


// Заблокировать
void flash_lock()
{
	FLASH->CR |= FLASH_CR_LOCK_Msk;
}


// Готовность flash
uint8_t flash_ready(void)
{
	if(FLASH->SR & FLASH_SR_EOP_Msk)
	{
		FLASH->SR &=~ FLASH_SR_EOP_Msk;
		return 1;
	}

	return 0;
}


// Стереть страницу
void flash_erase_page(uint32_t addr)
{
	// Выбор режима стирания
	FLASH->CR|= FLASH_CR_PER_Msk;


	FLASH->AR = addr;

	// Запустить стирание
	FLASH->CR|= FLASH_CR_STRT_Msk;

	while(!flash_ready());

	FLASH->CR &=~ FLASH_CR_PER_Msk;
}


// Чтение по адресу
uint32_t flash_read(uint32_t addr)
{
	return (*(__IO uint32_t*) addr);
}


// Запись по адресу
void flash_write(uint32_t addr,uint32_t data)
{
	// Выбор режима программирования
	FLASH->CR |= FLASH_CR_PG_Msk;

	while(!flash_ready());

	*(__IO uint16_t*) addr = (uint16_t) data;

	while(!flash_ready());

	addr += 2;
	data >>= 16;
	*(__IO uint16_t*)addr = (uint16_t) data;

	while(!flash_ready());

	FLASH->CR &=~ FLASH_CR_PG_Msk;
}
