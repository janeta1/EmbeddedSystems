#include "dd_relay.h"
#include <Arduino.h>

static int sPint = 0;
static RelayState sRequested = RELAY_OFF; // what user asked for
static RelayState sPending = RELAY_OFF;   // what we are waiting to apply
static RelayState sApplied = RELAY_OFF;   // what is currently applied
static int sDebounceCounter = 0;

void ddRelayInit(int relayPin) {
    sPint = relayPin;
    pinMode(sPint, OUTPUT);
    digitalWrite(sPint, LOW); // Start with relay off
}

void ddRelaySetRequested(RelayState state) {
    sRequested = state;
}

// Call once per conditioning cycle
void ddRelayStep() {
    if (sRequested != sPending) {
        sPending = sRequested;
        sDebounceCounter = 0; // reset debounce counter when state changes    
    } else if (sDebounceCounter < RELAY_DEBOUNCE_COUNT) {
        sDebounceCounter++;
    }

    if (sDebounceCounter >= RELAY_DEBOUNCE_COUNT) {
        sApplied = sPending;
        digitalWrite(sPint, sApplied == RELAY_ON ? HIGH : LOW);
    }
}

RelayState ddRelayGetRequested() {
    return sRequested;
}

RelayState ddRelayGetPending() {
    return sPending;
}

RelayState ddRelayGetApplied() {
    return sApplied;
}

int ddRelayGetDebounceCounter() {
    return sDebounceCounter;
}