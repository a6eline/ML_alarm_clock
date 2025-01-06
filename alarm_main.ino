// main ino file for the setup and loop functions 
// here the main actions of the actual alarm system will be initiated 
// whereas the specific functions using module libraries will be in other header file

#include "RTC_and_LCD.h"
#include "buzzer.h"
#include "functions.h"

// SQW PIN can not interfere with LCD pins or the screen will print unusual things...
#define CLOCK_INTERRUPT_PIN 8
#define BUTTON_PIN 10

void setup() {
  checkRTC();

  pinMode(BUTTON_PIN, INPUT); // setting button to be an input pin 
  pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP); // Setting sqw pinmode to high  
  attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), play_song, FALLING); // The interupt is triggered when the pin falls from high to low
  // attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), play_song, FALLING); --> this will become interupt for buzzer soon

  Serial.begin(9600); // Setup for serial monitor 
  Serial.println("Alarm Clock with DS3231 module and RTClib library..."); // Introduction

  timer_second(3);
  count_second(3);
  // play_song();
}

void loop() {
  display_time();

  if(alarm_status() == 1) { // while statement doesnt work
    play_song();

    if(digitalRead(BUTTON_PIN == HIGH)) { 
      clear();
    }
  }
}

