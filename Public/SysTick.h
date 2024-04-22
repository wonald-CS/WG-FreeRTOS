#ifndef _SysTick_H
#define _SysTick_H

#include "system.h"
#include "SysTick.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"

#define TRUE	(1)
#define FALSE	(0)

//��С����
#define ALIGN(size,align)	(((size) + (align) - 1) & ~((align) - 1))
//����ʵ����Ҫ�����ջ�ռ�
#define GET_TASK_STACK_SIZE(size)  (ALIGN(size,sizeof(StackType_t) / sizeof(StackType_t)))
//ʱ��(ms)ת��tick��
#define MS_TO_TICK(ms)		(ms/portTICK_PERIOD_MS)

//�¼�����Bits
#define EVENT_ALL_BITS 		0x00ffffffUL
#define EVENT_BITS_NUM 		24


//�߳����ȼ�����
typedef enum
{
    //�������ȼ�
    TASK_PRIORITY_TEST = 1,
    TASK_PRIORITY_LED,
    TASK_PRIORITY_TEMHUM,
    TASK_PRIORITY_KEY,
    TASK_PRIORITY_TFTLCD,
    TASK_PRIORITY_LORA,
    TASK_PRIORITY_4G,
    TASK_PRIORITY_WIFI,
    
	
	TASK_PRIORITY_HIGHEST,
    
	
}TASK_PRIORITY_DEFINE;



void SysTick_Init(u8 SYSCLK);
void delay_ms(u32 nms);
void delay_us(u32 nus);
void delay_xms(u32 nms);
u8 tick_check(u32 tick,u32 timeout);


#endif
