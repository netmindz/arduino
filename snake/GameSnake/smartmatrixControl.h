#include <SmartLEDShieldV4.h>  // comment out this line for if you're not using SmartLED Shield V4 hardware (this line needs to be before #include <SmartMatrix3.h>)
#include <SmartMatrix3.h>

const uint8_t kZoom = 4;

#define COLOR_DEPTH 24                  // This sketch and FastLED uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kRealMatrixWidth = 64;        // known working: 32, 64, 96, 128
const uint8_t kRealMatrixHeight = 64;       // known working: 16, 32, 48, 64

const uint8_t kMatrixWidth = (kRealMatrixWidth / kZoom);
const uint8_t kMatrixHeight = (kRealMatrixHeight / kZoom);

const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_64ROW_MOD32SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kRealMatrixWidth, kRealMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kRealMatrixWidth, kRealMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kRealMatrixWidth, kRealMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);

const bool    kMatrixSerpentineLayout = false;

#define NUM_LEDS ((kRealMatrixWidth/kZoom) * (kRealMatrixHeight/kZoom))
CRGB leds[NUM_LEDS];

void ledSetup() {
  matrix.addLayer(&backgroundLayer);
  matrix.addLayer(&scrollingLayer);
  matrix.begin();
  matrix.setBrightness( BRIGHTNESS );
}

int getScaledPixel(int i) {
  int srcRowIndex = (i / (kRealMatrixWidth / kZoom));
  int srcCIndex = i % (kRealMatrixWidth / kZoom);
  int pixel = ( (srcRowIndex * kZoom) * kRealMatrixWidth) + ( (srcCIndex * kZoom) );
  //  Serial.printf("i = %i, srcRowIndex = %i, pixel = %i\n", i, srcRowIndex, pixel);
  return pixel;
}

void ledLoop() {
  rgb24 *buffer = backgroundLayer.backBuffer();
  for (int i = 0; i < NUM_LEDS; i++) {
    int j = getScaledPixel(i);
    for (int x = 0; x < kZoom; x++) {
      for (int y = 0; y < kZoom; y++) {
        int p = (j + x + (kRealMatrixWidth * y));
        buffer[p].red = leds[i].r;
        buffer[p].green = leds[i].g;
        buffer[p].blue = leds[i].b;
      }
    }
  }
  backgroundLayer.swapBuffers();
}
