#include "FastLED.h"                                          // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 3                                             // Serial data pin
#define LED_CK 11                                             // Clock pin for WS2801 or APA102
#define COLOR_ORDER GRB                                       // It's GRB for WS2812B and GBR for APA102
#define LED_TYPE WS2812B                                     // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 41                                           // Number of LED's
DEFINE_GRADIENT_PALETTE( palette_gp ) {
  0,     0,  0,  0,
 64,     0,  0,  0,
128,   255,255,  0,
192,     0,  0,  0,
255,     0,  0,  0 };
CRGBPalette16 myPal = palette_gp;

// Initialize changeable global variables.
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Initialize global variables for sequences
uint8_t thisdelay = 8;                                        // A delay value for the sequence(s)
uint8_t heatindex = 0;                                          // Starting hue value.
uint8_t deltahue = 10;                                        // Hue change between pixels.

int actual_leds = NUM_LEDS + 6;                               // add missing leds (gap) for calculation.
int velocity = 10;                                            // incrementing heatindex changes the speed of animation.

void setup() {
//  Serial.begin(57600);

  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);         // For WS2812B
//  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(3.7, 100);               // FastLED 2.1 Power management set at 5V, 500mA
} // setup()
  

void loop () {
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking routine to update/display the sequence.
    rainbow_march();
  }
  show_at_max_brightness_for_power();  
} // loop()


void rainbow_march() {                                        // The fill_rainbow call doesn't support brightness levels
  heatindex=heatindex+velocity;                                                  // Increment the starting hue.
  //fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
  //fill_gradient_RGB(leds, NUM_LEDS, CRGB(0,0,200), CRGB(150,0,20));  // up to 4 CRGB (or long) values
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( myPal, heatindex+((i*255)/actual_leds)); // normal palette access
  }
} // rainbow_march()
