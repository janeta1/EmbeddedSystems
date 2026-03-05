#ifndef TASK_STATISTICS_H
#define TASK_STATISTICS_H

#include <Arduino_FreeRTOS.h>

#define STATS_OFFSET_MS 20     
#define STATS_REC 100          // 100ms recurrence period

void taskStatistics(void *pvParameters);

#endif // TASK_STATISTICS_H
