

//---------------------------------------------------------------RTC-and-LCD.cpp------------------------------------------------------------------------------------------------

    // all the pin declarations go in here so it is easy to find
    // they are in namespace and will be explicitly scoped 
    // using constexpr so that they wont be preprocessed
    // this way they are only declared when used


#pragma once

namespace PINS {
  //--LCD1602 (4-bit mode)----
  constexpr auto LCD_RS = 5;
  constexpr auto LCD_E  = 6;
  constexpr auto LCD_D4 = 7;
  constexpr auto LCD_D5 = 8;
  constexpr auto LCD_D6 = 9;
  constexpr auto LCD_D7 = 10;

  //--RTC DS3231--------------
  constexpr auto RTC_SQW = 12;

  //--INMP441 (I²S)-----------  
  constexpr auto I2S_SD   = 2; // serial data 
  constexpr auto I2S_WS   = 4; // word select 
  constexpr auto I2S_SCK  = 3; // serial clock

  //--Buzzer-------------------
  constexpr auto BUZZER = 13;

  //--Button-(Optional)--------
  constexpr auto BUTTON = 11;

  //--LED-(Optional)-----------
  constexpr auto LED = 11;
}
