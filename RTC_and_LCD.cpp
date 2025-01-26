
//---------------------------------------------------------------RTC-and-LCD.cpp------------------------------------------------------------------------------------------------

  //-------------------------FILE-INFO-------------------------------
  
    // it is seperate to functions.cpp as it has too many of its own complexities
    // RTC and LCD are in a .cpp/.h file together because... 
    //  ...declaring the objects needed to be in the same scope or it doesnt work

  //-------------------------FUNCTIONS-------------------------------

    // 1. the RTC will be set up to ensure I2c communication/battery works
    // 2. the screen will be set up to display the time and date 
    // 3. additional functions for setting alarms and timers...
    // 4. functions to delete alarms and check if alarm is fired
    //      ^further info about these functions are commented within the functions

    // the reason why i have not used the bool function for checking the status of if alarm has been fired is because... 
    //  ...i was experimenting with the pins, but all checks for SQW pin will be replaced with bool function soon

  //------------------------IMPORTANT-INFO----------------------------

    // rtc.adjust(DateTime(F(DATE), F(TIME))) should only be used once
    // RTC module is meant to be independent --> constant resets defeats the purpose
    //    ^ similar to resetting an anolog clock everytime you look at it

    // ONLY IF SOMETHING SEEMS WRONG: 
    // uncomment the adjustment function back in the code only when there seems to be something wrong
    // the function is already initialised in the checkRTC function uder the rtc.lostPower() condition 

    // info about DS3231 modes are in RTC_and_LCD.h

#include "RTC_and_LCD.h"
#include <EEPROM.h>

// Creating objects of the LCD and RTC 
LiquidCrystal lcd(5, 6, 7, 8, 9, 10); //Pins: RS, E, D4, D5, D6, D7
RTC_DS3231 rtc; 

//---------------------------------------------------------------------RTC----------------------------------------------------------------------------------------------

// RTC_and_LCD.cpp --- checks if the I2C communication is successful and if the RTC module has power
void checkRTC(void) {
  // try to initialize RTC 
  Serial.println(); Serial.println(); 
  Serial.println("Checking RTC"); 
  lcd.clear(); lcd.setCursor(0, 0);
  lcd.println("Checking RTC"); delay(2000); lcd.clear();

  //-------------------------CHECK-I2C/RTC-------------------------------
  // Check if I2C communication with RTC is unsuccessful
  while ( !rtc.begin() ) { 
    lcd.clear(); lcd.setCursor(0, 0); 
    lcd.print("RTC ERROR"); Serial.println("RTC ERROR");
    delay(1000); // wait before printing next message

    lcd.setCursor(0, 1); 
    lcd.print("Check connections"); Serial.println("Check connections");
    delay(2000); // wait 2 seconds before halting the program

    while (1); } // Halt the program after one failed RTC initialization attempt

  //-------------------------CHECK-POWER-------------------------------
  // If RTC has lost power
  if (rtc.lostPower()) { 
    lcd.print("RTC lost power");
    Serial.println("RTC lost power..."); 
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); }
  
  //-------------------------INITIALISED!-------------------------------
  else {
    lcd.print("RTC Initialized");
    Serial.println("RTC Initialized"); }
  
  Serial.println();  
  delay(2000); // display message for 2 seconds
}


// RTC_and_LCD.cpp --- set up the RTC module's pins, attatch pin interupt, disable previous alarms and more - check 
void rtc_setup(void) {

  // ONLY SET TIME ONCE 
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 

  pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP);                                     // Setting sqw pinmode to high --> LOW == alarm is fired 
  attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), on_alarm, FALLING); // The interupt is triggered when the pin falls from high to low
  rtc.writeSqwPinMode(DS3231_OFF);                                                // stop oscillating signals at SQW Pin otherwise setAlarm1 will fail
  
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

