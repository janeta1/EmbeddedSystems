#ifndef DD_BUTTON_H
#define DD_BUTTON_H

void ddButtonInit(int pin);
int ddButtonRead(int pin);
bool ddButtonIsPressed(int pin);

#endif // DD_BUTTON_H