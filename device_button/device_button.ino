
//=========================================================== device_button.ino ====================================================================

  //-------------------------FILE-INFO-------------------------------
    // this is the program for the remote button which will turn off the alarm

  //-------------------------OPERATIONS-------------------------------
    // ------------------SETUP-----------------
    // 

    // ------------------LOOP------------------
    // 

  //------------------------IMPORTANT-INFO----------------------------
    // 

//---------------------------------ESP32_ESP8266---------------------------------------

#if defined(ESP8266) || defined(ESP32)

#endif // defined(ESP8266) || defined(ESP32)

//---------------------------------ARDUINO_NANO33BLE---------------------------------------

#if defined(ARDUINO_ARDUINO_NANO33BLE)
  #include <ArduinoBLE.h>

  const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
  const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";
#endif // defined(ARDUINO_ARDUINO_NANO33BLE)

//=========================================================== setup() ====================================================================

void setup() {
  SerialSetup();
  ButtonSetup();

#if defined(ARDUINO_ARDUINO_NANO33BLE)

#endif // defined(ARDUINO_ARDUINO_NANO33BLE)

}

void loop() {
  // put your main code here, to run repeatedly:

}

