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

extern bool macara1Active;
extern unsigned long macara1StartTime;

extern bool macara2Active;
extern unsigned long macara2StartTime;

extern float litriConsumati;
extern float litriSetati;

extern float pragTempLaterale;
extern float pragTempVentil;
extern bool modAuto;

extern bool irigareInDesfasurare;

extern void deschideMacara1();
extern void inchideMacara1();
extern void deschideMacara2();
extern void inchideMacara2();

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

        if (comanda == "ON1") {
          litriSetati = valoareStr.toFloat();  // conversie text în float
          startRelay(1);
          litriConsumati = 0.0;
          irigareInDesfasurare = true;

          #if ENABLE_SERIAL_PRINT == 1
            client.println("RELAY1 ON - " + String(litriSetati) + " litri");
            Serial.println("Irigare cu " + String(litriSetati) + " litri pe ON1");
          #endif
          }else if (comanda == "AUTO") {
          modAuto = (valoareStr.toInt() == 1);
          #if ENABLE_SERIAL_PRINT == 1
            client.println("MOD AUTO setat la: " + String(modAuto));
            Serial.println("Mod auto setat la: " + String(modAuto));
          #endif

        } else if (comanda == "TEMP_LATERALE") {
          pragTempLaterale = valoareStr.toFloat();
          #if ENABLE_SERIAL_PRINT == 1
            client.println("Prag TEMP_LATERALE: " + String(pragTempLaterale));
            Serial.println("Prag temperatura laterală setat la: " + String(pragTempLaterale));
          #endif

        } else if (comanda == "TEMP_VENTIL") {
          pragTempVentil = valoareStr.toFloat();
          #if ENABLE_SERIAL_PRINT == 1
            client.println("Prag TEMP_VENTIL: " + String(pragTempVentil));
            Serial.println("Prag temperatura ventilator setat la: " + String(pragTempVentil));
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
        deschideMacara1();
        #if ENABLE_SERIAL_PRINT == 1
          client.println("MACARA 1 DESCHIDERE");
        #endif

      } else if (mesaj == "OFF2") {
        inchideMacara1();
        #if ENABLE_SERIAL_PRINT == 1
          client.println("MACARA 1 INCHIDERE");
        #endif

      } else if (mesaj == "ON3") {
        deschideMacara2();
        #if ENABLE_SERIAL_PRINT == 1
          client.println("MACARA 2 DESCHIDERE");
        #endif

      } else if (mesaj == "OFF3") {
        inchideMacara2();
        #if ENABLE_SERIAL_PRINT == 1
          client.println("MACARA 2 INCHIDERE");
        #endif

      } else if (mesaj == "ON4") {
        startRelay(4);

        #if ENABLE_SERIAL_PRINT == 1
          client.println("VENTILATOARE ON");
        #endif

      } else if (mesaj == "OFF4") {
        stopRelay(4);

        #if ENABLE_SERIAL_PRINT == 1
          client.println("VENTILATOARE OFF");
        #endif

      } else if (mesaj == "RESET_LITRI"){
        litriConsumati = 0.0;
        
        #if ENABLE_SERIAL_PRINT == 1
          client.println("LITRI_RESETAȚI");
          Serial.println("Litri consumați resetați la 0");
        #endif
      }
      
      
      
      else {

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
