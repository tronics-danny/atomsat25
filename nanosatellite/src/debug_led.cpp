#include <Arduino.h>
#include "debug_led.h"
#include "sat_defs.h"

void blinkDebugLED(int blinks, int onTime, int offTime) {
    for (int i = 0; i < blinks; i++) {
      digitalWrite(ledPin, HIGH);
      delay(onTime);
      digitalWrite(ledPin, LOW);
      delay(offTime);
    }
  }