
#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 6
CRGB leds[NUM_LEDS];
int a=255;
int b=0;
int c=0;
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
    leds[dot] = CRGB::Black;
    //delay(1);
  }
  for(int dot = NUM_LEDS-2; dot > 0; dot--) { 
    leds[dot].setRGB(a,b,c);
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    //delay(1);   
  }
}

