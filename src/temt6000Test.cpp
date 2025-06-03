#include <Arduino.h>
// TEMT6000 Light Sensor Test

#define LIGHT_SENSOR_PIN 34  // TEMT6000 OUT ↔ GPIO34

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Test TEMT6000 pe GPIO34");
}

void loop() {
  int lumina_raw = analogRead(LIGHT_SENSOR_PIN);
  
  Serial.print("Lumina (RAW): ");
  Serial.println(lumina_raw);  // Valori între ~0 și 4095
  
  delay(500);
}
