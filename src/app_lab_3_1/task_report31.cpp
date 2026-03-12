#include "task_report31.h"
#include "app_lab_3_1.h"
#include "srv_sns_temp/srv_sns_temp.h"
#include <Arduino_FreeRTOS.h>

static void printFloat(float value, const char *suffix)
{
    int whole = (int)value;
    int decimal = abs((int)((value - whole) * 10));
    printf("%d.%d%s", whole, decimal, suffix);
}

void taskReport31(void *pvParameters)
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
        // int potRaw = srvSnsTempGetPotRaw();
        // int potVoltage = srvSnsTempGetPotVoltage();
        // int potTemp = srvSnsTempGetPotCelsius();
        // int dsTemp = srvSnsTempGetDs18b20Celsius();
        int ntcRaw = srvSnsTempGetNtcRaw();
        int ntcVoltage = srvSnsTempGetNtcVoltage();
        float ntcTemp = srvSnsTempGetNtcCelsius();
        float dhtTemp = srvSnsTempGetDhtTemperature();
        float dhtHumidity = srvSnsTempGetDhtHumidity();
        AlertState alert1 = srvSnsTempGetAlert1();
        AlertState alert2 = srvSnsTempGetAlert2();

        printf("\x1B");
        printf("P:");
        printFloat(ntcTemp, "C");
        printf(" %s\n", alert1.alertActive ? "!ALERT" : "OK    ");
        printf("D:");
        printFloat(dhtTemp, "C");
        printf(" %s\n", alert2.alertActive ? "!ALERT" : "OK    ");

        printf("---NTC--- RAW:%4d  %4dmV  Alert:%s (%d/5)\n",
               ntcRaw, ntcVoltage,
               alert1.alertActive ? "ON " : "OFF", alert1.debounceCounter);
        printf("---DHT--- Hum:");
        printFloat(dhtHumidity, "%");
        printf("  Alert:%s (%d/5)\n",
               alert2.alertActive ? "ON " : "OFF", alert2.debounceCounter);
        printf("===========================================\n");
    }
}