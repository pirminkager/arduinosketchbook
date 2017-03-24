#include <math.h>

int table[] = {192,
249,
164,
176,
153,
146,
130,
248,
128,
144,
64,
121,
36,
48,
25,
18,
2,
120,
0,
16,
255};

int tablesel[] = {1,2,4,8};

int value[4] = {18,18,18,18};

unsigned long previousMillis = 0;
const long interval = 1000;

//int seldig1 = 8;
//int seldig2 = 9;
//int seldig3 = 10;
//int seldig4 = 11;
int i = 1;
int d = 0;
int c = 0;
float steigung = -0.2092;
float achsenabschnitt = 204.31;
void setup() {
  DDRB = 0b111111;
  DDRD = 0xff; 
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    gettemp();
  }  
  PORTD = table[value[0]];
  //delay(i);
  PORTB = tablesel[0];
  //PORTD = table[d];
  delay(i);
  PORTD = table[value[1]];
  //delay(i);
  PORTB = tablesel[1];
  //PORTD = table[d+1];
  delay(i);
  PORTD = table[value[2]];
  //delay(i);
  PORTB = tablesel[2];
  //PORTD = table[d+2];
  delay(i);
  PORTD = table[value[3]];
  //delay(i);
  PORTB = tablesel[3];
  //PORTD = table[d+3];
  delay(i);
  PORTD = table[20];
}

void gettemp() {
  //Read Analogvalue and Calculate Integer
  int sensorValue = analogRead(A5);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V) or to temp!
  float rawFloat = (steigung*sensorValue+achsenabschnitt)*10;// * (5.0 / 1023.0);
  int rawValue = rawFloat;
  if (rawValue >= 1000) {
   value[0] = floor(rawValue/1000);
   rawValue = rawValue - value[0] * 1000;
  }
  else {
   value[0] = 20;
  }
  if (rawValue >= 100) {
   value[1] = floor(rawValue/100);
   rawValue = rawValue - value[1] * 100;
  }
  else if (value[0] != 20) {
   value[1] = 0;
  }
  else {
   value[1] = 20;
  }
  if (rawValue >= 10) {
   value[2] = floor(rawValue/10);
   rawValue = rawValue - value[2] * 10;
  }
  else if (value[1] != 20) {
   value[2] = 0;
  }
  else {
   value[2] = 0;
  }
  if (rawValue >= 0) {
   value[3] = rawValue;
  }
  else if (value[2] != 20) {
   value[3] = 0;
  }
  else {
   value[3] = 0;
  }
  value[2] = value[2]+10;
  
  //for (int i=0;i<4;i++) {
    //somecode;
  //}
  //return sensorValue
}

