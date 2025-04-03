#include <Arduino.h>
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
  
  return t;

}