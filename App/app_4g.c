#include <stdio.h>
#include "app_4g.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "stm32F10x.h"
#include "string.h"
#include "event_groups.h"
#include "queue.h"
#include "hal_usart.h"
#include "hal_GPIO.h"


QueueHandle_t EC200N_Queue;

static unsigned char Powertime = 0;

/*******************************************************************************************
*@description:WG模块开机任务函数
*@param[in]：无
*@return：无
*@others：
********************************************************************************************/
static void EC200S_PutOnHandler(void)
{
	 if(Powertime < 20)
	 {
		Powertime ++;
		if(Powertime == 1){
			hal_GPIO_4GPowerKey_H();
		}		
		else if(Powertime == 15){	//把4G电源模块拉高750MS再拉低，以作开机
			hal_GPIO_4GPowerKey_L(); 
		}
	}
}



void EC200N_Task(void *pvParameters)
{
	uint8_t Data[50];
	uint8_t Ret,i;
	uint8_t Buff[5] = "AT\r\n";
	static unsigned int Tick=0;
    
	EC200N_Queue = xQueueCreate(10, 20*sizeof(uint8_t));	
    
	while(1)
	{
		EC200S_PutOnHandler();
		if(Powertime >= 20){
			// if (tick_check(Tick,MS_TO_TICK(5000)))
			// {
			// 	Tick = xTaskGetTickCount();
			// 	for(i = 0;i < 4;i++){
			// 		USART_SendData(USART2,Buff[i]);
			// 		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);      
			// 	}
			// }
			
			Ret = xQueueReceive(EC200N_Queue, &Data, 10);
			if (Ret)
			{
				printf("Data:");	
				printf("%s",Data);
				printf("\n");
			}
		}


			
		



		vTaskDelay(50);		
	}
}
