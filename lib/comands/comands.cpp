#include "comands.h"
#include "relay.h"
#include "Configuration.h"


void proceseazaComanda(String comanda, WiFiClient& client) {
  if (comanda == "RELEU1_ON") {
    startRelay(1);
    client.println("RELEU1 pornit");
  } else if (comanda == "RELEU1_OFF") {
    stopRelay(1);
    client.println("RELEU1 oprit");
  }
  // Extinde cu celelalte relee
  else {
    client.println("Comanda necunoscuta");
  }
}