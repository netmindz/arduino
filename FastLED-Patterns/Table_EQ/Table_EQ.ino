// Example 48.3 - tronixstuff.com/tutorials > chapter 48 - 30 Jan 2013
// MSGEQ7 spectrum analyser shield with a Freetronics DMD
// for DMD
#include "SPI.h" // SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <FastLED.h>
#include "MSGEQ7.h"
#define pinAnalogLeft A0
#define pinAnalogRight A1
#define pinReset 5
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(80)
#define MSGEQ7_SMOOTH true

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

int strobe = 4; // strobe pins on digital 4
int res = 5; // reset pins on digital 5

int band;

#define LED_PIN 2
#define CLOCK_PIN 6

#define COLOR_ORDER BGR
#define CHIPSET     DOTSTAR

#define BRIGHTNESS 20

#define WIDTH 15
#define HEIGHT 7

#define NUM_LEDS 225

CRGB leds[NUM_LEDS];

void setup()
{

  MSGEQ7.begin();

  // LEDS
  LEDS.setBrightness(BRIGHTNESS);
  FastLED.addLeds<DOTSTAR, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

}

void loop()
{
  int xpos;
  // analyze without delay
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Led strip output
  if (newReading) {
    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      xpos = map((band  + 1), 1, 7, 7, 1);
      int count = map(MSGEQ7.get(band), 0, 255, 0, HEIGHT);
      for (int i = 1; i <= WIDTH; i++) {
        if (i <= count) {
          leds[xytopixel(xpos, i)] = CRGB::Red;
        }
        else {
          leds[xytopixel(xpos, i)] = CRGB::Black;
        }
      }
    }

    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      xpos = 7 + band  + 1;
      int count = map(MSGEQ7.get(band), 0, 255, 0, HEIGHT);
      for (int i = 1; i <= WIDTH; i++) {
        if (i <= count) {
          leds[xytopixel(xpos, i)] = CRGB::Green;
        }
        else {
          leds[xytopixel(xpos, i)] = CRGB::Black;
        }
      }
    }

    //  leds[50] = CRGB::Red;

    FastLED.show();
  }
}


int xytopixel(int x, int y) {
  return ((y - 1) * WIDTH) + (x - 1);
}
