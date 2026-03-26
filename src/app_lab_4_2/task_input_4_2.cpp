#include "task_input_4_2.h"
#include "app_lab_4_2.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "srv_stdio_keypad/srv_stdio_keypad.h"
#include "dd_button/dd_button.h"
#include <Keypad.h>
#include <semphr.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "srv_serial_stdio/Serial.h"

static TaskInput s_cmd = {false, 0}; // Default command: relay OFF, servo at 0 degrees
static SemaphoreHandle_t s_mutex = NULL;

static void strToUpper(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char) toupper((unsigned char)str[i]);
    }
}

void taskInputInit42() {
    s_mutex = xSemaphoreCreateMutex();
}

TaskInput taskInputGetLatest42() {
    TaskInput copy = {false, 0}; // Default value in case of error
    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
        copy = s_cmd; // Copy the latest command
        xSemaphoreGive(s_mutex);
    }
    return copy;
}

void taskInput42(void *pvParameters) {
    (void) pvParameters;
    char command[16];
    char argument[16];

    printf("Lab 4.2 ready. Commands: RELAY ON | RELAY OFF | SERVO <0-180>\n");
    for (;;) {

        scanf("%15s", command); // Read command keyword
        strToUpper(command);
        if (strcmp(command, "RELAY") == 0) {
            if (scanf("%15s", argument) == 1) {
                strToUpper(argument);
                if (strcmp(argument, "ON") == 0) {
                    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                        s_cmd.relayRequested = true;
                        s_cmd.lastError[0] = '\0'; // Clear last error
                        xSemaphoreGive(s_mutex);
                    }
                } else if (strcmp(argument, "OFF") == 0) {
                    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                        s_cmd.relayRequested = false;
                        s_cmd.lastError[0] = '\0'; // Clear last error
                        xSemaphoreGive(s_mutex);
                    }
                } else {
                    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                        snprintf(s_cmd.lastError, sizeof(s_cmd.lastError), "Invalid relay argument '%s'", argument);
                        xSemaphoreGive(s_mutex);
                    }
                }
            }
        } else if (strcmp(command, "SERVO") == 0) {
            scanf("%15s", argument);
            char *endptr = NULL;
            long angle = strtol(argument, &endptr, 10); // try converting to long

            // check if the entire string was a valid number
            if (*endptr == '\0') {
                if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                    s_cmd.servoAngle = (int)angle;
                    s_cmd.lastError[0] = '\0'; // clear any previous error
                    xSemaphoreGive(s_mutex);
                }
            } else {
                if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                    snprintf(s_cmd.lastError, sizeof(s_cmd.lastError),
                            "Invalid servo angle '%s'", argument);
                    xSemaphoreGive(s_mutex);
                }
            }
        } else {
            xSemaphoreTake(s_mutex, portMAX_DELAY);
            snprintf(s_cmd.lastError, sizeof(s_cmd.lastError),
                    "Unknown command '%s'", command);
            xSemaphoreGive(s_mutex);
        }
        
    }
}