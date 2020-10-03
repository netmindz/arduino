#include <SmartLEDShieldV4.h>  // comment out this line for if you're not using SmartLED Shield V4 hardware (this line needs to be before #include <SmartMatrix3.h>)
#include <SmartMatrix3.h>
#include <FastLED.h>

#define COLOR_DEPTH 24                  // This sketch and FastLED uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kMatrixWidth = 64;        // known working: 32, 64, 96, 128
const uint8_t kMatrixHeight = 64;       // known working: 16, 32, 48, 64
const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_64ROW_MOD32SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);

#define BRIGHTNESS 55

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

void loop()
{
  vortex();
}


void ledtest() {
  int hue = 0;
  for (int y = 0; y < kMatrixHeight; y++) {
    for (int x = 0; x < kMatrixWidth; x++) {
      rgb24 *buffer = backgroundLayer.backBuffer();
      buffer[ XY(x, y)]  = CRGB(CHSV( hue, 255, 255));
      backgroundLayer.swapBuffers();
      delay(10);
      buffer = backgroundLayer.backBuffer();
      buffer[ XY(x, y)]  = CRGB(CRGB::Black);
      hue += 5;
      if (hue > 255) hue = 0;
    }
  }

}

void setup() {
  //  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  //  FastLED.addLeds<APA102, 7, 14, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  matrix.addLayer(&backgroundLayer);
  matrix.addLayer(&scrollingLayer);
  matrix.begin();
  matrix.setBrightness( BRIGHTNESS );
//  ledtest();
}
