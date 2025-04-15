#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SHT31.h>

// === Senzor sol ===
#define SOIL_MOISTURE_PIN 39
const int DRY_VALUE = 3200;
const int WET_VALUE = 1200;

// === Relee ===
#define RELAY1_PIN 13
#define RELAY2_PIN 26
#define RELAY3_PIN 14
#define RELAY4_PIN 27

// === DS18B20 ===
#define ONE_WIRE_BUS 32
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);
DeviceAddress sensor1, sensor2;

// === SHT31 ===
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// === Debimetru YF-DN40 ===
#define FLOW_SENSOR_PIN 33
volatile int flowPulseCount = 0;
unsigned long lastFlowTime = 0;
float flowRateLMin = 0.0;

void IRAM_ATTR flowISR() {
  flowPulseCount++;
}

// === Setup ===
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Sistem test senzori + module");

  // Relee
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);
  digitalWrite(RELAY3_PIN, LOW);
  digitalWrite(RELAY4_PIN, LOW);

  // DS18B20
  ds18b20.begin();
  if (ds18b20.getDeviceCount() < 2) {
    Serial.println("Atentie: Mai putin de 2 senzori DS18B20 detectati!");
  }
  ds18b20.getAddress(sensor1, 0);
  ds18b20.getAddress(sensor2, 1);

  // SHT31
  if (!sht31.begin(0x44)) {
    Serial.println("SHT31 nu a fost detectat!");
  }

  // Debimetru
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), flowISR, RISING);
}

// === Functii ===
int readSoilMoistureAvg() {
  int suma = 0;
  for (int i = 0; i < 10; i++) {
    suma += analogRead(SOIL_MOISTURE_PIN);
    delay(5);
  }
  return suma / 10;
}

void updateFlowRate() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastFlowTime >= 1000) {
    noInterrupts();
    int pulses = flowPulseCount;
    flowPulseCount = 0;
    interrupts();

    // YF-DN40: ~450 pulses per liter
    flowRateLMin = (pulses / 450.0) * 60.0;
    lastFlowTime = currentMillis;
  }
}

void toggleRelays() {
  static bool state = false;
  state = !state;
  digitalWrite(RELAY1_PIN, state);
  digitalWrite(RELAY2_PIN, state);
  digitalWrite(RELAY3_PIN, state);
  digitalWrite(RELAY4_PIN, state);
}

// === Loop ===
void loop() {
  Serial.println("======================================");

  // Umiditate sol
  int soilRaw = readSoilMoistureAvg();
  int soilPercent = map(soilRaw, DRY_VALUE, WET_VALUE, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);
  Serial.print("Umiditate sol: ");
  Serial.print(soilPercent);
  Serial.print("% (RAW: ");
  Serial.print(soilRaw);
  Serial.println(")");

  // SHT31
  float tempSHT = sht31.readTemperature();
  float humSHT = sht31.readHumidity();
  Serial.print("SHT31 - Temp: ");
  Serial.print(tempSHT);
  Serial.print("°C | Umiditate: ");
  Serial.print(humSHT);
  Serial.println("%");

  // DS18B20
  ds18b20.requestTemperatures();
  Serial.print("DS18B20 #1: ");
  Serial.print(ds18b20.getTempC(sensor1));
  Serial.print("°C | DS18B20 #2: ");
  Serial.print(ds18b20.getTempC(sensor2));
  Serial.println("°C");

  // Debimetru
  updateFlowRate();
  Serial.print("Debit: ");
  Serial.print(flowRateLMin);
  Serial.println(" L/min");

  // Relee test
  toggleRelays();

  delay(3000);
}
