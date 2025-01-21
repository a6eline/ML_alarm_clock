// this is the file for all the c++ functions needed in the arduino

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <Arduino.h>

#define LED_PIN 3
#define BUTTON_PIN 2

//------------------------SERIAL-------------------------------
void serial_setup(void);

//------------------------LED-------------------------------
void led_setup(void);
void led_status(int status); 

//------------------------BUTTON-------------------------------
void button_setup(void);
bool button_status(void);
void on_button();

//------------------------MISC-------------------------------
void count_second(int n);

#endif
