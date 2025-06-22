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
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Adafruit_SHT31.h"

// DS18B20 senzori:
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// Array pentru a stoca adresele senzorilor
DeviceAddress sensor1, sensor2;

// === SHT31 ===
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void updateFlowRate();
int readSoilMoistureAvg();
float readLightSensor();
String getSensorData();
void verificaIrigare();


// Debit senzor:
volatile int flowPulseCount = 0;
unsigned long lastFlowTime = 0;
float flowRateLMin = 0.0;

void IRAM_ATTR pulseCounter() {
  flowPulseCount++;
}

// soil moisture sensor
const int DRY_VALUE = 3200;
const int WET_VALUE = 1200;


float litriSetati = 0.0;
float temperatura1 = 0.0;
float temperatura2 = 0.0;
float tempSHT = 0.0f;
float humSHT = 0.0f;
int umiditateSol = 0;
float luminaLux = 0.0f;
float litriConsumati = 0.0;
float pragTempLaterale = 0.0;
float pragTempVentil = 0.0; 
float temperaturaMedie = 0.0f;
bool modAuto = false; 

bool irigareInDesfasurare = false;

bool macara1Active = false;
unsigned long macara1StartTime = 0;
bool macara2Active = false;
unsigned long macara2StartTime = 0;

const float histerezisVentil = 1.0;
static bool ventilPorni = false;

#if ENABLE_SERIAL_PRINT_DS18B20 == 1
  void printAddress(DeviceAddress deviceAddress) {
    for (uint8_t i = 0; i < 8; i++) {
      if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
      if (i < 7) Serial.print(" ");
    }
  }
#endif


// === Functii ===
void updateFlowRate() {
  int pulses;
  unsigned long currentMillis = millis();
  if (currentMillis < lastFlowTime) {
    // Overflow millis
    lastFlowTime = currentMillis;
    noInterrupts();
    flowPulseCount = 0;
    interrupts();
  }
  
  if (currentMillis - lastFlowTime >= 1000) {
    noInterrupts();
    pulses = flowPulseCount;
    flowPulseCount = 0;
    interrupts();

    // YF-DN40: ~450 pulses per liter
    flowRateLMin = (pulses / 450.0) * 60.0;
    float litriInSecunda = (pulses / 450.0); // litri/secundă (pulsuri pe 1 sec)

    litriConsumati += litriInSecunda; // acumulează litri consumați

    lastFlowTime = currentMillis;
  }
}


int readSoilMoistureAvg() {
  int suma = 0;
  for (int i = 0; i < 10; i++) {
    suma += analogRead(SOIL_MOISTURE_PIN);
    delay(5);
  }
  return suma / 10;
}

float readLightSensor() {
  int lumina_raw = analogRead(TEMT6000_PIN);
  // Convertire în procentaj
  float procent = (lumina_raw / 4095.0) * 100.0;
  float tensiune = (lumina_raw / 4095.0) * 3.3;
  float lux = tensiune / 3.3 * LUX_MAX; // Estimare lux (50000 lux la 3.3V)
  
  #if ENABLE_SERIAL_PRINT == 1
    Serial.print("Lumina RAW: "); Serial.print(lumina_raw);
    Serial.print(" | %: "); Serial.print(procent, 1);
    Serial.print(" | Lux estimat: "); Serial.println(lux, 1);
  #endif
  
  return static_cast<int>(lux);
}

String getSensorData() {
  return String(temperatura1, 1) + "," +
         String(temperatura2, 1) + "," +
         String(tempSHT, 1) + "," +
         String(humSHT, 1) + "," +
         String(flowRateLMin, 1) + "," +
         String(umiditateSol) + "," +
         String(luminaLux, 0)+ "," +
         String(litriConsumati, 2);
}

void verificaIrigare() {
  if (irigareInDesfasurare) {
    if (litriConsumati >= litriSetati) {
      stopRelay(1);
      irigareInDesfasurare = false;

      #if ENABLE_SERIAL_PRINT == 1
        Serial.println("✅ Irigare oprită automat (litri atinsi)");
      #endif
      
    }
  }
}

