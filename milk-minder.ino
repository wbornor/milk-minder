//*****************************************************************************
#include <SoftwareSerial.h>   
#include <SmartThings.h>

#define PIN_LED           13
#define PIN_THING_RX      3
#define PIN_THING_TX      2
#define PIN_A             4
#define PIN_B             7
#define OPEN              HIGH
#define PRESSED            LOW
#define PUSH_DELAY      1000  // milliseconds to keep the button "pushed"

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

void setup()
{
  // setup default state of global variables
  isDebugEnabled = true;
  stateLED = 0;                 // matches state of hardware pin set below
  stateNetwork = STATE_JOINED;  // set to joined to keep state off if off
  
  // setup hardware pins 
  pinMode(PIN_LED, OUTPUT);     // define PIN_LED as an output
  pinMode(PIN_A, INPUT);
  pinMode(PIN_B, INPUT);
  //digitalRead(PIN_A);
  //digitalRead(PIN_B);

  if (isDebugEnabled)
  { // setup debug serial port
    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("setup..");  // print out 'setup..' on start
  }
}

void loop()
{
  // run smartthing logic
  smartthing.run();
 
  // Code left here to help debut network connections
  setNetworkStateLED();
  
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(PIN_A);
  //print out the value of the pushbutton
  Serial.print("sensorVal: '");
  Serial.print( sensorVal );
  Serial.println("'");

  // Keep in mind the pullup means the pushbutton's
  // logic is inverted. It goes HIGH when it's open,
  // and LOW when it's pressed. Turn on pin PIN_A when the
  // button's pressed, and off when it's not:
  if (sensorVal == 1) {
    pushA();
  }
  else if ( sensorVal == 0 ){
    pushB();
  }
  else {
   error(); 
  }
}

void pushA()
{
  smartthing.send("Apushed");
  Serial.println("Apushed");
  smartthing.shieldSetLED(0, 0, 2); // blue
  //digitalWrite(PIN_LEFT,LOW);
  //delay(PUSH_DELAY);
  //digitalWrite(PIN_LEFT,HIGH);
  smartthing.shieldSetLED(0, 0, 0); // off
}

void pushB()
{
  smartthing.send("Bpushed");
  Serial.println("Bpushed");
  smartthing.shieldSetLED(0, 2, 0); // green
  //digitalWrite(PIN_LEFT,LOW);
  //delay(PUSH_DELAY);
  //digitalWrite(PIN_LEFT,HIGH);
  smartthing.shieldSetLED(0, 0, 0); // off
}

void error()
{
  Serial.println("not recognized");
  smartthing.shieldSetLED(2, 0, 0); // red
  //digitalWrite(PIN_LEFT,LOW);
  //delay(PUSH_DELAY);
  //digitalWrite(PIN_LEFT,HIGH);
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

  if (message.equals("pushA"))
  {
    //pushB();
  }
  else if (message.equals("pushB"))
  {
    //pushB();
  }   
}
