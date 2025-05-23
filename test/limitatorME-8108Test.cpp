#include <Arduino.h>
// Test pentru limitatoare de cursă

#define LIMITATOR_SUS1 5
#define LIMITATOR_JOS1 16
#define LIMITATOR_SUS2 17
#define LIMITATOR_JOS2 18

void setup() {
  Serial.begin(115200);

  pinMode(LIMITATOR_SUS1, INPUT_PULLUP);
  pinMode(LIMITATOR_JOS1, INPUT_PULLUP);
  pinMode(LIMITATOR_SUS2, INPUT_PULLUP);
  pinMode(LIMITATOR_JOS2, INPUT_PULLUP);
}

void loop() {
  // Citire stări
  bool sus1 = digitalRead(LIMITATOR_SUS1) == LOW;
  bool jos1 = digitalRead(LIMITATOR_JOS1) == LOW;
  bool sus2 = digitalRead(LIMITATOR_SUS2) == LOW;
  bool jos2 = digitalRead(LIMITATOR_JOS2) == LOW;

  if (sus1) Serial.println("Macara 1 sus - LIMITATOR ACTIVAT");
  if (jos1) Serial.println("Macara 1 jos - LIMITATOR ACTIVAT");
  if (sus2) Serial.println("Macara 2 sus - LIMITATOR ACTIVAT");
  if (jos2) Serial.println("Macara 2 jos - LIMITATOR ACTIVAT");

  delay(200); // mic delay ca să nu inunde serialul
}
