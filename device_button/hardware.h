
#pragma once


constexpr int BUTTON_PIN = 5;

//------------------------SERIAL----------------------------

void SerialSetup();

//------------------------BUTTON----------------------------

void ButtonSetup();    // button setup
bool ButtonPressed();  // checks if button has been pressed
void OnButton();       // button pressed serial print message


