#include <LiquidCrystal.h>
#define NUM_TIMER 10

LiquidCrystal lcd(12,11,7,8,9,10);
#include "customsymbols.h"

const int tempsolarPin=A0;
const int tempboilerPin=A1;
const int sourceButton=4;
const int modeButton=3;
const int valvestatePin1=1;
const int valvestatePin2=2;
const int togglevalvePin=0;
const int backlightredPin=5;
const int backlightbluePin=6;
//int animInterval = 300; //Necesary?
int animNumber=1;
unsigned int rawsolarValue=0;
unsigned int rawboilerValue=0;
unsigned int nReads=0;
long previousanimMillis;
float tempsolarValue;
float tempboilerValue;
float tempSolar;
float tempBoiler;
double rawValue=0;
//unsigned long timerStart;
//unsigned long timer;
const unsigned int nIterations=50;
const int debounceTime = 20;
volatile boolean sourcebuttonFlag=false;
volatile boolean modebuttonFlag=false;
volatile boolean sourceSelect=false;
volatile boolean modeSelect=false;
boolean stateS;
boolean stateB;
boolean previousvalveState=false;
char *mode[] = {"A","M"};
long timerValue[NUM_TIMER];


//void getsensorData() {
//  //for(int i=0; i<=nIterations; i++) {
//  if (nReads<nIterations) {
//    rawValue=rawValue+analogRead(tempsolarPin);
//  }
//  tempsolarValue=rawValue/nIterations;
//  rawValue=0;
//  for(int i=0; i<=nIterations; i++) {
//    rawValue=rawValue+analogRead(tempboilerValue);
//  }
//  tempboilerValue=rawValue/nIterations;
//  rawValue=0;
//}

void getsensorData() {
  rawsolarValue=rawsolarValue+analogRead(tempsolarPin);
  rawboilerValue=rawboilerValue+analogRead(tempboilerPin);
  nReads++;
  if (nReads==nIterations) {
    tempboilerValue=rawboilerValue/nIterations;
    tempsolarValue=rawsolarValue/nIterations;
    nReads=0;
    tempBoiler=calcfloatboiler(tempboilerValue);
    tempSolar=calcfloatsolar(tempsolarValue);
  }
}

float calcfloatsolar(int temp) {
  double y;
  double A=4;
  double B=5;
  double C=6;
  double D=7;
  
  y = A*pow(temp,3)+B*pow(temp,2)+C*temp+D;
  
  return y; 
}

float calcfloatboiler(int temp) {
  double y;
  double A=4;
  double B=5;
  double C=6;
  double D=7;
  
  y = A*pow(temp,3)+B*pow(temp,2)+C*temp+D;
  
  return y; 
}

void modebuttonDown() {
  modebuttonFlag = true;
  detachInterrupt (digitalPinToInterrupt (modeButton));
}

void sourcebuttonDown() {
  sourcebuttonFlag = true;
  detachInterrupt (digitalPinToInterrupt (sourceButton));
}

void modeSelection() {
  if (modebuttonFlag==true) {
    if (modeSelect==false) {
      modeSelect=true;
    }
    else {
      modeSelect=false;
    }
    deBounce(modeButton);
    EIFR = bit (INTF1);
    modebuttonFlag=false;
    attachInterrupt (digitalPinToInterrupt (modeButton), modeSelection, FALLING);
    
  }
}

void sourceSelection() {
  if (sourcebuttonFlag==true) {
    if (sourceSelect==false) {
      sourceSelect=true;
    }
    else {
      sourceSelect=false;
    }
    deBounce(sourceButton);
    EIFR = bit (INTF0);
    sourcebuttonFlag=false;
    attachInterrupt (digitalPinToInterrupt (sourceButton), sourceSelection, FALLING);
  }
}

void deBounce (int buttonPin) {
  unsigned long now = millis ();
  do {
    // on bounce, reset time-out
    if (digitalRead (buttonPin) == LOW)
      now = millis ();
  } 
  while (digitalRead (buttonPin) == LOW || (millis () - now) <= debounceTime);

}  // end of deBounce

void lcdSend (int col, int row, int len, String string) {
  for (int i=string.length();i<len;i++) {
    string += " ";
  }
  lcd.setCursor(col,row);
  lcd.print(string);
}

void lcdcustomSend (int col, int row, int character ) {
  lcd.setCursor(col,row);
  lcd.write(bool(character));
}

void checkvalvestate() {
  stateS=digitalRead(valvestatePin1);
  stateB=digitalRead(valvestatePin2);
}

void animation() {
  if (!stateS && !stateB) {
    if (!previousvalveState) {
      previousvalveState=true;
    }
//    if(millis() - previousanimMillis >= animInterval) { 
//      previousanimMillis = millis();
//      animNumber++;
//    }
//    lcd.setCursor(0,1);
//    lcd.write(byte(animNumber));
    lcdcustomSend (0,1,animNumber);
    animNumber++;
    if (animNumber == 3) {
      animNumber = 1;
    }
  }
  else if (previousvalveState && ( stateS || stateB )) {
    lcdcustomSend (0,1,5);
    previousvalveState=false;
  }
}

boolean timer(long interval, int timerCount) {
  long newValue = millis();
  if(newValue - timerValue[timerCount] > interval) {
    timerValue[timerCount]=newValue;
    return true;
  }
  else {
    return false;
  }
}
//void displayData () {
//  lcd.setCursor(0,0);
//  if (modeSelect) {
//    lcd.print("M");
//  }
//  else {
//    lcd.print("A");
//  }
//  
//  lcd.setCursor(2,0);
//  if (sourceSelect) {
//    lcd.write(byte(0));
//  }
//  else {
//    lcd.write(" ");
//  }
//  
//  lcd.setCursor(2,1);
//  if (!sourceSelect) {
//    lcd.write(byte(0));
//  }
//  else {
//    lcd.write(" ");
//  }
//}
    
void setup() {
  attachInterrupt (digitalPinToInterrupt (modeButton), modeSelection, FALLING);
  attachInterrupt (digitalPinToInterrupt (sourceButton), sourceSelection, FALLING);
  
  lcd.createChar(0, arrow);
  lcd.createChar(1, anim1);
  lcd.createChar(2, anim2);
  lcd.createChar(3, anim3);
  lcd.createChar(4, anim4);
  lcd.createChar(5, empty);
  lcd.begin(16,2);
  lcd.print("SOLAR TEMP");
  
  pinMode(togglevalvePin, OUTPUT);
  pinMode(valvestatePin1, INPUT);
  pinMode(valvestatePin2, INPUT);
  pinMode(modeButton, INPUT);
  pinMode(sourceButton, INPUT);
  
  //initialize stored timervalues!
  for(int i=0; i < NUM_TIMER; i++) {
    timerValue[i] = millis();
  }
  
}

void loop() {
  
  //readSensor and Calculate Temperature.
  if (timer(10000,0)) {
    
  }
  
  //check Valvestate and Animation
  if (timer(2000,1)) {
    checkvalvestate();
    animation();
  }
  
  
}
