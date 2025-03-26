#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "lcd_display.h"
#include "keypad_handler.h"
#include "gs_sharedVar.h"
#include "gs_lora.h"

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2); 


void initLCD(){
    // LCD initialization
    lcd.init();
    lcd.backlight();
    lcd.clear();
    delay(100);
    //resetInput();
    lcd.print("Enter Command:");
}

void processCommand() {
    lcd.clear();
    lcd.print("Processing...");
    delay(1000);
    
    // Translate the command
    lastCommand = translateCMD(inputBuffer);
  
    Serial.print("translated cmd: ");
    Serial.println(lastCommand);
  
    
    // Display result
    lcd.clear();
    lcd.print("CMD:");
    lcd.setCursor(0, 1);
    lcd.print(lastCommand);
    
    // Here you would add your LoRa transmission code
    // transmitCommand(lastCommand);
    
    delay(3000);  // Show command for 2 seconds
    resetInput();
  }

/*
Here we want ot display the commands that has been translated from the keypad input
this is the command that is now going to be send via LORA
*/

/*
void display_and_transmit_Cmd() {
    if (inputBuffer.length() == 0) return;  // Do nothing if empty

    lcd.clear();
    lcd.print("Processing...");

    Serial.print("Input Buffer: ");
    Serial.println(inputBuffer);  // Debugging: Print buffer before translation

    // Translate the command
    
    lastCommand = translateCMD(inputBuffer);
        

    Serial.print("Translated CMD: ");
    Serial.println(lastCommand);  // Debugging: Print translated command
    
    // Display result
    lcd.clear();
    lcd.print("CMD:");
    lcd.setCursor(0, 1);
    lcd.print(lastCommand);

    /*
    Here you would add your LoRa transmission code:
    Here we are calling the transmitPkt function so that 
    it can transmit our command via LORA
    //
   transmitPkt(lastCommand);

   delay(2000);  // Show command for 2 seconds
   resetInput();
}*/


String translateCMD(const String &buffer) {
    if (buffer == "123A") return CMD_REQ_TEMP;
    if (buffer == "456B") return CMD_REQ_PWR;
    //if (buffer == "789C") return "CMD_3";  // Example additional command
    //if (buffer == "*0#D") return "CMD_4";  // Example additional command
    
    return CMD_UNKNOWN;  // Default for unrecognized commands
}

/*
This function updates the contents of the inputBuffer variable which holds keys being pressed
on the keypad to the lcd display.
*/
void updateLCD() {
    lcd.setCursor(0, 1);
    lcd.print("                ");  // Clear line
    lcd.setCursor(0, 1);
    lcd.print(inputBuffer);
}

/*
This is a reset function for the inputBuffer variable and also the LCD. It clears the contents of the 
lcd and updates the enter command display on the display
*/
void resetInput() {
    inputBuffer = "";
    lcd.clear();
    lcd.print("Enter Command:");
}
    