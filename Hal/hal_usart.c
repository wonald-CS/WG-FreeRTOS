#include "hal_usart.h"
#include <stdio.h>
#include "stm32F10x.h"
#include "string.h"
#include "SysTick.h"


// 定义串口3的发送和接收队列
QueueHandle_t USART3RxQueue;
QueueHandle_t USART3TxQueue;



//串口初始化
void Hal_Usart_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;


    USART3RxQueue = xQueueCreate(10, 10*sizeof(uint8_t));
    USART3TxQueue = xQueueCreate(10, sizeof(uint8_t*));


	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD,ENABLE);

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5,ENABLE);


/****************GPIO Setting *******************************************/
///  USART1_TX -> PA9  		
	GPIO_InitStructure.GPIO_Pin = DEBUF_TX_PIN;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_TX_PORT, &GPIO_InitStructure);		   
//  USART1_RX -> PA10
	GPIO_InitStructure.GPIO_Pin = DEBUF_RX_PIN;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(DEBUG_RX_PORT, &GPIO_InitStructure);

///  USART5_TX -> PC12 			
	GPIO_InitStructure.GPIO_Pin = LORA_TX_PIN;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LORA_TX_PORT, &GPIO_InitStructure);	
	
/// USART5_RX -> PD2
	GPIO_InitStructure.GPIO_Pin = LORA_RX_PIN;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(LORA_RX_PORT, &GPIO_InitStructure);

///  USART3_TX -> PB10 			
	GPIO_InitStructure.GPIO_Pin = WIFI_TX_PIN;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WIFI_TX_PORT, &GPIO_InitStructure);	
	
/// USART3_RX -> PB11
	GPIO_InitStructure.GPIO_Pin = WIFI_RX_PIN;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(WIFI_RX_PORT, &GPIO_InitStructure);



	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(DEBUG_USART_PORT, &USART_InitStructure); 
	USART_Init(WIFI_USART_PORT, &USART_InitStructure); 
  	USART_Init(LORA_UART_PORT, &USART_InitStructure); 


//Usart1 NVIC  
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	

//Usart3 NVIC  
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 
	NVIC_Init(&NVIC_InitStructure);	 
  
///UART5 NVIC
  	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	 
	
	
  	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); 	
	
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART3, ENABLE);
	//USART_Cmd(UART5, ENABLE);
	
}




//***************     USART3     WIFI ESP8266串口通讯    ****************//
void USART3_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t data;
    
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        // 读取接收到的数据
        data = USART_ReceiveData(USART3);
        
        // 将数据发送到接收队列
        xQueueSendFromISR(USART3RxQueue, &data, &xHigherPriorityTaskWoken);
        
        // 清除接收中断标志
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
    
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}





int fputc( int ch, FILE *f )
{
	USART_TypeDef* USARTx = USART1;
	while ((USARTx->SR & (1<<7)) == 0);
	USARTx->DR = ch;
	return ch;
}
