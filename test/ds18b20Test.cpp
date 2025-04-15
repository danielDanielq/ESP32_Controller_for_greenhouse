#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 32  // Pinul unde sunt conectați senzorii

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Array pentru a stoca adresele senzorilor
DeviceAddress sensor1, sensor2;

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (i < 7) Serial.print(" ");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  sensors.begin();

  Serial.println("Caut senzori DS18B20...");

  int sensorCount = sensors.getDeviceCount();
  Serial.print("Senzori gasiti: ");
  Serial.println(sensorCount);

  if (sensorCount < 2) {
    Serial.println("Atentie: Mai putin de 2 senzori gasiti!");
  }

  // Ia adresele primilor 2 senzori gasiti
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

  // Setare rezolutie (optional)
  sensors.setResolution(sensor1, 12);
  sensors.setResolution(sensor2, 12);
}

void loop() {
  sensors.requestTemperatures();  // Trimite comanda de citire

  float temp1 = sensors.getTempC(sensor1);
  float temp2 = sensors.getTempC(sensor2);

  Serial.print("Temperatura senzor 1: ");
  Serial.print(temp1);
  Serial.println(" °C");

  Serial.print("Temperatura senzor 2: ");
  Serial.print(temp2);
  Serial.println(" °C");

  Serial.println("---------------------");
  delay(1000);
}
