//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

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

int pos1Val = 0;
int pos2Val = 0;
int pos3Val = 0;
int pos4Val = 1;
int pos5Val = 0;
int pos6Val = 0;
bool buttonReady = true;

double incVal = 1;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons

void updateFreq(String dir)
{
  Serial.println(String(freq)); //For debugging
  double newFreq = freq;
  String freqString;
  if (dir == "+") newFreq = freq + incVal; //increase or decrease frequency
  else if (dir == "-") newFreq = freq - incVal;
 

  if ((newFreq >= 0) && (newFreq <= 10000)) freq=newFreq; //

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
  //Serial.println(invFreqString);
  while (invFreqString.length() < 7){
    invFreqString +=" "; //Extra space
  }
  lcd.print(invFreqString);
  lcd.leftToRight();
}


void changeincVal(String dir)
{
  if (dir == "right") {
    Serial.println("1");
    if (incVal == 0.1) return;
    else {
      incVal = incVal / 10;
      //lcd.blink(1,5);
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

int read_LCD_buttons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) {
    return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  }


  if (adc_key_in < 50)   {

    return btnRIGHT;
  }
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  {

    return btnLEFT;
  }
  if (adc_key_in < 790)  return btnSELECT;



  return btnNONE;  // when all others fail, return this...
}


void setup()
{

// Setup du générateur
 pinMode(10, OUTPUT);
 digitalWrite(10,HIGH);
 //Timer1.initialize(150000);
 //Timer1.pwm(10, 12, 15000);
 


  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0, 0);
  lcd.print("Strob. par MxBoud!"); // print a simple message
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("enjoy :)");
  Serial.begin(9600);
}

void loop()
{
  lcd.setCursor(9, 0);
  lcd.print(String(25, HEX));

  


//  lcd.setCursor(9, 1);           // move cursor to second line "1" and 9 spaces over
//  lcd.print(String(freq));      // display seconds elapsed since power-up

  updateFreq("");


  lcd.setCursor(0, 1);           // move to the begining of the second line
  lcd_key = read_LCD_buttons();  // read the buttons
 




  switch (lcd_key)               // depending on which button was pushed, we perform an action
  {
    case btnRIGHT:
      {

        bool buttonReady = false;
        changeincVal("right");
        lcd.print(String(incVal));



        break;
      }
    case btnLEFT:
      {
        bool buttonReady = false;
        changeincVal("left");
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
    case btnSELECT:
      {
        lcd.print("SELECT");
        break;
      }
    case btnNONE:
      {

        lcd.print(String(incVal));
        break;
      }
  }


}

