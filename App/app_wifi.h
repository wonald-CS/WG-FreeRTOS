#ifndef _APP_WIFI_H
#define _APP_WIFI_H

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"


#define ESP12_AT_LEN  50 

extern QueueHandle_t Wifi_Queue;

typedef enum 
{
    WIFI_AT_RST,
    WIFI_AT_AT,
    WIFI_AT_E1,
    WIFI_AT_CWMODE,
    WIFI_AT_CWAUTOCONN,
    WIFI_AT_CWSTATE,
    WIFI_AT_CWSTARTSMART,
    WIFI_AT_CWSTOPSMART,
    WIFI_AT_CWLAP,
    WIFI_AT_CWJAP,
    
    WIFI_AT_MAX,
}En_Wifi_Status;



typedef enum
{
	ESP12_AT_RESPONSE_CWSTATE,	
	ESP12_AT_RESPONSE_CWJAP,
	ESP12_AT_RESPONSE_SMART_GET_WIFIWINFO,	
	
	ESP12_AT_RESPONSE_MQTTCONN,			//"+MQTTCONNECTED:0\0",
	ESP12_AT_RESPONSE_MQTTDISCONN,		//"+MQTTDISCONNECTED:0\0",
	ESP12_AT_RESPONSE_MQTTRECV,			//"+MQTTSUBRECV:0,\0",
	ESP12_AT_RESPONSE_OK,
	ESP12_AT_RESPONSE_MAX,
}En_esp12_atResponse;


typedef struct 
{
	unsigned char WIFI_Sta;
	unsigned char WIFI_Name[ESP12_AT_LEN];			//连接WIFI的名称
    unsigned char WIFI_Signal;			            //连接WIFI的信号强度
}En_WIFI_NetSta;


void Wifi_Task(void *pvParameters);
uint8_t WIFI_RxMsg_Analysis(unsigned char *pData,unsigned char *res,unsigned short num);
extern En_WIFI_NetSta WIFI_NetSta;
#endif
 
