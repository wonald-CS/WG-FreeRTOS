#include <stdio.h>
#include "app_wifi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "stm32F10x.h"
#include "string.h"
#include "event_groups.h"
#include "queue.h"
#include "hal_usart.h"
#include "hal_GPIO.h"
#include "Algorithm.h"

QueueHandle_t Wifi_Queue;


En_Wifi_Status Wifi_Sta = WIFI_AT_AT;
En_WIFI_NetSta WIFI_NetSta;
/*
const uint8_t ESP8266_AT[WIFI_AT_MAX][] = 
{
	"AT+RST\r\n",						//复位指令
	"AT\r\n",							///WIFI 测试指令
	"ATE1\r\n",							//打开回显
	"AT+CWMODE=1\r\n",					//设置WIFI模式为Station模式	1：Station模式 2：AP模式 3：Station+AP模式
	"AT+CWAUTOCONN=1\r\n",				//是否自动连接AP:0:上电不自动链接AP    1:上电自动链接AP
	"AT+CWSTATE?\r\n",					//获取WIFI 的链接状态
	"AT+CWSTOPSMART\r\n",				//停止SmartConfig	
	"AT+CWSTARTSMART=2\r\n",			//启动某种类型的SmartConfig模式  1：ESP=TOUCH  2:AirKiss  3:AirKiss+Esptouch

};
*/

const uint8_t *ESP8266_AT_Send[WIFI_AT_MAX] = 
{
	"AT+RST\r\n",						//复位指令
	"AT\r\n",							///WIFI 测试指令
	"ATE0\r\n",							//打开回显
	"AT+CWMODE=1\r\n",					//设置WIFI模式为Station模式	1：Station模式 2：AP模式 3：Station+AP模式
	"AT+CWAUTOCONN=1\r\n",				//是否自动连接AP:0:上电不自动链接AP    1:上电自动链接AP
	"AT+CWSTATE?\r\n",					//获取WIFI 的链接状态
	"AT+CWSTARTSMART=2\r\n",			//启动某种类型的SmartConfig模式  1：ESP=TOUCH  2:AirKiss  3:AirKiss+Esptouch
	"AT+CWSTOPSMART\r\n",				//停止SmartConfig	
	"AT+CWLAP=",					    //获取WIFI的信号强弱
    "AT+CWJAP?\r\n"
};



const uint8_t *ESP12_AT_ResPonse[ESP12_AT_RESPONSE_MAX]=
{
	"+CWSTATE:",    							//获取WIFI的链接状态	
												//0: ESP32 station 尚未进行任何 Wi-Fi 连接
												//1: ESP32 station 已经连接上 AP，但尚未获取到 IPv4 地址
												//2: ESP32 station 已经连接上 AP，并已经获取到 IPv4 地址
												//3: ESP32 station 正在进行 Wi-Fi 连接或 Wi-Fi 重连
												//4: ESP32 station 处于 Wi-Fi 断开状态
	"+CWJAP:",								    //获取WIFI的信号值
	"Smart get wifi info",					    //配置WIFI的名称和密码
	"AT+MQTTCONNECTED:0",
	"AT+MQTTDISCONNECTED:0",
	"AT+MQTTSUBRECV:0,",
	"OK\r\n\0",           						//WIFI模块返回值OK
};







/*******************************************************************************************
*@description:WIFI模块外电或电源接入
*@param[in]：fuc：复位或者空闲
*@return：无
*@others： 1. 如果没电断电的情况i下， 关闭WIFI电源   2. 电源复位操作
********************************************************************************************/
unsigned char wifi_PowerManage()
{
	static unsigned short StartTime = 0;
	if(hal_Gpio_AcStateCheck() == STA_AC_LINK)
	{///USB 外部供电的情况下 
        if(StartTime == 0)
		{
			//mt_wifi_init();
			hal_GPIO_WIFIPowerEN_H();
		}
		if(StartTime < 1000)
		{
			StartTime ++;
			if(StartTime > 30)
			{
				StartTime = 2000;
				hal_GPIO_WIFIPowerEN_L();
			}
			return 0;
		}	
		return 1;
	}else{
		return 0;
	}
	
}



/*******************************************************************************************
*@description:申请内存
*@param[in]：fuc：复位或者空闲
*@return：无
*@others： 
********************************************************************************************/
uint8_t Apply_Buff(char **ptr)
{
    *ptr = (char *)pvPortMalloc(100*sizeof(uint8_t));
    if(*ptr!=NULL){
        return TRUE;
        //strcpy(*ptr,(const char *)ESP8266_AT_Send[WIFI_AT_CWLAP]);
        //strcat((char *)ptr,(const char *)WIFI_NetSta.WIFI_Name);  
    }else{
        return FALSE;
    }
    
}



