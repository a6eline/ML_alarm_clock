// this is the file for all the c++ functions needed in the arduino
// this is so that the project participants who are not familiar with C++ or Arduino keywords can still participate 

#include "functions.h"
#include "buzzer.h"

//-------------------------------------------------------------------SERIAL------------------------------------------------------------------------------------------------

// functions.cpp --- sets up the serial monitor
void serial_setup(void) {
  Serial.begin(9600); // Setup for serial monitor 
  while (!Serial);
}

//-------------------------------------------------------------------LED------------------------------------------------------------------------------------------------

// functions.cpp --- void function, sets up the LED_PINs 
void led_setup(void) {
  pinMode(LED_PIN, OUTPUT);
}

// functions.cpp --- a void function with parameters 1 to turn on LED, 0 to turn it off.
void led_status(int status) {
  if (status == 1) {
    digitalWrite(LED_PIN, HIGH);
  }
  else if (status == 0) {
    digitalWrite(LED_PIN, LOW);
  }
  else {
    Serial.println("LED function input must be 0 or 1");
  }
}

//-------------------------------------------------------------------BUTTON------------------------------------------------------------------------------------------------

// functions.cpp --- sets up the button pins, attatches a silence interupt.
void button_setup(void) {
  pinMode(BUTTON_PIN, INPUT); // setting button to be an input pin 
  // attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), on_button, FALLING); 
}

// checks whether or not button has been pressed.
bool button_status(void) {
  if(digitalRead(BUTTON_PIN) == HIGH) {
    //Serial.println("Button Pressed!");
    return true; 
  }
  else {
    return false;
  }
}

// functions.cpp --- if button is on, it will serial print ("Button pressed!")
void on_button() {
  Serial.println("Button Pressed!");
}

//-------------------------------------------------------------------MISC------------------------------------------------------------------------------------------------

void count_second(int n) {
  for (int i = 1; i <= n; i++) { // Loop 10 times
    Serial.println(i); // Print the current count value [2, 3, 5]
    delay(1000); // Add a small delay between prints [3, 9]
  }
}




