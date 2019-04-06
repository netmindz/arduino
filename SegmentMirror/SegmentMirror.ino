#include <FastLED.h>

#define NUM_LEDS_PER_STRIP 50
#define NUM_STRIPS 8
const uint8_t kMatrixWidth = 50; // length of string
const uint8_t kMatrixHeight = 16; // number of strings
#define LED_TYPE WS2811
#define LED_PIN D4
#define COLOR_ORDER BGR

CRGB ledSegment[NUM_LEDS_PER_STRIP];

#define NUM_LEDS (NUM_STRIPS * NUM_LEDS_PER_STRIP)
CRGB leds[NUM_LEDS];

const bool kMatrixSerpentineLayout = true;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  //  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
}

void loop() {
  FillLEDsFromPaletteColors(0);
  showSegments();
  //FastLED.show();
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  for ( int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    ledSegment[i] = ColorFromPalette( RainbowColors_p, colorIndex, brightness, LINEARBLEND);
    colorIndex += 3;
  }
}


void showSegments() {
  for (int s = 0; s < NUM_STRIPS; s++) {
    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      int p = (s * NUM_LEDS_PER_STRIP) + i;
      leds[getPixel(p)] = ledSegment[i];
    }
  }
  FastLED.delay(1000);
}


uint16_t getPixel(int p)
{
  uint16_t i;

  if ( kMatrixSerpentineLayout == false) {
    i = p;
  }

  if ( kMatrixSerpentineLayout == true) {
    float f = ((float)(p + 1) / (float) kMatrixWidth);
    int r = ceil(f);

    if (r & 0x01)   {
      i = p;
    } else {
      i = ((kMatrixWidth * r) - (p % kMatrixWidth)) - 1;
    }
  }

  if (i >= (NUM_LEDS - 1)) return 0;
  return i;
}




