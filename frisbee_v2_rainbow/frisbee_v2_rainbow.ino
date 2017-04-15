#include <avr/pgmspace.h>
#include "FastLED.h" // FastLED library.
#include "ramp.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 3                                              // Serial data pin
#define LED_CK 11                                             // Clock pin for WS2801 or APA102
#define COLOR_ORDER GRB                                       // It's GRB for WS2812B and GBR for APA102
#define LED_TYPE WS2812B                                      // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 40                                           // Number of LED's

// Initialize changeable global variables.
uint8_t max_bright = 255;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

// Initialize global variables for sequences
uint8_t thisdelay = 8;                                        // A delay value for the sequence(s)
uint8_t rampindex = 0;                                       // Rampoffset.
uint8_t velocity = 10;                                        // rampindex is increased by velocity everytime the animation is updated
uint8_t hue = 0;                                              // Starting hue value.
uint8_t deltahue = 1;                                        // Hue change between completed maxcyclecount.

// global variables for hue-cyclecount
uint16_t cycle;                                            // Startingvalue for Counting Cycles
uint16_t maxcycle = 3;                                       // Cycle count when hue increase is triggered.

uint8_t actual_leds = NUM_LEDS + 9;                           // missing gaps! (des isch verwirrend... actual_leds sollt eigentlich klianor sein)

void setup() {
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // For WS2812B
  FastLED.setBrightness(max_bright);                            // vieleicht net nötig?
  set_max_power_in_volts_and_milliamps(3.7, 200);               // FastLED 2.1 Power management set at 5V, 500mA
}

void loop() {
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking routine to update/display the sequence.
    animation();
  }
  show_at_max_brightness_for_power();  //strip gets updated
} // end loop

void animation() {                                             // 
  rampindex += velocity;  // 
  cycle++;
  for(int i = 0; i < NUM_LEDS; i++) {
    //if (i==15||i==16||i==17||i==18||i==19||i==20||i==21||i==22||i==23) {}    // led skipping. Could improve smoothness?
    //else {
    uint8_t k = (rampindex+(i*255/actual_leds));
    uint8_t brightness = pgm_read_word_near(ramp + k);
    leds[i] = CHSV(hue,255,brightness); // leds get populated with colorvalues (hue) and brightness (value). Saturation is at max.
    //}
  }
  // increase color hue every n cycle
  if (cycle >= maxcycle) {
    hue += deltahue;
    cycle = 0;
  }
} // end animationmethod

// to adjust colorchanging speed change the variables maxcycle and deltahue (low value better transition. Colorwheel goes from 0-255)!
// to adjust rotating speed change velocity.
// by changing this delay the executioninterval of animation() is changed,
// thus also the colorchangingspeed and rotationanimation speed is changed.
