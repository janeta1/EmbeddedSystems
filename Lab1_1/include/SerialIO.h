#ifndef SearialIO_H
#define SearialIO_H

#include <stddef.h>

static int serialPutChar(char c, FILE *stream);
static int serialGetChar(FILE *stream); 
void serialInit(long baudRate);
void serialReadCommand(char *buffer, size_t length);
void serialPrint(const char *message);
#endif