
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
  constexpr auto I2S_BCLK = 3;
  constexpr auto I2S_WS   = 4;
  constexpr auto I2S_DOUT = 2;

  //-Buzzer--------------------
  constexpr auto BUZZER = 13;

  //--Button (Optional)--------
  constexpr auto BUTTON = 11;

  //--LED (Optional)-----------
  constexpr auto LED = 11;
}
