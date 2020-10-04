#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>

// Params for width and height
const uint8_t kMatrixWidth = 30; // length of string
const uint8_t kMatrixHeight = 30;
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

CRGB leds[NUM_LEDS];

bool kMatrixSerpentineLayout = true;

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if (x > kMatrixWidth) return 0;
  if (y > kMatrixHeight) return 0;

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

  if (i >= (NUM_LEDS - 1)) return 0;
  return i;
}

#include "snake.h"
#define MAX_SNAKES 4
GameSnake snakes[MAX_SNAKES];
CRGB playerColors[MAX_SNAKES] = {CRGB::Blue, CRGB::DarkMagenta, CRGB::Yellow, CRGB::OrangeRed };

#if defined(CORE_TEENSY)
#include "control_teensy.h"
#else
#include "control_esp.h"
#endif

void setup() {
  Serial.begin(115200);
  leds[0] = CRGB::Red;
  FastLED.setBrightness(50);
  FastLED.show();

  controlSetup();
  FastLED.addLeds<WS2812, 2, GRB>(leds, NUM_LEDS); //.setCorrection(TypicalSMD5050);

}

int incomingByte = 0;
void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    snakes[0].input(incomingByte);
  }
  controlLoop();
  EVERY_N_MILLISECONDS( snakes[0].d ) {
    for (int s = 0; s < MAX_SNAKES; s++) {
      snakes[s].frame();
    }
    FastLED.show();
    for (int s = 0; s < MAX_SNAKES; s++) {
      snakes[s].frameClear();
    }
  }
}
