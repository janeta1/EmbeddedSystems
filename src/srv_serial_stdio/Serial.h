#ifndef SearialIO_H
#define SearialIO_H

#include <stddef.h>

void serialInit(long baudRate);
int serialPutChar(char c, FILE *stream);
int serialGetChar(FILE *stream);

#endif