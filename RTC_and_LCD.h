
//----------------------------------------------------------------RTC-and-LCD.h------------------------------------------------------------------------------------------------

#pragma once

#include <Arduino.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
#include <Wire.h>

extern RTC_DS3231 rtc; 

// ------------------------ ENUM-ALIASING -------------------------------

// Ds3231Alarm1Mode & Ds3231Alarm2Mode is defined in RTClib.h line 55-76
//  ^ https://github.com/adafruit/RTClib/blob/master/src/RTClib.h#L378

namespace AlarmMode {
  // renaming Ds3231Alarm1Mode for better readability
  constexpr auto A1_SECOND  =   DS3231_A1_Second;
  constexpr auto A1_MINUTE  =   DS3231_A1_Minute;
  constexpr auto A1_HOUR    =   DS3231_A1_Hour;

  // renaming Ds3231Alarm1Mode for better readability
  constexpr auto A2_MINUTE  =   DS3231_A2_Minute;
  constexpr auto A2_HOUR    =   DS3231_A2_Hour;
}

// different alarm nums 
enum class Alarm {A1, A2};

// this is to set up the timers/Alarms 
struct AlarmTime {
  uint32_t hour;
  uint32_t minute;
  uint32_t second;
};

// AlarmDuration will be used for timers
using AlarmDuration = AlarmTime;

//------------------------SETUP-------------------------------

void checkRTC();      // checks if the RTC is working and if power is on
void rtc_setup();     // setup for RTC (pins and such)

//------------------------LCD-------------------------------

void lcd_setup();     // set up the LCD screen
void lcd_test();      // test LCD when pinning new board
void display_time();  // display the time on LCD --> may change the setup soon since its a bit ugly

//------------------------TIMER-------------------------------

void timer(int hour, int minute, int second, Ds3231Alarm1Mode alarm_mode);

//------------------------ALARM-------------------------------

void set_alarm();     // sets a hardcoded alarm for debug 
void set_daily_alarm(const AlarmTime& time); // sets a daily alarm (24HR parameters)

//------------------------RESET-------------------------------

void delete_alarm(Alarm alarm); // deletes the alarm for it to be reset

//------------------------FIRED-------------------------------

bool alarm_fired();   // checks if alarm has been fired or not --> will be used soon 

//------------------------MISC-------------------------------
void on_alarm();          // serial.print when alarm occures, used in --> rtc_setup attatchInterupt() function



//------------------------TEMPLATES-------------------------------
// RTC_and_LCD.cpp --- this is a timer template function which can go from seconds to hours and take in multiple types
// template<AlarmNum alarm>
// struct AlarmModeTraits {}


// RTC_and_LCD.cpp --- this is a timer template function which can go from seconds to hours and take in multiple types
template<Alarm alarm> struct DefaultMode {};
template<> struct DefaultMode<Alarm::A1> { static constexpr auto mode = AlarmMode::A1_HOUR; };
template<> struct DefaultMode<Alarm::A2> { static constexpr auto mode = AlarmMode::A2_HOUR; };

template<Alarm alarm, typename M>
void set_timer(const AlarmDuration& duration, M mode = DefaultMode<alarm>::mode) {
  const auto &d = duration;  
  const auto hour = d.hour, minute = d.minute, second = d.second;

  Serial.println();     Serial.print("TIMER --> ");
  Serial.print(d.hour);   Serial.print(" hr(s) and ");
  Serial.print(d.minute); Serial.print(" min(s) and ");
  Serial.print(d.second); Serial.print(" sec(s)");

  // using constexpr so that it is only branched when used, doesnt compile unused branch
  if constexpr (alarm == Alarm::A1) {
    if (rtc.setAlarm1(rtc.now() + TimeSpan(0, d.hour, d.minute, d.second), mode)) {
      Serial.println(" from now was SUCCESSFULLY set!");
    } else {
      Serial.println(" from now was NOT set!");
    }
  } else if constexpr (alarm == Alarm::A2) {
    if (rtc.setAlarm2(rtc.now() + TimeSpan(0, d.hour, d.minute, d.second), mode)) {
      Serial.println(" from now was SUCCESSFULLY set!");
    } else {
      Serial.println(" from now was NOT set!");
    }
  }
}







