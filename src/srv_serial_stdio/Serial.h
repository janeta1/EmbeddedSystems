#ifndef SearialIO_H
#define SearialIO_H

#include <stddef.h>

void serial_init(long baudRate);
int serial_put_char(char c, FILE *stream);
int serial_get_char(FILE *stream);
// void serial_read_command(char *buffer, size_t length);
// void serial_print(const char *message);

#endif