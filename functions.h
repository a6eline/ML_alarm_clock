// this is the file for all the c++ functions needed in the arduino

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <Arduino.h>

#define BUTTON_PIN 10

//------------------------BUTTON-------------------------------
void button_setup(void);
bool button_status(void);
void on_button();

//------------------------BUTTON-------------------------------
void serial_setup(void);

//------------------------MISC-------------------------------
void count_second(int n);

#endif
