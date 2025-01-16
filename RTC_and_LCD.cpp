
//-------------------------------------------------------------------RTC-and-LCD------------------------------------------------------------------------------------------------


// this file is for the LCD and RTC module
// it is seperate to functions.cpp as it has too many of its own complexities
// RTC and LCD are in one seperate header file together because... 
//  ...to display RTC info on the LCD, it will need both libraries, so incorporating them makes sense

// here:
// 1. the RTC will be set up to ensure I2c communication works
// 2. the screen will be set up to display the time and date 
// 3. additional functions for alarm functions (setting and resetting alarms)...
//      ^further info about these functions are commented within the functions

#include <LiquidCrystal.h>
// #include <RTClib.h>
#include <Wire.h>
#include "RTC_and_LCD.h"

// Creating objects of the LCD and RTC 
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Pins: RS, E, D4, D5, D6, D7
RTC_DS3231 rtc; 

//---------------------------------------------------------------------SETUP----------------------------------------------------------------------------------------------


void checkRTC(void) {
  lcd.clear(); // clear LCD so previous screen doesnt interfere
  if (!rtc.begin()) { // if I2C communication with RTC is unsuccessful 
    lcd.print("I2C + RTC Error");
    while (1); // Halt if RTC is not found
  }
  if (rtc.lostPower()) { // if RTC has lost power
    lcd.clear(); // clear the screen
    lcd.print("RTC lost power");
    delay(2000);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // adjust the date and time
  }
}

void alarm_setup(void) {
    lcd.clear();
    // update message to signal it is fine
    lcd.print("RTC + LCD Sucessful"); 
    delay(2000);
    lcd.clear();
    lcd.begin(16,2); // setting up the screen to be 16x2
    
    pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP); // Setting sqw pinmode to high  
    // The interupt is triggered when the pin falls from high to low
    attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), on_alarm, FALLING); 

    // stop oscillating signals at SQW Pin
    // otherwise setAlarm1 will fail
    rtc.writeSqwPinMode(DS3231_OFF);

    // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
    // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
    rtc.disableAlarm(1); rtc.clearAlarm(1);
    rtc.disableAlarm(2); rtc.clearAlarm(2);
    
    // turn off alarm 2 (in case it isn't off already)
    // again, this isn't done at reboot, so a previously set alarm could easily go overlooked
    rtc.disableAlarm(2);
}

//----------------------------------------------------------------DISPLAY-TIME---------------------------------------------------------------------------------------------------

// array to convert weekday number to a string 
// dayNames[0] = Sun, 1 = Mon, 2 = Tue...
const char* dayNames[] = {"Sun" , "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void display_time(void) {
  DateTime now = rtc.now();

  lcd.clear(); // Clear the LCD screen

  // Display the day of the week and time on the first line
  lcd.setCursor(0, 0); // Set cursor to column 0, row 0
  lcd.print(dayNames[now.dayOfTheWeek()]); // Day of the week
  lcd.print(' ');

  // Display the time in 12-hour format
  int hour = now.hour() % 12;
  if (hour == 0) hour = 12; // Adjust for 12-hour format
  if (hour < 10) lcd.print('0'); // Add leading zero for hour if necessary
  lcd.print(hour, DEC);
  lcd.print(':');
  if (now.minute() < 10) lcd.print('0'); // Add leading zero for minute if necessary
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  if (now.second() < 10) lcd.print('0'); // Add leading zero for second if necessary
  lcd.print(now.second(), DEC);
  lcd.print(now.hour() < 12 ? " AM" : " PM"); // Display AM/PM

  // Display the date on the second line
  lcd.setCursor(0, 1); // Set cursor to column 0, row 1
  if (now.day() < 10) lcd.print('0'); // Add leading zero for day if necessary
  lcd.print(now.day(), DEC);
  lcd.print('.');
  if (now.month() < 10) lcd.print('0'); // Add leading zero for month if necessary
  lcd.print(now.month(), DEC);
  lcd.print('.');
  lcd.print(now.year(), DEC);

  delay(100); // Update every second
}

//---------------------------------------------------------------------TIMER----------------------------------------------------------------------------------------------

// this sets an alarm n seconds from now
void timer_second(int second) {
    Serial.print("Alarm for "); Serial.print(second); Serial.print(" sec(s");
    if (!rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 0, second), DS3231_A1_Second)) {
         Serial.println(" from now has NOT been set!");
    }
    else {
        Serial.println(" from now has SUCCESSFULLY been set!"); 
    }
}

// this sets a timer for x minutes and y seconds from now
void timer_minute(int minute, int second) {
  Serial.print("Alarm for "); 
  Serial.print(minute); Serial.print(" min(s) and "); Serial.print(second); Serial.print(" sec(s)"); 

  if (!rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, minute, second), DS3231_A1_Minute)) {
        Serial.println(" from now was NOT set!");
  }
  else {
      Serial.println(" from now was SUCCESSFULLY set!");
  }
}

