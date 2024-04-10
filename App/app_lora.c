#include <stdio.h>
#include "app_lora.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SysTick.h"
#include "stm32F10x.h"
#include "string.h"
#include "event_groups.h"
#include "queue.h"
#include "Algorithm.h"
#include "hal_usart.h"


QueueHandle_t LoraQueue;

void Lora_Task(void *pvParameters)
{
	Data_AppNet_Receive AppNet_Rx_data;
    Data_Else_Receive Else_Rx_data;
    unsigned char Buff[50];
    unsigned char i;
    unsigned char CheckSum = 0;
	BaseType_t Ret;

    LoraQueue = xQueueCreate(2, sizeof(struct Data_AppNet_Re));
    
	while(1)
	{
        Ret = xQueueReceive(LoraQueue, &AppNet_Rx_data, 10);
        if (Ret == pdTRUE)
        {
            //校验接收数据
            switch (AppNet_Rx_data.Len)
            {
            case Else_Move_Len:
                Else_Rx_data.Head = AppNet_Rx_data.Head;
                Else_Rx_data.Len = AppNet_Rx_data.Len;
                Else_Rx_data.CheckSum = AppNet_Rx_data.AppNet_ValidData.MAC[0];
                memcpy(&Else_Rx_data.Else_ValidData,&AppNet_Rx_data.AppNet_ValidData,3); 
                CheckSum = Else_Rx_data.Else_ValidData.CMD + Else_Rx_data.Else_ValidData.CRC_Data[0] + Else_Rx_data.Else_ValidData.CRC_Data[1];
                if(CheckSum == Else_Rx_data.CheckSum){
                    //上行包校验成功，组下行包
                    Else_Rx_data.Else_ValidData.CMD += 0x80;
                    Else_Rx_data.CheckSum += 0x80;
                    memcpy(Buff,&Else_Rx_data,sizeof(Data_Else_Receive));
                    for(i = 0;i < sizeof(Data_Else_Receive);i++){
                        USART_SendData(UART5,Buff[i]);
                        while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);                    
                    }
                }                    
                break;

            case App_Net_Len:
                for(i = 0;i < 12;i++){
                    CheckSum += AppNet_Rx_data.AppNet_ValidData.MAC[i];
                }
                for(i = 0;i < 2;i++){
                    CheckSum += AppNet_Rx_data.AppNet_ValidData.CRC_Data[i];
                }
                CheckSum += AppNet_Rx_data.AppNet_ValidData.CMD;
                CheckSum += AppNet_Rx_data.AppNet_ValidData.Type;

                if(CheckSum == AppNet_Rx_data.CheckSum)
                {
                    //上行包校验成功，组下行包
                    AppNet_Rx_data.AppNet_ValidData.CMD += 0x80;
                    AppNet_Rx_data.CheckSum += 0x80;
                    memcpy(Buff,&AppNet_Rx_data,sizeof(Data_AppNet_Receive));
                    for(i = 0;i < sizeof(Data_AppNet_Receive);i++){
                        USART_SendData(UART5,Buff[i]);
                        while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);                    
                    }
                }
                
                break;
            
            default:
                break;
            }
            
            CheckSum = 0;
            
	                  
        }


		vTaskDelay(100);		
	}
}
