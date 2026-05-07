#ifndef SRV_FSM_H
#define SRV_FSM_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Generic Moore FSM state structure
// Mirrors Table 7.1 layout: output, delay, next states per input
struct SrvFsmState {
    unsigned long out;        // output value for this state
    unsigned long timeMs;     // how long to stay in this state (ms)
    unsigned long next[2];    // next[0] = input=0, next[1] = input=1
};

typedef const struct SrvFsmState SrvFsmSTyp;

// FSM handle — one per FSM instance
typedef struct {
    SrvFsmSTyp      *table;       // pointer to state table
    int              numStates;   // number of states in table
    int              current;     // current state index
    SemaphoreHandle_t mutex;      // protects current state
} SrvFsm;

// Initialize FSM with a state table and initial state
void srvFsmInit(SrvFsm *fsm, SrvFsmSTyp *table, int numStates, int initialState);

// Step: transition to next state based on input, returns new state index
int srvFsmStep(SrvFsm *fsm, int input);

// Get current state index (mutex-protected)
int srvFsmGetState(SrvFsm *fsm);

// Get output of current state (mutex-protected)
unsigned long srvFsmGetOutput(SrvFsm *fsm);

// Get delay of current state in ms (mutex-protected)
unsigned long srvFsmGetDelay(SrvFsm *fsm);

#endif // SRV_FSM_H