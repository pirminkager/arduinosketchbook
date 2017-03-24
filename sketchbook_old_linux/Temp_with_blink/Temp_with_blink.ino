int led = 13;
int ntc = 0;
int read = 0;
float x = 0.;
float j = 0.;
float k = 0.113853;
float d = 32.30612;
int temp = 0;
int temp2 = 0;
int code = 0;
int table[] = {0,
1,
8,
9,
4,
5,
12,
13,
2,
3,
16,
17,
24,
25,
20,
21,
28,
29,
18,
19,
128,
129,
136,
137,
132,
133,
140,
141,
130,
131,
144,
145,
152,
153,
148,
149,
156,
157,
146,                                                                                                                                      
147,                                                                                                                                              
64,                                                                                                                                               
65,                                                                                                                                               
72,                                                                                                                                               
73,                                                                                                                                               
68,                                                                                                                                               
69,                                                                                                                                               
76,                                                                                                                                               
77,                                                                                                                                               
66,                                                                                                                                               
67,                                                                                                                                               
80,
81,
88,
89,
84,
85,
92,
93,
82,
83,
192,
193,
200,
201,
196,
197,
204,
205,
194,
195,
208,
209,
216,
217,
212,
213,
220,
221,
210,
211,
32,
33,
40,
41,
36,
37,
44,
45,
34,
35,
48,
49,
56,
57,
52,
53,
60,
61,
50,
51};

const int ledPin = 13;

int tempvar = 10;
int count = 0;
int blinklen = 0;
int getTemp = LOW;
int ledState = LOW;
int ledBlink = LOW;
long previousMillis = 0;

long intervalOff = 30000;
long intervalOn = 1000;

// the setup routine runs once when you press reset:
void setup() {
// initialize the digital pin as an output.
  DDRD = 0xff;
//  Serial.begin(9600);
} 

void loop() {   
  delay(100);           
  read = analogRead(ntc);
  temp = int((k * read) - d);
//  Serial.println(temp);
//  if (temp > 99 or temp < 0) {
//    PORTD = table[0];  
//  }
  if (temp > 99) {
    PORTD = table[0];  
  }
  else if (temp < 0) {
    PORTD = table[0];
  }
  else {
    PORTD = table[(temp)-1];
  }
  
  //Blink
  unsigned long currentMillis = millis();
  if (ledState == LOW) {
    if(currentMillis - previousMillis > intervalOff) {
      previousMillis = currentMillis;
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
      getTemp = HIGH;
    }
  }
  if (getTemp) {
    blinklen = tempvar;
    getTemp = LOW;
  }
  if (ledState == HIGH) {
    if(currentMillis - previousMillis > intervalOn) {
      previousMillis = currentMillis;
      if (count > blinklen) {
        ledState = LOW;
        count = 0;
        digitalWrite(ledPin, ledState);
      }
      else {
        if (ledBlink == LOW) {
          count = count + 1;
          ledBlink = HIGH;
          digitalWrite(ledPin, ledBlink);
        }
        else {
          ledBlink = LOW;
          digitalWrite(ledPin, ledBlink);
        }
      }
    }
  }
}
