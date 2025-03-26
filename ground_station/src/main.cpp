#include <Arduino.h>
#include "gs_defs.h"
#include "gs_lora.h"
#include "gs_sharedVar.h"
#include "keypad_handler.h"
#include "lcd_display.h"

void initAll(){
  initLCD();                        // Initialize LCD
  initLoRa();                       // Initialize the LoRa module
  initKeyPad();                     // Initialize keypad
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial);
  Serial.println("Starting Ground Station...");
  
  initAll();
  //delay(1000);                      // Delay for 1 sec to set up
  Serial.println("System Ready");
        
}

void loop() {
  //keypadHandler();
  display_and_transmit_Cmd(); // Send a LoRa packet
  //delay(5000); // Wait 5 seconds before next packet
}
