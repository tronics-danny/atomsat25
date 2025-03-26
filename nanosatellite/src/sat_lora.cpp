
#include <LoRa.h>  
#include <SPI.h>
#include "sat_lora.h"
#include "sat_defs.h"
#include "dht_sensor.h"
#include "oled_display.h"

// LoRa module receive and send variables

String outMessage;                      // Outgoing message variable
String contents = "";                   // Receive message variable
const String CMD_REQ_TEMP = "REQ_TEMP"; // Incoming temp request command
const String CMD_REQ_PWR  = "REQ_PWR";  //Incoming power level request command

//String temp_dataRequest = "123";  // If string received is 123 then send temp data to GS
//String power_dataRequest = "456"; // If string received is 456 then send power level data to GS
 
// Source and destination addresses
byte cubeAddress = 0xBB;               // address of this device the nanosat
byte gsAddress = 0xCC;                  // adress of the GS in this case will be the receipient
byte broadCastAdress = 0xFF;           // This broadcasts the message


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

  /*
  - Upon receiving a packet that indicates a certain command from the GS act on f
  ullfilling the command.
  - The command received is Coded
  - for example a command to return to GS the current 
      -ambient Temperature of the Nanosat
      -power level remainig on the batterries
      -solar charge being currently received
      -the geo-location of the nanosat
      -orientation of the nanosat
  - these are just some of the commands that can be incoporated in this system. 
  - In this, we'll only have a few commands to be incoporated
      -Request of temperature information from GS
      -Request of power level in the batteries
  */
void onReceive(int packetSize) {
  if (packetSize == 0) return;

  // Receive the accompanying data packets with the message
  byte recipient = LoRa.read();         // Get the Receipeint address from the received message
  byte sender = LoRa.read();            // Get the sender address from the received message
  byte length = LoRa.read();            // Get the message lenght of the payload

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
  if (recipient != cubeAddress && recipient != broadCastAdress) return;

  /*
  When a LoRa message is sent, it's manually structured like this:
  [destination][sender][msg ID][length][actual message content]
  The length is the expected number of characters in the message.
  message.length() is the actual number of characters received.
  This if statement checks if what was expected and what was actually received match. 
  */
  if (length != incoming.length()) {
    Serial.println("Message length mismatch.");
    return;
  }

  //print the received command on the OLED and also on the serial monitor
  Serial.print("Received Command: ");
  Serial.println(incoming);
  
  //printing on oled
  String oledMsg = "Rcvd CMD: " + incoming;
  displayOnOLED(22, oledMsg);

  // action to be taken based on the received command
  if (incoming.equals(CMD_REQ_TEMP)) {
    /*
    - if the received command is equal to the request temperature command, read temp data
    and send it via the lora communication link
    */
    float temp = readTempData();      // Read the temperature data from the sensor
    String temp_data = "TEMP : " + String(temp, 1);

    /*
    - Validate if the temperature value returned is a true value and sent it via lora to GS
    - If temperature value returned is not a valid data then transmit and 
    Temperature read error to the GS

    */
    if (isnan(temp)) {
      // This calls the sendResponse function and passes in the Gsaddress, and the Error message parameters
      sendResponse(gsAdress, "TEMP_ERROR");
    } 
    else {
      //this calles the sendResponse funtion and passes in the destination address and the temp data
      sendResponse(gsAdress, temp_data);
    }
  }

  // Re-enable receiving
  LoRa.receive();

}


/*

***************
Work on this to send the responses requested from the commands received from the GS
***************

this function takes two parameters, 
  - the recipient adress 
  - the data that is to be send 
*/

void sendResponse(byte recipient, String data) {
  LoRa.beginPacket();
  LoRa.write(recipient);          // GS is destination
  LoRa.write(cubeAddress);         // CubeSat is sender
  LoRa.write(data.length());
  LoRa.print(data);
  LoRa.endPacket();
}



