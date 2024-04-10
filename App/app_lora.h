#ifndef _APP_LORA_H
#define _APP_LORA_H

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"


#define App_Net_Len 16
#define Else_Move_Len 3


struct Lora_AppNet_ValidData{
    unsigned char CMD;                     //功能码
    unsigned char CRC_Data[2];             //CRC校验值
    unsigned char MAC[12];                 //探测器MAC地址
    unsigned char Type;                    //探测器类型
};

struct Lora_Else_ValidData{
    unsigned char CMD;                     //功能码
    unsigned char CRC_Data[2];             //CRC校验值
};


typedef struct Data_AppNet_Re{
    unsigned char Head;                                   //帧头 0xFE
    unsigned char Len;                                    //有效数据长度
    struct Lora_AppNet_ValidData AppNet_ValidData;        //有效数据
    unsigned char CheckSum;                               //有效数据校验
}Data_AppNet_Receive;


typedef struct Data_Else_Re{
    unsigned char Head;
    unsigned char Len;
    struct Lora_Else_ValidData Else_ValidData;              //有效数据
    unsigned char CheckSum;
}Data_Else_Receive;



extern QueueHandle_t LoraQueue;

void Lora_Task(void *pvParameters);

#endif

