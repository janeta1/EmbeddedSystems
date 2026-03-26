#ifndef DD_SERVO_H
#define DD_SERVO_H

#define SERVO_ANGLE_MIN 0
#define SERVO_ANGLE_MAX 180

void ddServoInit(int servoPin);
void ddServoSetAngle(int angle);
int ddServoGetAngle();

#endif // DD_SERVO_H