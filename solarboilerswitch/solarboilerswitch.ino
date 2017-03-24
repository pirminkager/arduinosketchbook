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
int tempSolar;
int tempBoiler;
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
char buff[5];
long timerValue[NUM_TIMER];
const double calibrationSolar[4]={1,2,3,4};
const double calibrationBoiler[4]={1,2,3,4};

void getsensorData() {
  rawsolarValue=rawsolarValue+analogRead(tempsolarPin);
  rawboilerValue=rawboilerValue+analogRead(tempboilerPin);
  nReads++;
  if (nReads==nIterations) {
    tempboilerValue=rawboilerValue/nIterations;
    tempsolarValue=rawsolarValue/nIterations;
    nReads=0;
    tempBoiler=calcfloattemp(tempboilerValue, calibrationBoiler);
    tempSolar=calcfloattemp(tempsolarValue, calibrationSolar);
    sprintf(buff,"[%2d.%.1d]\n", (int) (tempBoiler/10), (int) (tempBoiler%10));
    lcdsendString(0,10,4,buff);
    sprintf(buff,"[%2d.%.1d]\n", (int) (tempSolar/10), (int) (tempSolar%10));
    lcdsendString(1,10,4,buff);
  }
}

int calcfloattemp(int temp, const double param[]) {
  double y;
  y = (param[0]*pow(temp,3)+param[1]*pow(temp,2)+param[2]*temp+param[3])*10;
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

void lcdsendString (int col, int row, int len, String string) {
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
