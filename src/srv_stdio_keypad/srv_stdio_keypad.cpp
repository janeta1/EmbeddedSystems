#include "srv_stdio_keypad.h"

#include <Keypad.h>
#include <stdio.h>
#include <Arduino.h>

#define SRV_KEYPAD_REPEAT_DELAY 100

// Configuring keypad parameters
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void srvStdioKeypadSetup() {
    // Initializing peripherals for the keypad service here
    // It doesn't need, initialization is done in the definition of customKeypad
    FILE *myStream = fdevopen(NULL, srvStdioKeypadGetKey);
    stdin = myStream;
}
  
int srvStdioKeypadGetKey(FILE *stream) {
    char customKey;
    do {
        customKey = customKeypad.getKey();
    } while (customKey == NO_KEY);

    delay(SRV_KEYPAD_REPEAT_DELAY); // read frequency 1/SRV_KEYPAD_REPEAT_DELAY ms
    return (int) customKey;
}