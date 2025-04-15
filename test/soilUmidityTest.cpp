#include <Arduino.h>
#include <Wire.h>

#define SOIL_MOISTURE_PIN 39  // A0, GPIO39

// Calibrare (ajustează după caz)
const int DRY_VALUE = 3200;   // valoarea citită când senzorul este complet uscat
const int WET_VALUE = 1200;   // valoarea când e complet în apă / umed

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Test senzor umiditate sol - ESP32");
}

int readSoilMoistureAvg(int pin) {
  int suma = 0;
  const int NUM_READS = 10;

  for (int i = 0; i < NUM_READS; i++) {
    suma += analogRead(pin);
    delay(10);
  }

  return suma / NUM_READS;
}

void loop() {
  int moistureRaw = readSoilMoistureAvg(SOIL_MOISTURE_PIN);

  // Transformă în procent între 0-100%
  int moisturePercent = map(moistureRaw, DRY_VALUE, WET_VALUE, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  Serial.print("Umiditate sol (RAW): ");
  Serial.print(moistureRaw);
  Serial.print(" | Procent: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  delay(2000); // Așteaptă 2 secunde între citiri
}
