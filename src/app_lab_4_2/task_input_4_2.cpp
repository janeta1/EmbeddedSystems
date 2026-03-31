#include "task_input_4_2.h"
#include "app_lab_4_2.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "dd_l298/dd_l298.h"
#include <Keypad.h>
#include <semphr.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "srv_serial_stdio/Serial.h"

static TaskInput42 s_cmd = {false, 0, MOTOR_FORWARD, false, false, false, ""}; // Default command: relay OFF, motor at 0 speed, forward direction
static SemaphoreHandle_t s_mutex = NULL;

static void strToUpper(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (char) toupper((unsigned char)str[i]);
    }
}

void taskInputInit42() {
    s_mutex = xSemaphoreCreateMutex();
}

TaskInput42 taskInputGetLatest42() {
    TaskInput42 copy = {false, 0, MOTOR_FORWARD, false, false, false, ""}; // Default value in case of error
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

    printf("Lab 4.2 ready. Commands: RELAY ON | RELAY OFF | MOTOR <0-255> | MOTOR FWD|BWD|STOP|START\n");
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
        } else if (strcmp(command, "MOTOR") == 0) {
            if (scanf("%15s", argument) == 1) {
                strToUpper(argument);

                if (strcmp(argument, "FWD") == 0) {
                    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                        s_cmd.motorDirection    = MOTOR_FORWARD;
                        s_cmd.motorDirectionSet = true;
                        s_cmd.motorStop         = false;
                        s_cmd.motorStart        = false;
                        s_cmd.lastError[0]      = '\0';
                        xSemaphoreGive(s_mutex);
                    }
                } else if (strcmp(argument, "BWD") == 0) {
                    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                        s_cmd.motorDirection    = MOTOR_BACKWARD;
                        s_cmd.motorDirectionSet = true;
                        s_cmd.motorStop         = false;
                        s_cmd.motorStart        = false;
                        s_cmd.lastError[0]      = '\0';
                        xSemaphoreGive(s_mutex);
                    }
                } else if (strcmp(argument, "STOP") == 0) {
                    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                        s_cmd.motorStop    = true;
                        s_cmd.motorStart   = false;
                        s_cmd.lastError[0] = '\0';
                        xSemaphoreGive(s_mutex);
                    }
                } else if (strcmp(argument, "START") == 0) {
                    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                        s_cmd.motorStart   = true;
                        s_cmd.motorStop    = false;
                        s_cmd.lastError[0] = '\0';
                        xSemaphoreGive(s_mutex);
                    }
                } else {
                    char *endptr = NULL;
                    long speed = strtol(argument, &endptr, 10);
                    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
                        if (*endptr == '\0') {
                            s_cmd.motorSpeed   = (int) speed;
                            s_cmd.lastError[0] = '\0';
                        } else {
                            snprintf(s_cmd.lastError, sizeof(s_cmd.lastError), "Invalid motor argument '%s'", argument);
                        }
                        xSemaphoreGive(s_mutex);
                    }
                }
            }
        } else {
            xSemaphoreTake(s_mutex, portMAX_DELAY);
            snprintf(s_cmd.lastError, sizeof(s_cmd.lastError), "Unknown command '%s'", command);
            xSemaphoreGive(s_mutex);
        }
        
    }
}