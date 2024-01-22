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



void Key_Task(void *pvParameters)
{	
  uint8_t Ret;
  static uint32_t Tick;
  xEventGroupClearBits(KeyEvent,EVENT_ALL_BITS);
  for(;;)
  {
    Ret = tick_check(Tick,4000);
    if (Ret)
    {
      Tick = xTaskGetTickCount();
     	printf("HeartBeat!!!\r\n");
    }
    
    vTaskDelay(100);
  }
}
