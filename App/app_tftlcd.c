#include "app_tftlcd.h"
#include "mt_tftlcd.h"
#include "lcdfont.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "event_groups.h"
#include "queue.h"
#include "app_wifi.h"


void showSystemTime(void)
{
	unsigned char displaytimebuf[16];
	
	LCD_ShowPicture32PixFont(COOR_ICON_AC_X,COOR_ICON_AC_Y,ICON_32X32_ACLINK,HUE_LCD_FONT,HUE_LCD_BACK,0);
	LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,ICON_32X32_BAT_LEVEL5,HUE_LCD_FONT,HUE_LCD_BACK,0);
	LCD_ShowPicture32PixFont(COOR_ICON_SIM_X,COOR_ICON_SIM_Y,ICON_32X32_GSM_NOCARD,HUE_LCD_FONT,HUE_LCD_BACK,0);
	LCD_ShowPicture32PixFont(COOR_ICON_SERVER_X,COOR_ICON_SERVER_Y,ICON_32X32_SERVER,HUE_LCD_FONT,HUE_LCD_BACK,0);

	
	LCD_ShowString(COOR_ICON_SYSTEMODE_X,COOR_ICON_SYSTEMODE_Y," DISARM ",HUE_LCD_FONT,HUE_LCD_BACK,48,0); 
	

	displaytimebuf[0]=0x32;
	displaytimebuf[1]=0x30;	
	displaytimebuf[2]='2';
	displaytimebuf[3]='4';;
	displaytimebuf[4]='-';	
	displaytimebuf[5]=0x31;
	displaytimebuf[6]=0x30;
	displaytimebuf[7]='-';	
	displaytimebuf[8]='0';
	displaytimebuf[9]='3';	
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



/*******************************************************************************************
*@description:WIFI图标显示
*@param[in]：
*@return：
*@others：
********************************************************************************************/
void Wifi_ICON_Display()
{
	static uint8_t wifi_icon = 0;
	static uint8_t wifi_signal_status = 0;
	static uint8_t wifi_signal_record = 0;

	if(wifi_icon != WIFI_NetSta.WIFI_Signal){
		wifi_icon = WIFI_NetSta.WIFI_Signal;

		if (wifi_icon <= 0x50) {
			wifi_signal_record = 4;
			//LCD_ShowPicture32PixFont(COOR_ICON_WIFI_X,COOR_ICON_WIFI_Y,ICON_32X32_WIFI_S4,HUE_LCD_FONT,HUE_LCD_BACK,0);
		} else if (wifi_icon > 0x50 && wifi_icon <= 0x70) {			
			wifi_signal_record = 3;
			//LCD_ShowPicture32PixFont(COOR_ICON_WIFI_X,COOR_ICON_WIFI_Y,ICON_32X32_WIFI_S3,HUE_LCD_FONT,HUE_LCD_BACK,0);
		} else if (wifi_icon > 0x70 && wifi_icon <= 0x80) {		
			wifi_signal_record = 2;
			//LCD_ShowPicture32PixFont(COOR_ICON_WIFI_X,COOR_ICON_WIFI_Y,ICON_32X32_WIFI_S2,HUE_LCD_FONT,HUE_LCD_BACK,0);
		} else if (wifi_icon > 0x80 && wifi_icon <= 0x99) {	
			wifi_signal_record = 1;
			//LCD_ShowPicture32PixFont(COOR_ICON_WIFI_X,COOR_ICON_WIFI_Y,ICON_32X32_WIFI_S1,HUE_LCD_FONT,HUE_LCD_BACK,0);
		} else {
			wifi_signal_record = 0;
			
		}
	
	}

	if(wifi_signal_status != wifi_signal_record){
		wifi_signal_status = wifi_signal_record;
		if(wifi_signal_status == 4){
			printf("WIFI signal best!\n");
			LCD_ShowPicture32PixFont(COOR_ICON_WIFI_X,COOR_ICON_WIFI_Y,ICON_32X32_WIFI_S4,HUE_LCD_FONT,HUE_LCD_BACK,0);
		}else if(wifi_signal_status == 3){
			printf("WIFI signal good!\n");
			LCD_ShowPicture32PixFont(COOR_ICON_WIFI_X,COOR_ICON_WIFI_Y,ICON_32X32_WIFI_S3,HUE_LCD_FONT,HUE_LCD_BACK,0);
		}else if(wifi_signal_status == 2){
			printf("WIFI signal normal!\n");
			LCD_ShowPicture32PixFont(COOR_ICON_WIFI_X,COOR_ICON_WIFI_Y,ICON_32X32_WIFI_S2,HUE_LCD_FONT,HUE_LCD_BACK,0);
		}else if(wifi_signal_status == 1){
			printf("WIFI signal worst!\n");
			LCD_ShowPicture32PixFont(COOR_ICON_WIFI_X,COOR_ICON_WIFI_Y,ICON_32X32_WIFI_S1,HUE_LCD_FONT,HUE_LCD_BACK,0);
		}else{
			printf("WIFI signal no!\n");
		}
	}

	
	
}

void TftLcd_Task(void *pvParameters)
{
	Tftlcd_Clear();
	showSystemTime();
	for(;;)
	{  
		Wifi_ICON_Display();
		
		vTaskDelay(100);
	}
}
