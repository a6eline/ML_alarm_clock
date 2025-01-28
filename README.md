# Nano ESP32 (Remote) Alarm Clock 
An alarm clock which has to be turned off remotely elsewhere using esp_now one way communication to split system into 2 devices. The purpose of this is to ensure that users get out of bed to turn it off instead of snoozing and going back to bed. 
Status: Ongoing 
> ML Voice Recognition Model is made by @abbie_bola

## Photos & Pinout
The photos of the pinout uses an Arduino nano board instead of an Arduino nano ESP32 because it is most similuar visually to the board's pinout. All jumpwires are pinned in the same place and SDA/SDL are in A4/A5 respectively - the [Arduino nano ESP32](https://docs.arduino.cc/resources/pinouts/ABX00083-full-pinout.pdf) for reference.
### Alarm Clock Board
![alarm_clock_pinout](https://github.com/user-attachments/assets/84e0cc8f-e5a3-41ba-81e9-2eeefc2f4cef)
![button_pinout](https://github.com/user-attachments/assets/b55ec6d9-76c8-48b1-8c9a-9a881498567f)

## Materials Used
- Arduino NANO ESP32 Board
- LCD1602 + Potentiometer
- DS3231 RTC Module
- Mic Module + Passive Buzzer + Button

## [Project Documentations & Log](https://docs.google.com/document/d/1WiQw86Ue8yddEHVPHRZVkMrpHQOxgGgfX3WtC2Yl9dU/edit?usp=sharing)

## Libraries Used
### RTC_and_LCD.cpp/h
  - RTC_lib.h
  - Wire.h
  - LiquidCrystal.h
### esp_connection.cpp/h
  - esp_now.h
  - WiFi.h

## Pinout 
