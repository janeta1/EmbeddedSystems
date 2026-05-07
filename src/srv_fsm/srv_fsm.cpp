#include "srv_fsm.h"

void srvFsmInit(SrvFsm *fsm, SrvFsmSTyp *table, int numStates, int initialState) {
    fsm->table     = table;
    fsm->numStates = numStates;
    fsm->current   = initialState;
    fsm->mutex     = xSemaphoreCreateMutex();
}

int srvFsmStep(SrvFsm *fsm, int input) {
    int next = fsm->current;
    if (xSemaphoreTake(fsm->mutex, portMAX_DELAY) == pdTRUE) {
        next = fsm->table[fsm->current].next[input];
        fsm->current = next;
        xSemaphoreGive(fsm->mutex);
    }
    return next;
}

int srvFsmGetState(SrvFsm *fsm) {
    int state = 0;
    if (xSemaphoreTake(fsm->mutex, portMAX_DELAY) == pdTRUE) {
        state = fsm->current;
        xSemaphoreGive(fsm->mutex);
    }
    return state;
}

unsigned long srvFsmGetOutput(SrvFsm *fsm) {
    unsigned long out = 0;
    if (xSemaphoreTake(fsm->mutex, portMAX_DELAY) == pdTRUE) {
        out = fsm->table[fsm->current].out;
        xSemaphoreGive(fsm->mutex);
    }
    return out;
}

unsigned long srvFsmGetDelay(SrvFsm *fsm) {
    unsigned long delay = 0;
    if (xSemaphoreTake(fsm->mutex, portMAX_DELAY) == pdTRUE) {
        delay = fsm->table[fsm->current].timeMs;
        xSemaphoreGive(fsm->mutex);
    }
    return delay;
}