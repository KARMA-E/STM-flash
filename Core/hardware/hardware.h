#ifndef _HARDWARE_H
#define _HARDWARE_H

#include "stm32f1xx_hal.h"



#define LED_BLUE_PIN			GPIO_PIN_12
#define LED_BLUE_PORT			GPIOB
#define LED_RED_PIN				GPIO_PIN_13
#define LED_RED_PORT			GPIOB

#define GPIO_SET(port, pin)		(port->BSRR = pin)
#define GPIO_RESET(port, pin)	(port->BSRR = (pin<< 16))
#define GPIO_READ(port, pin)	(!(!(port->ODR & pin)))

#define TIME_START_USEC 	TIM2->CNT = 0; TIM3->CNT = 0;
#define TIME_GET_USEC		(TIM3->CNT + (10000 * TIM2->CNT))


void 	CS_set(void);
void	CS_reset(uint8_t cs);


#endif


