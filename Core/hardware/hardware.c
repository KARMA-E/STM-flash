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
#if MULTI_CHIP_EN
	for(int i = 0; i < CHIP_QTY; i++)
	{
		GPIO_SET(_cs_port_map[i], _cs_pin_map[i]);
	}
#else
	GPIO_SET(_cs_port_map[SINGLE_CHIP_NUM], _cs_pin_map[SINGLE_CHIP_NUM]);
#endif
}

void CS_reset(uint8_t cs)
{
#if MULTI_CHIP_EN
	if(cs >= 0 && cs < CHIP_QTY)
	{
		CS_set();
		volatile uint32_t wait = 10;
		while(--wait);
		GPIO_RESET(_cs_port_map[cs], _cs_pin_map[cs]);
	}
#else
	GPIO_RESET(_cs_port_map[SINGLE_CHIP_NUM], _cs_pin_map[SINGLE_CHIP_NUM]);
#endif
}



