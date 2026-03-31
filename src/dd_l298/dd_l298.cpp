#include "dd_l298.h"
#include <Arduino.h>

static int sEnaPin = 0;
static int sIn1Pin = 0;
static int sIn2Pin = 0;

static int sRequestedSpeed = 0;
static int sAppliedSpeed = 0;
static MotorDirection sDirection = MOTOR_STOP;
static MotorDirection sLastDirection = MOTOR_STOP;
static bool sRunning = false;

void ddL298Init(int enaPin, int in1Pin, int in2Pin) {
    sEnaPin = enaPin;
    sIn1Pin = in1Pin;
    sIn2Pin = in2Pin;

    pinMode(sEnaPin, OUTPUT);
    pinMode(sIn1Pin, OUTPUT);
    pinMode(sIn2Pin, OUTPUT);

    // Start with motor stopped
    digitalWrite(sEnaPin, 0);
    digitalWrite(sIn1Pin, LOW);
    digitalWrite(sIn2Pin, LOW);
}

static void applyDirection() {
    switch (sDirection) {
        case MOTOR_STOP:
        default:
            digitalWrite(sIn1Pin, LOW);
            digitalWrite(sIn2Pin, LOW);
            break;
        case MOTOR_FORWARD:
            digitalWrite(sIn1Pin, HIGH);
            digitalWrite(sIn2Pin, LOW);
            break;
        case MOTOR_BACKWARD:
            digitalWrite(sIn1Pin, LOW);
            digitalWrite(sIn2Pin, HIGH);
            break;
    }
}

void ddL298SetSpeed(int speed) {
    if (speed < L298_SPEED_MIN) speed = L298_SPEED_MIN;
    if (speed > L298_SPEED_MAX) speed = L298_SPEED_MAX;
    sRequestedSpeed = speed;
    if (sRunning) {
        sAppliedSpeed = sRequestedSpeed;
        analogWrite(sEnaPin, sAppliedSpeed);
    }
}

void ddL298SetDirection(MotorDirection dir) {
    if (dir == MOTOR_STOP) {
        ddL298Stop();
        return;
    }
    sDirection = dir;
    sLastDirection = dir;
    sRunning = true;
    sAppliedSpeed = sRequestedSpeed;
    applyDirection();
    analogWrite(sEnaPin, sAppliedSpeed);
}

void ddL298Stop() {
    sDirection = MOTOR_STOP;
    sRunning = false;
    sAppliedSpeed = 0;
    applyDirection();
    analogWrite(sEnaPin, 0);
}

void ddL298Start() {
    sDirection = sLastDirection;
    sRunning = true;
    sAppliedSpeed = sRequestedSpeed;
    applyDirection();
    analogWrite(sEnaPin, sAppliedSpeed);
}

int ddL298GetSpeed() {
    return sRequestedSpeed;
}

int ddL298GetAppliedSpeed() {
    return sAppliedSpeed;
}

MotorDirection ddL298GetDirection() {
    return sDirection;
}

bool ddL298IsRunning() {
    return sRunning;
}