#include "task_input_5_1.h"
#include "app_lab_5_1.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "srv_serial_stdio/Serial.h"

static TaskInput51 s_cmd = {DEFAULT_SETPOINT, HYSTERESIS_DELTA, ""};
static SemaphoreHandle_t s_mutex = NULL;

static void strTrim(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len-1] == '\r' || str[len-1] == '\n' || str[len-1] == ' ')) {
        str[--len] = '\0';
    }
}

static void strToUpper(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char) toupper((unsigned char)str[i]);
    }
}

void taskInputInit51() {
    s_mutex = xSemaphoreCreateMutex();
}

TaskInput51 taskInputGetLatest51() {
    TaskInput51 copy = {DEFAULT_SETPOINT, HYSTERESIS_DELTA, ""};
    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
        copy = s_cmd;
        xSemaphoreGive(s_mutex);
    }
    return copy;
}

void taskInput51(void *pvParameters) {
    (void) pvParameters;

    char command[16];
    char argument[16];

    printf("Lab 5.1 ready.\n");
    printf("Available commands:\n");
    printf("  SP <value>    - Set the desired temperature setpoint (°C)\n");
    printf("  HYS <value>   - Set the hysteresis delta (°C)\n");
    printf("  HELP          - Show this help message\n");

    for (;;) {
        scanf("%15s", command);
        strToUpper(command);

        if (strcmp(command, "SP") == 0) {
            if (scanf("%15s", argument) == 1) {
                strTrim(argument);
                float value = (float) atof(argument);

                if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                    s_cmd.setPoint     = value;
                    s_cmd.lastError[0] = '\0';
                    xSemaphoreGive(s_mutex);
                }
            }
        } else if (strcmp(command, "HYS") == 0) {
            if (scanf("%15s", argument) == 1) {
                strTrim(argument);
                float value = (float) atof(argument);

                if (value < 0.1f) value = 0.1f; // enforce minimum hysteresis

                if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                    s_cmd.hysteresis   = value;
                    s_cmd.lastError[0] = '\0';
                    xSemaphoreGive(s_mutex);
                }
             }
        } else if (strcmp(command, "HELP") == 0) {
            printf("Available commands:\n");
            printf("  SP <value>    - Set the desired temperature setpoint (°C)\n");
            printf("  HYS <value>   - Set the hysteresis delta (°C)\n");
            printf("  HELP          - Show this help message\n");
        } else {
            if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                snprintf(s_cmd.lastError, sizeof(s_cmd.lastError), "Unknown command '%s'", command);
                xSemaphoreGive(s_mutex);
            }
        }
    }
}