#ifdef ESP32
  #define FASTLED_ALLOW_INTERRUPTS 0
  // Params for width and height
  const uint8_t kMatrixWidth = 8; // length of string
  const uint8_t kMatrixHeight = 8;
  bool kMatrixSerpentineLayout = false;
#elif
  // Params for width and height
  const uint8_t kMatrixWidth = 30; // length of string
  const uint8_t kMatrixHeight = 30;
  bool kMatrixSerpentineLayout = true;
#endif

#include <FastLED.h>

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];


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

#ifdef ESP32
#include "control_esp.h"
#else
#include "control_teensy.h"
#endif

void setup() {
  Serial.begin(115200);
  leds[0] = CRGB::Red;
  FastLED.setBrightness(150);
  FastLED.show();

  controlSetup();
  
  #ifdef ESP32
    FastLED.addLeds<WS2812, 2, GRB>(leds, NUM_LEDS); //.setCorrection(TypicalSMD5050);
  #elif
    #define LED_PIN 7
    #define CLOCK_PIN 14
    #define COLOR_ORDER BGR
    FastLED.addLeds<APA102, LED_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(8)>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  #endif

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
