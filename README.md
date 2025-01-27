# Nano ESP32 (Remote) Alarm Clock 
An alarm clock which has to be turned off remotely else where using esp_now one way communication. The purpose of this is to ensure that users get out of bed to turn it off instead of snoozing and going back to bed. 

## Materials Used
- Arduino NANO ESP32 Board
- LCD1602 + Potentiometer
- DS3231 RTC Module
- Mic Module + Passive Buzzer + Button

## Libraries Used
* RTC_and_LCD.cpp/h
  - RTC_lib.h
  - Wire.h
  - LiquidCrystal.h
* esp_connection.cpp/h
  - esp_now.h
  - WiFi.h

## Pinout 
