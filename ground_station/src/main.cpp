#include <SPI.h>              // include libraries
#include <LoRa.h>
#include <Keypad.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#ifdef ARDUINO_SAMD_MKRWAN1300
#error "This example is not compatible with the Arduino MKR WAN 1300 board!"
#endif

// define LoRa module connections
#define csPin 5
#define rstPin 14
#define irqPin 2

//Keypad pins
//Row pins
#define R1 12
#define R2 13
#define R3 25
#define R4 26

//Column pins
#define C1 27
#define C2 32
#define C3 33
#define C4 15


//String lastCommand = "";

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

String inputBuffer = "";      //To store keypad input
String lastCommand = "";      // To store Translated keypad input

String outgoing;              // outgoing message
//byte msgCount = 0;            // count of outgoing messages
byte GS_Address = 0xCC;     // address of this device the GS
byte broadcast = 0xFF;  
byte satAddress = 0xBB;    // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

// Command definitions
const String CMD_REQ_TEMP = "REQ_TEMP";
const String CMD_REQ_PWR = "REQ_PWR";
const String CMD_UNKNOWN = "UNKNOWN_CMD";

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2); 

//Update LCD Flag
bool updateLcdFlag;
//gloabal variable to store the received message form tha sat node
String displayMsg;

int row1 = 1;     // row one of the lcd display
int row2 = 2;     // row 2 of the lcd display

void sendMessage(String outgoing);
void onReceive(int packetSize);
void initKeyPad();
void initLoRa();
void initLCD();
void initALL();
void display_onLCD(String updateMsg);
String translateCMD(String buffer);
void resetInput();


void initLCD(){
    // LCD initialization
    lcd.init();
    lcd.backlight();
    lcd.clear();
    delay(100);
    lcd.print("Receiving:");
}

//initiallize all components
void initALL(){
  initLoRa();
  initKeyPad();
  initLCD();
}

void setup() {
  Serial.begin(9600);                   // initialize serial
  while (!Serial);

  Serial.println("Starting Ground Station");
  initALL();

}

void loop() {
  //here we are checking if the updateLCD fla dhas been set the display our received packet on the LCD
  if(updateLcdFlag){
    //if true, then display on LCD the rreceived temp value reading form the sat
    display_onLCD(displayMsg);
    updateLcdFlag = false;
  }

  //Here we want ot display the enter command message;
  //String enter_cmd = "Enter Command:";
  //display_onLCD(row1, enter_cmd);

  char key = keypad.getKey();

  if (!key) return;  // No key pressed
  
  // Handle input characters
  if (isalnum(key)) {
    if (inputBuffer.length() < 16) {  // Prevent overflow
      inputBuffer += key;
      display_onLCD(inputBuffer);
    }
  }
  // Handle delete
  else if (key == '*' && inputBuffer.length() > 0) {
    inputBuffer.remove(inputBuffer.length() - 1);
    Serial.print(inputBuffer);
    display_onLCD(inputBuffer);
  }
  // Handle command submission
  else if (key == '#' && inputBuffer.length() > 0) {
    /*
    After the # key has been pressed to submit the input, 
    the keypad iput is translated into a command and transmitted over Lora
    */

    if (millis() - lastSendTime > interval) {
      String message = String(inputBuffer);   // send a message
      sendMessage(message);                   //calling the sendMessage function that transmits the comtents of message over LORA
      
      //Validate if there is anything held in last command then display on serial
      if(lastCommand.length()>0){
        Serial.println("Sending from GS: " +  lastCommand);
        Serial.println();
      }

      //Serial.println("Sending from GS: " +  lastCommand);
      //Serial.println();
      lastSendTime = millis();            // timestamp the message
      interval = random(2000) + 1000;     // 2-3 seconds
    }

    LoRa.receive();                     // go back into receive mode
  }
}



void initKeyPad() { 
    // Initialize column pins with pullups
    for (int i = 0; i < COLS; i++) {
        pinMode(colPins[i], INPUT_PULLUP);
    }
  
}

void initLoRa(){
  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, rstPin, irqPin);// set CS, reset, IRQ pin

  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  LoRa.onReceive(onReceive);
  LoRa.receive();
  Serial.println("Ground Station LoRa init succeeded.");
}

//This fucntion will be called whenever something is to be printed on the LCD
//Takes two parameters the row on which to print on and the message to be printed
void display_onLCD(String updateMsg){
    lcd.setCursor(0, 1);
    lcd.print("                ");  // Clear line
    lcd.setCursor(0, 1);
    lcd.print(updateMsg);
}


/*
Here we want ot display the command that has been translated from the keypad input
this is the command that is now going to be send via LORA
*/
String translateCMD(String buffer) {
    if (buffer == "123A") return CMD_REQ_TEMP;
    if (buffer == "456B") return CMD_REQ_PWR;
    //if (buffer == "789C") return "CMD_3";  // Example additional command
    //if (buffer == "*0#D") return "CMD_4";  // Example additional command
    
    return CMD_UNKNOWN;  // Default for unrecognized commands
}

//This function resets the input on the keypad and clears the LCD
void resetInput() {
    inputBuffer = "";
    lcd.clear();
    lcd.print("Enter Command:");
}

//Adding a keypad input to be send via lora to the Sat



void sendMessage(String outgoing) {
  lcd.clear();
  lcd.print("Processing...");
  delay(1000);

  // Translate the command
  lastCommand = translateCMD(outgoing);

  Serial.print("Translated cmd: ");
  Serial.println(lastCommand);

  // Display result
  lcd.clear();
  lcd.print("CMD:");
  lcd.setCursor(0, 1);
  lcd.print(lastCommand); 

  //validate if last command is a valid command then send via LoRa
  if (lastCommand != CMD_UNKNOWN){
    //Now send the processed command via LoRa
    LoRa.beginPacket();                   // start packet
    LoRa.write(satAddress);              // add destination address
    LoRa.write(GS_Address);             // add sender address
    LoRa.write(lastCommand.length());        // add payload length
    LoRa.print(lastCommand);                 // add payload
    LoRa.endPacket();                     // finish packet and send it
  }
  else{
    Serial.println("Invalid Command! Cannot be transmitted!");
    lastCommand = "";
    Serial.println();
  }

  delay(3000);  // Show command for 3 seconds
  resetInput();
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  //byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";                 // payload of packet

  while (LoRa.available()) {            // can't use readString() in callback, so
    incoming += (char)LoRa.read();      // add bytes one by one
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != GS_Address && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }
  //set a flag here that will trigger a display to the LCD upon receiving a packet from the Sat
  updateLcdFlag = true;
  //Store the received message in a global variable
  displayMsg = incoming;

  // if message is for this device, or broadcast, print details:
  Serial.println("Received from Sat: 0x" + String(sender, HEX));
  Serial.println("Sent to GS: 0x" + String(recipient, HEX));
  //Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
}

