#include <Arduino.h>

#define RELAY_1 13 // Pinul la care este conectat releul
#define RELAY_2 26 // Pinul la care este conectat releul
#define RELAY_3 14 // Pinul la care este conectat releul
#define RELAY_4 27 // Pinul la care este conectat releul

#define RELAY_ON LOW // Starea pentru a activa releul (LOW sau HIGH)    



void setup() {
  Serial.begin(115200);

  // Setăm pinii ca OUTPUT
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  // Dezactivăm releele la start (HIGH = OFF)
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);

}

void loop() {
  Serial.println("Activam releul 1...");
    digitalWrite(RELAY_1, LOW);  // ON
    delay(1000);
    digitalWrite(RELAY_1, HIGH); // OFF
    delay(500);

    Serial.println("Activam releul 2...");
    digitalWrite(RELAY_2, LOW);
    delay(1000);
    digitalWrite(RELAY_2, HIGH);
    delay(500);

    Serial.println("Activam releul 3...");
    digitalWrite(RELAY_3, LOW);
    delay(1000);
    digitalWrite(RELAY_3, HIGH);
    delay(500);

    Serial.println("Activam releul 4...");
    digitalWrite(RELAY_4, LOW);
    delay(1000);
    digitalWrite(RELAY_4, HIGH);
    delay(500);
}
