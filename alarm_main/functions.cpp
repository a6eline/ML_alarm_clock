// this is the file for all the c++ functions needed in the arduino
// this is so that the project participants who are not familiar with C++ or Arduino keywords can still participate 

#include "functions.h"
#include "buzzer.h"


//-------------------------------------------------------------------BUTTON------------------------------------------------------------------------------------------------
void button_setup(void) {
  pinMode(BUTTON_PIN, INPUT); // setting button to be an input pin 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), silence, FALLING); 
}

// checks whether or not button has been pressed.
bool button_status(void) {
  if(digitalRead(BUTTON_PIN) == HIGH) {
    Serial.println("Button Pressed!");
    return true; 
  }
  return false;
}

void on_button() {
  Serial.println("Button Pressed!");
}

void serial_setup(void) {
  Serial.begin(9600); // Setup for serial monitor 
  while (!Serial);
}

//-------------------------------------------------------------------MISC------------------------------------------------------------------------------------------------
void count_second(int n) {
  for (int i = 1; i <= n; i++) { // Loop 10 times
    Serial.println(i); // Print the current count value [2, 3, 5]
    delay(1000); // Add a small delay between prints [3, 9]
  }
}




