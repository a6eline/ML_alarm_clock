
//----------------------------------------------------------------functions.h------------------------------------------------------------------------------------------------

#pragma once

#include <Arduino.h>

#define LED_PIN 1 // for when you have an LED
#define BUTTON_PIN 2  
#define BUZZER_PIN 3

//------------------------SERIAL----------------------------

void serial_setup(void);  // serial monitor begin setup

//------------------------LED-------------------------------

void buzzer_setup(void);  // buzzer setup pins
void silence(void);       // silences buzzer
void beep(void);          // beeps the buzzer

//------------------------LED-------------------------------

void led_setup(void);     // sets up LEDs
void led_status(int status); // toggles LED on or off

//------------------------BUTTON----------------------------

void button_setup(void);  // button setup
bool button_status(void); // checks if button has been pressed
void on_button();         // button pressed serial print message

//------------------------MISC------------------------------

void count_second(int n); // counts seconds to debug timers accuracy
