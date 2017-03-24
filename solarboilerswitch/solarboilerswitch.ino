#include <LiquidCrystal.h>
#define NUM_TIMER 10

// Functionparameters
#define A_B 10
#define B_B 20
#define C_B 30
#define D_B 40
#define A_S 10
#define B_S 10
#define C_S 20
#define D_S 12

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
boolean previousvalveState;
boolean undefinedvalveState=false;
char *mode[] = {"A","M"};
char buff[4];
long timerValue[NUM_TIMER];
const double calibrationSolar[4]={A_S,B_S,C_S,D_S};
const double calibrationBoiler[4]={A_B,B_B,C_B,D_B};

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
    sprintf(buff,"%2d.%.1d", (int) (tempBoiler/10), (int) (tempBoiler%10));
    //lcdsendString(0,10,4,buff);
    lcd.setCursor(0,10);
    lcd.write(buff,4);
    sprintf(buff,"%2d.%.1d", (int) (tempSolar/10), (int) (tempSolar%10));
    lcd.setCursor(1,10);
    lcd.write(buff,4);
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
    if (modeSelect==true) {
      lcd.setCursor(0,0);
      lcd.write('A');
    }
    else if (modeSelect==false) {
      lcd.setCursor(0,0);
      lcd.write('M');
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
    if (sourceSelect==true && modeSelect == false) {
      digitalWrite(togglevalvePin, true);
    }
    else if (sourceSelect==false && modeSelect == false) {
      digitalWrite(togglevalvePin, false);
    }
    deBounce(sourceButton);
    EIFR = bit (INTF0);
    sourcebuttonFlag=false;
    attachInterrupt (digitalPinToInterrupt (sourceButton), sourceSelection, FALLING);
  }
}

void automaticMode() {
  if (modeSelect == true) {
    if (tempBoiler - 50 > tempSolar) {
      digitalWrite(togglevalvePin, true);
    }
    else if (tempSolar - 50 > tempBoiler) {
      digitalWrite(togglevalvePin, false);
    }
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
  if (stateS) {
    previousvalveState=0;
    lcdcustomSend (previousvalveState,2,0);
  }
  if (stateB) {
    previousvalveState=1;
    lcdcustomSend (previousvalveState,2,0);
  }
}

void animation() {
  if (!stateS && !stateB) {
    if (!undefinedvalveState) {
      undefinedvalveState=true;
    }
    lcdcustomSend (digitalRead(togglevalvePin),2,animNumber);
    animNumber++;
    if (animNumber == 3) {
      animNumber = 1;
    }
  }
  else if (previousvalveState && ( stateS || stateB )) {
    lcdcustomSend (0,2,5);
    undefinedvalveState=false;
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
  lcd.setCursor(0,3);
  lcd.write("Solar");
  lcd.setCursor(1,3);
  lcd.write("Boiler");
  lcd.setCursor(0,14);
  lcd.print(char(223));
  lcd.write('C');
  lcd.setCursor(1,14);
  lcd.print(char(223));
  lcd.write('C');
  
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
  if (timer(1000,0)) {
    getsensorData();
  }
  
  //check Valvestate and Animation
  if (timer(200,1)) {
    checkvalvestate();
    animation();
    modeSelection();
    sourceSelection();
    automaticMode();
  }
  
}
