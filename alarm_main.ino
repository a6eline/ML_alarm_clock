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

    // -----------------FUNCTIONS---------------
    // check_and_print_current_state --> checks state change and prints...
    // print_state --> prints state information in serial

  //------------------------IMPORTANT-INFO----------------------------

    // a state for checking voice recognition will be added in soon
  // default state is so that the device only checks for the SQW pin being high WHEN the alarm is off
  // so that there arent multiple alarms going off at once which can affect the system turning off the buzzer

#include "RTC_and_LCD.h"
// #include "esp_connection.h"
#include "functions.h"
#include "pins.h"

//---------------------------------FINITE-STATE-MACHINE-ENUM---------------------------------------
// this enum is for the states which the alarm will have
enum class AlarmState : uint8_t {
  Default,       // input alarm    --> inputting alarm fired     ---> state = AlarmOff
  AlarmOn,       // beeps buzzer   --> inputting button pressed  ---> state = AlarmOn
  AlarmOff       // silence buzzer --> deletes alarm             ---> state = DEFAULT
}; 

//---------------------------------------------------------------------SETUP----------------------------------------------------------------------------------------------

void setup() {
  delay(5000); // delay to allow certain boards to upload safely

  //------------------------------------------SETUP-FUNCTIONS---------------------------------------
  lcd_setup();   
  checkRTC();
  rtc_setup();    
  display_time();
  serial_setup();
  buzzer_setup();
  button_setup(); 

  //------------------------------------------TIMER-------------------------------------------------
  set_timer<Alarm::A1>(AlarmDuration {0, 0, 3}, AlarmMode::A1_HOUR);
  //count_seconds(3); // test the

  //------------------------------------------ALARM-------------------------------------------------
  // set_daily_alarm(AlarmTime {12, 0, 0}); // everyday alarm at HH:MM
}

//---------------------------------------------------------------------ALARM-LOOP----------------------------------------------------------------------------------------------

static auto current_state = AlarmState::Default;    // current state
static auto previous_state = AlarmState::AlarmOff;  // previous state

void loop() {
  display_time(); // display the time onto LCD screen (RTC_and_LCD.cpp/h)

  //--------------------------------------FINITE-STATE-MACHINE----------------------------------------------

  // function to check and print the state changes
  // check_and_print_current_state(current_state, previous_state, first_state);

  if (current_state != previous_state) {
    print_state(current_state);
    previous_state = current_state;
  }

  switch (current_state) {
    //-------------------------DEFAULT-------------------------------
  case AlarmState::Default:
    // checking if alarm is fired --> DS3231 SQW pin == LOW
    if (digitalRead(PINS::RTC_SQW) == LOW) {
      current_state = AlarmState::AlarmOn;
    }
    break;

    //-------------------------AlarmOn-------------------------------
  case AlarmState::AlarmOn:
    // beep();
    pulsed_beep();
    if (button_pressed()) {
      current_state = AlarmState::AlarmOff;
    }
    break;

    //-------------------------AlarmOff---------------------------
  case AlarmState::AlarmOff:
    delete_alarm(Alarm::A1);
    silence();
    delay(500); // Allow some time for state to stabilize
    current_state = AlarmState::Default;
    break;

    //-------------------------DEFAULT-CASE---------------------------
  default:
    current_state = AlarmState::Default; // Reset to default state to avoid instability
  }
}

//---------------------------------------------------------------------EXTRA-FUNCTIONS----------------------------------------------------------------------------------------------

// this prints the current state
void print_state(AlarmState state) { //  <---  no need to use & as this is a "read-only" state
  switch (state) {                   //        plus, enum is lightweight so copying it has little damage to performance
  case AlarmState::Default:
    Serial.println();
    Serial.println("STATE --> DEFAULT");
    break;

  case AlarmState::AlarmOn:
    Serial.println();
    Serial.println("STATE --> ALARM ON");
    Serial.println("    Buzzer on, beep. . .");
    break;

  case AlarmState::AlarmOff:
    Serial.println();
    Serial.println("STATE --> ALARM OFF");
    Serial.println("    Buzzer disabled, silence. . .");
    break;
  }
}

// function to test modules work after new pinouts
void test_components() {
  led_status(button_pressed() ? Signal::High : Signal::Low);
}


