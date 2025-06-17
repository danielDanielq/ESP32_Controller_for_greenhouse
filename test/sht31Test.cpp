#include <Wire.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL pins for ESP32
  
  if (!sht31.begin(0x44)) { // 0x45 e adresa alternativă
    Serial.println("SHT31 not found!");
    while (1) delay(1);
  }
}

void loop() {
  float temp = sht31.readTemperature();
  float hum = sht31.readHumidity();

  if (!isnan(temp)) {
    Serial.print("Temp: "); Serial.print(temp); Serial.print(" °C");
    Serial.print(" | Hum: "); Serial.print(hum); Serial.println(" %");
  } else {
    Serial.println("Failed to read from SHT31");
  }

  delay(2000);
}
