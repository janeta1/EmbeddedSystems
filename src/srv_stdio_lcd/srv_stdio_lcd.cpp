#include "srv_stdio_lcd.h"
#include <Arduino.h>
#include <stdio.h>
#include <LiquidCrystal_I2C.h>

// The character used to clear the LCD when printed. 
#define CLEAR_KEY 0x1B

// Define LCD adress and dimensions
int lcdColumns = 16;
int lcdRows = 2;

// Create an LCD objects with address of 0x27 (common for 16x2 LCDs) 
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

int srvStdioLCDPutChar(char c, FILE *stream) {
    // Print character to LCD
    if (c == CLEAR_KEY){
        lcd.clear();
        lcd.setCursor(0, 0);
    } else {
        lcd.print(c);
    }
    return 0;
}

void srvStdioLCDSetup() {
    // Initialize LCD
    lcd.init();

    // Turn on the backlight
    lcd.backlight();

    // Clear the LCD and set the cursor to first position
    lcd.clear();
    lcd.setCursor(0, 0);

    // Create a stream for the LCD and set it as stdout
    FILE *srv_stdio_lcd_stream = fdevopen(srvStdioLCDPutChar, NULL);
    stdout = srv_stdio_lcd_stream;
}
 