void deschideMacara1() {
  analogWrite(RPWM1, 0);
  analogWrite(LPWM1, 255);
  macara1Active = true;
  macara1StartTime = millis();
}

void inchideMacara1() {
  analogWrite(RPWM1, 255);
  analogWrite(LPWM1, 0);
  macara1Active = true;
  macara1StartTime = millis();
}

void deschideMacara2() {
  analogWrite(RPWM2, 255);
  analogWrite(LPWM2, 0);
  macara2Active = true;
  macara2StartTime = millis();
}

void inchideMacara2() {
  analogWrite(RPWM2, 0);
  analogWrite(LPWM2, 255);
  macara2Active = true;
  macara2StartTime = millis();
}


void setup() {
  Serial.begin(115200);
  delay(1000);

  // Drivere
  pinMode(RPWM1, OUTPUT);
  pinMode(LPWM1, OUTPUT);
  pinMode(RPWM2, OUTPUT);
  pinMode(LPWM2, OUTPUT);

  // Limitatoare
  pinMode(LIMITATOR_SUS1, INPUT_PULLUP);
  pinMode(LIMITATOR_JOS1, INPUT_PULLUP);
  pinMode(LIMITATOR_SUS2, INPUT_PULLUP);
  pinMode(LIMITATOR_JOS2, INPUT_PULLUP);

  sensors.begin();

  if (!sht31.begin(0x44)) {
    Serial.println("SHT31 nu a fost detectat!");
  }

  #if ENABLE_SERIAL_PRINT_DS18B20 == 1
    Serial.println("Caut senzori DS18B20...");

    int sensorCount = sensors.getDeviceCount();
    Serial.print("Senzori gasiti: ");
    Serial.println(sensorCount);

    if (sensorCount < 2) {
      Serial.println("Atentie: Mai putin de 2 senzori gasiti!");
    }

    if (!sensors.getAddress(sensor1, 0)) Serial.println("Nu pot citi adresa senzorului 1.");
    else {
      Serial.print("Senzor 1: ");
      printAddress(sensor1);
      Serial.println();
    }

    if (!sensors.getAddress(sensor2, 1)) Serial.println("Nu pot citi adresa senzorului 2.");
    else {
      Serial.print("Senzor 2: ");
      printAddress(sensor2);
      Serial.println();
    }
  #endif
    
  // Setare rezolutie (optional)
  sensors.setResolution(sensor1, 12);
  sensors.setResolution(sensor2, 12);
  

  // Debimetru
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);  // cu pullup intern
  
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);


  connectToWiFi(DEBUG_WIFI_SSID, DEBUG_WIFI_PASSWORD);
  initRelay();
  initTCPServer();
}


