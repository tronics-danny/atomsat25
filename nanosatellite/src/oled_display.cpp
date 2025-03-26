#include <Arduino.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <sat_defs.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

/*fucntion to display a value on the oled on multiple lines
the line will be specified when calling the function
Parameter definitions in the funtion:
    1st int - line to be printed on
    2nd int - value to be printed on the OLED
*/

void initOLED(){
  //U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
  u8g2.begin();
}

//takes an int parameter for the line and a float param for the temp val to output
void displayTempReadingOLED(int line1, float temp){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x13_tf);

  if (isnan(temp)) {
    u8g2.drawStr(10, line1, "DHT Failed!!");
  } else {
    String msg = "Temp: " + String(temp, 1) + " C";
    u8g2.drawStr(2, line1, msg.c_str());

    // Optional: You could add a second line here like:
    // u8g2.drawStr(2, line1 + 15, "Status: OK");
  }

  u8g2.sendBuffer();
}


// This is a function to be called whenever something else is to be displayed on the oled
// It takes two variables, the line at which the message is to be displayed at and the message to be displayed
void displayOnOLED(int line, String msg){
  u8g2.clearBuffer();                       //Clear the screen
  u8g2.setFont(u8g2_font_7x13_tf);          //Set the font to be used

  u8g2.drawStr(2, line, msg.c_str());       //Take the message and store in memory

  u8g2.sendBuffer();                        //Forward the message to the screen
}

