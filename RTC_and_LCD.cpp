
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

#include "RTC_and_LCD.h"

// Creating objects of the LCD and RTC 
LiquidCrystal lcd(5, 6, 7, 8, 9, 10); //Pins: RS, E, D4, D5, D6, D7
RTC_DS3231 rtc; 

//---------------------------------------------------------------------RTC----------------------------------------------------------------------------------------------


void checkRTC(void) {
  //lcd.clear(); // clear LCD so previous screen doesnt interfere
  if (!rtc.begin()) { // if I2C communication with RTC is unsuccessful 
    lcd.print("I2C + RTC Error");
    Serial.println("I2C + RTC Error...");
    while (1); // Halt if RTC is not found
  }
  if (rtc.lostPower()) { // if RTC has lost power
    lcd.print("RTC lost power");
    Serial.println("RTC lost power...");
    delay(2000);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // adjust the date and time
  }
  else { 
    Serial.println("RTC working!");
  }
  delay(3000);
}

// RTC_and_LCD.cpp --- set up the RTC module's pins, attatch pin interupt, disable previous alarms and more - check 
void rtc_setup(void) {
  pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP); // Setting sqw pinmode to high  
  attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), on_alarm, FALLING); // The interupt is triggered when the pin falls from high to low
  rtc.writeSqwPinMode(DS3231_OFF); // stop oscillating signals at SQW Pin otherwise setAlarm1 will fail
  
  // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
  // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
  rtc.disableAlarm(1); rtc.clearAlarm(1); rtc.disableAlarm(2); rtc.clearAlarm(2); 
  
  // turn off alarm 2 (in case it isn't off already)
  // again, this isn't done at reboot, so a previously set alarm could easily go overlooked
  rtc.disableAlarm(2);
}

//----------------------------------------------------------------LCD-1602---------------------------------------------------------------------------------------------------

// RTC_and_LCD.cpp --- clear LCD, print success signal and set the screen to 16x2
void lcd_setup(void) {
  lcd.begin(16,2); // setting up the screen to be 16x2
  lcd.print("LCD Sucessful!!"); // update message to signal it is fine
  delay(2000); lcd.clear();
}

// RTC_and_LCD.cpp --- testing the LCD by outputting a string
void lcd_test(void) {
  lcd.setCursor(0, 0);
  lcd.print("Test");
}

// array to convert weekday number to a string 
// dayNames[0] = Sun, 1 = Mon, 2 = Tue...
const char* dayNames[] = {"Sun" , "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

// RTC_and_LCD.cpp --- display the current time onto the LCD screen via DS3231 RTC data
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

// RTC_and_LCD.cpp --- this is a timer which can go from seconds to hours 
void timer(int hour, int minute, int second, Ds3231Alarm1Mode alarm_mode) {
  Serial.print("Alarm for "); 
  Serial.print(hour); Serial.print(" hr(s) and "); Serial.print(minute); Serial.print(" min(s) and "); Serial.print(second); Serial.print(" sec(s)"); 

  if (!rtc.setAlarm1 (rtc.now() + TimeSpan(0, hour, minute, second), alarm_mode) ) {
        Serial.println(" from now was NOT set!");
  }
  else {
      Serial.println (" from now was SUCCESSFULLY set!");
  }
}

// templated timer function so both Ds3231Alarm1Mode and Ds3231Alarm2Mode can be used:
// when using in main type:
//    timer<ds3231Alarm( 1 or 2 )Mode> (hourInt, minInt, secondInt, A1_SECOND);
//    timer<auto> (hourInt, mintInt, secondInt, A1_SECOND);
// template <typename alarmT> 
// void timer_template(int hour, int minute, int second, alarmT alarm_mode) {
//   Serial.print("Alarm for "); 
//   Serial.print(hour); Serial.print(" hr(s) and "); Serial.print(minute); Serial.print(" min(s)"); Serial.print(second); Serial.print(" sec(s)");

//   if (!rtc.setAlarm1 (rtc.now() + TimeSpan(0, hour, minute, second), alarm_mode) ) {
//         Serial.println(" from now was NOT set!");
//   }
//   else {
//       Serial.println (" from now was SUCCESSFULLY set!");
//   }
// }

//---------------------------------------------------------------------ALARM----------------------------------------------------------------------------------------------

// this is an alarm that is set daily at HOUR:MINUTE time perday, set through 24 hr
// TODO - make this 12 hour? with AM and PM to distinguish?
// void set_daily_alarm(int hour, int minute, char meridiem[2]) // meridiem is a 

// RTC_and_LCD.cpp --- this function sets an alarm everyday for HOUR:MINUTE in 24hr code
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

// RTC_and_LCD.cpp --- this sets a hardcoded alarm for debugging 
void set_alarm(void) {
    if(!rtc.setAlarm1(DateTime(0, 0, 0, 0, 0, 0), DS3231_A1_Second)) {
        Serial.println("Error, alarm for every minute wasn't set!");
    }
    else {
        Serial.println("Alarm is set for every minute!");
    }
}

//------------------------------------------------------------------DELETE-ALARM-------------------------------------------------------------------------------------------------

// RTC_and_LCD.cpp --- deletes alarm, which is needed to clear the fired state. This alarm should be set again after triggered output state (such as buzzeer or LED) is turned off.
void delete_alarm(int alarmNumber) {
  // there can only be alarm 1 or 2 - other numbers can result in a bug
  if (alarmNumber == 1 || alarmNumber == 2) {
    // resetting SQW and alarm 1 flag
    // using setAlarm1, the next alarm could now be configurated
    // parameter int n so that user can chose to reset alarm 1 or 2
    if (digitalRead (CLOCK_INTERRUPT_PIN) == HIGH) {
        rtc.disableAlarm(alarmNumber);
        rtc.clearAlarm(alarmNumber); 
        Serial.println("    - Alarm cleared");
    }
  }
  else {
    Serial.println("Wrong parameter inputted for delete_alarm function in RTC_and_LCD.cpp");
  }
}

//-------------------------------------------------------------------MISC------------------------------------------------------------------------------------------------

// RTC_and_LCD.cpp --- serial print that the alarm has occured. needed to attatch interupt to the alarm.
void on_alarm() {
    Serial.println("    -Alarm occured!");
}



