#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H
#include <Arduino.h>

void initLCD();
//void display_and_transmit_Cmd();
String translateCMD(const String &buffer);

void updateLCD();
void resetInput();

void processCommand();


#endif