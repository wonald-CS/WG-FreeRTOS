#include <stdio.h>
#include "app_key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "event_groups.h"
#include "queue.h"


//����
QueueHandle_t KeyQueue;
//�¼�
EventGroupHandle_t KeyEvent;


//�������ְ���ֵֻ�ö��м�����ɹ��ܣ��������Լ����¼�����������Ϥ�÷���
void Key_Task(void *pvParameters)
{	
	xEventGroupClearBits(KeyEvent,EVENT_ALL_BITS);
	for(;;)
	{

		vTaskDelay(100);
	}
}