/*******************************************************************************************
*@description:解析通过串口接收ESP8266的应答数据
*@param[in]：*pData：WIFI_RxBuff[0],  *res:接收的数据的枚举值,  num:接收数据长度
*@return：0：匹配成功； 0xff：匹配失败；
*@others：
********************************************************************************************/
uint8_t WIFI_RxMsg_Analysis(unsigned char *pData,unsigned char *res,unsigned short num)
{
	unsigned char i,j;
	for (j = 0; j < ESP12_AT_RESPONSE_MAX; j++)
	{
		i = SeekSrting(pData,(unsigned char*)ESP12_AT_ResPonse[j],num);
		if (i!=0xFF)
		{
			*res = j;
			return TRUE;
		}
		
	}
	return FALSE;
}



/*******************************************************************************************
*@description:保存连接WIFI的名称、连接状态和信号强度
*@param[in]：*ptr：WIFI_Rec_Data[0],  len:接收数据长度
*@return：TRUE：保存完成； FALSE：保存失败；
*@others：
********************************************************************************************/
uint8_t Wifi_Name_Power_Save(uint8_t *ptr,uint8_t sta)
{
	uint8_t Index = 0;
    uint8_t signal_Sec;
    uint8_t signalChar;
    
	switch (sta)
	{
	case ESP12_AT_RESPONSE_CWSTATE:
		while (*ptr != ':')
		{
			ptr ++;
		}

		ptr ++;

		// 提取WiFi状态
		if (*ptr >= '0' && *ptr <= '9') {
			WIFI_NetSta.WIFI_Sta = *ptr - '0';
		} else {
			return FALSE; // 状态不是数字
		}

		while (*ptr != '"')     //第一个"
		{
			ptr ++;
		}
		WIFI_NetSta.WIFI_Name[Index++] = *ptr;
		ptr ++;

		while (*ptr != '"')     //结束的"
		{
			WIFI_NetSta.WIFI_Name[Index++] = *ptr;
			ptr ++;
		}
		WIFI_NetSta.WIFI_Name[Index++] = *ptr;
		break;

	case ESP12_AT_RESPONSE_CWJAP:
		while (*ptr != '-')
		{
			ptr ++;
		}
		ptr ++;
        
        //0x35 -> 0x50
        signalChar = *ptr;
		signalChar <<= 4;
		ptr ++;
        
        //0x33 -> 0x03
        signal_Sec = *ptr;
        signal_Sec &= 0x0F;
        
        //0x50| 0x03 = 0x53
		signalChar |= signal_Sec;
		WIFI_NetSta.WIFI_Signal = signalChar;
        //printf("WIFI_Signal:%d\n",WIFI_NetSta.WIFI_Signal);
	break;
		
	default:
		break;
	}



	
    return TRUE;
	
}



/*******************************************************************************************
*@description:WIFI_AT指令发送
*@param[in]：*ptr:发送的AT数据，len：AT数据长度
*@return：无
*@others：
********************************************************************************************/
void Wifi_Send_AT(const uint8_t *ptr,uint8_t len)
{
	uint8_t i;
    printf("data:%s",&(ptr[0]));    	//%s是以\0为结束标志的字符串，所以可以把整个AT指令都打印出来
	for(i = 0;i < len;i++){
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
        USART_SendData(USART3,ptr[i]);   //ptr[0] = *ptr
	}

}



/*******************************************************************************************
*@description:WIFI接收函数
*@param[in]：
*@return：无
*@others：
********************************************************************************************/
void Wifi_Rx_Pro()
{
	uint8_t Ret,len,Res,ret;
	uint8_t Wifi_Rec_Data[300];

	Ret = xQueueReceive(Wifi_Queue, &Wifi_Rec_Data, 10);
	if (Ret)
	{
		len = strlen((char *)Wifi_Rec_Data);
		ret = WIFI_RxMsg_Analysis(Wifi_Rec_Data,&Res,len);
		if(ret)
		{
			switch (Res)
			{
				case ESP12_AT_RESPONSE_OK:
					switch (Wifi_Sta)
					{
						case WIFI_AT_AT:
							Wifi_Sta = WIFI_AT_E1;
						break;

                        
						case WIFI_AT_E1:
							Wifi_Sta = WIFI_AT_CWMODE;
						break;


						case WIFI_AT_CWMODE:
							Wifi_Sta = WIFI_AT_CWSTATE;
						break;

                        
//                        case WIFI_AT_CWSTOPSMART:
//							Wifi_Sta = WIFI_AT_CWLAP; 
//						break;

//                        
//						case WIFI_AT_CWSTARTSMART:
//							Wifi_Sta = WIFI_AT_CWSTOPSMART;  
//						break;



					}
					
				break;


				case ESP12_AT_RESPONSE_CWSTATE:				
					Wifi_Name_Power_Save(&Wifi_Rec_Data[0],ESP12_AT_RESPONSE_CWSTATE);
                    Wifi_Sta = WIFI_AT_CWJAP;  
				break;

                
				case ESP12_AT_RESPONSE_CWJAP:				
					Wifi_Name_Power_Save(&Wifi_Rec_Data[0],ESP12_AT_RESPONSE_CWJAP);
				break;
	
                    
			}
		}
		
        printf("%s\n",&(Wifi_Rec_Data[0]));
	}
}



