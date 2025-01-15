// functions for the buzzer

#include "buzzer.h"

#define BUZZER_PIN 9

// void buzzer_toggle()

void buzzer_setup(void) {
  pinMode(BUZZER_PIN, OUTPUT);
}

void silence(void) {
  noTone(BUZZER_PIN);
  Serial.println("Buzzer disabled, silence. . .");
}

void beep(void) {
  tone(BUZZER_PIN, 440); //A$
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