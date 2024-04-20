#include <stdio.h>
#include "stm32f10x.h"
#include "system.h"
#include "SysTick.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hal_init.h"
#include "Test.h"
#include "app_led.h"
#include "app_key.h"
#include "app_tftlcd.h"
#include "app_al6630.h"
#include "app_wifi.h"
#include "app_lora.h"



/*
 * Retargets the C library printf function to the USART.
 */
int fputc( int ch, FILE *f );


//������
TaskHandle_t LedTask_Handle;
TaskHandle_t KeyTask_Handle;
TaskHandle_t TftLcd_Task_Handle;
TaskHandle_t TemHum_Task_Handle;
TaskHandle_t Test_Task_Handle;
TaskHandle_t Wifi_Task_Handle;
TaskHandle_t Lora_Task_Handle;


int main(void)
{
	//ϵͳ��ʼ��
	SysTick_Init(72);//ϵͳʱ��
	Hal_Init();	//Ӳ����ʼ��
    

	
	//������ʼ����
	xTaskCreate((TaskFunction_t )Led_Task,            			//������
				(const char*    )"Led_Task",          			//��������
				(uint16_t       )GET_TASK_STACK_SIZE(50),       //�����ջ��С
				(void*          )NULL,                			//���ݸ��������Ĳ���
				(UBaseType_t    )TASK_PRIORITY_LED,   			//�������ȼ�
				(TaskHandle_t*  )&LedTask_Handle);    			//������   
				 

	xTaskCreate(Key_Task, "Key_Task", GET_TASK_STACK_SIZE(200), NULL, TASK_PRIORITY_KEY, &KeyTask_Handle);   
	xTaskCreate(TftLcd_Task, "TftLcd_Task", GET_TASK_STACK_SIZE(300), NULL, TASK_PRIORITY_TFTLCD, &TftLcd_Task_Handle);   
	xTaskCreate(TemHum_Task, "TemHum_Task", GET_TASK_STACK_SIZE(200), NULL, TASK_PRIORITY_TEMHUM, &TemHum_Task_Handle);
    xTaskCreate(Lora_Task, "Lora_Task", GET_TASK_STACK_SIZE(200), NULL, TASK_PRIORITY_LORA, &Lora_Task_Handle);                  
	xTaskCreate(Test_Task, "Test_Task", GET_TASK_STACK_SIZE(1024), NULL, TASK_PRIORITY_TEST, &Test_Task_Handle);             
    xTaskCreate(Wifi_Task, "Wifi_Task", GET_TASK_STACK_SIZE(1024), NULL, TASK_PRIORITY_WIFI, &Wifi_Task_Handle);

	vTaskStartScheduler();   
}



