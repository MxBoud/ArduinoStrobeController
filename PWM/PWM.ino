/*
 *  Timer1 library example
 *  June 2008 | jesse dot tane at gmail dot com
 */

#include "TimerOne.h"

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

double period = 1000000; //Default values for the period (in microseconds)
double duty = 100; //Defaul value for the Dutycycle (uint 12, i think). 

void setup()
{
  Serial.begin(9600);
  inputString.reserve(200);
  pinMode(10, OUTPUT);
  Timer1.initialize(500000);         // initialize timer1, and set a 1/2 second period
  Timer1.pwm(9, duty,period);                // setup pwm on pin 9, 50% duty cycle
}


void loop()
{
  // your program here...
  serialEvent();
  if (stringComplete) {
    Serial.println(inputString); //Input must be in format as in 
    //these examples: D50, D1.1, F10, F1000

    char mode = inputString[0];
    inputString.remove(0,1);
    float value = inputString.toFloat();
    
    if (mode == 'D'){
      duty = value;
      Serial.println(value);  
    }
    else if (mode = 'P') {
      period = value;
      Serial.println(value);
    }
    Timer1.pwm(9,duty,period);
    inputString = "";
    stringComplete = false;
  }
   
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    
    
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      
    }
    else {
      inputString += inChar;
    }
  }
}