// this sets an alarm for x hours and y minutes from now
void timer_hour(int hour, int minute) {
  Serial.print("Alarm for "); 
  Serial.print(hour); Serial.print(" hr(s) and "); Serial.print(minute); Serial.print(" min(s)"); 

  if (!rtc.setAlarm1 (rtc.now() + TimeSpan(0, hour, minute, 0), DS3231_A1_Hour) ) {
        Serial.println(" from now was NOT set!");
  }
  else {
      Serial.println (" from now was SUCCESSFULLY set!");
  }
}

// this is a timer which can go from seconds to hours
// Ds3231Alarm1Mode is an enum which is defined in RTClib.h line 55 
//  ^https://github.com/adafruit/RTClib/blob/master/src/RTClib.h#L378
 
void timer(int hour, int minute, int second, Ds3231Alarm1Mode alarm_mode) {
  Serial.print("Alarm for "); 
  Serial.print(hour); Serial.print(" hr(s) and "); Serial.print(minute); Serial.print(" min(s)"); 

  if (!rtc.setAlarm1 (rtc.now() + TimeSpan(0, hour, minute, second), alarm_mode) ) {
        Serial.println(" from now was NOT set!");
  }
  else {
      Serial.println (" from now was SUCCESSFULLY set!");
  }
}


// typedef enum 
// {
//     Hr,Min,Sec
// } time_mode;

// void timer_hour(int hour, int minute, char *[]) {
//   Serial.print("Alarm for "); 
//   Serial.print(hour); Serial.print(" hr(s) and "); Serial.print(minute); Serial.print(" min(s)"); 

//   switch ()
//   if(!rtc.setAlarm1(rtc.now() + TimeSpan(0, hour, minute, 0), DS3231_A1_Hour)) {
//         Serial.println(" from now was NOT set!");
//   }
//   else {
//       Serial.println(" from now was SUCCESSFULLY set!");
//   }


// TODO 
// make an enum or class for modes to use as parameter 
// remake set_alarm function to take in number parameters for the dateTime(...)

// void set_timer(int hour, int minute, int second) {
//   if(!rtc.setAlarm1(rtc.now() + TimeSpan(0, hour, minute, second), DS3231_A1_Hour)) {
//       Serial.print("Error, alarm for "); Serial.print(minute); Serial.println(" seconds from now was NOT set!");
//   }
//   else {
//       Serial.print("Alarm is set for "); Serial.print(minute); Serial.println(" seconds from now!"); 
//   }
// }

//---------------------------------------------------------------------ALARM----------------------------------------------------------------------------------------------

// this is an alarm that is set daily at HOUR:MINUTE time perday, set through 24 hr
// TODO - make this 12 hour? with AM and PM to distinguish?
// void set_daily_alarm(int hour, int minute, char meridiem[2]) // meridiem is a 

// this sets a hardcoded alarm for debugging 
void set_alarm(void) {
    if(!rtc.setAlarm1(DateTime(0, 0, 0, 0, 0, 0), DS3231_A1_Second)) {
        Serial.println("Error, alarm for every minute wasn't set!");
    }
    else {
        Serial.println("Alarm is set for every minute!");
    }
}

// this function sets an alarm everyday for HOUR:MINUTE in 24hr code
void set_daily_alarm(int hour, int minute) {
  // "Alarm for HH:MM has NOT/SUCCESSFULLY been set"
  Serial.print("Alarm for "); Serial.print(hour); Serial.print(":"); Serial.print(minute);

  if(!rtc.setAlarm1(DateTime(0, 0, 0, hour, minute, 0), DS3231_A1_Hour)) 
  {
      Serial.println(" has NOT been set! (ERROR)");
  }
  else {
      Serial.println(" has SUCCESSFULLY been set!");
  }
}

//------------------------------------------------------------------DELETE-ALARM-------------------------------------------------------------------------------------------------

bool delete_alarm(int n) {
  // there can only be alarm 1 or 2 - other numbers can result in a bug
  if (n == 1 || n == 2) {
    // resetting SQW and alarm 1 flag
    // using setAlarm1, the next alarm could now be configurated
    // parameter int n so that user can chose to reset alarm 1 or 2
    if (rtc.alarmFired(n)) {
        rtc.disableAlarm(n);
        rtc.clearAlarm(n); 
        Serial.println(" - Alarm cleared");
        return 1;
    }
  }
  else {
    Serial.println("Wrong parameter inputted for reset_alarm function in RTC_and_LCD.cpp");
    return 0;
  }
}

//-------------------------------------------------------------------ALARM------------------------------------------------------------------------------------------------
void on_alarm() {
    Serial.println("Alarm occured!");
}


// // bool to check if the alarm has been cleared or not
// bool alarm_status(void) {
//   if (rtc.alarmFired(1)) {
//       Serial.print("Alarm has been fired!!!");
//       return true;
//   }
//   else {
//     return false;
//   }
// }


