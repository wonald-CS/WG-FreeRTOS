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
#include "app_4g.h"
#include "app_battery.h"



/*
 * Retargets the C library printf function to the USART.
 */
//int fputc( int ch, FILE *f );


//任务句柄
TaskHandle_t LedTask_Handle;
TaskHandle_t KeyTask_Handle;
TaskHandle_t TftLcd_Task_Handle;
TaskHandle_t TemHum_Task_Handle;
TaskHandle_t Battery_Task_Handle;
TaskHandle_t Test_Task_Handle;
TaskHandle_t Wifi_Task_Handle;
TaskHandle_t Lora_Task_Handle;
TaskHandle_t EC200N_Task_Handle;




void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	printf("任务：%s 发现栈溢出\r\n", pcTaskName);    
}



int main(void)
{
	//系统初始化
	SysTick_Init(72);//系统时钟
	Hal_Init();	//硬件初始化
    

	
	//创建开始任务
	xTaskCreate((TaskFunction_t )Led_Task,            			//任务函数
				(const char*    )"Led_Task",          			//任务名称
				(uint16_t       )GET_TASK_STACK_SIZE(100),       //任务堆栈大小
				(void*          )NULL,                			//传递给任务函数的参数
				(UBaseType_t    )TASK_PRIORITY_LED,   			//任务优先级
				(TaskHandle_t*  )&LedTask_Handle);    			//任务句柄   
				 

	xTaskCreate(Key_Task, "Key_Task", GET_TASK_STACK_SIZE(200), NULL, TASK_PRIORITY_KEY, &KeyTask_Handle);   
	xTaskCreate(TftLcd_Task, "TftLcd_Task", GET_TASK_STACK_SIZE(500), NULL, TASK_PRIORITY_TFTLCD, &TftLcd_Task_Handle);   
	xTaskCreate(TemHum_Task, "TemHum_Task", GET_TASK_STACK_SIZE(200), NULL, TASK_PRIORITY_TEMHUM, &TemHum_Task_Handle);
//	xTaskCreate(Battery_Task, "Battery_Task", GET_TASK_STACK_SIZE(200), NULL, TASK_PRIORITY_BATTERY, &Battery_Task_Handle);
    xTaskCreate(Lora_Task, "Lora_Task", GET_TASK_STACK_SIZE(500), NULL, TASK_PRIORITY_LORA, &Lora_Task_Handle);                  
//	xTaskCreate(EC200N_Task, "EC200N_Task", GET_TASK_STACK_SIZE(1024), NULL, TASK_PRIORITY_4G, &EC200N_Task_Handle);            
    xTaskCreate(Wifi_Task, "Wifi_Task", GET_TASK_STACK_SIZE(1024), NULL, TASK_PRIORITY_WIFI, &Wifi_Task_Handle);
//	xTaskCreate(Test_Task, "Test_Task", GET_TASK_STACK_SIZE(2048), NULL, TASK_PRIORITY_TEST, &Test_Task_Handle); 

	vTaskStartScheduler();   
}



