#ifndef GS_LORA_MODULE_H
#define GS_LORA_MODULE_H
#include <Arduino.h>

void initLoRa();
void transmitPkt(String payload);

#endif