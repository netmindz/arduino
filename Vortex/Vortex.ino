#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>

// Params for width and height
const uint8_t kMatrixWidth = 8; // length of string
const uint8_t kMatrixHeight = 8;

#define LED_PIN  1

#define COLOR_ORDER RGB
#define CHIPSET     WS2812SERIAL

#define BRIGHTNESS 255

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = false;


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
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if ( x >= kMatrixWidth) return -1;
  if ( y >= kMatrixHeight) return -1;
  return XY(x, y);
}


// Demo that USES "XY" follows code below

void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for ( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;
    for ( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}


void loop()
{
  vortex();
}


void ledtest() {
    int hue = 0;
  for (int y = 0; y < kMatrixHeight; y++) {
    for (int x = 0; x < kMatrixWidth; x++) {
      leds[ XY(x, y)]  = CHSV( hue, 255, 255);
      FastLED.show();
      delay(10);
      leds[ XY(x, y)]  = CRGB::Black;
      hue += 5;
      if (hue > 255) hue = 0;
    }
  }

}

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  ledtest();
}
