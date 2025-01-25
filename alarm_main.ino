// main ino file for the setup and loop functions 
// here the main actions of the actual alarm system will be initiated 
// whereas the specific functions using module libraries will be in other header file
// PIN CONFIGURATION IS IN pinout.h

#include "RTC_and_LCD.h"
#include "buzzer.h"
#include "functions.h"

//---------------------------------FINITE-STATE-MACHINE-ENUM---------------------------------------
// this enum is for the states which the alarm will have
// a state will be added to check for the voice commands too soon 
enum class ALARM_STATE : uint8_t { 
  DEFAULT_STATE,  // input alarm    --> inputting alarm fired     ---> state = ALARM_OFF
  ALARM_ON,       // beeps buzzer   --> inputting button pressed  ---> state = ALARM_ON
  ALARM_OFF,      // silence buzzer --> deletes alarm             ---> state = DEFAULT
}; 

//---------------------------------------------------------------------SETUP----------------------------------------------------------------------------------------------

void setup() {

  delay(1000); // delay is NEEDED to wait for upload. It usually take around 3-5 seconds, depending on the board.
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

void loop() {

  display_time(); // display the time onto LCD screen (RTC_and_LCD.cpp/h)

  //--------------------------------------FINITE-STATE-MACHINE----------------------------------------------
  
  static ALARM_STATE alarm_state = ALARM_STATE::DEFAULT_STATE;    // current state 
  static ALARM_STATE previous_state = ALARM_STATE::DEFAULT_STATE; // previous state 
  static bool first_state = true;                                 // first state (used for printing)

  // function to check and print the state changes
  check_and_print_current_state(alarm_state, previous_state, first_state);

  switch(alarm_state) {
    //-------------------------DEFAULT-------------------------------
    case ALARM_STATE::DEFAULT_STATE: {
      // checking if alarm is fired --> DS3231 SQW pin == LOW
      if ( digitalRead (CLOCK_INTERRUPT_PIN) == LOW ) { 
        alarm_state = ALARM_STATE::ALARM_ON; }
      break; } 

    //-------------------------ALARM_ON-------------------------------
    case ALARM_STATE::ALARM_ON: {
      beep();
      if( button_status() == true) { 
        alarm_state = ALARM_STATE::ALARM_OFF; }
      break; }    
      
    //-------------------------ALARM_OFF---------------------------
    case ALARM_STATE::ALARM_OFF: {
      delete_alarm(1);
      silence(); 
      delay(500); // Allow some time for state to stabilize
      
      if (digitalRead(CLOCK_INTERRUPT_PIN) == HIGH) {
        Serial.println("Alarm cleared successfully.");
      } else {
        Serial.println("Failed to clear alarm.");
      }

      alarm_state = ALARM_STATE::DEFAULT_STATE;
      break; } 

    //-------------------------DEFAULT-CASE---------------------------
    default: 
      alarm_state = ALARM_STATE::DEFAULT_STATE; // Reset to default state to avoid instability

  }

}

//---------------------------------------------------------------------EXTRA-FUNCTIONS----------------------------------------------------------------------------------------------

// function to handle and print initial state and state changes for the finite state machine
void check_and_print_current_state(ALARM_STATE &current_state, ALARM_STATE &previous_state, bool &first_state) {
  if (first_state) {                      // ^& here we pass the variables by reference so that new copies of current
    print_state(current_state);           //    or previous states and first states variables arent made. this is because 
    first_state = false; }                //    the purpose of these variables are to track changes - so by copying them 
                                          //    the changes will not reflect outsie this function.
  if (current_state != previous_state) {
    print_state(current_state); 
    previous_state = current_state; }
}

// this prints the current state 
void print_state(ALARM_STATE state) { //  <---  no need to use & as this is a "read-only" state
  switch (state) {                    //        plus, enum is lightweight so copying it has little damage to performance              
    case ALARM_STATE::DEFAULT_STATE:
      Serial.println(); Serial.println("STATE --> DEFAULT");
      break;
    case ALARM_STATE::ALARM_ON:      
      Serial.println(); Serial.println("STATE --> ALARM ON");
      Serial.println("Buzzer on!!!! BEEEEEEEEEEEP. . .");
      break;
    case ALARM_STATE::ALARM_OFF:
      Serial.println(); Serial.println("STATE --> ALARM OFF");
      break;
    default:
      Serial.println(); Serial.print("Error: Unknown state encountered in ALARM_STATE! State ID: ");
      // type casting the enum variable "state" to a uint8_t (number from 0-225) 
      Serial.println(static_cast<uint8_t>(state));  
      break;}
}

// function area to test certain components
void test_components(void) {
  if (button_status()) {
    led_status(1); // Turn on LED
  } else {
    led_status(0); // Turn off LED
  }
}

