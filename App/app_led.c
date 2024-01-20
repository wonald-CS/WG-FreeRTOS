#include "hal_led.h"
#include "SysTick.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_led.h"


void Led_Task(void *pvParameters)
{
	unsigned char u8Ret = 0;
	static unsigned int u32LedTick = 0; 
	
	while(1)
	{
		u8Ret = tick_check(u32LedTick,MS_TO_TICK(2000));
		if(u8Ret){
			u32LedTick = xTaskGetTickCount();
			hal_LedTurn();
		}
		
		vTaskDelay(200);
	}
}
