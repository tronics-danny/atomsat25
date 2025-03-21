// Includes
#include <LoRa.h>  
#include <SPI.h>
#include "gs_defs.h"
#include "gs_lora.h"

//a variable declaration
byte msgCount = 0;

void initLoRa(){
  //set up lora module
  LoRa.setPins(csPin, rstPin, irqPin);

  Serial.println("LoRa sender Test");

  if (!LoRa.begin(433E6)){
    Serial.println(
      "Starting LoRa failed!"
    );
    while(1)
          ;
  }    

}
void sendPkt(){
    Serial.print("Sending packet: ");
    Serial.println(msgCount);

    // Send packet
    LoRa.beginPacket();
    LoRa.print("Packet ");
    LoRa.print(msgCount);
    LoRa.endPacket();

    // Increment packet counter
    msgCount++;

    // 5-second delay
    delay(5000);


}