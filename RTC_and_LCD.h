#ifndef RTC_AND_LCD_H
#define RTC_AND_LCD_H
#include <Arduino.h>
#include <RTClib.h>

#define CLOCK_INTERRUPT_PIN 8

//------------------------SETUP-------------------------------
void checkRTC(void);
void alarm_setup(void);

//------------------------DISPLAY-------------------------------
void display_time(void);

//------------------------TIMER-------------------------------
void timer_second(int second);
void timer_minute(int minute, int second);
void timer_hour(int hour, int minute);
void timer(int hour, int minute, int second, Ds3231Alarm1Mode alarm_mode);

//------------------------ALARM-------------------------------
void set_alarm(void);
void set_daily_alarm(int hour, int minute);

//------------------------RESET-------------------------------
bool delete_alarm(int n);

//------------------------MISC-------------------------------
void on_alarm();

#endif


// // Here's your header for renaming shit

// constexpr auto whatever_name_you_want = DS3231_A1_Minute;
// constexpr auto whatever_name_you_want = DS3231_A1_Hour;
// // etc...

// // For your timer function you can do this

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

