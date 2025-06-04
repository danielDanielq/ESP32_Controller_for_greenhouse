#include <Arduino.h>

#define LIGHT_SENSOR_PIN 34  // TEMT6000 ↔ GPIO34

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Test TEMT6000 pe GPIO34");
}

void loop() {
  int lumina_raw = analogRead(LIGHT_SENSOR_PIN);

  // Convertire în procentaj
  float procent = (lumina_raw / 4095.0) * 100.0;
  float tensiune = (lumina_raw / 4095.0) * 3.3;
  float lux = tensiune / 3.3 * 30000.0; // Estimare lux (30000 lux la 3.3V)

  Serial.print("RAW: "); Serial.print(lumina_raw);
  Serial.print(" | %: "); Serial.print(procent, 1);
  Serial.print(" | Lux estimat: "); Serial.println(lux, 1);

  delay(500);
}
