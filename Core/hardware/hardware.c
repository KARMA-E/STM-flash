#include "hardware.h"
#include "config.h"



uint16_t const _cs_pin_map[5] =
{
	GPIO_PIN_3,
	GPIO_PIN_15,
	GPIO_PIN_14,
	GPIO_PIN_13,
	GPIO_PIN_0
};

GPIO_TypeDef * const _cs_port_map[5] =
{
	GPIOA,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOB,
};


void CS_set(void)
{
#ifdef DEV_BOARD
	GPIO_SET(_cs_port_map[4], _cs_pin_map[4]);
#endif

#ifdef LUT_BOARD
	for(int i = 0; i < CHIP_QTY; i++)
	{
		GPIO_SET(_cs_port_map[i], _cs_pin_map[i]);
	}
#endif
}

void CS_reset(uint8_t cs)
{
#ifdef DEV_BOARD
	GPIO_RESET(_cs_port_map[4], _cs_pin_map[4]);
#endif

#ifdef LUT_BOARD
	if(cs >= 0 && cs < CHIP_QTY)
	{
		CS_set();
		volatile uint32_t wait = 10;
		while(--wait);
		GPIO_RESET(_cs_port_map[cs], _cs_pin_map[cs]);
	}
#endif
}



