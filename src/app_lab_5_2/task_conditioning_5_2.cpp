#include "task_conditioning_5_2.h"
#include "task_acquisition_5_2.h"
#include "task_input_5_2.h"
#include "app_lab_5_2.h"
#include "dd_relay/dd_relay.h"
#include "dd_l298/dd_l298.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// dt in seconds — matches TASK_COND52_MS
#define PID_DT (TASK_COND52_MS / 1000.0f)

// Global snapshot, read by task_report
TaskConditioningState52 s_condState52 = {
    DEFAULT_SETPOINT,       // setPoint
    DEFAULT_KP,           // kp
    DEFAULT_KI,           // ki
    DEFAULT_KD,           // kd
    0.0f,                   // temperature
    0.0f,                   // humidity
    0.0f,                   // error
    0.0f,                   // pidOutput
    RELAY_OFF,              // relayRequested
    RELAY_OFF,              // relayApplied
    false,                  // relayPending
    0,                       // relayDebounce
    0,                      // fanSpeed
    false                   // fanRunning
};
SemaphoreHandle_t s_condMutex52 = NULL;

void taskConditioningInit52() {
    s_condMutex52 = xSemaphoreCreateMutex();
}

void taskConditioning52(void *pvParameters) {
    (void) pvParameters; // Unused parameter

    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        // 1. Get latest sensor reading
        TaskAcquisitionState52 acq = taskAcquisitionGetLatest52();
        TaskInput52 input = taskInputGetLatest52();

        float dt = TASK_COND52_MS / 1000.0f; // Convert ms to seconds for PID calculations
        static float integral = 0.0f; // Integral term accumulator
        static float prevError = 0.0f; // Previous error for derivative calculation

        float sp   = input.setPoint; // SetPoint set by user via serial
        float kp   = input.kp;      // Proportional gain set by user via serial
        float ki   = input.ki;      // Integral gain set by user via serial
        float kd   = input.kd;      // Derivative gain set by user via serial
        float temp = acq.temperature;
        float humidity = acq.humidity;

        // 2. PID control logic
        static float prevSetpoint = DEFAULT_SETPOINT; 
        // Reset integral if setpoint changed significantly
        float spDelta = sp - prevSetpoint;
        if (spDelta < 0.0f) spDelta = -spDelta;
        if (spDelta > 0.05f) {
            integral   = 0.0f;
            prevError  = 0.0f;
        }
        prevSetpoint = sp;

        float error = sp - temp;
        integral += error * dt;
        // Anti-windup: clamp integral term to prevent excessive overshoot
        if (integral > PID_INTEGRAL_MAX) integral = PID_INTEGRAL_MAX;
        if (integral < PID_INTEGRAL_MIN) integral = PID_INTEGRAL_MIN;

        float derivative = (error - prevError) / dt;
        prevError = error;

        float output = kp * error + ki * integral + kd * derivative;
        float rawOutput = output; 

        // 3. Map PID output to relay and fan control
        if (output > 0.0f) {
            ddRelaySetRequested(RELAY_ON);
            ddL298Stop();
        } else {
            ddRelaySetRequested(RELAY_OFF);
            int fanSpeed = (int)((-rawOutput / PID_INTEGRAL_MAX) * 255.0f);
            if (fanSpeed > 100) fanSpeed = 100;
            ddL298SetDirection(MOTOR_FORWARD);
            ddL298SetSpeed(fanSpeed); 
        }

        ddRelayStep();

        // 4. Update shared snapshot for task_report
        if (xSemaphoreTake(s_condMutex52, portMAX_DELAY) == pdTRUE) {
            s_condState52.setPoint       = sp;
            s_condState52.kp     = kp;
            s_condState52.ki     = ki;
            s_condState52.kd     = kd;
            s_condState52.temperature    = temp;
            s_condState52.humidity       = humidity;
            s_condState52.error          = error;
            s_condState52.pidOutput      = output;
            s_condState52.relayRequested = ddRelayGetRequested();
            s_condState52.relayApplied   = ddRelayGetApplied();
            s_condState52.relayPending   = ddRelayGetPending() != ddRelayGetApplied();
            s_condState52.relayDebounce  = ddRelayGetDebounceCounter();
            s_condState52.fanSpeed       = (int)(-rawOutput);
            s_condState52.fanRunning     = ddL298IsRunning();
            xSemaphoreGive(s_condMutex52);
        }

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK_COND52_MS));
    }
}