void loop() {

  handleTCPClient();
  verificaIrigare();

  // Senzor SHT31
  tempSHT = sht31.readTemperature();
  humSHT = sht31.readHumidity();
  
  #if ENABLE_SERIAL_PRINT_SHT31 == 1
    Serial.print("SHT31 - Temp: ");
    Serial.print(tempSHT);
    Serial.print("°C | Umiditate: ");
    Serial.print(humSHT);
    Serial.println("%");
  #endif

  // Umiditate sol
  int soilRaw = readSoilMoistureAvg();
  umiditateSol = map(soilRaw, DRY_VALUE, WET_VALUE, 0, 100);
  umiditateSol = constrain(umiditateSol, 0, 100);

  #if ENABLE_SERIAL_PRINT_SOIL == 1
    Serial.print("Umiditate sol: ");
    Serial.print(umiditateSol);
    Serial.print("% (RAW: ");
    Serial.print(soilRaw);
    Serial.println(")");
  #endif


  // Senzori DS18B20
  sensors.requestTemperatures();  // Trimite comanda de citire

  temperatura1 = sensors.getTempC(sensor1);
  temperatura2 = sensors.getTempC(sensor2);

  #if ENABLE_SERIAL_PRINT_DS18B20 == 1
    Serial.print("Temperatura senzor 1: ");
    Serial.print(temperatura1);
    Serial.println(" °C");

    Serial.print("Temperatura senzor 2: ");
    Serial.print(temperatura2);
    Serial.println(" °C");

    Serial.println("---------------------");
    delay(500);
  #endif

  // Senzor de lumina TEMT6000
  luminaLux = readLightSensor();
  #if ENABLE_SERIAL_PRINT_TEMT6000 == 1
    Serial.print("Lumina (Lux): ");
    Serial.println(luminaLux);
  #endif

  // MOD AUTOMAT
  if (modAuto && !isnan(tempSHT) && pragTempLaterale > 0.0f && pragTempVentil > 0.0f) {
    temperaturaMedie = tempSHT;

    // CONTROL LATERALE - ambele macarale
    if ((temperaturaMedie >= pragTempLaterale) && !macara1Active && !macara2Active) {
      deschideMacara1();
      deschideMacara2();
    } else if (temperaturaMedie <= (pragTempLaterale - histerezisVentil) && macara1Active && macara2Active) {
      // Inchide Macara 1
      inchideMacara1();
      inchideMacara2();
    }

    // CONTROL VENTILATIE
    if (temperaturaMedie >= pragTempVentil && !ventilPorni) {
      startRelay(4);  // Ventilatoare ON
      ventilPorni = true;
      
      #if ENABLE_SERIAL_PRINT == 1
        Serial.println("Auto: Ventilatoare activate");
      #endif
    } else if (temperaturaMedie <= (pragTempVentil - histerezisVentil) && ventilPorni) {
      stopRelay(4);   // Ventilatoare OFF
      ventilPorni = false;

      #if ENABLE_SERIAL_PRINT == 1
        Serial.println("Auto: Ventilatoare oprite");
      #endif
    }
  }


  // Debimetru
  updateFlowRate(); //se aplica din secunda in secunda

  #if ENABLE_SERIAL_PRINT_YFdn40 == 1
    Serial.print("litri setati: ");
    Serial.print(litriSetati);
    Serial.println(" L");
    Serial.print("Debit: ");
    Serial.print(flowRateLMin);
    Serial.println(" L/min");
    delay(500);
  #endif
  
  // Verificare limitatoare pentru macarale
  if(macara1Active) {
    if (digitalRead(LIMITATOR_SUS1) == LOW || digitalRead(LIMITATOR_JOS1) == LOW) {
      analogWrite(RPWM1, 0);
      analogWrite(LPWM1, 0);
      digitalWrite(RPWM1, LOW);
      digitalWrite(LPWM1, LOW);
      macara1Active = false; // Oprire macara 1 automat

      #if ENABLE_SERIAL_PRINT_MACARA == 1
        Serial.println("Macara 1 oprit automat - limitator activ.");
      #endif
    } 
    // Timeout de siguranță
    else if (millis() - macara1StartTime > SAFETY_TIMEOUT){
      analogWrite(RPWM1, 0);
      analogWrite(LPWM1, 0);
      macara1Active = false;

      #if ENABLE_SERIAL_PRINT_MACARA == 1
        Serial.println("Macara 1 oprită - SAFETY timeout!");
      #endif
    }
  }

  if (macara2Active) {
    if (digitalRead(LIMITATOR_SUS2) == LOW || digitalRead(LIMITATOR_JOS2) == LOW) {
      analogWrite(RPWM2, 0);
      analogWrite(LPWM2, 0);
      macara2Active = false;
      #if ENABLE_SERIAL_PRINT_MACARA == 1
        Serial.println("Macara 2 oprit automat - limitator activ.");
      #endif
    }
    // Timeout de siguranță
    else if (millis() - macara2StartTime > SAFETY_TIMEOUT) {
      analogWrite(RPWM2, 0);
      analogWrite(LPWM2, 0);
      macara2Active = false;
      #if ENABLE_SERIAL_PRINT_MACARA == 1
        Serial.println("Macara 2 oprită - SAFETY timeout!");
      #endif
    }
  }

}
