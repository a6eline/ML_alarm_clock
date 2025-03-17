
//=========================================================== pins.h ====================================================================

    // all the pin declarations go in here so it is easy to find
    // they are in namespace and will be explicitly scoped 
    // using constexpr so that they wont be preprocessed
    // this way they are only declared when used


#pragma once

namespace PINS {
  //--LCD1602----------------
  constexpr int LCD_RS = 5;
  constexpr int LCD_E  = 6;
  constexpr int LCD_D4 = 7;
  constexpr int LCD_D5 = 8;
  constexpr int LCD_D6 = 9;
  constexpr int LCD_D7 = 10;

  //--RTC-DS3231--------------
  constexpr int RTC_SQW = 2;

  //--Buzzer-------------------
  constexpr int BUZZER = 3;

  //--Button-(Optional)--------
  constexpr int BUTTON = 4;

  //--LED-(Optional)-----------
  constexpr int LED = 11;
}
