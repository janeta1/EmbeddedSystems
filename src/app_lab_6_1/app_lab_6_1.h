#ifndef APP_LAB_6_1_H
#define APP_LAB_6_1_H

#include <Arduino_FreeRTOS.h>
#include "srv_fsm/srv_fsm.h"

// ── Pin definitions ───────────────────────────────────────────────
#define BUTTON_PIN_61   2
#define LED_PIN_61      13

// ── Task periods ──────────────────────────────────────────────────
#define TASK_REPORT_61_MS  500

// ── FSM state indices ─────────────────────────────────────────────
#define FSM_LED_OFF  0
#define FSM_LED_ON   1

// ── FSM state table (Moore, Button-LED) ───────────────────────────
// { output, delay_ms, { next[in=0], next[in=1] } }
// Button not pressed (in=0) → stay, pressed (in=1) → toggle
static SrvFsmSTyp buttonLedFsmTable[] = {
    { 0, 100, { FSM_LED_OFF, FSM_LED_ON  } },   // state 0: LED OFF
    { 1, 100, { FSM_LED_ON,  FSM_LED_OFF } },   // state 1: LED ON
};

#define FSM_TABLE_SIZE  (sizeof(buttonLedFsmTable) / sizeof(buttonLedFsmTable[0]))

// ── Shared FSM instance (defined in app_lab_6_1.cpp) ─────────────
extern SrvFsm g_fsm61;

// ── App entry points ──────────────────────────────────────────────
void appLab61Setup();
void appLab61Loop();

#endif // APP_LAB_6_1_H