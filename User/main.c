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


//任务句柄
TaskHandle_t LedTask_Handle;
TaskHandle_t KeyTask_Handle;



int main(void)
{
	//系统初始化
	SysTick_Init(72);//系统时钟
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	Hal_Init();	//硬件初始化


	//队列初始化
	KeyQueue = xQueueCreate(10, sizeof(uint16_t));
	//事件初始化
	KeyEvent = xEventGroupCreate();

	
	//创建开始任务
	xTaskCreate((TaskFunction_t )Led_Task,            //任务函数
				(const char*    )"Led_Task",          //任务名称
				(uint16_t       )50,        		  //任务堆栈大小
				(void*          )NULL,                //传递给任务函数的参数
				(UBaseType_t    )TASK_PRIORITY_LED,   //任务优先级
				(TaskHandle_t*  )&LedTask_Handle);    //任务句柄    

	xTaskCreate(Key_Task, "Key_Task", 200, NULL, TASK_PRIORITY_KEY, &KeyTask_Handle);   


	vTaskStartScheduler();   
}



