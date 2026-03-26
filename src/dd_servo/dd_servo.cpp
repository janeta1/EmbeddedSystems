#include "dd_servo.h"
#include <Arduino.h>
#include <Servo.h>

static Servo sServo;
static int sCurrentAngle = 0;

void ddServoInit(int servoPin) {
    sServo.attach(servoPin);
    sServo.write(sCurrentAngle); // Start at 0 degrees
}

void ddServoSetAngle(int angle) {
    if (angle < SERVO_ANGLE_MIN) {
        angle = SERVO_ANGLE_MIN;
    } else if (angle > SERVO_ANGLE_MAX) {
        angle = SERVO_ANGLE_MAX;
    }
    sCurrentAngle = angle;
    sServo.write(sCurrentAngle);
}

int ddServoGetAngle() {
    return sCurrentAngle;
}