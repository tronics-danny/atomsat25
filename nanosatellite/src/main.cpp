#include <Arduino.h>
#include <LoRa.h>  
#include <SPI.h>
#include "sat_defs.h"
#include "sat_SharedVars.h"
#include "sat_lora.h"
#include "dht_sensor.h"
#include "oled_display.h"
#include "debug_led.h"


//initiallize all components
void initALL(){
  initLoRa();
  initDHT();
  initOLED();
}

void setPinModes(){
  pinMode(ledPin, OUTPUT);  // Set pin 12 as output
}



void setup() {
  Serial.begin(SERIAL_BAUD_RATE);               // initialize serial
  while (!Serial);

  setPinModes();
  Serial.println("Starting Satellite.");
  initALL();
  
}

void loop() {
  //Check if the update flag for oled is updated
  if (updateOLEDFlag) {
    updateOLEDFlag = false;  // Reset the flag
    blinkDebugLED(3,200, 200);
    display_onOLED(22, receivedCmd);  // Update the OLED outside the ISR
    //blinkDebugLED(3,100,100);

    // process the received command and send response to GS
    processReceivedCmd(commandRequest);
    //String oledMsg = "Sending to GS:"
    display_onOLED(34, "Sending to GS:");  // Update the OLED outside the ISR
    //display_onOLED(46, commandRequest);  // Update the OLED outside the ISR
  }

  //Display temperature value reading on the oled display
  float temp_val = readTempData();

  //Check if there is a temperature reading from the sensor and display on the OLED
  //If no reading from the sensors, show a failure message on the display
  
  if (isnan(temp_val)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    displayTempReadingOLED(34, NAN);  // Display error message on OLED
    //Transmit DHT failure to GS
    String errorMsg = "TempRead Failed!";
    sendMessage(errorMsg);
    blinkDebugLED(2, 200, 200);
    return;
  }
  //Now dispplay on Oled
  displayTempReadingOLED(10, temp_val);  // Now safe to display

  //Transmit via Lora and Display on serial
  if (millis() - lastSendTime > interval) {
    String message = "SatTemp: " + String(temp_val, 2);   // send a message
    
    //transmit temp readings
    sendMessage(message);
    // Display the transmissted Temperature reading on Serial
    Serial.println("Sending " + message);
    lastSendTime = millis();            // timestamp the message
    //interval = 2000 + 1000;     // 2-3 seconds
    LoRa.receive();                     // go back into receive mode
  }
}



