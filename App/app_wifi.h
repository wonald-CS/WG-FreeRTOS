#ifndef _APP_WIFI_H
#define _APP_WIFI_H

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"

extern QueueHandle_t Wifi_Queue;

void Wifi_Task(void *pvParameters);

#endif

