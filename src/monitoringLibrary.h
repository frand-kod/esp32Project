// monitoringLibrary.h

#ifndef monitoringLibrary
#define monitoringLibrary

#include <Arduino.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <NewPingESP8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Setup
// Pin Setup
#define DHTPIN 2
#define DHTTYPE DHT11
extern DHT dht11;

#define DS_PIN 22
extern OneWire oneWire;
extern DallasTemperature sensors;

#define TRIG_PIN 18
#define ECHO_PIN 19
extern NewPingESP8266 sonar;

#define SDA_PIN 21
#define SCL_PIN 22
extern LiquidCrystal_I2C lcd;

#define LED_YELLOW 13 // LED Yellow
#define LED_GREEN 11  // LED Green

#define RELAYPUMP_PIN 4 // Relay untuk Pompa Air
#define BUZZER_PIN 5    // Buzzer

// Deklarasi fungsi setup
void setupSerial();
void setupSensordht();
void setupLcd_I2C();
void setupSensorDallas();
void setupLed();
void setupPump();

int dhtProvider(String ValueOption);
int dsTemp();
int distanceSensor();
void displayText(int line, int column, const char *text);
void kontrolPompa(int temp, int treshold);

#endif // monitoringLibrary
