// this is the file for all the c++ functions needed in the arduino
// this is so that the project can run smoother for those who are not familiar with C++ or Arduino keywords

#include "functions.h"

void count_second(int n) {

  for (int i = 1; i <= n; i++) { // Loop 10 times
    Serial.println(i); // Print the current count value [2, 3, 5]
    delay(1000); // Add a small delay between prints [3, 9]
  }
}
