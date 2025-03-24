#include <Arduino.h>
#include "sat_defs.h"
#include "sat_lora.h"
#include "dht_sensor.h"
#include "oled_display.h"
#include "debug_led.h"


void initALL(){
  initLoRa();
  initDHT();
  initOLED();
}

void setPinModes(){
  pinMode(ledPin, OUTPUT);  // Set pin 12 as output
}



void setup() {
  
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial);

  Serial.println("Starting Nanosat LoRa Test...");
  //initialize lora module communication
  initALL();
  setPinModes();
  
}


void loop() {
  // Receive the lora Packet send from the GS
  receivePkt();

  //Display temperature value reading on the oled display
  float temp_val = readTempData();

  //Check if there is a temperature reading from the sensor and display on the OLED
  //If no reading from the sensors, show a failure message on the display
  
  if (isnan(temp_val)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    displayOLED(34, NAN);  // Display error message on OLED
    blinkDebugLED(2);
    return;
  }

  Serial.print("Temp: ");
  Serial.println(String(temp_val,1));

  displayOLED(10, temp_val);  // Now safe to display
}



