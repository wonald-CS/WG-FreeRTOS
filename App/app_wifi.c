#include <stdio.h>
#include "app_wifi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "stm32F10x.h"
#include "string.h"
#include "event_groups.h"
#include "queue.h"
#include "hal_usart.h"
#include "hal_GPIO.h"

QueueHandle_t Wifi_Queue;
/*******************************************************************************************
*@description:WIFI模块外电或电源接入
*@param[in]：fuc：复位或者空闲
*@return：无
*@others： 1. 如果没电断电的情况i下， 关闭WIFI电源   2. 电源复位操作
********************************************************************************************/
unsigned char wifi_PowerManage()
{
	static unsigned short StartTime = 0;
	if(hal_Gpio_AcStateCheck() == STA_AC_LINK)
	{///USB 外部供电的情况下 
        if(StartTime == 0)
		{
			//mt_wifi_init();
			hal_GPIO_WIFIPowerEN_H();
		}
		if(StartTime < 1000)
		{
			StartTime ++;
			if(StartTime > 150)
			{
				StartTime = 2000;
				hal_GPIO_WIFIPowerEN_L();
			}
			return 0;
		}	
		return 1;
	}else{
		return 0;
	}
	
}



void Wifi_Task(void *pvParameters)
{
	uint8_t Data[50];
	uint8_t Ret;
    
	Wifi_Queue = xQueueCreate(10, 50*sizeof(uint8_t));	
    
	while(1)
	{
		if(wifi_PowerManage())
		{
			// USART_SendData(USART3,Buff[i]);
			// while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);                           

			Ret = xQueueReceive(Wifi_Queue, &Data, 10);
			if (Ret)
			{	

			}

			
		
		}



		vTaskDelay(100);		
	}
}
