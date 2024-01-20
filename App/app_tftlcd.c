#include "app_tftlcd.h"
#include "mt_tftlcd.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "event_groups.h"
#include "queue.h"





void TftLcd_Task(void *pvParameters)
{	
	LCD_Fill(0,0,LCD_W,LCD_H,YELLOW);
	for(;;)
	{    
		vTaskDelay(100);
	}
}
