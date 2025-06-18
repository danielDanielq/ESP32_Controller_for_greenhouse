
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>
#include <WiFi.h>

#define RELAY_1 13 // -> A1 -> in1 =irigation
#define RELAY_2 26 // -> A2 -> in2 =left crane
#define RELAY_3 14 // -> A3 -> in3 =right crane
#define RELAY_4 27 // -> A4 -> in4 =ventilation

// Drivere
#define RPWM1 4
#define LPWM1 5
#define RPWM2 16
#define LPWM2 19

// Limitatoare
#define LIMITATOR_SUS1 18
#define LIMITATOR_JOS1 25
#define LIMITATOR_SUS2 17
#define LIMITATOR_JOS2 23

#define ONE_WIRE_BUS 32 // x2 DS18B20

#define FLOW_SENSOR_PIN 33 // flow sensor

#define SOIL_MOISTURE_PIN 39 // soil moisture sensor

#define TEMT6000_PIN 34 // TEMT6000 light sensor

#define ENABLE_SERIAL_PRINT 1
#define ENABLE_SERIAL_PRINT_DS18B20 1
#define ENABLE_SERIAL_PRINT_YFdn40 1
#define ENABLE_SERIAL_PRINT_SOIL 1
#define ENABLE_SERIAL_PRINT_SHT31 1
#define ENABLE_SERIAL_PRINT_TEMT6000 1
#define ENABLE_SERIAL_PRINT_MACARA 1

#endif