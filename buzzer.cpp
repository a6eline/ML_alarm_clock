// functions for the buzzer

#include "buzzer.h"

void buzzer_setup(void) {
  pinMode(BUZZER_PIN, OUTPUT);
}

void silence(void) {
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("Buzzer disabled, silence. . .");
}

void beep(void) {
  digitalWrite(BUZZER_PIN, HIGH);
}

// do re mi fa so
void play_song(void) {
  // for(int n, )
  Serial.println(" Do Re Mi Fa So. . .");
  tone(BUZZER_PIN, 440); delay(1000); // A4
  tone(BUZZER_PIN, 494); delay(1000); // B4
  tone(BUZZER_PIN, 523); delay(1000); // C4
  tone(BUZZER_PIN, 587); delay(1000); // D4
  tone(BUZZER_PIN, 659); delay(1000); // E4
  tone(BUZZER_PIN, 698); delay(1000); // F4 
  tone(BUZZER_PIN, 784); delay(1000); // G4
  noTone(BUZZER_PIN); delay(200); // short pause
}