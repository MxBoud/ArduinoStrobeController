//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <Wire.h>

/*******************************************************
This program is used with a DFRobot LCD keypad shield. It implements a simple UI for setting values to 
the variable freq and duty through the buttons of the shield. 

Code written by Maxime Boudreau (February 2017) using the example code from Mark Bramwell (July 2010)

********************************************************/

//Initial values for the ajustable variables. 
double freq = 1; //1 Hz
double duty = 50;// 50%

int lcd_key     = 0;
int adc_key_in  = 0;


// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons



int buttonDelay = 120; //Speed at wich the Arduino respond to user entry.

double incValF = 1;
double incValD = 1;

int displayMode = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define mFreq     0
#define mDuty     1


void setup()
{
  lcd.begin(16, 2);              // start the library

  //Intro display of the LCD
  lcd.setCursor(0, 0);
  lcd.print("Strob. by MxBoud");
  delay(1000);

  Serial.begin(9600);
  updateLCD();

}

void loop()
{
  lcd_key = readLCDcontroller();  // read the buttons
  respondToButtonInput(); //Do what the button is supposed to do
}

void changeincVal(String dir) //This function set the decimal position of the increment (between 0.1 and 1000)
{
  if (displayMode == mFreq)
  {
    if (dir == "left") {
      if (incValF == 0.1) return;
      else {
        incValF = incValF / 10;
      }
    }
    if (dir == "right") {
      if (incValF == 1000) return;
      else {
        incValF = incValF * 10;
      }
    }
  }
  else if (displayMode == mDuty)
  {
    if (dir == "right") {
      if (incValD == 0.1) return;
      else {
        incValD = incValD / 10;
      }
    }
    if (dir == "left") {
      if (incValD == 100) return;
      else {
        incValD = incValD * 10;
      }
    }
    delay(buttonDelay * 2);
  }
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
        updateLCD();
        delay(buttonDelay*2);
        break;
      }
    case btnLEFT:
      {
        changeincVal("left");
        updateLCD();
        delay(buttonDelay*2);
        break;
      }
    case btnUP:
      {
        updateFreqDuty("+");
        updateLCD();
        delay(buttonDelay);
        break;
      }
    case btnDOWN:
      {
        updateFreqDuty("-");
        updateLCD();
        delay(buttonDelay);
        break;
      }
    case btnSELECT: // Switch between frequency mode and dutycycle mode
      {
        if (displayMode == mFreq) {
          displayMode = mDuty;
        }
        else if (displayMode == mDuty) {
          displayMode = mFreq;
        }
        updateLCD();
        delay(4 * buttonDelay);

        break;
      }
    case btnNONE:
      {
        //updateLCD();
        break;
      }
  }

}

void updateFreqDuty(String dir) // Increment frequency or DutyCycle
{
  if (displayMode == mFreq) {
    float newFreq = freq;
    if (dir == "+") newFreq = freq + incValF; //increase or decrease frequency
    else if (dir == "-") newFreq = freq - incValF;

    if ((newFreq >= 0.01) && (newFreq <= 10000)) freq = newFreq; //Make sure the new

    float period = (1/freq)*1e6; //Period in microseconds
    
    Serial.println("P"+String(period));
    //frequency remains within accepted boundary.
  }
  if (displayMode == mDuty) {
    float newDuty = duty;
    if (dir == "+") newDuty = duty + incValD; //increase or decrease duty
    else if (dir == "-") newDuty = duty - incValD;

    if ((newDuty >= 0.01) && (newDuty <= 99)) duty = newDuty; //Make sure the new
    //duty remains within accepted boundary.
    
    Serial.println("D"+String(duty/100*1023));
  }
}

void updateLCD() 
{
  lcd.clear();

  //Display mode
  lcd.setCursor(0, 0);
  if (displayMode == mFreq) {
    lcd.print("Frequency (Hz)");
  }
  else if (displayMode == mDuty) {
    lcd.print("Duty cycle (%)");
  }

  //Increment
  lcd.setCursor(0, 1);
  if (displayMode == mFreq) {
    lcd.print("I=" + String(incValF));
  }

  else if (displayMode == mDuty) {
    lcd.print("I=" + String(incValD));
  }


  //Current freq/duty

  if (displayMode == mFreq) {
    String freqString;
    freqString = String(freq, 1);
    lcd.setCursor(15, 1);
    lcd.rightToLeft();
    //Right justify
    int fLength = freqString.length();
    String invFreqString = "";
    for (int i = 0; i < fLength; i++) {
      invFreqString += freqString[fLength - 1 - i];
    }
    while (invFreqString.length() < 7) {
      invFreqString += " "; //Extra space
    }
    lcd.print(invFreqString);
    lcd.leftToRight();
  }

  else if (displayMode == mDuty) {

    String dutyString;
    dutyString = String(duty, 1);
    lcd.setCursor(15, 1);
    lcd.rightToLeft();
    //Right justify
    int dLength = dutyString.length();
    String invDutyString = "";
    for (int i = 0; i < dLength; i++) {
      invDutyString += dutyString[dLength - 1 - i];
    }
    while (invDutyString.length() < 7) {
      invDutyString += " "; //Extra space
    }
    lcd.print(invDutyString);
    lcd.leftToRight();
  }

}

