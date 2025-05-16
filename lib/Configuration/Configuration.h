#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>
#include <WiFi.h>

#define RELAY_1 13 // -> A1 -> in1 =irigation
#define RELAY_2 26 // -> A2 -> in2 =left crane
#define RELAY_3 14 // -> A3 -> in3 =right crane
#define RELAY_4 27 // -> A4 -> in4 =ventilation

#define ONE_WIRE_BUS 32 // x2 DS18B20

#define FLOW_SENSOR_PIN 33 // flow sensor
extern int litri;

#define SOIL_MOISTURE_PIN 39 // soil moisture sensor


#define ENABLE_SERIAL_PRINT 1
#define ENABLE_SERIAL_PRINT_DS18B20 1
#define ENABLE_SERIAL_PRINT_YFdn40 0
#define ENABLE_SERIAL_PRINT_SOIL 0
#define ENABLE_SERIAL_PRINT_SHT31 1

#endif