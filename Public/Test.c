#include <stdio.h>
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




void Test_Task(void *pvParameters)
{
	unsigned char u8Ret = 0;
	static unsigned int u32TestTick = 0; 
	char pbuffer[2048];
//	unsigned char Send_Data[50] = "Lora_Data_Send\n\r";
    
	SemaphoreHandle_t Test_Mutex = xSemaphoreCreateMutex();
	
	while(1)
	{
		
		u8Ret = tick_check(u32TestTick,MS_TO_TICK(5000));
		if(u8Ret){
            if(xSemaphoreTake(Test_Mutex,portMAX_DELAY == pdFALSE)){
                u32TestTick = xTaskGetTickCount();
                vTaskList(pbuffer);
                printf("--------------------����------------------------\r\n");
                printf("����           ״̬   ���ȼ�   ʣ��ջ  ���\r\n");
                printf("%s", pbuffer);
                printf("B������  R������  D��ɾ��  S����ͣ  X������ \r\n");
                //xQueueSend(LoraQueue,&Send_Data,10);
     			xSemaphoreGive(Test_Mutex);                
            }
	
	
		}
        
        
        
	
				
			
			
			 
			
		
		vTaskDelay(100);
	}
}

