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


typedef struct{
	uint8_t RET;
	uint8_t BBA;
	uint8_t CCD;
}Test_Pra;

Test_Pra Test_Str;

void Test_Task(void *pvParameters)
{
	unsigned char u8Ret = 0;
	static unsigned int u32TestTick = 0; 
	char pbuffer[2048];
	
	unsigned char Ret = 0;
	static unsigned int TestTick = 0; 
	uint8_t TX_Buff[4] ={"ATOK"};
	uint8_t RX_Buff[4];
	uint8_t i;

	
	QueueHandle_t StrTxQueue;
	unsigned char Bet = 0;
	static unsigned int BestTick = 0; 	
	Test_Str.BBA = 1;
	Test_Str.CCD = 2;
	Test_Str.RET = 3;
	Test_Pra Test_Rx;
	StrTxQueue = xQueueCreate(1, sizeof(Test_Pra));

	SemaphoreHandle_t Test_Mutex = xSemaphoreCreateMutex();
	
	while(1)
	{
		
		u8Ret = tick_check(u32TestTick,MS_TO_TICK(30000));
		if(u8Ret){
			u32TestTick = xTaskGetTickCount();
			vTaskList(pbuffer);
			printf("--------------------����------------------------\r\n");
			printf("����           ״̬   ���ȼ�   ʣ��ջ  ���\r\n");
			printf("%s", pbuffer);
			printf("B������  R������  D��ɾ��  S����ͣ  X������ \r\n");
			
		}
	
		
//		Ret = tick_check(TestTick,MS_TO_TICK(5000));
//		if(Ret){
//			TestTick = xTaskGetTickCount();
			if(xQueueSemaphoreTake(Test_Mutex,portMAX_DELAY))
			{
				if(!TestTick)
				{
					TestTick = 1;
					if(xQueueSend(USART3TxQueue, &TX_Buff, portMAX_DELAY))
					{
						if(xQueueReceive(USART3TxQueue, RX_Buff, portMAX_DELAY))
						{
							printf("���д��������Ӧ�ã� \r\n");
							for(i=0;i<4;i++){
								printf("RX_Buff[%d]:%c\r\n",i,RX_Buff[i]);
							}
						}
					}
				}
				xSemaphoreGive(Test_Mutex);
			}


//		}
			
			
//		Bet = tick_check(BestTick,MS_TO_TICK(7000));
//		if(Bet){
//			BestTick = xTaskGetTickCount();
			if(!BestTick)
			{
				BestTick = 1;
				if(xQueueSend(StrTxQueue, &Test_Str, portMAX_DELAY))
				{
					if(xQueueReceive(StrTxQueue, &Test_Rx, portMAX_DELAY))
					{
						printf("���д���ṹ���Ӧ�ã� \r\n");
						printf("Test_Rx.BBA:%d\r\n",Test_Rx.BBA);
						printf("Test_Rx.CCD:%d\r\n",Test_Rx.CCD);
						printf("Test_Rx.RET:%d\r\n",Test_Rx.RET);
					}
				}
			}

			
//		}
			
			 
			
		
		vTaskDelay(100);
	}
}

