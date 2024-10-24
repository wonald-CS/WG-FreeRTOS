#include <stdio.h>
#include "stm32f10x.h"
#include "SysTick.h"
#include "FreeRTOS.h"
#include "Test.h"
#include "task.h"
#include "SysTick.h"
#include "event_groups.h"
#include "queue.h"
#include "hal_usart.h"
#include "semphr.h"
#include "app_lora.h"
#include "hal_wtn6.h"




void Test_Task(void *pvParameters)
{
	unsigned char u8Ret = 0;
	static unsigned int u32TestTick = 0; 
	char pbuffer[2048];
    
	SemaphoreHandle_t Test_Mutex = xSemaphoreCreateMutex();
	
	while(1)
	{
		
		u8Ret = tick_check(u32TestTick,MS_TO_TICK(5000));
		if(u8Ret){
            if(xSemaphoreTake(Test_Mutex,portMAX_DELAY == pdFALSE)){
                u32TestTick = xTaskGetTickCount();
                vTaskList(pbuffer);
                printf("--------------------任务------------------------\r\n");
                printf("名称           状态   优先级   剩余栈  序号\r\n");
                printf("%s", pbuffer);
                printf("B：阻塞  R：就绪  D：删除  S：暂停  X：运行 \r\n");
                //hal_Wtn6_Play(WTN6_HOMEARM);
                //xQueueSend(LoraQueue,&Send_Data,10);
     			xSemaphoreGive(Test_Mutex);                
            }
	
	
		}
         
			
		
		vTaskDelay(100);
	}
}

