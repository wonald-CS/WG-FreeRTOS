#ifndef _HAL_LED_H
#define _HAL_LED_H

#define LED1_PORT			GPIOC
#define LED1_PIN			GPIO_Pin_14

#define LED2_PORT			GPIOC
#define LED2_PIN			GPIO_Pin_15


void hal_LedInit(void);
void hal_LedTurn(void);
	
#endif

