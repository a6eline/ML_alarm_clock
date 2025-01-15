// main ino file for the setup and loop functions 
// here the main actions of the actual alarm system will be initiated 
// whereas the specific functions using module libraries will be in other header file

#include "RTC_and_LCD.h"
#include "buzzer.h"
#include "functions.h"

// SQW PIN can not interfere with LCD pins or the screen will print unusual things...
#define CLOCK_INTERRUPT_PIN 8
#define BUTTON_PIN 10

// typedef enum {
//     BTN_ON,
//     BTN_OFF
// } button_state;

// button_state state = BTN_OFF; // button starts OFF 


void setup() {
  Serial.begin(9600); // Setup for serial monitor 
  while (!Serial);

  checkRTC();
  alarm_setup();

  pinMode(BUTTON_PIN, INPUT); // setting button to be an input pin 
  pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP); // Setting sqw pinmode to high  
  attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), on_alarm, FALLING); // The interupt is triggered when the pin falls from high to low
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), silence, FALLING); //--> this will become interupt for buzzer soon

  Serial.println("Alarm Clock with DS3231 module and RTClib library..."); // Introduction

  // timer_second(3);
  // count_second(3);

  //set_alarm(); // timer for every minute
  set_daily_alarm(12, 15);
}

void loop() {
  display_time();

  if(digitalRead(CLOCK_INTERRUPT_PIN) == LOW) { 
    on_alarm();
    beep();
    if(digitalRead(BUTTON_PIN) == HIGH) {
      delete_alarm(1);       
      on_button();
      silence();
      // below, if you want the alarm to repeat...
      //  ...copy the alarm/timer function from setup below.
      set_daily_alarm(12, 15);
    }
  }

  // if(digitalRead(BUTTON_PIN) == HIGH) { 
  //   Serial.println("Button Pressed");
  // }
}

