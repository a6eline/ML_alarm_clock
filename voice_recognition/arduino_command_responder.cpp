
// THESE ARE NOTES FOR ME TO INVESTIGATE THE LIBRARY/EXAMPLES FOR MY OWN USE 

//==================================================================== arduino_command_responder.cpp ====================================================================

  //-------------------------FILE-OVERVIEW-------------------------------
    // this file only has one function
    // the function's purpose is to take in the recognised command word 
    // and output varied LED colours based on the first index of the string

  //-------------------------MODIFICATIONS-------------------------------
    // leave all parameters and branches alone
    // modify the found_command[0] letter recognition
    // if the first letters are the same (on and off)... 
      // ...perhaps strcmp instead or look at the second index. 
    
    

//---------------------------------------------------------------------PREPROCESSOR ----------------------------------------------------------------------------------------------

#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE

#include "Arduino.h"
#include "command_responder.h"
#include "tensorflow/lite/micro/micro_log.h"

//-------------------------------------------------------------------RespondToCommand------------------------------------------------------------------------------------------------

// arduino_command_responder.cpp --> toggles the built-in LED based on the command word
void RespondToCommand(int32_t current_time, const char* found_command, uint8_t score, bool is_new_command) {
  
  //-------------------------statics---------------------------------
  // this is global only when it is used - it will only be done once. 
  static bool is_initialized = false;
  if (!is_initialized) {
    pinMode(LED_BUILTIN, OUTPUT);
    // Pins for the built-in RGB LEDs on the Arduino Nano 33 BLE Sense
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    // Ensure the LED is off by default.
    // Note: The RGB LEDs on the Arduino Nano 33 BLE
    // Sense are on when the pin is LOW, off when HIGH.
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    is_initialized = true;
  }
  //-------------------------statics---------------------------------
  static int32_t last_command_time = 0;
  static int count = 0;

  //-------------------------change-LED-------------------------------
  if (is_new_command) {
    MicroPrintf("Heard %s (%d) @%dms", found_command, score, current_time);
    // If we hear a command, light up the appropriate LED
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);

    if (found_command[0] == 'y') {
      digitalWrite(LEDG, LOW);  // Green for yes
    } else if (found_command[0] == 'n') {
      digitalWrite(LEDR, LOW);  // Red for no
    } else if (found_command[0] == 'u') {
      digitalWrite(LEDB, LOW);  // Blue for unknown
    } else {
      // silence
    }

    last_command_time = current_time;
  }

  //-----------------------turn-off-LED-------------------------------
  // If last_command_time is non-zero but was >3 seconds ago, zero it
  // and switch off the LED.
  if (last_command_time != 0) {
    if (last_command_time < (current_time - 3000)) {
      last_command_time = 0;
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, HIGH);
    }
  }

  //-------------------------toggle-LED---------------------------------
  // Otherwise, toggle the LED every time an inference is performed.
  ++count;
  if (count & 1) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

#endif  // ARDUINO_EXCLUDE_CODE
