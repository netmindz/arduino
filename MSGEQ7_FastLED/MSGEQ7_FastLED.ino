/*
 Copyright (c) 2014-2015 NicoHood
 See the readme for credit to other people.

 MSGEQ7 FastLED example
 Output via Led strip and FastLED library

 Reads MSGEQ7 IC with 7 different frequencies from range 0-255
 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
*/

// FastLED
#include "FastLED.h"

#define LED_PIN 1
#define CLOCK_PIN 6

#define COLOR_ORDER BGR
#define CHIPSET     WS2812
#define NUM_LEDS    120

#define BRIGHTNESS  55  // reduce power consumption
#define DITHER      255  // try 0 to disable flickering
#define CORRECTION TypicalLEDStrip

CRGB leds[NUM_LEDS]; // Define the array of leds

// MSGEQ7
#include "MSGEQ7.h"
//#define pinAnalogLeft A0
//#define pinAnalogRight A1
//#define pinReset 5
//#define pinStrobe 4

// ESP32
#define LED_PIN 19
#define pinAnalogLeft 4
#define pinAnalogRight 4
#define pinReset 34
#define pinStrobe 35

#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH true

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

void setup() {
  // FastLED setup
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(CORRECTION);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setDither(DITHER);
  FastLED.show(); // needed to reset leds to zero

  // this will set the IC ready for reading
  MSGEQ7.begin();
}

void loop() {
  // analyze without delay
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Led strip output
  if (newReading) {
    // visualize the average bass of both channels
    uint8_t val = MSGEQ7.get(MSGEQ7_BASS);

    // reduce noise
 //   val = MSGEQ7.map(val);
    
    // visualize leds to the beat
    CRGB color = CRGB::Blue;
    color.nscale8_video(val);
    fill_solid(leds, NUM_LEDS, color);

    // update Leds
    FastLED.show();
  }
}

