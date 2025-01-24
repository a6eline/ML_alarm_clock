// this is the h file which will have the prototypes of the functions
// this header file will also include <Arduino.h> so it will not have to be included in the cpp file either

#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>

#define BUZZER_PIN 3

void buzzer_setup(void);
void silence(void);
void beep(void);

#endif
