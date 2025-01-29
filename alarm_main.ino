
//---------------------------------------------------------------alarm_main.ino------------------------------------------------------------------------------------------------

  //-------------------------FILE-INFO-------------------------------
  
    // main ino file for the setup and loop functions 
    // here the main actions of the actual alarm system will be initiated 
    // whereas the specific functions using module libraries will be in other header files
    // RTC_and_LCD --> for the rtc/lcd functions
    // esp_conbnection --> for esp connections
    // functions --> for additional

  //-------------------------OPERATIONS-------------------------------

    // ENUM CLASS: setup finite state machine  

    // ------------------SETUP-----------------
    // SETUP: sets up the rtc, lcd, buzzer, button. 
    // Setup: also sets an alarm/timer for tests

    // ------------------LOOP------------------
    // updates time every second
    // sets a static state and defaults to idle
    // does the main FSM system
    //    ^ waits for an alarm fired
    //    ^ beeps when alarm is fired
    //    ^ stops when button is pressed

    // -----------------FUNCTIONS---------------
    // check_and_print_current_state --> checks state change and prints...
    // print_state --> prints state information in serial

  //------------------------IMPORTANT-INFO----------------------------

    // static keyword means it will only be declared once - this is to avoid making it global
    // a state for checking voice recognition will be added in soon 
    // 

#include "RTC_and_LCD.h"
#include "esp_connection.h"
#include "functions.h"

//void serial_setup(void);  // serial monitor begin setup

//---------------------------------FINITE-STATE-MACHINE-ENUM---------------------------------------
// this enum is for the states which the alarm will have
enum class ALARM_STATE : uint8_t { 
  DEFAULT_STATE,  // input alarm    --> inputting alarm fired     ---> state = ALARM_OFF
  ALARM_ON,       // beeps buzzer   --> inputting button pressed  ---> state = ALARM_ON
  ALARM_OFF,      // silence buzzer --> deletes alarm             ---> state = DEFAULT
}; // ^^^ A STATE TO CHECK VOICE COMMANDS WILL BE ADDED SOON 


//---------------------------------------------------------------------SETUP----------------------------------------------------------------------------------------------

void setup() {

  delay(1000); // delay to allow certain boards to upload safely

  //------------------------------------------SETUP-FUNCTIONS---------------------------------------
  lcd_setup(); // setup the LCD
  checkRTC(); rtc_setup(); // check that the rtc is working 
  display_time();
  serial_setup(); 
  led_setup(); buzzer_setup(); button_setup();// extra modules/component setups 
  
  //------------------------------------------TIMER-------------------------------------------------
  //timer(0, 0, 3, A1_HOUR); // timer for 3 seconds timer(int hour, int minute, int second);
  //count_second(3); // to test if you have a timer for n seconds, prints seconds in Serial Monitor

  //------------------------------------------ALARM-------------------------------------------------
  set_daily_alarm(13, 0); // everyday alarm at HH:MM   
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

  switch(alarm_state) 
  {
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
  if (first_state) {                      // ^& pass the variables by reference so that new copies of variable arent made
    print_state(current_state);           //    ^ the purpose of these variables are to track changes 
    first_state = false; }                //    ^ so by copying them the changes will not reflect outsie this function.

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
      Serial.println("    Buzzer on, beep. . .");
      break;

    case ALARM_STATE::ALARM_OFF:
      Serial.println(); Serial.println("STATE --> ALARM OFF");
      Serial.println("    Buzzer disabled, silence. . .");
      break;

    default:
      Serial.println(); Serial.print("ERROR: Unknown state encountered in ALARM_STATE! State ID: ");
      // type casting the enum variable "state" to a uint8_t (number from 0-225) 
      Serial.println(static_cast<uint8_t>(state));  
      break;}
}

// function to test modules work after new pinouts
void test_components(void) {
  if (button_status()) {
    led_status(1); } // Turn on LED
  else {
    led_status(0); } // Turn off LED
}

