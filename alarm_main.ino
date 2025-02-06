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

//---------------------------------FINITE-STATE-MACHINE-ENUM---------------------------------------
// this enum is for the states which the alarm will have
enum class AlarmState : uint8_t {
  Default,       // input alarm    --> inputting alarm fired     ---> state = AlarmOff
  AlarmOn,       // beeps buzzer   --> inputting button pressed  ---> state = AlarmOn
  AlarmOff       // silence buzzer --> deletes alarm             ---> state = DEFAULT
}; // ^^^ A STATE TO CHECK VOICE COMMANDS WILL BE ADDED SOON

// NOTE (remove before merging): Is a default state even required? What other state is there besides the alarm being on/off? Try drawing out the FSA


//---------------------------------------------------------------------SETUP----------------------------------------------------------------------------------------------

void setup() {
  delay(1000); // delay to allow certain boards to upload safely

  //------------------------------------------SETUP-FUNCTIONS---------------------------------------
  lcd_setup(); // setup the LCD
  checkRTC();
  rtc_setup(); // check that the rtc is working
  display_time();
  serial_setup();
  led_setup();
  buzzer_setup();
  button_setup();// extra modules/component setups

  //------------------------------------------TIMER-------------------------------------------------
  //timer(0, 0, 3, A1_HOUR); // timer for 3 seconds timer(int hour, int minute, int second);
  //count_seconds(3); // to test if you have a timer for n seconds, prints seconds in Serial Monitor

  set_timer<Alarm::A1>(AlarmDuration {0, 0, 3}, AlarmMode::A1_HOUR);

  //------------------------------------------ALARM-------------------------------------------------
  // set_daily_alarm(13, 0); // everyday alarm at HH:MM
  //set_daily_alarm(AlarmTime {13, 0, 0}); // everyday alarm at HH:MM
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
    // if (alarm_fired()) { // NOTE (remove before merging): bool alarm_fired() checks if CLOCK_INTERRUPT_PIN is HIGH, is this a mistake?
    if (digitalRead(CLOCK_INTERRUPT_PIN) == LOW) {
      current_state = AlarmState::AlarmOn;
    }
    break;

    //-------------------------AlarmOn-------------------------------
  case AlarmState::AlarmOn:
    beep();
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


