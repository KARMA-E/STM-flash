#include "flash.h"
#include "string.h"

extern UART_HandleTypeDef huart1;

void flash_debug_print(char* x)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)x, strlen((const char *)x), 100);
}

// Разблокировать
void flash_unlock(void)
{
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}


// Заблокировать
void flash_lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK_Msk;
}


// Готовность flash
static uint8_t _flash_ready(void)
{
#if (0)
	if(FLASH->SR & FLASH_SR_EOP)
	{
		FLASH->SR &=~ FLASH_SR_EOP;
		return 1;
	}

	return 0;
#else
	return !(FLASH->SR & FLASH_SR_BSY);
#endif
}


// Стереть страницу
static void _flash_erase_page(uint32_t addr)
{
	// Выбор режима стирания
	FLASH->CR|= FLASH_CR_PER;


	FLASH->AR = addr;

	// Запустить стирание
	FLASH->CR|= FLASH_CR_STRT;

	while(!_flash_ready());

	FLASH->CR &=~ FLASH_CR_PER;
}


// Чтение по адресу
static uint32_t _flash_read(uint32_t addr)
{
	return (*(__IO uint32_t*) addr);
}


// Запись по адресу
static void _flash_write(uint32_t addr,uint32_t data)
{
	// Выбор режима программирования
	FLASH->CR |= FLASH_CR_PG_Msk;

	while(!_flash_ready());

	*(__IO uint16_t*) addr = (uint16_t) data;

	while(!_flash_ready());

	addr += 2;
	data >>= 16;
	*(__IO uint16_t*)addr  = (uint16_t) data;

	while(!_flash_ready());

	FLASH->CR &=~ FLASH_CR_PG_Msk;
}


void FLASH_sector_write(uint32_t sect_num, uint8_t* buf)
{
	uint32_t cur_addr = 0;
	uint32_t cur_word = 0;

	cur_addr = FL_START_ADDR + (sect_num * FL_PAGE_SIZ);
	if(cur_addr < FL_END_ADDR) _flash_erase_page(cur_addr);

	for(uint32_t k = 0; k < FL_PAGE_SIZ; k += 4)
	{
		cur_word = 0;

		for(int8_t i = 3; i >= 0; i--)
		{
			cur_word += buf[k + i];
			if(i != 0) cur_word <<= 8;
		}

		_flash_write(cur_addr, cur_word);

		cur_addr += 4;
	}
}

void FLASH_sector_read(uint32_t sect_num, uint8_t* buf)
{
	uint32_t cur_addr = 0;
	uint32_t cur_word = 0;

	for(uint32_t k = 0; k < FL_PAGE_SIZ; k += 4)
	{
		cur_addr = FL_START_ADDR + (sect_num * FL_PAGE_SIZ) + k;
		if(cur_addr < FL_END_ADDR) cur_word = _flash_read(cur_addr);

		for(int i = 0; i < 4; i++)
		{
			buf[k + i] = (uint8_t)(cur_word & 0xFF);
			if(i != 3)cur_word >>= 8;
		}
	}
}

