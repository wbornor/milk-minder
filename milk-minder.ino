//*****************************************************************************
#include <SoftwareSerial.h>   
#include <SmartThings.h>
#include "Statistic.h" //http://playground.arduino.cc/Main/Statistics

#define PIN_LED           13
#define PIN_THING_RX      3
#define PIN_THING_TX      2

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


int sensorPin = A0;    // select the input pin for the potentiometer
int sensorPin2 = A1;
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int sensorValue2 = 0;
Statistic stats;


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
  
  stats.clear();
}

void loop() {
  // run smartthing logic
  smartthing.run();
  
  // Code left here to help debut network connections
  setNetworkStateLED();
  
  //Serial.println("waiting");
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  Serial.print("fsr1: ");
  Serial.print(sensorValue);
  
  sensorValue2 = analogRead(sensorPin2);
  Serial.print(", fsr2: ");
  Serial.print(sensorValue2);
  
  sensorValue = sensorValue + sensorValue2;
  Serial.print(", TOTAL: ");
  Serial.print(sensorValue);
  
  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map!
  sensorValue = map(sensorValue, 1000, 1300, 0, 100);

  //offset  middle tier reading
  //sensorValue = sensorValue >= 90 && sensorValue <= 95 ? map(sensorValue, 90, 95, 10, 99) : sensorValue;
  
  //set caps
  sensorValue = sensorValue > 100 ? 100 : sensorValue;
  sensorValue = sensorValue < 0 ? 0 : sensorValue;
  
  Serial.print(", MAPPED: ");
  Serial.print(sensorValue);
  
  stats.add(sensorValue);

  Serial.print(", cnt: ");
  Serial.print(stats.count()); 

  Serial.print(", avg: ");
  Serial.print(stats.average(), 4);

  Serial.print(", stddev: ");

  Serial.print(stats.pop_stdev(), 4);
 
  if (stats.pop_stdev() > 5  || stats.count() > 500) {
    announceForce(sensorValue);
    stats.clear();
  }
  
  if ( stats.count() < 10 ) {
    announceForce(sensorValue);
  }
  
  // turn the ledPin on
  digitalWrite(ledPin, HIGH);
  
  // stop the program for <sensorValue> milliseconds:
  delay(sensorValue);
  
  // turn the ledPin off:
  digitalWrite(ledPin, LOW);
  Serial.println();
}

void announceForce(int force) {
    Serial.print(", sending force: ");
    Serial.print(sensorValue);
    smartthing.send("humidity: " + String(sensorValue) );
    networkTrafficLED();
}

void networkTrafficLED() {
    smartthing.shieldSetLED(0, 3, 0); // green
    smartthing.shieldSetLED(1, 0, 0); // red
    smartthing.shieldSetLED(0, 0, 0); // off 
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


