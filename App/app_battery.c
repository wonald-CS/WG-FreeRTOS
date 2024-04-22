#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "stm32F10x.h"
#include "string.h"
#include "event_groups.h"
#include "queue.h"
#include "lcdfont.h"
#include "hal_usart.h"
#include "hal_GPIO.h"
#include "mt_tftlcd.h"
#include "app_battery.h"
#include "app_tftlcd.h"


str_batVolt batVolt;

/*******************************************************************************************
*@description:获取ADC值
*@param[in]：
*@return：
*@others：
********************************************************************************************/
static unsigned short hal_adc_GetAdcDat(unsigned char ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}


/*******************************************************************************************
*@description:电量ADC检测流程
*@param[in]：
*@return：
*@others：
********************************************************************************************/
///3.2  3.10 3.12 3.13 3.15 3.16 3.10 3.12 3.13 3.15 3.16  / 10
static en_batlevel adc_batVoltCheck(void)
{
	unsigned int batVoltage;
	switch((unsigned char)batVolt.step)
	{
		case STEP_IDLE://空闲
		{
			batVolt.adcTimes = 0;
			batVolt.BatSumVolt = 0;
			batVolt.VrefSumVolt = 0;
			return LEVEL_IDLE;
		}
//		break;
		case STEP_STARTGET_VOLT://开始ADC 检测
		{
			batVolt.BatSumVolt += hal_adc_GetAdcDat(ADC_Channel_8);				//电池电压的ADC值
			batVolt.VrefSumVolt += hal_adc_GetAdcDat(ADC_Channel_10);			//2.5V基准电压的ADC值
			
			batVolt.adcTimes ++;
			if(batVolt.adcTimes >= HAL_ADC_GETTIMES)
			{
				batVolt.adcTimes = 0;
				batVolt.step = STEP_GETVOLT_FINISH;
			}				
		}
		break;
		case STEP_GETVOLT_FINISH:// 电池电压检测完成
		{
			batVolt.Adc_Vref = batVolt.VrefSumVolt/HAL_ADC_GETTIMES;
			batVolt.Adc_Bat = batVolt.BatSumVolt/HAL_ADC_GETTIMES;
			batVolt.step = STEP_DATEHANLE;
		}
		break;	
		case STEP_DATEHANLE://底层数据分析
		{
			 batVolt.step = STEP_IDLE;
			///// 讲获取的ADC 的值转换成实际的电压值  电压值扩大100倍 *2是因为电压被分压
			 batVoltage = 2*((250*batVolt.Adc_Bat)/batVolt.Adc_Vref); 
			 if(batVoltage > BATTERY_LEV4_VOLT_VAL)
			 {
				return LEVEL_FULL;
			 }
			 else if(batVoltage > BATTERY_LEV3_VOLT_VAL)
			 {
				return LEVEL_VOLT_4;
			 }
			 else if(batVoltage > BATTERY_LEV4_VOLT_VAL)
			 {
				return LEVEL_VOLT_3;
			 }				 
			 else if(batVoltage > BATTERY_LEV1_VOLT_VAL)
			 {
				return LEVEL_VOLT_2;
			 }
			 else if(batVoltage > BATTERY_LEV0_VOLT_VAL)
			 {
				return LEVEL_VOLT_1;
			 }
			 else
			 {
				return LEVEL_LOW;
			 } 
		}
	}
	return LEVEL_NODATA;
}

/*******************************************************************************************
*@description:ADC值数据处理
*@param[in]：
*@return：
*@others：
********************************************************************************************/
void ADC_DataHandle(void)
{
    
	static unsigned short interalTim = 0;
	static en_batlevel level = LEVEL_FULL;
	static unsigned char times = 0;
	static en_batlevel batlevelSta;
    
    
	batlevelSta = adc_batVoltCheck();
	if(batlevelSta == LEVEL_IDLE)
	{
		interalTim ++; ///电池电量检测间隔时间
		if(interalTim > HAL_ADC_GET_INTERVAL) ///50*10 =500
		{
			interalTim = 0;
			batVolt.step = STEP_STARTGET_VOLT;
		}		
	}
	else if(batlevelSta == LEVEL_NODATA)
	{	
	}
	else
	{
		if(batlevelSta != level)
		{	
			times ++;   ///之前检测的电池电量 和新获取的电池电量不一样的次数
			if(times > 3)
			{
				times = 0;
				level = batlevelSta;
				batVolt.batRank = level;
			}
		}			
		else
		{
			times = 0;
		}	 
	}	
}

