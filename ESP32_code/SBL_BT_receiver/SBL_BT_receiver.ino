#include "BluetoothSerial.h"
#include "analogWrite.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define BUZ_PIN (23)

BluetoothSerial SerialBT;
uint8_t control_byte;
uint8_t i;


void setup() {
  Serial.begin(115200);
  SerialBT.begin("SBLGruppo2"); //Bluetooth device name

  i = 0;
}

void loop() {
  if (SerialBT.available()) {
    i = 0;
    control_byte = (uint8_t) SerialBT.read(); 

    if (control_byte & 0x80) {
      analogWrite(BUZ_PIN, 127, 987.77);
    } else {
      analogWrite(BUZ_PIN, 0, 987.77);
    }
  } else if (i++ == 15) {
    i = 0;
    control_byte = 0x00;
  }
  Serial.write(control_byte);
  delay(300);
}
