// Definitions

#define statusLed 2
#define sensorLed 13
#define sensorPin A5 //Digital Pin Number 19
#define dacPin A0
#define opampPin A1

const unsigned long windowSize = 2000; //millseconds
unsigned long startTime = 0;
unsigned long currentTime = 0;
unsigned long sampleTime = 0;
unsigned long sampleLength = 0;
unsigned long sensorTime = 0;
bool sensorState = 0;
bool previousState = 0;
bool noMeasure = 1;
uint8_t adcOutput = 0;

void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(sensorLed, OUTPUT);

  digitalWrite(statusLed, HIGH);

  DDRB=0b11111111;

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  //sensorState = analogRead(sensorPin);
  if (analogRead(sensorPin) >= 550) {
    sensorState = 1;
  }
  else {
    sensorState = 0;
  }
  if (currentTime - startTime <= windowSize) {
    if (sensorState) {
      if (!previousState) {
        previousState = 1;
        noMeasure = 0;
        sampleTime = currentTime;
        digitalWrite(sensorLed, HIGH);
      }
    }
    else {
      if (previousState) {
        previousState = 0;
        sensorTime = sensorTime + currentTime - sampleTime;
        digitalWrite(sensorLed, LOW);
      }
    }
  }
  else {
    if (sensorState & !noMeasure) {
        sensorTime = sensorTime + currentTime - sampleTime;
    }
    adcOutput = (sensorTime * 63 / windowSize);// & 0b00111111;
    Serial.print("Time: ");
    Serial.println(sensorTime);
    Serial.print("ADC Value: ");
    Serial.println(adcOutput);
    PORTB=0b11000000 | adcOutput;
    startTime = currentTime;
    sensorTime = 0;
    previousState = 0;
    noMeasure = 1; //test this
  }
}
