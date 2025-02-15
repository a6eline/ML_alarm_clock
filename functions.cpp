
//----------------------------------------------------------------functions.cpp------------------------------------------------------------------------------------------------

  // this is the file for all the extra modules and functions 

#include "functions.h"
#include "pins.h"

//-------------------------------------------------------------------SERIAL------------------------------------------------------------------------------------------------

// functions.cpp --- sets up the serial monitor
void serial_setup() {
  Serial.begin(9600); // Setup for serial monitor 
  while (!Serial); 
}

//-------------------------------------------------------------------BUZZER------------------------------------------------------------------------------------------------
// functions.cpp --- setting up the buzzer
void buzzer_setup() {
  pinMode(PINS::BUZZER, OUTPUT);
  digitalWrite(PINS::BUZZER, LOW); 
}

// functions.cpp --- shut the buzzer up
void silence() {
  digitalWrite(PINS::BUZZER, LOW); 
}

// functions.cpp --- buzzer beeping noise 
void beep(void) {
  digitalWrite(PINS::BUZZER, HIGH); 
}

// functions.cpp --- buzzer pulse beeping with half a second intervals
void pulsed_beep() {
  static auto  beep_millis = millis(); // a static variable so it only executes once at the start
  constexpr auto pause = 500; 

  if (millis() - beep_millis > pause) {
      digitalWrite(PINS::BUZZER, !digitalRead(PINS::BUZZER)); 
    beep_millis = millis();
  }
}

//-------------------------------------------------------------------LED------------------------------------------------------------------------------------------------

// functions.cpp --- void function, sets up the LED_PINs 
void led_setup() {
  pinMode(PINS::LED, OUTPUT); 
}

// functions.cpp --- a void function with parameters 1 to turn on LED, 0 to turn it off.
void led_status(Signal status) {
  switch(status) { 
  case Signal::High:
    digitalWrite(PINS::LED, HIGH);
    break;
  case Signal::Low:
    digitalWrite(PINS::LED, LOW);
    break;
  default:
    Serial.println("LED function input must be 0 or 1");
  }
}

//-------------------------------------------------------------------BUTTON------------------------------------------------------------------------------------------------

// functions.cpp --- sets up the button pins, attatches a silence interupt.
void button_setup() {
  pinMode(PINS::BUTTON, INPUT);  // setting button to be an input pin 
  // attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), on_button, FALLING); 
  //    ^ uncommented because not needed with FSM, but maybe can include if you want
}

// functions.cpp --- checks if button has been pressed + debounce checker of 50ms
bool button_pressed() {
  static auto start_time = 0;   // tracks the previous press time which is initially 0 
  constexpr auto DEBOUNCE_DELAY = 50;  // constant debounce variable
  const auto current_time = millis();

  if (current_time - start_time > DEBOUNCE_DELAY) { 
    if (digitalrRead(PINS::BUTTON)) {
      start_time = current_time;
      return true;
    }
  }
  return false;
}

// functions.cpp --- if button is on, it will serial print ("Button pressed!")
//    ^ this is for attachInterupt function in button_setup()
void on_button() {
  Serial.println("Button Pressed!");
}

//-------------------------------------------------------------------MISC------------------------------------------------------------------------------------------------

// functions.cpp --- counts 3 seconds without delay, doesnt work tho, i'll sort that out later
void count_second(int n) {
  constexpr auto one_sec = 1000;
  auto timer_millis = millis();
  
  for (int i = 0; i < n;) { // Loop n times
    if ((millis() - timer_millis) > one_sec) {
      Serial.println(n);
      timer_millis = millis();
      i++;
    }
  }
}




