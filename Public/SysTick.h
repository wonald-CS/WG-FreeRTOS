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

//ʱ��(ms)ת��tick��
#define MS_TO_TICK(ms)		(ms/portTICK_PERIOD_MS)

//�¼�����Bits
#define EVENT_ALL_BITS 		0x00ffffffUL
#define EVENT_BITS_NUM 		24


//�߳����ȼ�����
typedef enum
{
    //�������ȼ�
    TASK_PRIORITY_LED,
    TASK_PRIORITY_KEY,
	
}TASK_PRIORITY_DEFINE;



void SysTick_Init(u8 SYSCLK);
void delay_ms(u32 nms);
void delay_us(u32 nus);
void delay_xms(u32 nms);
u8 tick_check(u32 tick,u32 timeout);


#endif
