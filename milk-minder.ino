//*****************************************************************************
#include <SoftwareSerial.h>   
#include <SmartThings.h>

#define PIN_LED           13
#define PIN_THING_RX      3
#define PIN_THING_TX      2
#define MIN_THRESHOLD     0

SmartThingsCallout_t messageCallout;    // call out function forward decalaration
SmartThings smartthing(PIN_THING_RX, PIN_THING_TX, messageCallout);  // constructor

bool isDebugEnabled;    // enable or disable debug in this example
int stateLED;           // state to track last set value of LED
int stateNetwork;       // state of the network 


void setNetworkStateLED()
{
  SmartThingsNetworkState_t tempState = smartthing.shieldGetLastNetworkState();
  if (tempState != stateNetwork)
  {
    switch (tempState)
    {
      case STATE_NO_NETWORK:
        if (isDebugEnabled) Serial.println("NO_NETWORK");
        smartthing.shieldSetLED(2, 0, 0); // red
        break;
      case STATE_JOINING:
        if (isDebugEnabled) Serial.println("JOINING");
        smartthing.shieldSetLED(2, 0, 0); // red
        break;
      case STATE_JOINED:
        if (isDebugEnabled) Serial.println("JOINED");
        smartthing.shieldSetLED(0, 0, 0); // off
        break;
      case STATE_JOINED_NOPARENT:
        if (isDebugEnabled) Serial.println("JOINED_NOPARENT");
        smartthing.shieldSetLED(2, 0, 2); // purple
        break;
      case STATE_LEAVING:
        if (isDebugEnabled) Serial.println("LEAVING");
        smartthing.shieldSetLED(2, 0, 0); // red
        break;
      default:
      case STATE_UNKNOWN:
        if (isDebugEnabled) Serial.println("UNKNOWN");
        smartthing.shieldSetLED(0, 2, 0); // green
        break;
    }
    stateNetwork = tempState; 
  }
}



/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13.
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead().

 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground

 * Note: because most Arduinos have a built-in LED attached
 to pin 13 on the board, the LED is optional.


 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe

 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/AnalogInput

 */

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorPin2 = A1;
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int sensorValue2 = 0;
boolean isSensing = false;


void setup() {
  // setup default state of global variables
  isDebugEnabled = true;
  
  stateLED = 0;                 // matches state of hardware pin set below
  stateNetwork = STATE_JOINED;  // set to joined to keep state off if off
  
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  if (isDebugEnabled)
  { // setup debug serial port
    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("setup..");  // print out 'setup..' on start
  }
}

void loop() {
  // run smartthing logic
  smartthing.run();
  
  // Code left here to help debut network connections
  setNetworkStateLED();
  
  //Serial.println("waiting");
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  sensorValue2 = analogRead(sensorPin2);
  sensorValue = sensorValue + sensorValue2;
  Serial.print("total sensorValue: ");
  Serial.print(sensorValue);
  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map!
  sensorValue = map(sensorValue, 1000, 1300, 0, 100);
  Serial.print(", mapped value: ");
  Serial.println(sensorValue);
  
  
 //offset  middle tier reading
  sensorValue = sensorValue >= 90 && sensorValue <= 95 ? map(sensorValue, 90, 95, 10, 99) : sensorValue;
  
  //set caps
  sensorValue = sensorValue > 100 ? 100 : sensorValue;
  sensorValue = sensorValue < 0 ? 0 : sensorValue;
  
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);
  
  
  if(sensorValue >= MIN_THRESHOLD){
    Serial.print("sending force: ");
    Serial.println(sensorValue);
    smartthing.send("humidity: " + String(sensorValue) );
    isSensing = true;
    //smartthing.shieldSetLED(0, 2, 0); // green
    cycleLED();
  }
  
  if(sensorValue <= MIN_THRESHOLD && isSensing) {
    isSensing = false;
    Serial.print("sending force: ");
    Serial.println(sensorValue);
    smartthing.send("humidity: " + String(sensorValue) );
    smartthing.shieldSetLED(0, 0, 0); // off
  }
  
  // stop the program for <sensorValue> milliseconds:
  delay(sensorValue);
  
  //smartthing.shieldSetLED(0, 0, 0); // off
  // turn the ledPin off:
  digitalWrite(ledPin, LOW);
}

void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }

}

void cycleLED() {
    smartthing.shieldSetLED(1, 0, 0); // red
    smartthing.shieldSetLED(0, 1, 0); // green
    smartthing.shieldSetLED(0, 0, 1); // blue
    smartthing.shieldSetLED(1, 0, 0); // red
    smartthing.shieldSetLED(0, 1, 0); // green
    smartthing.shieldSetLED(0, 0, 1); // blue
    smartthing.shieldSetLED(0, 0, 0); // off 
}  
