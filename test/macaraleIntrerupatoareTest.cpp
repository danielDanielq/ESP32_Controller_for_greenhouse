#include <Arduino.h>

// === Limitatoare ===
#define LIMITATOR_SUS1 18 // l1
#define LIMITATOR_JOS1 25 // l2
#define LIMITATOR_SUS2 17 // l3
#define LIMITATOR_JOS2 23 // l4


// === Motoare ===
#define RPWM1 4
#define LPWM1 5
#define RPWM2 16
#define LPWM2 19

void setup() {
  Serial.begin(115200);
  Serial.println("Test motoare + limitatoare. Introdu 1-4 in Serial Monitor.");

  // Limitatoare
  pinMode(LIMITATOR_SUS1, INPUT_PULLUP);
  pinMode(LIMITATOR_JOS1, INPUT_PULLUP);
  pinMode(LIMITATOR_SUS2, INPUT_PULLUP);
  pinMode(LIMITATOR_JOS2, INPUT_PULLUP);

  // Motoare
  pinMode(RPWM1, OUTPUT);
  pinMode(LPWM1, OUTPUT);
  pinMode(RPWM2, OUTPUT);
  pinMode(LPWM2, OUTPUT);

  // Oprire inițială
  analogWrite(RPWM1, 0);
  analogWrite(LPWM1, 0);
  analogWrite(RPWM2, 0);
  analogWrite(LPWM2, 0);
}

void loop() {
  // Citire limitatoare
  if (digitalRead(LIMITATOR_SUS1) == LOW) Serial.println("Macara 1 SUS - LIMITATOR ACTIVAT");
  if (digitalRead(LIMITATOR_JOS1) == LOW) Serial.println("Macara 1 JOS - LIMITATOR ACTIVAT");
  if (digitalRead(LIMITATOR_SUS2) == LOW) Serial.println("Macara 2 SUS - LIMITATOR ACTIVAT");
  if (digitalRead(LIMITATOR_JOS2) == LOW) Serial.println("Macara 2 JOS - LIMITATOR ACTIVAT");

  // Verificare comandă din Serial Monitor
  if (Serial.available()) {
    char comanda = Serial.read();

    // Oprire motoare înainte de orice
    analogWrite(RPWM1, 0);
    analogWrite(LPWM1, 0);
    analogWrite(RPWM2, 0);
    analogWrite(LPWM2, 0);

    switch (comanda) {
      case '1':
        Serial.println("Motor 1: STANGA");
        analogWrite(RPWM1, 200);
        analogWrite(LPWM1, 0);
        break;

      case '2':
        Serial.println("Motor 1: DREAPTA");
        analogWrite(RPWM1, 0);
        analogWrite(LPWM1, 200);
        break;

      case '3':
        Serial.println("Motor 2: STANGA");
        analogWrite(RPWM2, 200);
        analogWrite(LPWM2, 0);
        break;

      case '4':
        Serial.println("Motor 2: DREAPTA");
        analogWrite(RPWM2, 0);
        analogWrite(LPWM2, 200);
        break;

      default:
        Serial.println("Comanda necunoscuta. Foloseste 1, 2, 3 sau 4.");
        break;
    }

    delay(2000); // Rulare 2 secunde
    // Oprire după mișcare
    analogWrite(RPWM1, 0);
    analogWrite(LPWM1, 0);
    analogWrite(RPWM2, 0);
    analogWrite(LPWM2, 0);
    Serial.println("STOP\n");
  }

  delay(100);
}
