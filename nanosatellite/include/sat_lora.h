#ifndef LORA_MODULE_H
#define LORA_MODULE_H
#include <Arduino.h>

void initLoRa();
void sendMessage(String outgoing);
void processReceivedCmd(String incoming);
void onReceive(int packetSize);


#endif
