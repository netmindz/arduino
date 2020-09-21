/*
  Copyright (c) 2014-2016 NicoHood
  See the readme for credit to other people.

  MSGEQ7 FastLED example
  Output via Led strip and FastLED library

  Reads MSGEQ7 IC with 7 different frequencies from range 0-255
  63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
*/

// FastLED
#include "FastLED.h"

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B // WS2811 LPD8806
#define NUM_LEDS    241

#define BRIGHTNESS  255  // reduce power consumption
#define LED_DITHER  255  // try 0 to disable flickering
#define CORRECTION  TypicalLEDStrip

CRGB leds[NUM_LEDS]; // Define the array of leds

// MSGEQ7
#include "MSGEQ7.h"
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH true

// ESP8266
//#define LED_PIN     D4
//#define pinAnalogLeft A0
//#define pinAnalogRight A0
//#define pinReset D3
//#define pinStrobe D2

// ESP32
#define LED_PIN 19
#define pinAnalogLeft 4
#define pinAnalogRight 4
#define pinReset 34
#define pinStrobe 35



CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

void setup() {

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  
  // FastLED setup
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(CORRECTION);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setDither(LED_DITHER);
  FastLED.show(); // needed to reset leds to zero

  leds[0] = CRGB::Red;
  FastLED.delay(2000);

  // This will set the IC ready for reading
  MSGEQ7.begin();
}

void loop() {
  // Analyze without delay
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Led strip output
  if (newReading) {
    for(int i = 0; i < 7; i++) { 
      // visualize the average bass of both channels
      uint8_t val = MSGEQ7.get(i);
  
      // Reduce noise
      val = mapNoise(val);
  
      // Visualize leds to the beat
      CRGB color = ColorFromPalette(currentPalette, val, 255, currentBlending);
      color.nscale8_video(val);
      setRing(i, color);
    }

    // Update Leds
    FastLED.show();
  }
}


void setRing(int ring, CRGB colour) {
  int offset = 0;
  int count = 0;
  switch (ring) {
    case 1:
      count = 1;
      break;
    case 2:
      offset = 1;
      count = 8;
      break;
    case 3:
      offset = 9;
      count = 12;
      break;
    case 4:
      offset = 21;
      count = 16;
      break;
    case 5:
      offset = 37;
      count = 24;
      break;
    case 6:
      offset = 61;
      count = 32;
      break;
    case 7:
      offset = 93;
      count = 40;
      break;
    case 8:
      offset =133;
      count = 48;
      break;
    case 9:
      offset = 181;
      count = 60;
      break;
  }
  for (int i = 0; i < count; i++) {
    leds[(offset + i)] = colour;
  }
}

