#include "hal_usart.h"
#include <stdio.h>
#include "stm32F10x.h"
#include "string.h"
#include "SysTick.h"
#include "app_wifi.h"
#include "app_lora.h"
#include "app_4g.h"



//串口初始化
void Hal_Usart_Init(void)
{     
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD,ENABLE);

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5,ENABLE);


/****************GPIO Setting *******************************************/
//  USART1_TX -> PA9  		
	GPIO_InitStructure.GPIO_Pin = DEBUF_TX_PIN;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_TX_PORT, &GPIO_InitStructure);		   
//  USART1_RX -> PA10
	GPIO_InitStructure.GPIO_Pin = DEBUF_RX_PIN;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(DEBUG_RX_PORT, &GPIO_InitStructure);

//  USART2_TX -> PA2 ,		
	GPIO_InitStructure.GPIO_Pin = EC200N_TX_PIN;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EC200N_TX_PORT, &GPIO_InitStructure);		
	
//  USART2_RX ->	PA3
	GPIO_InitStructure.GPIO_Pin = EC200N_RX_PIN;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(EC200N_RX_PORT, &GPIO_InitStructure);

//  USART5_TX -> PC12 			
	GPIO_InitStructure.GPIO_Pin = LORA_TX_PIN;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LORA_TX_PORT, &GPIO_InitStructure);	
	
//  USART5_RX -> PD2
	GPIO_InitStructure.GPIO_Pin = LORA_RX_PIN;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(LORA_RX_PORT, &GPIO_InitStructure);

//   USART3_TX -> PB10 			
	GPIO_InitStructure.GPIO_Pin = WIFI_TX_PIN;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WIFI_TX_PORT, &GPIO_InitStructure);	
	
//  USART3_RX -> PB11
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
	
	USART_Init(EC200N_USART_PORT, &USART_InitStructure); 
	USART_Init(DEBUG_USART_PORT, &USART_InitStructure); 
	USART_Init(WIFI_USART_PORT, &USART_InitStructure); 
  	USART_Init(LORA_UART_PORT, &USART_InitStructure); 


//  Usart1 NVIC  
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);

//  Usart2 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4 ;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	

//  Usart3 NVIC  
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5 ; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 
	NVIC_Init(&NVIC_InitStructure);	 
  
//  UART5 NVIC
  	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	 
	
	
  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);      //打开接收中断。
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);      //打开空闲中断。 	
//	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); 	
	
	USART_Cmd(USART1, ENABLE);
//	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
//	USART_Cmd(UART5, ENABLE);
	
}



//***************     UART5     LORA 串口通讯    ****************//
void UART5_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    unsigned char Rec_Data[50];
    static unsigned char len;
	static unsigned char dataIndex = 0;
    Data_AppNet_Receive *Que_data = NULL;
    
    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {

        // 读取接收到的数据
        Rec_Data[dataIndex++] = USART_ReceiveData(UART5);


		if(Rec_Data[0] != 0xFE){
			len = 0;
			dataIndex = 0;
            // 清除接收中断标志
            USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		}else{
            // 根据数据包类型设置长度
			if (Rec_Data[1] == Else_Move_Len){
				len = sizeof(Data_Else_Receive);
			}else if (Rec_Data[1] == App_Net_Len){
				len = sizeof(Data_AppNet_Receive);
			}
        }
            	

    }



    //单包数据完整，发送到队列，清除接收中断标志
	if (dataIndex == len)
	{

        
        Que_data = pvPortMalloc(len);
        if (Que_data != NULL)
        {
            Que_data = (Data_AppNet_Receive *)Rec_Data;
            xQueueSendFromISR(LoraQueue, Que_data, &xHigherPriorityTaskWoken);
        }

        len = 0;
        dataIndex = 0;
        if (Que_data != NULL)
        {
            vPortFree(Que_data); // 释放内存
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

		// 清除接收中断标志
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);          
        

	}

  
}


//***************     USART3     WIFI ESP8266串口通讯    ****************//
void USART3_IRQHandler(void)
{
	uint8_t Clear = Clear;
	uint8_t Data[300];
	static uint8_t dataIndex = 0;
     
    
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)       //如果接收到1个字节
	{
        USART_ClearITPendingBit(USART3,USART_IT_RXNE); 
		Data[dataIndex++] = USART_ReceiveData(USART3);
        if(dataIndex == 300){
            dataIndex = 0;
        }

	}



    if(USART_GetITStatus(USART3,USART_IT_IDLE) != RESET) //如果接收到了一帧数据  //能让死循环中断跳出
    {   
        xQueueSendFromISR(Wifi_Queue, &Data, 0);
        dataIndex = 0;
        memset(Data,0,300);
        Clear = USART3->SR; // 读取SR寄存器
        Clear = USART3->DR; // 读DR寄存器(先读SR再读DR寄存器，清楚IDLE中断标志位)
        USART_ClearITPendingBit(USART1,USART_IT_IDLE);//清除空闲中断标志位        
   
       
    }   
 

//    if (USART_GetITStatus(USART3, USART_IT_ORE) != RESET)         //ORE是检测到数据溢出，但是这里没有能够跳出 死循环进入串口中断
//    {
//        USART_ReceiveData(USART3);
//    } 	

}


void USART2_IRQHandler(void)
{
//    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//    unsigned char Data[50];
//    static unsigned char dataIndex = 0;
//    
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{							
//		Data[dataIndex++] = USART_ReceiveData(USART2);
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
// 
//	} 
//    
//    if(dataIndex == 4 || dataIndex == 5 || dataIndex == 6){
//		dataIndex = 0;
//		xQueueSendFromISR(EC200N_Queue, &Data, &xHigherPriorityTaskWoken);	
//		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//	}
}

#define USART_REC_LEN 1000

u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0;     //接收状态标记

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
    u8 Res;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res = USART_ReceiveData(USART1);	//读取接收到的数据

        if((USART_RX_STA & 0x8000) == 0) //接收未完成
        {
            if(USART_RX_STA & 0x4000) //接收到了0x0d
            {
                if(Res != 0x0a)USART_RX_STA = 0; //接收错误,重新开始
                else USART_RX_STA |= 0x8000;	//接收完成了
                USART_ClearITPendingBit(USART1,USART_IT_RXNE);
            }
            else //还没收到0X0D
            {
                if(Res == 0x0d)USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res ;
                    USART_RX_STA++;
                    if(USART_RX_STA > (USART_REC_LEN - 1))
                    {        
                        USART_RX_STA = 0; //接收数据错误,重新开始接收
                        USART_ClearITPendingBit(USART1,USART_IT_RXNE);                        
                    }
                }
            }
        }
    }
}


#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

    

int fputc( int ch, FILE *f )
{
//	USART_TypeDef* USARTx = USART1;
//	while ((USARTx->SR & (1<<7)) == 0);
//	USARTx->DR = ch;
//	return ch;
    
    /* 发送一个字节数据到串口 */
    USART_SendData(USART1, (uint8_t) ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		

    return (ch);

}
