#include "task_report3_2.h"
#include "app_lab_3_2.h"
#include "srv_sns_temp/srv_sns_temp.h"
#include "task_conditioning3_2.h"
#include <Arduino_FreeRTOS.h>

static void printFloat(float value, const char *suffix)
{
    int whole = (int)value;
    int decimal = abs((int)((value - whole) * 10));
    printf("%d.%d%s", whole, decimal, suffix);
}

void taskReport3_2(void *pvParameters)
{
    (void)pvParameters; // Unused parameter

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(TASK_REPORT_MS); // 500ms period

    if (TASK_REPORT_OFFSET_MS > 0)
    {
        vTaskDelay(pdMS_TO_TICKS(TASK_REPORT_OFFSET_MS));
    }

    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        // Read sensor values and alert states with mutex protection

        int ntcRaw = srvSnsTempGetNtcRaw();
        int ntcVoltage = srvSnsTempGetNtcVoltage();
        float ntcFiltered = srvSnsTempGetNtcFiltered();
        float dhtTemp = srvSnsTempGetDhtTemperature();
        float dhtHumidity = srvSnsTempGetDhtHumidity();
        AlertState alert1 = srvSnsTempGetAlert1();
        AlertState alert2 = srvSnsTempGetAlert2();

        float ntcSat = gNtcSat;
        float ntcMed = gNtcMedian;
        float ntcWeighted = gNtcFiltered;

        printf("\x1B");
        printf("P:");
        printFloat(ntcFiltered, "C");
        printf(" %s\n", alert1.alertActive ? "!ALERT" : "OK    ");
        printf("D:");
        printFloat(dhtTemp, "C");
        printf(" %s\n", alert2.alertActive ? "!ALERT" : "OK    ");

        printf("\n---NTC--- RAW:%4d  %4dmV  Alert:%s (%d/5)\n",
               ntcRaw, ntcVoltage,
               alert1.alertActive ? "ON " : "OFF", alert1.debounceCounter);
        printf("  SAT:");
        printFloat(ntcSat, " ");
        printf("  MED:");
        printFloat(ntcMed, " ");
        printf("  WEIGHTED:");
        printFloat(ntcWeighted, " ");
        printf("\n\n---DHT--- Hum:");
        printFloat(dhtHumidity, "%");
        printf("  Alert:%s (%d/5)\n",
               alert2.alertActive ? "ON " : "OFF", alert2.debounceCounter);
        printf("===========================================\n");
    }
}