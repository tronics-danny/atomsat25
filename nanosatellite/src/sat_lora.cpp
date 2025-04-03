#include <Arduino.h>
#include <LoRa.h>  
#include <SPI.h>
#include "sat_lora.h"
#include "sat_defs.h"
#include "dht_sensor.h"
#include "oled_display.h"
#include "sat_SharedVars.h"
#include "debug_led.h"

const String CMD_REQ_TEMP = "REQ_TEMP"; // Incoming temp request command
const String CMD_REQ_PWR  = "REQ_PWR";  //Incoming power level request command

String outgoing;              // outgoing message
//byte msgCount = 0;            // count of outgoing messages
byte satAddress = 0xBB;     // address of this device the sat
byte broadcast = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 5000;          // interval between sends

// Prepare the received command string
String receivedCmd;
String commandRequest;
bool updateOLEDFlag;  // Flag for OLED update

/*
Initialize the LoRa Module
*/
void initLoRa(){
  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, rstPin, irqPin);// set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  LoRa.onReceive(onReceive);
  LoRa.receive();
  Serial.println("Satellite LoRa init succeeded.");
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
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  //byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";                 // payload of packet

  while (LoRa.available()) {            // can't use readString() in callback, so
    incoming += (char)LoRa.read();      // add bytes one by one
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != satAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  // On Oled
  // Prepare the received command string
  receivedCmd = "Rcvd: " + incoming;
  updateOLEDFlag = true;  // Set the flag for OLED update
  commandRequest = incoming;      //Save the incoming command in a global variable 

  // On Serial
  Serial.println();
  Serial.println("Received from GS: 0x" + String(sender, HEX));
  Serial.println("Sent to Sat: 0x" + String(recipient, HEX));
  //Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();

}


/*
this function takes  
  - the data that is to be send
Function used for sending LoRa packets
*/
void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(broadcast);              // add broadcasting address
  LoRa.write(satAddress);             // add sender address
  //LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  //blink led at every send packet
  blinkDebugLED(1,100, 100);
  //msgCount++;                           // increment message ID

}

/*
This function processes the received command and executes its instruction
*/
void processReceivedCmd(String incoming){
  // action to be taken based on the received command
  if (incoming.equals(CMD_REQ_TEMP)) {
    /*
    - if the received command is equal to the request temperature command, read temp data
    and send it via the lora communication link
    */
    float tempValue = readTempData();      // Read the temperature data from the sensor
    String temp_data = "TEMP : " + String(tempValue, 1);

    /*
    - Validate if the temperature value returned is a true value and sent it via lora to GS
    - If temperature value returned is not a valid data then transmit and 
    Temperature read error to the GS

    */
    if (isnan(tempValue)) {
      // This calls the sendResponse function and passes in the Gsaddress, and the Error message parameters
      sendMessage("TEMP_ERROR");
    } 
    else {
      //this calles the sendResponse funtion and passes in the destination address and the temp data
      sendMessage(temp_data);
    }

  }

}



