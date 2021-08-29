
//========================================================= Подключение заголовочного файла =========================================================//

#include "flash.h"											// Заголовочный файл библиотеки работы FLASH

//====================================================== Функции, используемые в основном коде ======================================================//

void PVD_Init(void)											// Инициализация детектора напряжения питания
{
	__HAL_RCC_PWR_CLK_ENABLE();								// Включение тактирования детектора напряжения
	PWR_PVDTypeDef ConfigPVD;								// Переменная конфигурации детектора

	#ifdef STM32F1
		ConfigPVD.PVDLevel = PWR_CR_PLS_2V9;				// Напряжение детектирования 2.9V для F1
	#endif

	#ifdef STM32F4
		ConfigPVD.PVDLevel = PWR_CR_PLS_LEV7;				// Напряжение детектирования 2.9V для F4
	#endif

	ConfigPVD.Mode = PWR_PVD_MODE_IT_RISING;				// Прерывание по переднему фронту бита состояния
	HAL_PWR_ConfigPVD(&ConfigPVD);							// Конфигурация PVD

	uint8_t load = 0;										// Счетчик статуса нормального напряжения
	while(load<10)											// Пока счетчик меньше 10
	{
		if(PWR->CSR & PWR_CSR_PVDO) load = 0;				// Если напряжение ниже порогового сбросить счетчик
		else load++;										// Если напряжение в норме инкрементировать счетчик
		HAL_Delay(10);										// суммарная задержка алгоритма не менее 10 х 10мс
	}
	HAL_PWR_EnablePVD();									// Включение PVD
}


#ifdef STM32F1

void flash_unlock(void)										// РАЗБЛОКИРОВАТЬ FLASH
{
	FLASH->KEYR = FLASH_KEY1;								// Запись в регистр первого ключа
	FLASH->KEYR = FLASH_KEY2;								// Запись в регистр второго ключа
}

void flash_lock()											// ЗАБЛОКИРОВАТЬ FLASH
{
	FLASH->CR |= FLASH_CR_LOCK_Msk;							// Установка бита блокировки FLASH
}

uint8_t flash_ready(void)									// ПРОВЕРКА ГОТОВНОСТИ FLASH
{
	if(FLASH->SR & FLASH_SR_EOP_Msk)						// Если установлен бит завершения работы
	{
		FLASH->SR &=~ FLASH_SR_EOP_Msk;						// Сброс бита (сбрасывается только вручную)
		return 1;											// Возврат единицы
	}
	else													// Если бит не установлен
		return 0;											// Возврат нуля	
}

void flash_erase_page(uint32_t address)						// СТИРАНИЕ СТРАНИЦЫ FLASH
{
	FLASH->CR|= FLASH_CR_PER_Msk;							// Установка бита разрешения очистки страницы
	FLASH->AR = address;									// Запись адреса страницы
	FLASH->CR|= FLASH_CR_STRT_Msk;							// Запуск операции стирания
	while(!flash_ready());									// Ожидание завершения процесса стирания
	FLASH->CR &=~ FLASH_CR_PER_Msk;							// Сброс бита разрешения очистки страницы
}

uint32_t flash_read(uint32_t address)						// ЧТЕНИЕ FLASH
{
	return (*(__IO uint32_t*) address);						// Возвращает значение слова во FLASH по указанному адресу
}

void flash_write(uint32_t address,uint32_t data)			// ЗАПИСЬ FLASH
{

	FLASH->CR |= FLASH_CR_PG_Msk;							// Установка бита разрешения программирования
	while(!flash_ready());									// Ожидание готовности FLASH

	*(__IO uint16_t*)address = (uint16_t)data;				// Запись младшего полуслова по указанному адресу
	while(!flash_ready());									// Ожидание готовности FLASH

	address+=2;												// Смещение адреса на два байта (одно полуслово)
	data>>=16;												// Смещение данных для записи старшего полуслова
	*(__IO uint16_t*)address = (uint16_t)data;				// Запись старшего полуслова по указанному адресу
	while(!flash_ready());									// Ожидание готовности FLASH

	FLASH->CR &=~ FLASH_CR_PG_Msk;							// Сброс бита разрешения программирования
}

#endif

#ifdef STM32F4

void flash_unlock(void)										// РАЗБЛОКИРОВАТЬ FLASH
{
	;
}

void flash_lock()											// ЗАБЛОКИРОВАТЬ FLASH
{
	;
}

uint8_t flash_ready(void)									// ПРОВЕРКА ГОТОВНОСТИ FLASH
{
	return 1;
}

void flash_erase_page(uint32_t address)						// СТИРАНИЕ СТРАНИЦЫ FLASH
{
	;
}

uint32_t flash_read(uint32_t address)						// ЧТЕНИЕ FLASH
{
	return (*(__IO uint32_t*) address);						// Возвращает значение слова во FLASH по указанному адресу
}

void flash_write(uint32_t address,uint32_t data)			// ЗАПИСЬ FLASH
{
	;
}

#endif

//===================================================================================================================================================//
