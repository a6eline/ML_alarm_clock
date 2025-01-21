// main ino file for the setup and loop functions 
// here the main actions of the actual alarm system will be initiated 
// whereas the specific functions using module libraries will be in other header file

#include "RTC_and_LCD.h"
#include "buzzer.h"
#include "functions.h"

//---------------------------------------------------------------------SETUP----------------------------------------------------------------------------------------------

void setup() {

  delay(5000); // delay is NEEDED to wait for upload. It usually take around 3-5 seconds, depending on the board.
  //  ...if the delay board takes longer to upload/compile, then make the delay longer.
  //  ...you can tell if the Serial message below does not print.

  //------------------------------------------SETUP-FUNCTIONS---------------------------------------

  // checkRTC(); // check that the rtc is working 
  // alarm_setup(); // setup the LCD and RTC pins
  serial_setup(); 
  Serial.println("Alarm Clock with DS3231 module and RTClib library..."); 
  // button_setup(); 
  // buzzer_setup(); 
  led_setup(); // extra modules/component setups

  // //------------------------------------------TIMER-------------------------------------------------

  // // //timer_second(3);
  // timer(0, 0, 3, A1_HOUR);
  // count_second(3); // to test if you have a timer for n seconds, prints seconds in Serial Monitor

  // //------------------------------------------ALARM-------------------------------------------------

  // set_alarm(); // timer for every minute
  // set_daily_alarm(7, 15); // everyday alarm at HH:MM 

}


//---------------------------------------------------------------------LOOP----------------------------------------------------------------------------------------------

void loop() {
  
  // display_time(); // display the time onto LCD screen (RTC_and_LCD.cpp/h)

  // //------------------------------------------ALARM-SYSTEM-------------------------------------------------

  // // if the alarm is fired... (maybe experiment with do while, or while loops?)
  // if ( digitalRead (CLOCK_INTERRUPT_PIN) == LOW ) { 
  //   on_alarm(); // give output message in Serial Monitor
  //   beep(); // sound the alarm

  //   // once button is pressed (while alarm has been triggered)...
  //   if (button_status()) {
  //     delete_alarm(1); // delete alarm
  //     on_button(); // give output message in Serial Monitor
  //     silence(); // shut the alarm up!!
      
  //     // immediately below, if you want the alarm to repeat...
  //     //  ...copy the alarm/timer function from setup above.
  //     set_daily_alarm(11, 30);
  //   }
  // }

  //------------------------------------------TEST-CODE---------------------------------------------------

  // led_status(1); // set LED off

  // if (button_status() == true) {
  //   led_status(1); } // turn on LED

}

