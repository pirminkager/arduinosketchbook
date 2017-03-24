
#include <FastLED.h>
#define NUM_LEDS 24
#define DATA_PIN 3
CRGB leds[NUM_LEDS];
int a=80;
int b=0;
int c=20;
int inca=1;
int incb=1;
int incc=1;
int del=100;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}


void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i < NUM_LEDS; i++) { 
    leds[i].setRGB(a,b,c);
    FastLED.show();
    // clear this led for the next time around the loop
    leds[i] = CRGB::Black;
    delay(del);
  }
  for(int dot = NUM_LEDS - 2; dot > 0; dot=dot-1) { 
    leds[dot].setRGB(a,b,c);
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(del);
  }
  //del--;
}

