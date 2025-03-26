#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "gs_defs.h"
#include "lcd_display.h"
#include "gs_sharedVar.h"


// Command definitions
const String CMD_REQ_TEMP = "REQ_TEMP";
const String CMD_REQ_PWR = "REQ_PWR";
const String CMD_UNKNOWN = "UNKNOWN_CMD";

String inputBuffer = "";
String lastCommand = "";

// Keypad Setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

byte rowPins[ROWS] = {R1, R2, R3, R4};  // Rows
byte colPins[COLS] = {C1, C2, C3, C4};  // Columns

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void initKeyPad() { 
    // Initialize column pins with pullups
    for (int i = 0; i < COLS; i++) {
        pinMode(colPins[i], INPUT_PULLUP);
    }
  
}

void display_and_transmit_Cmd(){
  char key = keypad.getKey();

  if (!key) return;  // No key pressed
  
  // Handle input characters
  if (isalnum(key)) {
    if (inputBuffer.length() < 16) {  // Prevent overflow
      inputBuffer += key;
      updateLCD();
    }
  }
  // Handle delete
  else if (key == '*' && inputBuffer.length() > 0) {
    inputBuffer.remove(inputBuffer.length() - 1);
    updateLCD();
  }
  // Handle command submission
  else if (key == '#' && inputBuffer.length() > 0) {
    processCommand();
  }
}



/*
void keypadHandler() {
  //updateLCD();
  char key = keypad.getKey();

  if (!key) return;  // No key pressed

  Serial.print("Key Pressed: ");
  Serial.println(key);  // Debugging
  
  // Handle input characters
  if (isalnum(key)) {
    if (inputBuffer.length() < 16) {  // Prevent overflow
      inputBuffer += key;

      Serial.print("Updated Input Buffer: ");
      Serial.println(inputBuffer);  // Debugging
      /*
      Here we are calling the function updateLCD so that as the keys are being 
      pressed they are being reflected on the LCD
      //
      updateLCD();
    }
  }

  // Handle delete
  else if (key == '*' && inputBuffer.length() > 0) {
    inputBuffer.remove(inputBuffer.length() - 1);
    updateLCD();
  }

  /*
  Handle command submission:
  Here when the # key is pressed, were are submitting whatever we have input on the keypad.
  the keypad input is first processed and send over LORA
  //
  else if (key == '#') {
    if (inputBuffer.length() > 0) {
      Serial.println("Submitting command...");
      display_and_transmit_Cmd();
    }
  }
}*/


