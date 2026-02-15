#ifndef SRV_STDIO_LCD_H
#define SRV_STDIO_LCD_H

#include <stdio.h>

void srvStdioLCDSetup();
int srvStdioLCDPutChar(char c, FILE *stream);

#endif