/*******************************************************************************************
*@description:WIFI发送函数
*@param[in]：
*@return：无
*@others：
********************************************************************************************/
void Wifi_Tx_Pro()
{
	//可以申请动态内存void *ptr = pvPortMalloc(sizeof(uint8_t));
	//释放内存vPortFree(ptr);
	//不过用静态内存Buff也可以，因为单片机的内存够大
	
	static uint32_t Time_DelayTick = 0;
	uint8_t Ret;
    char *ptr = NULL;  
    uint8_t len = 0;


	switch (Wifi_Sta)
	{
	case WIFI_AT_AT:
		Ret = tick_check(Time_DelayTick,MS_TO_TICK(3000));
		if(Ret){
			Time_DelayTick = xTaskGetTickCount();
			if(ESP8266_AT_Send[WIFI_AT_AT] != NULL)			//直接把AT指令用指针数组保存
			{
				len = strlen((char *)ESP8266_AT_Send[WIFI_AT_AT]);
				Wifi_Send_AT(ESP8266_AT_Send[WIFI_AT_AT],len);	
			}
			//此处在接收到ATOK再作初始化处理，如果AT指令发送失败，则重新发送	
		}
		

		break;

	case WIFI_AT_E1:
		if(ESP8266_AT_Send[WIFI_AT_E1] != NULL)			//直接把AT指令用指针数组保存
		{
			len = strlen((char *)ESP8266_AT_Send[WIFI_AT_E1]);
			Wifi_Send_AT(ESP8266_AT_Send[WIFI_AT_E1],len);		
		}
	break;

	case WIFI_AT_CWMODE:
		if(ESP8266_AT_Send[WIFI_AT_CWMODE] != NULL)			//直接把AT指令用指针数组保存
		{
			len = strlen((char *)ESP8266_AT_Send[WIFI_AT_CWMODE]);
			Wifi_Send_AT(ESP8266_AT_Send[WIFI_AT_CWMODE],len);		
		}
	break;

	case WIFI_AT_CWAUTOCONN:
		if(ESP8266_AT_Send[WIFI_AT_CWAUTOCONN] != NULL)			//直接把AT指令用指针数组保存
		{
			len = strlen((char *)ESP8266_AT_Send[WIFI_AT_CWAUTOCONN]);
			Wifi_Send_AT(ESP8266_AT_Send[WIFI_AT_CWAUTOCONN],len);		
		}
	break;

	case WIFI_AT_CWSTATE:
		if(ESP8266_AT_Send[WIFI_AT_CWSTATE] != NULL)			//直接把AT指令用指针数组保存
		{
			len = strlen((char *)ESP8266_AT_Send[WIFI_AT_CWSTATE]);
			Wifi_Send_AT(ESP8266_AT_Send[WIFI_AT_CWSTATE],len);	
            Time_DelayTick = xTaskGetTickCount();            
		}
	break;

/*
	case WIFI_AT_CWSTOPSMART:
		if(ESP8266_AT_Send[WIFI_AT_CWSTOPSMART] != NULL)			//直接把AT指令用指针数组保存
		{
			len = strlen((char *)ESP8266_AT_Send[WIFI_AT_CWSTOPSMART]);
			Wifi_Send_AT(ESP8266_AT_Send[WIFI_AT_CWSTOPSMART],len);		
		}
	break;	


	case WIFI_AT_CWSTARTSMART:
		if(ESP8266_AT_Send[WIFI_AT_CWSTARTSMART] != NULL)			//直接把AT指令用指针数组保存
		{
			len = strlen((char *)ESP8266_AT_Send[WIFI_AT_CWSTARTSMART]);
			Wifi_Send_AT(ESP8266_AT_Send[WIFI_AT_CWSTARTSMART],len);		
		}
	break;
   


	case WIFI_AT_CWLAP:  
        if(Apply_Buff(&ptr)){
            strcpy(ptr,(const char *)ESP8266_AT_Send[WIFI_AT_CWLAP]);
            strcat((char *)ptr,(const char *)WIFI_NetSta.WIFI_Name); 
            len = strlen(ptr);
            Wifi_Send_AT((const uint8_t*)ptr,len);        
        }
        
        vPortFree(ptr);   
        Wifi_Sta = WIFI_AT_AT;        
 
	break;	
*/     

    case WIFI_AT_CWJAP:
		Ret = tick_check(Time_DelayTick,MS_TO_TICK(10000));
		if(Ret){
			Time_DelayTick = xTaskGetTickCount();
			if(ESP8266_AT_Send[WIFI_AT_CWJAP] != NULL)			//直接把AT指令用指针数组保存
			{
				len = strlen((char *)ESP8266_AT_Send[WIFI_AT_CWJAP]);
				Wifi_Send_AT(ESP8266_AT_Send[WIFI_AT_CWJAP],len);     
			}
		}

	break;	
        
        
	default:
		break;
	}

	

}



void Wifi_Task(void *pvParameters)
{

    
	Wifi_Queue = xQueueCreate(1, 300*sizeof(uint8_t));	
    
	while(1)
	{
		if(wifi_PowerManage())
		{
            Wifi_Rx_Pro();
			Wifi_Tx_Pro();
		}

		vTaskDelay(10);		
	}
}
