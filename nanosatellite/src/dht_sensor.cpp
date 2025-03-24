#include "DHT.h"
#include <DHT.h>
#include "sat_defs.h"

DHT dht(DHTPIN, DHTTYPE);

void initDHT() {
  dht.begin();
}

//Here we will only take temperature data of the ambient air in the nanosat
float readTempData() {
  delay(2000);
  //Read the temp from the sensor
  float t = dht.readTemperature();
/*
 // Check if any reads failed and exit early (to try again).
  if (isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
*/
 
  return t;

}