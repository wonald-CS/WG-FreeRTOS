#include "app_tftlcd.h"
#include "mt_tftlcd.h"
#include "lcdfont.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "event_groups.h"
#include "queue.h"


void showSystemTime(void)
{
	unsigned char displaytimebuf[16];
	
	LCD_ShowPicture32PixFont(COOR_ICON_AC_X,COOR_ICON_AC_Y,ICON_32X32_ACLINK,HUE_LCD_FONT,HUE_LCD_BACK,0);
	LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,ICON_32X32_BAT_LEVEL5,HUE_LCD_FONT,HUE_LCD_BACK,0);
	LCD_ShowPicture32PixFont(COOR_ICON_WIFI_X,COOR_ICON_WIFI_Y,ICON_32X32_WIFI_S4,HUE_LCD_FONT,HUE_LCD_BACK,0);
	LCD_ShowPicture32PixFont(COOR_ICON_SIM_X,COOR_ICON_SIM_Y,ICON_32X32_GSM_NOCARD,HUE_LCD_FONT,HUE_LCD_BACK,0);
	LCD_ShowPicture32PixFont(COOR_ICON_SERVER_X,COOR_ICON_SERVER_Y,ICON_32X32_SERVER,HUE_LCD_FONT,HUE_LCD_BACK,0);
	LCD_ShowString(COOR_ICON_SYSTEMODE_X,COOR_ICON_SYSTEMODE_Y," DISARM ",HUE_LCD_FONT,HUE_LCD_BACK,48,0); 
	

	displaytimebuf[0]=0x32;
	displaytimebuf[1]=0x30;	
	displaytimebuf[2]='2';
	displaytimebuf[3]='4';;
	displaytimebuf[4]='-';	
	displaytimebuf[5]='0';
	displaytimebuf[6]='1';
	displaytimebuf[7]='-';	
	displaytimebuf[8]='2';
	displaytimebuf[9]='2';	
	displaytimebuf[10]=0;
	LCD_ShowString(20,200,displaytimebuf,HUE_LCD_FONT,HUE_LCD_BACK,24,0);
	
	
	displaytimebuf[0]='1';
	displaytimebuf[1]='2';
	displaytimebuf[2]=':';	
	displaytimebuf[3]='0';
	displaytimebuf[4]='0';		
	displaytimebuf[5]=0;	
	LCD_ShowString(160,200,displaytimebuf,HUE_LCD_FONT,HUE_LCD_BACK,24,0);	
  	LCD_ShowString(260,200,"Mon",HUE_LCD_FONT,HUE_LCD_BACK,24,0);	
}


void Tftlcd_Clear(void)
{
	LCD_Fill(0,0,LCD_W,LCD_H,HUE_LCD_BACK);
}



void TftLcd_Task(void *pvParameters)
{	
	Tftlcd_Clear();
	showSystemTime();
	for(;;)
	{    	
		vTaskDelay(100);
	}
}
