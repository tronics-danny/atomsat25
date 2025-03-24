#ifndef OLED_DISP_H
#define OLED_DISP_H

/*fucntion to display a value on the oled on multiple lines
the line will be specified when calling the function
Parameter definitions in the funtion:
    1st int - line to be printed on
    2nd int - value to be printed on the OLED
*/

void displayOLED(int, float);
void initOLED();




#endif