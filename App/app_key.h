#ifndef _APP_KEY_H
#define _APP_KEY_H

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"


extern QueueHandle_t KeyQueue;
extern EventGroupHandle_t KeyEvent;

void Key_Task(void *pvParameters);

#endif
