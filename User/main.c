#include <stdio.h>
#include "stm32f10x.h"
#include "system.h"
#include "SysTick.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hal_init.h"
#include "app_led.h"
#include "app_key.h"


/*
 * Retargets the C library printf function to the USART.
 */
int fputc( int ch, FILE *f );


//������
TaskHandle_t LedTask_Handle;
TaskHandle_t KeyTask_Handle;



int main(void)
{
	//ϵͳ��ʼ��
	SysTick_Init(72);//ϵͳʱ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	Hal_Init();	//Ӳ����ʼ��


	//���г�ʼ��
	KeyQueue = xQueueCreate(10, sizeof(uint16_t));
	//�¼���ʼ��
	KeyEvent = xEventGroupCreate();

	
	//������ʼ����
	xTaskCreate((TaskFunction_t )Led_Task,            //������
				(const char*    )"Led_Task",          //��������
				(uint16_t       )50,        		  //�����ջ��С
				(void*          )NULL,                //���ݸ��������Ĳ���
				(UBaseType_t    )TASK_PRIORITY_LED,   //�������ȼ�
				(TaskHandle_t*  )&LedTask_Handle);    //������    

	xTaskCreate(Key_Task, "Key_Task", 200, NULL, TASK_PRIORITY_KEY, &KeyTask_Handle);   


	vTaskStartScheduler();   
}



