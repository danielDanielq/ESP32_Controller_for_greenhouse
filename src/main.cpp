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

#include "Configuration.h"
#include "comands.h"
#include "relay.h"
#include "tcp_server.h"
#include "wifi_manager.h"
#include "WifiConnection.h"

void setup() {
  Serial.begin(115200);
  connectToWiFi(DEBUG_WIFI_SSID, DEBUG_WIFI_PASSWORD);
  initRelay();
  initTCPServer();

}

void loop() {
  handleTCPClient();
}
