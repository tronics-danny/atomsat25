
#include <LoRa.h>  
#include <SPI.h>
#include "sat_lora.h"
#include "sat_defs.h"

void initLoRa() {
  //set up lora module
  LoRa.setPins(csPin, rstPin, irqPin);

  Serial.println("LoRa Receiver Test");

  if (!LoRa.begin(433E6)){
    Serial.println(
      "Starting LoRa failed!"
    );
    while(1)
    ;
  }
}

void receivePkt(){
  // Try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Received a packet
    Serial.print("Received '");
 
    // Read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
 
    // Print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

