#ifndef RTOS_H
#define RTOS_H

// #include "system.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN 2

TaskHandle_t BlinkTaskHandle = NULL;

void BlinkTask_Init(void);
void BlinkTask(void* parameter);

#endif