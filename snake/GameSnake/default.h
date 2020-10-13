#include <FastLED.h>

#ifdef ESP32
#define FASTLED_ALLOW_INTERRUPTS 0
// Params for width and height
const uint8_t kMatrixWidth = 8; // length of string
const uint8_t kMatrixHeight = 8;
bool kMatrixSerpentineLayout = false;
#else
// Params for width and height
const uint8_t kMatrixWidth = 30; // length of string
const uint8_t kMatrixHeight = 30;
bool kMatrixSerpentineLayout = true;
#endif


/*
   Default config file with normal FastLED stuff
*/
#define DATA_PIN    2
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    64
CRGB leds[NUM_LEDS];

void ledSetup() {

#ifdef ESP32
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
#else
  #define LED_PIN 7
  #define CLOCK_PIN 14
  #define COLOR_ORDER BGR
  FastLED.addLeds<APA102, LED_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(8)>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
#endif

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void ledLoop() {
  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  //  FastLED.delay(1000 / FRAMES_PER_SECOND);
}
