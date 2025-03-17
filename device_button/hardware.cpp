
#include "hardware.h"

//-------------------------------------------------------------SerialSetup---------------------------------------------------------------------------

// hardware.cpp --- sets up the serial monitor
void SerialSetup() {
  Serial.begin(9600); // Setup for serial monitor
  while (!Serial); 
}

//-------------------------------------------------------------buttonSetup---------------------------------------------------------------------------

// hardware.cpp --- sets up the button pins, attatches a silence interupt.
void ButtonSetup() {
  pinMode(PINS::BUTTON,INPUT);  // there is an externel pullup resistor on the board
  // attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), on_button, FALLING); 
  //    ^ uncommented because not needed with FSM, but maybe can include if you want

}

//-------------------------------------------------------------buttonPressed---------------------------------------------------------------------------

// hardware.cpp --- checks if button has been pressed + debounce checker of 50ms
bool ButtonPressed() {
  static auto prev_btn_time = 0;   // tracks the previous press time which is initially 0
  constexpr auto DEBOUNCE_DELAY = 50;  // constant debounce variablec
  const auto current_time = millis();

  if (digitalRead(PINS::BUTTON) == HIGH) {
    if ((current_time - prev_btn_time) > DEBOUNCE_DELAY) {
      prev_btn_time = current_time;
    }
    Serial.println("    -- BUTTON PRESSED");
    return true;
  } else {
    return false;
  }
}

//-------------------------------------------------------------onButton---------------------------------------------------------------------------

// hardware.cpp --- if button is on, it will serial print ("Button pressed!")
//    ^ this is for attachInterupt function in button_setup()
void OnButton() {
  Serial.println("Button Pressed!");
}