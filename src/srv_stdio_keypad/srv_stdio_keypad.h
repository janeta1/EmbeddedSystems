#ifndef SRV_STDIO_KEYPAD_H
#define SRV_STDIO_KEYPAD_H

#include <stdio.h>

void srvStdioKeypadSetup();
int srvStdioKeypadGetKey(FILE *stream);
char srvStdioKeypadPoll();

#endif