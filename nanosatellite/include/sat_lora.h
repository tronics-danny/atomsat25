#ifndef LORA_MODULE_H
#define LORA_MODULE_H

//Initializing the LoRa module
void initLoRa();

//Funtion to receive lora packets from the GS
void receivePkt();

//Function to be called after receiving a packet
//Consists of action to be taken upon certain commands received from the GS
void onReceive(int packetSize);

//void sendLoRaData(float temp, float hum);


#endif
