#ifndef _APP_4G_H
#define _APP_4G_H

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"

extern QueueHandle_t EC200N_Queue;

void EC200N_Task(void *pvParameters);

#endif

