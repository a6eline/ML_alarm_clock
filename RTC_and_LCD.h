
//----------------------------------------------------------------RTC-and-LCD.h------------------------------------------------------------------------------------------------

#pragma once

#include <Arduino.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define CLOCK_INTERRUPT_PIN 4

// ------------------------ ENUM-ALIASING -------------------------------

// Ds3231Alarm1Mode & Ds3231Alarm2Mode is defined in RTClib.h line 55-76
//  ^ https://github.com/adafruit/RTClib/blob/master/src/RTClib.h#L378

// renaming Ds3231Alarm1Mode for better readability
constexpr auto A1_SECOND  =   DS3231_A1_Second;
constexpr auto A1_MINUTE  =   DS3231_A1_Minute;
constexpr auto A1_HOUR    =   DS3231_A1_Hour;

// renaming Ds3231Alarm1Mode for better readability
constexpr auto A2_MINUTE  =   DS3231_A2_Minute;
constexpr auto A2_HOUR    =   DS3231_A2_Hour;

//------------------------SETUP-------------------------------

void checkRTC(void);      // checks if the RTC is working and if power is on
void rtc_setup(void);     // setup for RTC (pins and such)

//------------------------LCD-------------------------------

void lcd_setup(void);     // set up the LCD screen
void lcd_test(void);      // test LCD when pinning new board
void display_time(void);  // display the time on LCD --> may change the setup soon since its a bit ugly

//------------------------TIMER-------------------------------

void timer(int hour, int minute, int second, Ds3231Alarm1Mode alarm_mode);
// template <typename alarmT> 
//   void timer_template(int hour, int minute, int second, alarmT alarm_mode);

//------------------------ALARM-------------------------------

void set_alarm(void);     // sets a hardcoded alarm for debug 
void set_daily_alarm(int hour, int minute); // sets a daily alarm (24HR parameters)

//------------------------RESET-------------------------------

void delete_alarm(int alarmNumber); // deletes the alarm for it to be reset

//------------------------FIRED-------------------------------

bool alarm_fired(void);   // checks if alarm has been fired or not --> will be used soon 

//------------------------MISC-------------------------------
void on_alarm();          // serial.print when alarm occures, used in --> rtc_setup attatchInterupt() function

// // header for renaming 

// constexpr auto whatever_name_you_want = DS3231_A1_Minute;
// constexpr auto whatever_name_you_want = DS3231_A1_Hour;
// // etc...

// // for timer function, can be done like this 

// struct Duration {int hour = 0; int minute = 0; int second = 0};
// enum class AlarmMode {Alarm1, Alarm2}:

// template<AlarmMode mode>
// auto set_timer(const Duration& duration) -> void
// {
// 	auto& [hour, minute, second] = duration;

//     Serial.print("Alarm for "); 
//     Serial.print(minute); Serial.print(" min(s) and "); Serial.print(second); Serial.print(" sec(s)"); 
    
//     if constexpr (mode == AlarmMode::Alarm1) {
//         if (!rtc.setAlarm1(rtc.now() + TimeSpan(0, hour, minute, second), DS3231_A1_Hour)) {
//               Serial.println(" from now was NOT set!");
//         } else {
//             Serial.println(" from now was SUCCESSFULLY set!");
//         }
//     } else if constexpr (mode == AlarmMode::Alarm2) {
//         if (!rtc.setAlarm2(rtc.now() + TimeSpan(0, hour, minute, second), DS3231_A2_Hour)) {
//               Serial.println(" from now was NOT set!");
//         } else {
//             Serial.println(" from now was SUCCESSFULLY set!");
//         }
// 	}
// }

// set_timer<AlarmMode::Alarm1>({.minute = 1});
// set_timer<AlarmMode::Alarm2>({.hour = 1, .minute = 2});
// {
//     using AlarmMode::Alarm1, AlarmMode::Alarm2;
//     set_timer<Alarm1>({.minute = 1});
//     set_timer<Alarm2>({.hour = 1, .minute = 2});
// }

