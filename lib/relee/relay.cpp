#include "relay.h"
#include "Configuration.h"

void initRelay() {
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

  void startRelay(int nr) {
    switch (nr) {
      case 1: digitalWrite(RELAY_1, LOW); break;
      case 2: digitalWrite(RELAY_2, LOW); break;
      case 3: digitalWrite(RELAY_3, LOW); break;
      case 4: digitalWrite(RELAY_4, LOW); break;
    }
  }

  void stopRelay(int nr) {
    switch (nr) {
      case 1: digitalWrite(RELAY_1, HIGH); break;
      case 2: digitalWrite(RELAY_2, HIGH); break;
      case 3: digitalWrite(RELAY_3, HIGH); break;
      case 4: digitalWrite(RELAY_4, HIGH); break;
    }
  }

  