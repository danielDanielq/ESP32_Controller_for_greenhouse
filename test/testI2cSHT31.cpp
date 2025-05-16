#include "Configuration.h"
#include "comands.h"
#include "relay.h"
#include "tcp_server.h"
#include "wifi_manager.h"
#include "WifiConnection.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Adafruit_SHT31.h"

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\nI2C Scanner");

  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("I2C device found at address 0x");
      Serial.println(addr, HEX);
    }
  }
}

void loop() {}
