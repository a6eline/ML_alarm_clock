// main ino file for the setup and loop functions 
// here the main actions of the actual alarm system will be initiated 
// whereas the specific functions using module libraries will be in other header file

#include "RTC_and_LCD.h"
#include "buzzer.h"
#include "functions.h"

// button pin is 2
// buzzer pin is 3
// lcd pins are 5-10


//---------------------------------------------------------------------SETUP----------------------------------------------------------------------------------------------

uint32_t prev_time = 0;
uint32_t next_time = 0;


void setup() {

  delay(2000); // delay is NEEDED to wait for upload. It usually take around 3-5 seconds, depending on the board.
  //  ...if the delay board takes longer to upload/compile, then make the delay longer.
  //  ...you can tell if the Serial message below does not print.

  //------------------------------------------SETUP-FUNCTIONS---------------------------------------
  lcd_setup(); // setup the LCD
  checkRTC(); rtc_setup(); // check that the rtc is working 
  display_time();
  serial_setup(); led_setup(); buzzer_setup(); button_setup();// extra modules/component setups // 
  Serial.println("Serial Test for Alarm Clock"); 
  
  //------------------------------------------TIMER-------------------------------------------------

  // //timer_second(3);
  timer(0, 0, 3, A1_HOUR); // timer for 3 seconds timer(int hour, int minute, int second);
  count_second(3); // to test if you have a timer for n seconds, prints seconds in Serial Monitor

  //------------------------------------------ALARM-------------------------------------------------

  // set_alarm(); // timer for every minute
  // set_daily_alarm(7, 15); // everyday alarm at HH:MM   
}


//---------------------------------------------------------------------ALARM-LOOP----------------------------------------------------------------------------------------------

enum class ALARM_STATE : uint8_t {
  DEFAULT_STATE,  // input alarm    --> inputting alarm fired     ---> state = ALARM_OFF
  ALARM_ON,       // beeps buzzer   --> inputting button pressed  ---> state = ALARM_ON
  ALARM_OFF,      // silence buzzer --> deletes alarm             ---> state = DEFAULT
};
static ALARM_STATE alarm_state = ALARM_STATE::DEFAULT_STATE; // setting up the initial alarm state

void loop() {

  display_time(); // display the time onto LCD screen (RTC_and_LCD.cpp/h)
  // uint32_t timeMs = millis(); // fetch the current time

  //--------------------------------------FINITE-STATE-MACHINE----------------------------------------------

  switch(alarm_state) {

    //-------------------------DEFAULT-------------------------------
    case ALARM_STATE::DEFAULT_STATE: {
      Serial.println("STATE --> DEFAULT");

      // checking if alarm is fired --> DS3231 SQW pin == HIGH
      if ( digitalRead (CLOCK_INTERRUPT_PIN) == HIGH ) { 
        alarm_state = ALARM_STATE::ALARM_ON; }
      break; } 

    //-------------------------ALARM_ON-------------------------------
    case ALARM_STATE::ALARM_ON: {
      Serial.println("STATE --> ALARM ON");
      beep();
      if( button_status() == true) { 
        alarm_state = ALARM_STATE::ALARM_OFF; }
      break; }    
      
    //-------------------------ALARM_OFF---------------------------
    case ALARM_STATE::ALARM_OFF: {
      Serial.println("STATE --> ALARM OFF");
      silence(); 
      alarm_state = ALARM_STATE::DEFAULT_STATE;
      break; } 

    default: 
      Serial.print("Something is wrong ---> switch case is default... "); 
  }
  

  //------------------------------------------TEST-CODE---------------------------------------------------
  
  // this code is for testing the board when i repin to make sure that the modules work
  //lcd_test();
  // if (button_status() == true) {
  //   led_status(1); } // turn on LED
  // else if (button_status() == false) {
  //   led_status(0); // turn off LED
  // }

}











// // if the alarm is fired... (maybe experiment with do while, or while loops?) (make a function for if SQW == high to abstract this)
// if ( digitalRead (CLOCK_INTERRUPT_PIN) == HIGH ) { 
//   beep(); // sound the alarm

//   // once button is pressed (while alarm has been triggered)...
//   if ( button_status() == true ) {
//     delete_alarm(1); // delete alarm 1
//     silence(); // shut the alarm up!!

//     // immediately below, if you want the alarm to repeat...
//     //  ...copy the alarm/timer function from setup above.
//     //set_daily_alarm(11, 30);

//   }
// }
