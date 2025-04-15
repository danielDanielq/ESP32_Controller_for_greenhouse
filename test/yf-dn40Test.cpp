#include<Arduino.h>

volatile int pulseCount = 0;
unsigned long lastTime = 0;
float flowRate = 0;

#define FLOW_SENSOR_PIN 33 // modifică dacă folosești alt pin GPIO

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);  // cu pullup intern
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);

  Serial.println("Pornit test senzor YF-DN40...");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= 1000) { // calculează la fiecare 1 sec
    detachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN));

    // formula de conversie — poate varia în funcție de modelul senzorului
    // pentru YF-DN40 de obicei: 1L = 450 impulsuri
    flowRate = (pulseCount / 7.5);  // Litri/min (estimativ)
    
    Serial.print("Pulsuri pe secunda: ");
    Serial.print(pulseCount);
    Serial.print(" | Debit: ");
    Serial.print(flowRate);
    Serial.println(" L/min");

    pulseCount = 0;
    lastTime = currentTime;

    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
  }
}
