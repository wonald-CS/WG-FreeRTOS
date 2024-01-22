#include "app_tftlcd.h"
#include "app_al6630.h"
#include "hal_al6630.h"
#include "mt_tftlcd.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "event_groups.h"
#include "queue.h"

static unsigned short HumidityValue = 0;
static unsigned short TemperatureValue = 0;

void TemperatureValue_Show(void)
{
    unsigned char displayBuf[6],idx = 0;

    if((TemperatureValue > 99) && (TemperatureValue < 1000)) 
    {
        displayBuf[idx ++] = TemperatureValue/100 +'0';  //1 +0x30 =0x31		
    }
    displayBuf[idx ++] = TemperatureValue%100/10 +'0';
    displayBuf[idx ++] = '.';
    displayBuf[idx ++] = TemperatureValue%10 +'0';
    displayBuf[idx ++] = '!';  
    displayBuf[idx ++] = 0;	
    LCD_ShowString(70,3,displayBuf,HUE_LCD_FONT,HUE_LCD_BACK,24,0);
}


void HumidityValue_Show(void)
{
    unsigned char displayBuf[6],idx = 0;

    if((HumidityValue > 99) && (HumidityValue < 1000)) 
    {
        displayBuf[idx ++] = HumidityValue/100 +'0';
    }
    displayBuf[idx ++] = HumidityValue%100/10 +'0';
    displayBuf[idx ++] = '.';
    displayBuf[idx ++] = HumidityValue%10 +'0';
    displayBuf[idx ++] = '%';
    displayBuf[idx ++] = 0;		
    LCD_ShowString(135,3,displayBuf,HUE_LCD_FONT,HUE_LCD_BACK,24,0);	
}


void App_TempHum_show()
{
	unsigned char updateFlag = 0;

	if(TemperatureValue != Get_Temperature_Data())
	{
		TemperatureValue = Get_Temperature_Data();
		updateFlag = 1;
	}

	if(HumidityValue != Get_Humidity_Data())
	{
		HumidityValue = Get_Humidity_Data();
		updateFlag = 1;
	}	
	
	if(updateFlag == 1)
	{
        TemperatureValue_Show();
        HumidityValue_Show();		
	}    
}


void TemHum_Task(void *pvParameters)
{	
	for(;;)
	{    
		hal_GetTemHum_Proc();
		App_TempHum_show();
		vTaskDelay(100);
	}
}

