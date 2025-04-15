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

#include "wifi_manager.h"

void connectToWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);

  #if ENABLE_SERIAL_PRINT == 1
    Serial.print("Conectare la WiFi");
  #endif

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

    #if ENABLE_SERIAL_PRINT == 1
      Serial.print(".");
    #endif
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #if ENABLE_SERIAL_PRINT == 1
      Serial.print(".");
    #endif
  }
  
  #if ENABLE_SERIAL_PRINT == 1
    Serial.println("\nConectat la WiFi!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  #endif
  
}
