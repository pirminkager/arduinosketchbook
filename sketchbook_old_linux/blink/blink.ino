const int ledPin = 13;

int tempvar = 30;
int count = 0;
int blinklen = 0;
int getTemp = LOW;
int ledState = LOW;
int ledBlink = LOW;
long previousMillis = 0;
unsigned long currentMillis = 0;

long intervalOff = 5000;
long intervalOn = 1;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
}
void loop() {
//Blink
  delay(100);
  currentMillis = millis();
  Serial.println(currentMillis);
  if (ledState == LOW) {
    //Serial.println("if1.0");
    if (currentMillis - previousMillis > intervalOff) {
      previousMillis = currentMillis;
      ledState = HIGH;
      //digitalWrite(ledPin, ledState);
      getTemp = HIGH;
      //Serial.println("if1.1");
    }
  }
  if (getTemp) {
    blinklen = tempvar;
    getTemp = LOW;
    //Serial.println("if2.0");
  }
  if (ledState == HIGH) {
    //Serial.println("if3");
    if (currentMillis - previousMillis > intervalOn) {
      previousMillis = currentMillis;
      //Serial.println("if3.1");
      if (count >= blinklen) {
        //Serial.println("if3.2");
        ledState = LOW;
        count = 0;
        digitalWrite(ledPin, ledState);
      }
      else {
        //Serial.println("else1");
        if (ledBlink == LOW) {
          //Serial.println("if3.3");
          count = count + 1;
          ledBlink = HIGH;
          digitalWrite(ledPin, ledBlink);
          Serial.println(count);
        }
        else {
          //Serial.println("else2");
          ledBlink = LOW;
          digitalWrite(ledPin, ledBlink);
        }
      }
    }
  }
}
