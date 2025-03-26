#ifndef GS_DEFS_H
#define GS_DEFS_H
#include <Arduino.h>

// Pin definitions for all components connected to the esp32 in the ground station

//LoRa Module Pins
#define csPin 5
#define rstPin 14
#define irqPin 2


//Keypad pins
//Row pins
#define R1 12
#define R2 13
#define R3 25
#define R4 26

//Column pins
#define C1 27
#define C2 32
#define C3 33
#define C4 15


//Serial Communication definitions
#define SERIAL_BAUD_RATE 9600

#endif