// array to convert weekday number to a string ---> dayNames[0] = Sun, 1 = Mon, 2 = Tue...
const char* dayNames[] = {"Sun" , "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

// RTC_and_LCD.cpp --- display the current time onto the LCD screen via DS3231 RTC data. This setup is kind of ugly, I might change it in the future :P
void display_time(void) {
  static unsigned long display_millis = millis(); // a static variable so it only executes once at the start
  const int one_sec = 1000;                       // 1000 millis is 1 second

  if (millis() - display_millis > one_sec) {
    DateTime now = rtc.now();                     // update the now time
    lcd.clear();                                  // Clear the LCD screen

    //-------------------------DISPLAY-WEEKDAY-------------------------------
    lcd.setCursor(0, 0);                          // set cursor to column 0, row 0
    lcd.print(dayNames[now.dayOfTheWeek()]);      // day of the week
    lcd.print(' ');                               // printing whitespace before time display

    //-------------------------DISPLAY-12HR-------------------------------
    int hour = now.hour() % 12;
    if (hour == 0) hour = 12;                     // adjust for 12-hour format
    if (hour < 10) lcd.print('0');                // add leading zero for hour if necessary
    lcd.print(hour, DEC); lcd.print(':');         // printing the current time hour

    //-------------------------DISPLAY-MINUTE-------------------------------
    if (now.minute() < 10) lcd.print('0');        // add leading zero for minute if necessary
    lcd.print(now.minute(), DEC); lcd.print(':'); // print the current minute
    
    //-------------------------DISPLAY-SECOND-------------------------------
    if (now.second() < 10) lcd.print('0');        // add leading zero for second if necessary
    lcd.print(now.second(), DEC);                 // print the current second
    lcd.print(now.hour() < 12 ? " AM" : " PM");   // display AM/PM

    //-------------------------DISAPLY-DAY-------------------------------
    lcd.setCursor(0, 1);                          // set cursor to column 0, row 1
    if (now.day() < 10) lcd.print('0');           // add leading zero for day if necessary
    lcd.print(now.day(), DEC); lcd.print('.');    // print the current day 
    
    //-------------------------DISPLAY-DATE-------------------------------
    if (now.month() < 10) lcd.print('0');         // add leading zero for month if necessary
    lcd.print(now.month(), DEC); lcd.print('.');  // print the current month
    lcd.print(now.year(), DEC);                   // print the current year
    
    //-------------------------UPDATE-MILLIS-------------------------------
    display_millis = millis();
  }
}

//---------------------------------------------------------------------TIMER----------------------------------------------------------------------------------------------

// RTC_and_LCD.cpp --- this is a timer which can go from seconds to hours 
void timer(int hour, int minute, int second, Ds3231Alarm1Mode alarm_mode) {
  Serial.println(); Serial.print("TIMER --> "); 
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
  Serial.println();
  Serial.print("ALARM ---> "); Serial.print(hour); Serial.print(":"); Serial.print(minute);

  if(!rtc.setAlarm1(DateTime(0, 0, 0, hour, minute, 0), DS3231_A1_Hour)) {
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
  // There can only be alarm 1 or 2 - other numbers can result in a bug
  if ( alarmNumber == 1 || alarmNumber == 2 ) {
    rtc.disableAlarm(alarmNumber);    // disable the specified alarm
    rtc.clearAlarm(alarmNumber);      // clear the specified alarm

        // digitalWrite(CLOCK_INTERRUPT_PIN, LOW);  // Set the interrupt pin LOW
        delay(200);

    rtc.writeSqwPinMode(DS3231_OFF);  // disable square wave output
    rtc.clearAlarm(alarmNumber);      // clear lingering flags
    Serial.print("    Interrupt pin state after clearing alarm: ");
    Serial.println(digitalRead(CLOCK_INTERRUPT_PIN));

    if (digitalRead( CLOCK_INTERRUPT_PIN) == HIGH ) {
      Serial.println("       -- ALARM cleared successfully."); } 
    else {
      Serial.println("       -- ERROR: Failed to clear alarm."); } 
  }
  else {
    Serial.println("Wrong parameter inputted for delete_alarm function in RTC_and_LCD.cpp");
  }
}

//------------------------------------------------------------------ALARM-FIRED-BOOL-------------------------------------------------------------------------------------------------

bool alarm_fired(void) {
  if (digitalRead( CLOCK_INTERRUPT_PIN) == HIGH ) {
    return 1; }
  else {
    return 0; }
}

//-------------------------------------------------------------------MISC------------------------------------------------------------------------------------------------

// RTC_and_LCD.cpp --- serial print that the alarm has occured. needed to attatch interupt to the alarm.
void on_alarm() {
    Serial.println("    -- ALARM occured!");
}



