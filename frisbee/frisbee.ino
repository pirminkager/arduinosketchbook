#include "FastLED.h" // FastLED library.
#include "ramp2.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 3                                             // Serial data pin
#define LED_CK 11                                             // Clock pin for WS2801 or APA102
#define COLOR_ORDER GRB                                       // It's GRB for WS2812B and GBR for APA102
#define LED_TYPE WS2812B                                     // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 10                                           // Number of LED's
DEFINE_GRADIENT_PALETTE( palette_gp1 ) {
  0,     0,  0,  0,
 64,     0,  0,  0,
128,   255,  0,  0,
192,     0,  0,  0,
255,     0,  0,  0 };
DEFINE_GRADIENT_PALETTE( palette_gp2 ) {
  0,     0,  0,  0,
 64,     0,  0,  0,
128,     0,255,  0,
192,     0,  0,  0,
255,     0,  0,  0 };
DEFINE_GRADIENT_PALETTE( palette_gp3 ) {
  0,     0,  0,  0,
 64,     0,  0,  0,
128,     0,  0,255,
192,     0,  0,  0,
255,     0,  0,  0 };
CRGBPalette16 myPalr = palette_gp1;
CRGBPalette16 myPalg = palette_gp2;
CRGBPalette16 myPalb = palette_gp3;

// Initialize changeable global variables.
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Initialize global variables for sequences
uint8_t thisdelay = 8;                                        // A delay value for the sequence(s)
uint8_t heatindex = 0;                                          // Starting hue value.
uint8_t deltahue = 10;                                        // Hue change between pixels.

int actual_leds = NUM_LEDS + 5;
int velocity = 10;

void setup() {

  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);         // For WS2812B

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(3.7, 200);               // FastLED 2.1 Power management set at 5V, 500mA
} // setup()
  

void loop () {
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking routine to update/display the sequence.
    animation();
  }
  show_at_max_brightness_for_power();  
} // loop()


void animation() {                                                        // The fill_rainbow call doesn't support brightness levels
  heatindex=heatindex+velocity;                                           // Increment the starting hue.
  for(int i = 0; i < NUM_LEDS; i++) {
    if (i == 7) {
    }
    uint8_t test = ramp[heatindex];
    leds[i] = ColorFromPalette( myPalr, heatindex+((i*255)/actual_leds)); // normal palette access
  }
} // rainbow_march()
