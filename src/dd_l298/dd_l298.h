#ifndef DD_L298_H
#define DD_L298_H

#include <Arduino.h>

#define L298_SPEED_MIN 0
#define L298_SPEED_MAX 255

typedef enum {
    MOTOR_STOP    = 0,
    MOTOR_FORWARD = 1,
    MOTOR_BACKWARD = 2
} MotorDirection;

void ddL298Init(int enaPin, int in1Pin, int in2Pin);
void ddL298SetSpeed(int speed);
void ddL298SetDirection(MotorDirection dir);
void ddL298Stop();
void ddL298Start();

int             ddL298GetSpeed();
int             ddL298GetAppliedSpeed();
MotorDirection  ddL298GetDirection();
bool            ddL298IsRunning();

#endif // DD_L298_H