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
	uint8_t Tx_data,Rx_data;
	uint8_t Ret,ret;

	for(;;)
	{
		if(wifi_PowerManage())
		{

			Ret = xQueueReceive(USART3TxQueue, &Tx_data, 10);
			if (Ret)
			{			
				// 发送数据到串口
					USART_SendData(USART3, Tx_data);
					
				// 等待发送完成
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
			}
			
			ret = xQueueReceive(USART3RxQueue, &Rx_data, 10);
			if (ret)
			{
				
			}			
		}



		vTaskDelay(100);		
	}
}
