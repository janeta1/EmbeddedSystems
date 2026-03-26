#ifndef DD_RELAY_H
#define DD_RELAY_H

#include <Arduino.h>

#define RELAY_DEBOUNCE_COUNT 5

typedef enum {
    RELAY_OFF = 0,
    RELAY_ON = 1
} RelayState;

// Function prototypes
void ddRelayInit(int relayPin);
void ddRelaySetRequested(RelayState state);
void ddRelayStep();

// Getters for snapshots
RelayState ddRelayGetRequested();
RelayState ddRelayGetPending();
RelayState ddRelayGetApplied();
int ddRelayGetDebounceCounter();
#endif // DD_RELAY_H