//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <Wire.h>

/*******************************************************

This program will test the LCD panel and the buttons
Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;

int curPos = 4; //position décimale par défaut du curseur
double freq = 1; //Fréquence par défaut du stroboscope
int buttonDelay = 120;

bool buttonReady = true;

double incVal = 1;
int displayMode = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define mFreq 0
#define mDuty 1



void updateFreq(String dir) // Increment frequency
{
  Serial.println(String(freq)); //For debugging
  float newFreq = freq;
  String freqString;
  if (dir == "+") newFreq = freq + incVal; //increase or decrease frequency
  else if (dir == "-") newFreq = freq - incVal;

  if ((newFreq >= 0) && (newFreq <= 10000)) freq=newFreq; //Make sure the new 
  //frequency remains within accepted boundary. 
  

  //Printing freq on lcd
  freqString = String(freq,1);
  lcd.setCursor(15,1);
  lcd.rightToLeft();
  //Right justify
  int fLength = freqString.length();
  String invFreqString = "";
  for (int i=0;i<fLength;i++) {
    invFreqString += freqString[fLength-1-i];
  }
  while (invFreqString.length() < 7){
    invFreqString +=" "; //Extra space
  }
  lcd.print(invFreqString);
  lcd.leftToRight();
  
  
}


void changeincVal(String dir) //This function set the decimal position of the increment (between 0.1 and 1000)
{
  if (dir == "right") {
    Serial.println("1");
    if (incVal == 0.1) return;
    else {
      incVal = incVal / 10;
    }
  }
  if (dir == "left") {
    Serial.println("0");
    if (incVal == 1000) return;
    else {
      incVal = incVal * 10;
    }
  }
  delay(buttonDelay * 2);
}



void setup()
{
  lcd.begin(16, 2);              // start the library

  //Intro display of the LCD 
  lcd.setCursor(0, 0);
  lcd.print("Strob. by  MxBoud"); // print a simple message
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("life's good.");
  Serial.begin(9600);
  //Wire.begin();
}

void loop()
{


  lcd_key = readLCDcontroller();  // read the buttons
  respondToButtonInput(); //Do what the button is supposed to do

}
int readLCDcontroller() //Tells wich button is selected
{
  adc_key_in = analogRead(0);      // read the value from the sensor
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  return btnLEFT;
  if (adc_key_in < 790)  return btnSELECT;
  return btnNONE;  // when all others fail, return this...
}

void respondToButtonInput() //Respond to current lcd_key and initiate corresponding callback.
{
 
    switch (lcd_key)               // depending on which button was pushed, we perform an action
  {
    case btnRIGHT:
      {
        changeincVal("right");
        lcd.setCursor(0,1);
        lcd.print(String(incVal));
        break;
      }
    case btnLEFT:
      {
        changeincVal("left");
        lcd.setCursor(0,1);
        lcd.print(String(incVal));
        break;
      }
    case btnUP:
      {
        lcd.print("UP    ");
        updateFreq("+");
        delay(buttonDelay);
        break;
      }
    case btnDOWN:
      {
        lcd.print("DOWN  ");
        updateFreq("-");
        delay(buttonDelay);
        break;
      }
    case btnSELECT: // Switch between frequency mode and dutycycle mode
      {
        lcd.setCursor(0,0);
        
        if (displayMode == mFreq) {
          displayMode = mDuty;
          lcd.print("Duty cycle");
        }
        else if (displayMode == mDuty) {
          displayMode = mFreq;
          lcd.print("Frequency");
          
        }
        delay(buttonDelay);
        
        break;
      }
    case btnNONE:
      {
        lcd.setCursor(0,1);
        lcd.print(String(incVal));
        break;
      }
  }

}

