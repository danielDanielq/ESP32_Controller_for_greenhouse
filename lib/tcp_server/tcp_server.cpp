/*
* Copyright 2025 Daniel Gheorghe Sarca
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*   http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "tcp_server.h"
#include "Configuration.h"
#include "WifiConnection.h"
#include "relay.h"

WiFiServer tcpServer(DEBUG_HOST_PORT);
WiFiClient client;

extern String getSensorData();

void initTCPServer() {
  WiFi.softAP(DEBUG_WIFI_SSID, DEBUG_WIFI_PASSWORD);
  tcpServer.begin();

  #if ENABLE_SERIAL_PRINT == 1
    Serial.println("Access Point activ: " + WiFi.softAPIP().toString());
  #endif

}

void handleTCPClient() {
  if (!client || !client.connected()) {
    client = tcpServer.available();
    return;
  }
  
  
  if (client && client.connected()) {
    if (client.available()) {
      String mesaj = client.readStringUntil('\n');
      mesaj.trim(); // elimină \n, \r etc.

      #if ENABLE_SERIAL_PRINT == 1
        Serial.print("Mesaj: ");
        Serial.println(mesaj);

        // răspuns
        client.println("Am primit: " + mesaj);
      #endif

      int separatorIndex = mesaj.indexOf(':');
      if (separatorIndex != -1) {
        String comanda = mesaj.substring(0, separatorIndex);
        String valoareStr = mesaj.substring(separatorIndex + 1);
        litri = valoareStr.toInt();  // conversie text în int

        if (comanda == "ON1") {
          startRelay(1);
          // TODO: Folosește `litri` pentru a controla durata irigării
          // De exemplu: calculează timpul necesar bazat pe debitul senzorului
          #if ENABLE_SERIAL_PRINT == 1
            client.println("RELAY1 ON - " + String(litri) + " litri");
            Serial.println("Irigare cu " + String(litri) + " litri pe ON1");
          #endif
        }

        // Putem extinde și pentru ON2, ON3 etc., la fel
      }


      if (mesaj == "SEND DATA") {
        String data = getSensorData();
        client.println(data); // Trimite datele către client
        #if ENABLE_SERIAL_PRINT == 1
          Serial.println("Date trimise: " + data);
        #endif
      } else if (mesaj == "OFF1") {
        stopRelay(1);

        #if ENABLE_SERIAL_PRINT == 1
          client.println("RELAY1 OFF");
        #endif

      } else if (mesaj == "ON2") {
        startRelay(2);

        #if ENABLE_SERIAL_PRINT == 1
          client.println("RELAY2 ON");
        #endif

      } else if (mesaj == "OFF2") {
        stopRelay(2);

        #if ENABLE_SERIAL_PRINT == 1
          client.println("RELAY2 OFF");
        #endif

      } else if (mesaj == "ON3") {
        startRelay(3);

        #if ENABLE_SERIAL_PRINT == 1
          client.println("RELAY3 ON");
        #endif

      } else if (mesaj == "OFF3") {
        stopRelay(3);
        
        #if ENABLE_SERIAL_PRINT == 1
          client.println("RELAY3 OFF");
        #endif

      } else if (mesaj == "ON4") {
        startRelay(4);

        #if ENABLE_SERIAL_PRINT == 1
          client.println("RELAY4 ON");
        #endif

      } else if (mesaj == "OFF4") {
        stopRelay(4);

        #if ENABLE_SERIAL_PRINT == 1
          client.println("RELAY4 OFF");
        #endif

      } else {

        #if ENABLE_SERIAL_PRINT == 1
          client.println("Comandă necunoscută");
        #endif

      }
    }
  }
  //client.stop();
  // #if ENABLE_SERIAL_PRINT == 1
  //   Serial.println("Client disconnected");
  // #endif
}

void sendPong() {
  unsigned long lastPing = 0;

  if (millis() - lastPing > 3000) {
    client.print("PONG\n");
    lastPing = millis();
  }
}
