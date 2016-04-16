/*
 * Based on SpectrumAnalyzerBasic by Paul Stoffregen included in the Teensy Audio Library
 * Modified by Jason Coon for the SmartMatrix Library
 * Requires Teensyduino 1.20 and the Teensy Audio Library
 * Also requires FastLED 3.0 or higher
 * If you are having trouble compiling, see
 * the troubleshooting instructions here:
 *
 * Requires the following libraries:
 * Teensy Audio Library: https://github.com/PaulStoffregen/Audio
 * FastLED v3.0 or higher: https://github.com/FastLED/FastLED/releases
 *
 * Uses line in on pin A0.  For more information, and a recommended analog input circuit, see: http://www.pjrc.com/teensy/gui/?info=AudioInputAnalog
 *
 * You can change the pin used for ADC with the ADC_INPUT_PIN definition below.
 * There are no dedicated ADC pins brought out on the SmartMatrix Shield,
 * but even if you've used all the pins on the SmartMatrix expansion header,
 * you can use solder pins directly to the Teensy to use A14/DAC, A11, or A10
 *
 */

// all these libraries are required for the Teensy Audio Library
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Audio.h>
#include <SerialFlash.h>
#include <FastLED.h>

#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kMatrixWidth = 16;        // known working: 32, 64, 96, 128
const uint8_t kMatrixHeight = 8;       // known working: 16, 32, 48, 64
const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const bool    kMatrixSerpentineLayout = true;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);


#define ADC_INPUT_PIN   A0

AudioInputAnalog         input(ADC_INPUT_PIN);
AudioAnalyzeFFT256       fft;
AudioConnection          audioConnection(input, 0, fft, 0);

// The scale sets how much sound is needed in each frequency range to
// show all 32 bars.  Higher numbers are more sensitive.
float scale = 256.0;

// An array to hold the 16 frequency bands
float level[16];

// This array holds the on-screen levels.  When the signal drops quickly,
// these are used to lower the on-screen level 1 bar per update, which
// looks more pleasing to corresponds to human sound perception.
int shown[16];

byte status = 0;

void setup()
{
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, 6>(leds, (kMatrixWidth * kMatrixHeight));
  FastLED.setBrightness(50);

  Serial.println("Setup");

  // Audio requires memory to work.
  AudioMemory(12);
}

void loop()
{
  if (fft.available()) {
    // read the 128 FFT frequencies into 16 levels
    // music is heard in octaves, but the FFT data
    // is linear, so for the higher octaves, read
    // many FFT bins together.

    // I'm skipping the first two bins, as they seem to be unusable
    // they start out at zero, but then climb and don't come back down
    // even after sound input stops
    level[0] = fft.read(2);
    level[1] = fft.read(3);
    level[2] = fft.read(4);
    level[3] = fft.read(5, 6);
    level[4] = fft.read(7, 8);
    level[5] = fft.read(9, 10);
    level[6] = fft.read(11, 14);
    level[7] = fft.read(15, 19);
    level[8] = fft.read(20, 25);
    level[9] = fft.read(26, 32);
    level[10] = fft.read(33, 41);
    level[11] = fft.read(42, 52);
    level[12] = fft.read(53, 65);
    level[13] = fft.read(66, 82);
    level[14] = fft.read(83, 103);
    level[15] = fft.read(104, 127);

    //fill_solid(leds , (kMatrixWidth * kMatrixHeight), CRGB::Black);
    fadeToBlackBy(leds, (kMatrixWidth * kMatrixHeight), 90);


    Serial.println("FFT");

    for (int i = 0; i < 16; i++) {
      // TODO: conversion from FFT data to display bars should be
      // exponentially scaled.  But how to keep it a simple example?
      int val = level[i] * scale;

      // trim the bars vertically to fill the matrix height
      if (val >= kMatrixHeight) val = kMatrixHeight - 1;

      if (val >= shown[i]) {
        shown[i] = val;
      }
      else {
        if (shown[i] > 0) shown[i] = shown[i] - 1;
        val = shown[i];
      }

      // color hue based on band
      CRGB color = CRGB(CHSV(i * 15, 255, 255));

      // draw the levels on the matrix
      if (shown[i] >= 0) {
        // scale the bars horizontally to fill the matrix width
        for (int j = 0; j < kMatrixWidth / 16; j++) {
          leds[XY((i * 2 + j), ((kMatrixHeight - 1) - val))] =  color;
        }
      }
    }

    FastLED.show();
  }
}
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

  if(i > ((kMatrixWidth * kMatrixHeight) - 1)) {
      Serial.println("Off the edge");
      i = 0;
  }
  return i;
}

