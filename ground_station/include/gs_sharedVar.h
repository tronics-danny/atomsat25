#ifndef GS_SHAREDVAR_H
#define GS_SHAREDVAR_H
#include <Arduino.h>

// Command definitions
extern const String CMD_REQ_TEMP;
extern const String CMD_REQ_PWR;
extern const String CMD_UNKNOWN;

// Variables to store the keypad input
extern String inputBuffer ;
// The processed keypad input to be now send via LORA
extern String lastCommand ;

// Source and destination addresses
extern byte cubeAddress;               // address of this device the nanosat
extern byte gsAddress;                  // adress of the GS in this case will be the receipient
extern byte broadCastAdress;           // This broadcasts the message


#endif