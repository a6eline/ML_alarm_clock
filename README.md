# Nano ESP32 (Remote) Alarm Clock 
An alarm clock which has to be turned off remotely elsewhere using esp_now one way communication to split system into 2 devices. The purpose of this is to ensure that users get out of bed to turn it off instead of snoozing and going back to bed. 
Implemented using: C/C++, Python, Tensorflow, Keras, Arduino, Librosa, & Audiomentations 
Status: Ongoing 
> ML Voice Recognition Model is made by @abbie_bola

## Photos & Pinout
<img align="centre" src="https://github.com/user-attachments/assets/6ddc1d16-701f-4b6f-b7a4-b18933028ffa"/> 
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
