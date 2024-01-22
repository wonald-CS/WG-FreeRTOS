#include <stdio.h>
#include "app_key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "event_groups.h"
#include "queue.h"


//队列
QueueHandle_t KeyQueue;
//事件
EventGroupHandle_t KeyEvent;


//评估发现按键值只用队列即可完成功能，不过可以加上事件组来做（熟悉用法）
void Key_Task(void *pvParameters)
{	
	xEventGroupClearBits(KeyEvent,EVENT_ALL_BITS);
	for(;;)
	{

		vTaskDelay(100);
	}
}
