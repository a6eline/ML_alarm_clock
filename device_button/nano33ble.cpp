
//=========================================================== nano33ble.cpp ====================================================================

  //-------------------------FILE-INFO-------------------------------
    // wireless connection functions for the nano 33 ble board

#include "nano33ble.h "

void SetupBLE() {
  // initialize the Bluetooth® Low Energy hardware
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    return 1;
  }
  Serial.println("Bluetooth® Low Energy Central - LED control");
  // start scanning for peripherals
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}