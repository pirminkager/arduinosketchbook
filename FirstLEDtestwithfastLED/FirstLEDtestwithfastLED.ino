
#include <FastLED.h>
#define NUM_LEDS 8
#define DATA_PIN 6
CRGB leds[NUM_LEDS];
int a=0;
int b=85;
int c=170;
int inca=1;
int incb=1;
int incc=1;

void setup() { 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}


void loop() {
  // put your main code here, to run repeatedly:
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
    leds[dot].setRGB(a,b,c);
    FastLED.show();
    // clear this led for the next time around the loop
    //leds[dot] = CRGB::Black;
    a=a+inca;
    b=b+incb;
    c=c+incc;
    if((a == 0)||(a == 255)) {
      inca = inca*-1;
    }
    if((b == 0)||(b == 255)) {
      incb = incb*-1;
    }
    if((c == 0)||(c == 255)) {
      incc = incc*-1;
    }
    delay(1);
  }
}
