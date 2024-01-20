#ifndef _HAL_USART_H
#define _HAL_USART_H


#define DEBUG_TX_PORT			GPIOA
#define DEBUF_TX_PIN	        GPIO_Pin_9
#define DEBUG_RX_PORT			GPIOA
#define DEBUF_RX_PIN	        GPIO_Pin_10


void Hal_Usart_Init(void);
	

#endif
