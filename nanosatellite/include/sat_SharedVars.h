#ifndef SAT_SHAREDVARS_H
#define SAT_SHAREDVARS_H
#include <Arduino.h>

extern String outgoing;              // outgoing message
//byte msgCount = 0;            // count of outgoing messages
extern byte satAddress;     // address of this device the sat
extern byte broadcast;      // destination to send to
extern long lastSendTime;        // last send time
extern int interval;          // interval between sends

// Prepare the received command string
extern String receivedCmd;
extern String commandRequest;
extern bool updateOLEDFlag;  // Flag for OLED update

#endif