#ifndef _HAL_USART_H
#define _HAL_USART_H

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"


#define DEBUG_TX_PORT	GPIOA
#define DEBUF_TX_PIN	GPIO_Pin_9

#define DEBUG_RX_PORT	GPIOA
#define DEBUF_RX_PIN	GPIO_Pin_10

#define LORA_RX_PORT	GPIOD
#define LORA_RX_PIN	    GPIO_Pin_2

#define LORA_TX_PORT	GPIOC
#define LORA_TX_PIN	    GPIO_Pin_12

#define WIFI_TX_PORT	GPIOB
#define WIFI_TX_PIN	    GPIO_Pin_10

#define WIFI_RX_PORT	GPIOB
#define WIFI_RX_PIN	    GPIO_Pin_11


#define DEBUG_USART_PORT	USART1
#define WIFI_USART_PORT     USART3
#define LORA_UART_PORT      UART5




void Hal_Usart_Init(void);


#endif
