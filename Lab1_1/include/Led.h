#ifndef LED_H
#define LED_H

class Led {
    private:
        int pin; // which arduino pin controls the LED
    
    public:
        Led(int pinNumber);
        void turnOn();
        void turnOff();
};

#endif