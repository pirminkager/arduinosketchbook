const int ledPin = 13;

int tempvar = 3;
int count = 0;
int blinklen = 0;
int getTemp = LOW;
int ledState = LOW;
int ledBlink = LOW;
long previousMillis = 0;
unsigned long currentMillis = 0;

long intervalOff = 5000;
long intervalOn = 1000;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
}
void loop() {
//Blink
  currentMillis = millis();
  if (ledState == LOW) {
    if (currentMillis - previousMillis > intervalOff) {
      previousMillis = currentMillis;
      ledState = HIGH;
      getTemp = HIGH;
    }
  }
  if (getTemp) {
    blinklen = tempvar;
    getTemp = LOW;
  }
  if (ledState == HIGH) {
    if (currentMillis - previousMillis > intervalOn) {
      previousMillis = currentMillis;
      if (count >= blinklen) {
        ledState = LOW;
        count = 0;
        digitalWrite(ledPin, ledState);
      }
      else {
        if (ledBlink == LOW) {
          count = count + 1;
          ledBlink = HIGH;
          digitalWrite(ledPin, ledBlink);
          Serial.println(count);
        }
        else {
          ledBlink = LOW;
          digitalWrite(ledPin, ledBlink);
        }
      }
    }
  }
}
