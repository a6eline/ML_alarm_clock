
//----------------------------------------------------------------functions.h------------------------------------------------------------------------------------------------

#pragma once

#include <Arduino.h>

constexpr auto LED_PIN = 1; // for when you have an LED
constexpr auto BUTTON_PIN = 2;  
constexpr auto BUZZER_PIN = 3;

enum class Signal { Low, High };

//------------------------SERIAL----------------------------

void serial_setup();  // serial monitor begin setup

//------------------------LED-------------------------------

void buzzer_setup();  // buzzer setup pins
void silence();       // silences buzzer
void beep();          // beeps the buzzer

//------------------------LED-------------------------------

void led_setup();     // sets up LEDs
void led_status(Signal status); // toggles LED on or off

//------------------------BUTTON----------------------------

void button_setup();  // button setup
bool button_pressed(); // checks if button has been pressed
void on_button();         // button pressed serial print message

//------------------------MISC------------------------------

void count_second(int n); // counts seconds to debug timers accuracy
