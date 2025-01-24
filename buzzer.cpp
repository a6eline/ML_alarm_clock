// functions for the buzzer

#include "buzzer.h"

void buzzer_setup(void) {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void silence(void) {
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("Buzzer disabled, silence. . .");
}

void beep(void) {
  digitalWrite(BUZZER_PIN, HIGH);
  Serial.print("Buzzer on!!!! BEEEEEEEEEEEP. . .");
}
