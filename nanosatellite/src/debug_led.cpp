#include <Arduino.h>
#include "debug_led.h"
#include "sat_defs.h"

//This is a fution to be called whenever we want to light the led on the sat
void blinkDebugLED(int blinks, int onTime, int offTime) {
  for (int i = 0; i < blinks; i++) {
    digitalWrite(ledPin, HIGH);
    delay(onTime);
    digitalWrite(ledPin, LOW);
    delay(offTime);
    Serial.println("Blinking LED");
  }
}