#ifndef OLED_DISP_H
#define OLED_DISP_H
#include <Arduino.h>

/*fucntion to display a value on the oled on multiple lines
the line will be specified when calling the function
Parameter definitions in the funtion:
    1st int - line to be printed on
    2nd int - value to be printed on the OLED
*/

void displayTempReadingOLED(int line1, float temp);
void initOLED();
void display_onOLED(int line, String msg);




#endif