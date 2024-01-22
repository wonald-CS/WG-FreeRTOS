#include <stdio.h>
#include "SysTick.h"
#include "FreeRTOS.h"
#include "Test.h"
#include "task.h"
#include "SysTick.h"
#include "event_groups.h"
#include "queue.h"


void Test_Task(void *pvParameters)
{
	unsigned char u8Ret = 0;
	static unsigned int u32TestTick = 0; 
	char pbuffer[2048];
	
	while(1)
	{

		u8Ret = tick_check(u32TestTick,MS_TO_TICK(60000));
		if(u8Ret){
			u32TestTick = xTaskGetTickCount();
			vTaskList(pbuffer);
			printf("--------------------����------------------------\r\n");
			printf("����           ״̬   ���ȼ�   ʣ��ջ  ���\r\n");
			printf("%s", pbuffer);
		}
		
		vTaskDelay(100);
	}
}
