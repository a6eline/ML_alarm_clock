#ifndef RTC_AND_LCD_H
#define RTC_AND_LCD_H
#include <Arduino.h>
#include <RTClib.h>

#define CLOCK_INTERRUPT_PIN 8

// Renaming Alarm mode enums as it was too lengthy
// Ds3231Alarm1Mode is an enum which is defined in RTClib.h line 55 
//  ^https://github.com/adafruit/RTClib/blob/master/src/RTClib.h#L378

// ORIGINAL DS3231 Alarm modes for alarm 1 and 2 . . .

// enum Ds3231Alarm1Mode {
//   DS3231_A1_PerSecond = 0x0F,
//   DS3231_A1_Second = 0x0E,
//   DS3231_A1_Minute = 0x0C,
//   DS3231_A1_Hour = 0x08,
//   DS3231_A1_Date = 0x00,
//   DS3231_A1_Day = 0x10
// };

// enum Ds3231Alarm2Mode {
//   DS3231_A2_PerMinute = 0x7,
//   DS3231_A2_Minute = 0x6,
//   DS3231_A2_Hour = 0x4,
//   DS3231_A2_Date = 0x0,
//   DS3231_A2_Day = 0x8
// };

// changing Ds3231Alarm1Mode names...
constexpr auto A1_SECOND = DS3231_A1_Second;
constexpr auto A1_MINUTE = DS3231_A1_Minute;
constexpr auto A1_HOUR = DS3231_A1_Hour;

// changing Ds3231Alarm2Mode names...
constexpr auto A2_MINUTE = DS3231_A2_Minute;
constexpr auto A2_HOUR = DS3231_A2_Hour;


//------------------------SETUP-------------------------------
void checkRTC(void);
void alarm_setup(void);

//------------------------DISPLAY-------------------------------
void display_time(void);

//------------------------TIMER-------------------------------
void timer(int hour, int minute, int second, Ds3231Alarm1Mode alarm_mode);
// template <typename alarmT> 
//   void timer_template(int hour, int minute, int second, alarmT alarm_mode);

//------------------------ALARM-------------------------------
void set_alarm(void);
void set_daily_alarm(int hour, int minute);

//------------------------RESET-------------------------------
bool delete_alarm(int n);

//------------------------MISC-------------------------------
void on_alarm();

#endif


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

