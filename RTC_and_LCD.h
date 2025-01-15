#ifndef RTC_AND_LCD_H
#define RTC_AND_LCD_H
#include <Arduino.h>

//------------------------SETUP-------------------------------
void checkRTC(void);
void alarm_setup(void);

//------------------------DISPLAY-------------------------------
void display_time(void);

//------------------------TIMER-------------------------------
void timer_second(int second);
void timer_minute(int minute, int second);

//------------------------ALARM-------------------------------
void set_alarm(void);
void set_daily_alarm(int hour, int minute);

//------------------------RESET-------------------------------
bool delete_alarm(int n);
void stop_alarm(void);

#endif
