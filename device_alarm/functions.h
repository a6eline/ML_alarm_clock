
//=========================================================== functions.h ====================================================================

#pragma once 

#include <Arduino.h>

enum class Signal { Low, High };

//------------------------SERIAL----------------------------

void serial_setup();  // serial monitor begin setup

//------------------------BUZZER-------------------------------

void buzzer_setup();  // buzzer setup pins
void silence();       // silences buzzer
void beep();          // beeps the buzzer
void pulsed_beep();   // pulsed buzzer

//------------------------LED-------------------------------

void led_setup();     // sets up LEDs
void led_status(Signal status); // toggles LED on or off

//------------------------BUTTON----------------------------

void button_setup();  // button setup
bool button_pressed(); // checks if button has been pressed
void on_button();         // button pressed serial print message

//------------------------MISC------------------------------

void count_second(int n); // counts seconds to debug timers accuracy
