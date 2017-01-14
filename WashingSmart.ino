/*
Cayenne MKR1000 and Cayenne IoT Symple

This sketch connects to the Cayenne server using an Arduino/Genuino MKR1000 and runs the main communication loop.
The Project control a tilt sensor. When the sensor read movements, switch on V1, and the process start.
After the delay, Arduino MKR1000 control if the sensor have received others input.
If the sensor haven't received input, switch on the V2 Virtual pin thet still ON until the sensor receive others input.
You must trig the V1 pin for start, and V2 pin for Stop the process.

The Cayenne Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

In Cayenne Dashboard add
Digital pin D7 for control the sensor input
Virtual Pin V1 for control the start process
Virtual Pin V2 for control the stop process

*/

//#define CAYENNE_DEBUG         // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMKR1000.h>

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "*************";
// Your network name and password.
char ssid[] = "**************";
char password[] = "**************";

unsigned long timer;
boolean input = false;
int threshold = 30000; // Each 30 sec put on Cayenne Dashboard the values
boolean trig = false;
boolean start = false;
boolean touch = false;
int TiltSwitch = 7;

void setup()
{
	pinMode (TiltSwitch, INPUT_PULLUP);
	Serial.begin(9600);
	Cayenne.begin(token, ssid, password);
}

void loop()
{
  Cayenne.run();
}

CAYENNE_OUT(V1)
{  
  input = digitalRead(TiltSwitch);
  timer = millis();
  if (input) 
  {
    touch = true;
  }
  
  // The stop after start
  if (start && !trig && (timer >= threshold))
  {
    CAYENNE_LOG("Data request stop", V2);
    Cayenne.virtualWrite(V2, HIGH);
    start = false;
  }

  // The start
  if (!start && trig && (timer >= threshold))
  {
    threshold = timer;
    CAYENNE_LOG("Data request start", V1);
    Cayenne.virtualWrite(V1, HIGH);
    delay(1000);
    Cayenne.virtualWrite(V1, LOW);
    Cayenne.virtualWrite(V2, LOW);
    trig = false;
    start = true;
  }

  // Read the D7 input
  if (touch) {
    trig = true;
  }
  else
  {
    trig = false;
  } 
}