/*******************************************************************************************
*@description:电量显示
*@param[in]：
*@return：
*@others：
********************************************************************************************/
void Battery_Show(void)
{
	static unsigned short InteralTim;
	if(hal_Gpio_AcStateCheck() == STA_AC_LINK)
	{///AC Link：显示电池充电中
		LCD_ShowPicture32PixFont(COOR_ICON_AC_X,COOR_ICON_AC_Y,ICON_32X32_ACLINK,HUE_LCD_FONT,HUE_LCD_BACK,0);
		if(batVolt.batRank == LEVEL_FULL)
		{
			InteralTim = 0;
			LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,(ICON_32X32_BAT_LEVEL0+batVolt.batRank),HUE_LCD_FONT,HUE_LCD_BACK,0);	
		}
		else 
		{
			InteralTim ++;
			if(InteralTim == 50)
			{
				LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,ICON_32X32_BAT_LEVEL0,HUE_LCD_FONT,HUE_LCD_BACK,0);	
			}
			else if(InteralTim == 100)
			{
				LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,ICON_32X32_BAT_LEVEL1,HUE_LCD_FONT,HUE_LCD_BACK,0);	
			}
			else if(InteralTim == 150)
			{
				LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,ICON_32X32_BAT_LEVEL2,HUE_LCD_FONT,HUE_LCD_BACK,0);	
			}				
			else if(InteralTim == 200)
			{
				LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,ICON_32X32_BAT_LEVEL3,HUE_LCD_FONT,HUE_LCD_BACK,0);	
			}
			else if(InteralTim == 250)
			{
				LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,ICON_32X32_BAT_LEVEL4,HUE_LCD_FONT,HUE_LCD_BACK,0);	
			}	
			else if(InteralTim == 300)
			{
				InteralTim = 0;
				LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,ICON_32X32_BAT_LEVEL5,HUE_LCD_FONT,HUE_LCD_BACK,0);	
			}	
		}
	}
	else
	{///AC break; add wifi state
		LCD_ShowPicture32PixFont(COOR_ICON_AC_X,COOR_ICON_AC_Y,ICON_32X32_ACBREAK,HUE_LCD_FONT,HUE_LCD_BACK,0);
		////电池图标显示部分
		if(batVolt.batRank  <= LEVEL_VOLT_1)
		{
			InteralTim ++;
			if(InteralTim == 50) //500ms
			{
				LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,(ICON_32X32_BAT_LEVEL0+batVolt.batRank),HUE_LCD_FONT,HUE_LCD_BACK,0);		
			}
			else if(InteralTim > 100)
			{
				InteralTim = 0;
				LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,ICON_32X32_CLEAR,HUE_LCD_FONT,HUE_LCD_BACK,0);		
			}
		}
		else
		{
			InteralTim = 0;
			LCD_ShowPicture32PixFont(COOR_ICON_BAT_X,COOR_ICON_BAT_Y,(ICON_32X32_BAT_LEVEL0+batVolt.batRank),HUE_LCD_FONT,HUE_LCD_BACK,0);			
		}
	}
}


void batVolt_Init(void)
{
	batVolt.step = STEP_IDLE;
	batVolt.adcTimes = 0;
	batVolt.BatSumVolt = 0;
	batVolt.VrefSumVolt = 0;
	batVolt.batRank = LEVEL_FULL;
	batVolt.chargeSta = STA_BAT_CHARGING;	
	batVolt.chargeCheckDelay = 0;   
}


void Battery_Task(void *pvParameters)
{
 	batVolt_Init();
	while(1)
	{	
		ADC_DataHandle();
		Battery_Show();


		vTaskDelay(10);		
	}
}
