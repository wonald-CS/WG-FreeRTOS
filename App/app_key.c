#include <stdio.h>
#include "app_key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "event_groups.h"
#include "queue.h"
#include "hal_key.h"
#include "hal_wtn6.h"
#include "mt_tftlcd.h"
#include "app_tftlcd.h"


//����
QueueHandle_t KeyQueue;
//�¼�
EventGroupHandle_t KeyEvent;




void Key_Handle(uint8_t keys)
{
//    unsigned char keysta[3];

//    keysta[0] = keys/10 + 0x30;
//    keysta[1] = keys%10 + 0x30;
//    keysta[2] = 0;

//    LCD_ShowString(0,40,"KEY:",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
	hal_Wtn6_Play(WTN6_VOLUE_DI);
	
    switch((unsigned char)keys)
    {
        case KEY0:
        {
            //LCD_ShowString(50,40,"KEY0",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY0\n");
        }
        break;
        case KEY1:
        {
            //LCD_ShowString(50,40,"KEY1",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY1\n");
        }
        break;			
        case KEY2_UP:
        {
            //LCD_ShowString(50,40,"KEY2",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY2_UP\n");
        }
        break;	
        case KEY3:
        {
            //LCD_ShowString(50,40,"KEY3",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY3\n");
        }
        break;	
        case KEY4_LEFT:
        {
           //LCD_ShowString(50,40,"KEY4",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY4_LEFT\n");
        }
        break;			
        case KEY5:
        {
            //LCD_ShowString(50,40,"KEY5",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY5\n");
        }
        break;
        case KEY6_RIGHT:
        {
            //LCD_ShowString(50,40,"KEY6",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY6_RIGHT\n");
        }
        break;
        case KEY7:
        {
            //LCD_ShowString(50,40,"KEY7",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY7\n");
        }
        break;									
        case KEY8_DOWN:
        {
            //LCD_ShowString(50,40,"KEY8",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY8_DOWN\n");
        }
        break;
        case KEY9:
        {
            //LCD_ShowString(50,40,"KEY9",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY9\n");
        }
        break;			
        case KEY_DISARM:
        {
            //LCD_ShowString(50,40,"DISA",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY_DISARM\n");
        }
        break;
        
        case KEY_HOMEARM:
        {
            //LCD_ShowString(50,40,"HOME",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY_HOMEARM\n");
        }
        break;
        
        case KEY_AWARARM:
        {
            //LCD_ShowString(50,40,"AWAY",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY_AWARARM\n");
        }
        break;
        
        case KEY_CANCEIL_DAIL:
        {
            //LCD_ShowString(50,40,"DAIL",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
             printf("����:KEY_CANCEIL_DAIL\n");
        }
        break;

        case KEY_SOS_DEL:
        {
            //LCD_ShowString(50,40,"SOS",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY_SOS_DEL\n");
        }
        break;
        case KEY_MENU:
        {
            //LCD_ShowString(50,40,"MENU",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
            printf("����:KEY_MENU\n");
        }
        break;
    }

//    LCD_ShowString(120,40,"KEYDBC:",HUE_LCD_FONT,HUE_LCD_BACK,24,0);
//    LCD_ShowString(200,40,keysta,HUE_LCD_FONT,HUE_LCD_BACK,24,0);
}



//�������ְ���ֵֻ�ö��м�����ɹ���
//�¼��������Ϥ
void Key_Task(void *pvParameters)
{	
	uint8_t u8Ret,KeyVal;
    
	//���г�ʼ��
	KeyQueue = xQueueCreate(2, sizeof(uint16_t));
	//�¼���ʼ��
	//KeyEvent = xEventGroupCreate();
	//xEventGroupClearBits(KeyEvent,EVENT_ALL_BITS);
	
	while(1)
	{
		hal_Key_Proc();
		u8Ret = xQueueReceive(KeyQueue, &KeyVal, KEY_CHECK_INTERVAL);
		if(u8Ret)
		{
			Key_Handle(KeyVal);		
		}



		// u32EventFlag = xEventGroupWaitBits(KeyEvent,EVENT_ALL_BITS,pdFALSE,pdFALSE,portMAX_DELAY);
		// u8Ret = tick_check(u32KeyTick,KEY_CHECK_INTERVAL);
		// if (u8Ret)
		// {
		// 	for (u8iLoop = 0; u8iLoop < Key_Num; u8iLoop++)
		// 	{
		// 		if(u32EventFlag & (1<<u8iLoop))
		// 		{

		// 		}
		// 		xEventGroupClearBits(KeyEvent,(1<<u8iLoop));
		// 	}
			
		// }
		
		vTaskDelay(10);
	}
}
