#ifndef SAT_DEFS_H
#define SAT_DEFS_H
#include <Arduino.h>

// Pin definitions for all components connected to the esp32 in the nanosat

//LoRa Module Pins
#define csPin 5
#define rstPin 14
#define irqPin 2

//DHT 11 pin
#define DHTPIN 13 //Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

//Debugging LED
#define ledPin 12

//Serial Communication definition
#define SERIAL_BAUD_RATE 115200

#endif