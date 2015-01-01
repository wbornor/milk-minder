//*****************************************************************************
#include <SoftwareSerial.h>   

#define PIN_LED           13
#define PIN_THING_RX      3
#define PIN_THING_TX      2


bool isDebugEnabled;    // enable or disable debug in this example




int sensorPin = A0;    // select the input pin for the potentiometer
int sensorPin2 = A1;
int ledPin = 8;      // select the pin for the LED
int LEDbrightness;
int fridgePin0 = 9;  // variable to store the value coming from the sensor
int fridgePin1 = 10;


void setup() {
  // setup default state of global variables
  isDebugEnabled = true;
  
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  if (isDebugEnabled)
  { // setup debug serial port
    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("setup..");  // print out 'setup..' on start
  }
}

void loop() {
  Serial.print("fridgePin0: ");
  Serial.print(fridgePin0);
  Serial.print(", fridgePin1: ");
  Serial.print(fridgePin1);
  Serial.println();
  if(digitalRead(fridgePin0) == HIGH && digitalRead(fridgePin1) != HIGH ){
    LEDbrightness = 255;
    // turn the ledPin on
    digitalWrite(ledPin, LEDbrightness);
    delay(1000);
  }
  digitalWrite(ledPin, 0);
}
