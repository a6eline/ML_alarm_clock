// main ino file for the setup and loop functions 
// here the main actions of the actual alarm system will be initiated 
// whereas the specific functions using module libraries will be in other header file

#include "RTC_and_LCD.h"
#include "buzzer.h"
#include "functions.h"


void setup() {
  checkRTC();
  serial_setup(); button_setup(); alarm_setup();
  Serial.println("Alarm Clock with DS3231 module and RTClib library..."); 

  timer_second(3);
  count_second(3);

  //set_alarm(); // timer for every minute
  //set_daily_alarm(12, 15); // everyday alarm at HH:MM 
}

void loop() {
  display_time();

  if (digitalRead(CLOCK_INTERRUPT_PIN) == LOW) { 
    on_alarm();
    beep();
    if (button_status()) {
      delete_alarm(1);       
      on_button();
      silence();
      // below, if you want the alarm to repeat...
      //  ...copy the alarm/timer function from setup below.
      // set_daily_alarm(12, 15);
    }
  }
}

