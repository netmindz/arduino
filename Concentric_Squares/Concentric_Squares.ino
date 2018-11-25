#include <FastLED.h>

// Params for width and height
const uint8_t kMatrixWidth = 30;
const uint8_t kMatrixHeight = 30;

#define LED_PIN  7

#define COLOR_ORDER RGB
#define CHIPSET     APA102

#define BRIGHTNESS 5

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;


uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, 14, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}

int hue = 0;
void loop() {
  FastLED.clear();
  for(int i = 0; i < (kMatrixWidth / 2); i++) {
    square(i, CHSV(hue, 255, 255));
    hue += 15;
  }
  FastLED.delay(100);
}

void square(int s, CRGB color) {
  int p = (kMatrixWidth - 1) - s;
  for(int i = s; i <= p; i++) {
    leds[XY(i,s)] = color;
  }
  for(int i = s; i <= p; i++) {
    leds[XY(s,i)] = color;
  }
  for(int i = s; i <= p; i++) {
    leds[XY(p,i)] = color;
  }
  for(int i = s; i <= p; i++) {
    leds[XY(i,p)] = color;
  }
}

