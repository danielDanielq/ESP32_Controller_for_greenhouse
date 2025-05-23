#include <Arduino.h>
//ren, len pe 3,3ve
// Macara 1
#define RPWM1 4
#define LPWM1 0

// Macara 2
#define RPWM2 2
#define LPWM2 15

void setup() {
  // Setare pini ca output
  pinMode(RPWM1, OUTPUT);
  pinMode(LPWM1, OUTPUT);
  pinMode(RPWM2, OUTPUT);
  pinMode(LPWM2, OUTPUT);
}

void loop() {
  // Macara 1 merge înainte (ex: sus)
  analogWrite(RPWM1, 200);  // între 0 și 255 (viteza)
  analogWrite(LPWM1, 0);
  delay(2000);

  // Macara 1 stop
  analogWrite(RPWM1, 0);
  analogWrite(LPWM1, 0);
  delay(5000);

  // Macara 1 merge înapoi (ex: jos)
  analogWrite(RPWM1, 0);
  analogWrite(LPWM1, 200);
  delay(2000);

  // Stop
  analogWrite(RPWM1, 0);
  analogWrite(LPWM1, 0);
  delay(5000);

  // Macara 2 sus
  analogWrite(RPWM2, 200);
  analogWrite(LPWM2, 0);
  delay(2000);

  // Macara 2 jos
  analogWrite(RPWM2, 0);
  analogWrite(LPWM2, 200);
  delay(2000);

  // Stop
  analogWrite(RPWM2, 0);
  analogWrite(LPWM2, 0);
  delay(5000);
}
