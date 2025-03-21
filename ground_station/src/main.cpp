#include <Arduino.h>
#include "gs_defs.h"
#include "gs_lora.h"

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial);

  Serial.println("Starting Ground Station LoRa Test...");
  
  initLoRa();  // Initialize the LoRa module
}

void loop() {
  sendPkt();   // Send a LoRa packet
  delay(5000); // Wait 5 seconds before next packet
}
