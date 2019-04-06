#include <FastLED.h>

#define LED_PIN  6

#define COLOR_ORDER RGB
#define CHIPSET     WS2811

#define NUM_LEDS 900

CRGB leds[NUM_LEDS];      //naming our LED array

#include "stars.h"

void setup() {
//  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<APA102, 7, 14, RGB, DATA_RATE_MHZ(8)>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  FastLED.setBrightness( 130 );
  FastLED.setDither(0);

  for (int  i = 0; i < NUM_LEDS; i++) {
    stars[i].setPixel(i);
  }
}

void loop() {
  showStars();
}

int p = 0;
int hue = 0;



