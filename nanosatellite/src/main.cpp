#include <Arduino.h>
//#include "dht_sensor.h"
//#include "oled_display.h"
#include "sat_defs.h"
#include "sat_lora.h"




void setup() {
  // put your setup code here, to run once:

  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial);

  Serial.println("Starting Nanosat LoRa Test...");
  //initialize lora module communication
  initLoRa();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  receivePkt();

}

