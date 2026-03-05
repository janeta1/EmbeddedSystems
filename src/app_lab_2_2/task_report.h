#ifndef TASK_REPORT_H
#define TASK_REPORT_H

#include <Arduino_FreeRTOS.h>

#define REPORT_OFFSET_MS 100   
#define REPORT_REC 10000       // 10 second recurrence period

void taskReport(void *pvParameters);

#endif // TASK_REPORT_H
