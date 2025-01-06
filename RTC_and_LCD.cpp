
// THIS IS CODE FOR THE SET UP DISPLAY FOR THE RTC AND LCD SCREEN

// function checkRTC checks if the RTC communication is successful or not
// function display_time uses the LCD screen to display the current time 

#include <LiquidCrystal.h>
#include <RTClib.h>
#include <Wire.h>

#include "RTC_and_LCD.h"


// Creating objects of the LCD and RTC 
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Pins: RS, E, D4, D5, D6, D7
RTC_DS3231 rtc; 

// array to convert weekday number to a string 
// dayNames[0] = Sun, 1 = Mon, 2 = Tue...
const char* dayNames[] = {"Sun" , "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void checkRTC(void) {
  lcd.begin(16,2);

  if (!rtc.begin()) // if I2C communication with RTC is unsuccessful 
  {
    lcd.print("I2C + RTC Error");
    while (1); // Halt if RTC is not found
  }

  if (rtc.lostPower())  // if RTC has lost power
  {
    lcd.clear(); // clear the screen
    lcd.print("RTC lost power");
    delay(2000);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // adjust the date and time
  }

  lcd.clear();
  // update message to signal it is fine
  lcd.print("RTC + LCD Sucessful"); 
  delay(2000);
  lcd.clear();
}

void alarm_setup(void) {
    // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
    // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);

    // stop oscillating signals at SQW Pin
    // otherwise setAlarm1 will fail
    rtc.writeSqwPinMode(DS3231_OFF);
    // turn off alarm 2 (in case it isn't off already)
    // again, this isn't done at reboot, so a previously set alarm could easily go overlooked
    rtc.disableAlarm(2);
}

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
  // lcd.print(':');
  // if (now.second() < 10) lcd.print('0'); // Add leading zero for second if necessary
  // lcd.print(now.second(), DEC);
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

  delay(1000); // Update every second
}

// this sets an alarm 10 seconds from now
void timer_second(int n) {
    if(!rtc.setAlarm1(rtc.now() + TimeSpan(n), DS3231_A1_Second)) {
        Serial.print("Error, alarm for "); Serial.print(n); Serial.println(" seconds from now was NOT set!");
    }
    else {
        Serial.print("Alarm is set for "); Serial.print(n); Serial.println(" seconds from now!"); 
    }
}

void set_alarm(void) {
    if(!rtc.setAlarm1(DateTime(0, 0, 0, 10, 30, 0), DS3231_A1_Hour)) {
        Serial.println("Error, alarm for 10:30AM wasn't set!");
    }
    else {
        Serial.println("Alarm is set for 10:30AM!");
    }
}

// bool to check if the alarm has been cleared or not
bool alarm_status(void) {
  if (rtc.alarmFired(1)) {
      Serial.print("Alarm has been fired!!!");
      return true;
  }
  else {
    return false;
  }
}

void clear(void) {
  rtc.clearAlarm(1);
  Serial.println("...Alarm cleared");
}

