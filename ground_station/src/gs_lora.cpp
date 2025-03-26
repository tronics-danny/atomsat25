// Includes
#include <LoRa.h>  
#include <SPI.h>
#include "gs_defs.h"
#include "gs_lora.h"
#include "gs_sharedVar.h"
#include "keypad_handler.h"


// Source and destination addresses
byte cubeAddress = 0xBB;               // address of this device the nanosat
byte gsAddress = 0xCC;                  // adress of the GS in this case will be the receipient
byte broadCastAdress = 0xFF;           // This broadcasts the message

void initLoRa(){
  //set up lora module
  LoRa.setPins(csPin, rstPin, irqPin);

  Serial.println("LoRa sender Test");

  if (!LoRa.begin(433E6)){
    Serial.println(
      "Starting LoRa failed!"
    );
    while(1);
  }    
}


void transmitPkt(String payload){
  Serial.print("Sending packet: ");
  Serial.println(payload);
  
  // Format of the message to be send: <destination><source><length><payload>
  LoRa.beginPacket();

  LoRa.write(cubeAddress);              // destination: CubeSat
  LoRa.write(gsAddress);                // source: Ground Station
  /*
  Here now we get a command input from the keypad which is now then transmitted via lora
  well call a function from the keypad.cpp that gives us the keypad input
  */
  LoRa.write(payload.length());         // Take the length of the payload and include it in the packet beiing send 
  LoRa.print(payload);                  // now transmit the payload
  
  LoRa.endPacket();

}

void onReceive(int packetSize) {
  if (packetSize == 0) return;

  // Receive the accompanying data packets with the message
  byte recipient = LoRa.read();         // Get the Recipeint address from the received message
  byte sender = LoRa.read();            // Get the sender address from the received message
  byte length = LoRa.read();            // Get the message length of the payload

  //Stores the received message into a string variable incoming
  String incoming = "";

  //Now read the data that was transmitted
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  /*
  Check if the message received is intented for that particular receiver node 
  or if its a broadcast message
  */
  if (recipient != gsAddress && recipient != broadCastAdress) return;

  /*
  When a LoRa message is sent, it's manually structured like this:
  [destination][sender][length][actual message content]
  The length is the expected number of characters in the message.
  message.length() is the actual number of characters received.
  This if statement checks if what was expected and what was actually received match. 
  */
  if (length != incoming.length()) {
    Serial.println("Message length mismatch.");
    return;
  }

  //print the received command on the LCD and also on the serial monitor
  Serial.print("Received Command: ");
  Serial.println(incoming);
  
  //Remember ot include the printing on the LCD code
  //printing on LCD

}


