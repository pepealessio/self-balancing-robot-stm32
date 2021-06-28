#include "BluetoothSerial.h"
#include "analogWrite.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define BUZ_PIN (23)

BluetoothSerial SerialBT;
uint8_t control_byte;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("SBLGruppo2"); //Bluetooth device name
}

void loop() {
  if (SerialBT.available()) {
    control_byte = (uint8_t) SerialBT.read()   ; 
    Serial.write(control_byte);

    if (control_byte & 0x80) {
      analogWrite(BUZ_PIN, 127, 987.77);
    } else {
      analogWrite(BUZ_PIN, 0, 987.77);
    }
  }
}

