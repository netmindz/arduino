/*
   DMX Required Connections
   --------------------
   pin 0: RO
   DE + RE = LOW

*/

// **********************************************************************************************************

#define FASTLED_ALLOW_INTERRUPTS   1 // setting 0 fixes flutter, causes crash
//#define FASTLED_INTERRUPT_RETRY_COUNT 0

#define LED_PIN  7
#define MIC_PIN  A0

// #define CHIPSET NEOPIXEL

const uint8_t kMatrixWidth = 1; // numer of strings
const uint8_t kMatrixHeight = 50; // length of string
#define NUM_LEDS_PER_STRIP 50
#define NUM_STRIPS 8


#define NUM_AUDIO_LEDS NUM_LEDS_PER_STRIP // should same as length of string

#define NUM_LEDS (NUM_STRIPS * NUM_LEDS_PER_STRIP)

const bool    kMatrixSerpentineLayout = true;

#define FRAMES_PER_SECOND  120

// **********************************************************************************************************
#define USE_OCTOWS2811
#include<OctoWS2811.h>

#include <TeensyDmx.h>
#include <FastLED.h>

// **********************************************************************************************************

TeensyDmx Dmx(Serial1);

CRGB leds[NUM_LEDS];
CRGB ledsAudio[NUM_AUDIO_LEDS];

// **********************************************************************************************************

uint8_t STEPS = 150;
uint8_t BRIGHTNESS = 255;
uint8_t SPEEDO = 180;
uint8_t FADE = 180;

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

// **********************************************************************************************************

void autoRun();
void rainbow();
void rainbowWithGlitter();
void confetti();
void sinelon();
void juggle();
void bpm();
void rainbowSweep();
void Rainbow();
void RainbowWash();
void Ripple();
void shimmer();
void sinwave_1();
void snake();
void dsnake();

// ***********************
// Audio values
// ***********************

// Global timer value
uint8_t timeval = 20;                                                           // Currently 'delay' value. No, I don't use delays, I use EVERY_N_MILLIS_I instead.
uint16_t loops = 0;                                                             // Our loops per second counter.

// Global sound variables used in other routines.
uint16_t oldsample = 0;                                                         // Previous sample is used for peak detection and for 'on the fly' values.
bool     samplepeak = 0;                                                        // The oldsample is well above the average, and is a 'peak'.
uint16_t sampleavg = 0;                                                         // Average of the last 64 samples.


// Global visual variables used in display and other routines.
bool thisdir = 0;

#include "soundmems.h"                                                          // Sample the sounds and provide a current sample, average of last 64 samples and boolean peak (for twinkles).
#include "support.h"                                                            // A few extra routines for good luck.

// Use qsuba for smooth pixel filtering and qsubd for digital pixel filtering.
#define qsubd(x, b)  ((x>b)?b:0)                                                // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                                              // Analog Unsigned subtraction macro. if result <0, then => 0. By Andrew Tuline.

// Main sound reactive routines
#include "pixels.h"         // Long line of colours
#include "fillnoise.h"      // Center to edges with base color and twinkle
#include "jugglep.h"        // Long line of sinewaves
#include "ripple.h"         // Juggle with twinkles
#include "pixel.h"          // Long line of colours
#include "matrix.h"         // Start to end with twinkles
#include "onesine.h"        // Long line of shortlines
#include "noisefire.h"      // Start to end
#include "rainbowbit.h"     // Long line of short lines with twinkles
#include "noisefiretest.h"  // Center to edges
#include "rainbowg.h"       // Long line with flashes
#include "noisewide.h"      // Center to edges
#include "plasma.h"         // Long line of short lines
#include "besin.h"          // center to edges with black
#include "noisepal.h"       // Long line


// *************
// End audio
// *************


typedef void (*SimplePatternList[])();
// snake, 
SimplePatternList gPatterns = { autoRun, rainbowSweep, Rainbow, dsnake, RainbowWash, Ripple, shimmer, sinwave_1, rainbow, confetti, sinelon, juggle, bpm, rainbowWithGlitter, pixels, fillnoise, jugglep, ripple, pixel, matrix, onesine, noisefire, rainbowbit, noisefiretest, rainbowg, noisewide, plasma, besin, noisepal, pixels, fillnoise, jugglep, ripple, pixel, matrix, onesine, noisefire, rainbowbit, noisefiretest, rainbowg, noisewide, plasma, besin, noisepal
};
// shimmer, confetti, sinelon,
SimplePatternList gAutoPatterns = { rainbowSweep, Rainbow, dsnake, RainbowWash, Ripple, sinwave_1, rainbow,  bpm };

// **********************************************************************************************************
// Setup
// **********************************************************************************************************

void setup() {
  /* USB serial */
  Serial.begin(115200);

  Dmx.setMode(TeensyDmx::DMX_IN);

  pinMode(LED_BUILTIN, OUTPUT);

  // FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  LEDS.addLeds<OCTOWS2811, RGB>(leds, NUM_LEDS_PER_STRIP);
  
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("Setup");
  ledtest();
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
int gPatternCount = ARRAY_SIZE(gPatterns);
int gAutoPatternCount = ARRAY_SIZE(gAutoPatterns);

int led = 0;
elapsedMillis elapsed;

// **********************************************************************************************************
// Main
// **********************************************************************************************************
int pattern = 0;
void loop()
{
    Dmx.loop();
  if (Dmx.newFrame()) {

    led = !led;
    digitalWrite(LED_BUILTIN, led);

    int b = Dmx.getBuffer()[1];
    if (b != BRIGHTNESS) {
      BRIGHTNESS = b;
      FastLED.setBrightness(BRIGHTNESS);
    }
    STEPS = Dmx.getBuffer()[2];
    SPEEDO = Dmx.getBuffer()[3];
  }

  int p = Dmx.getBuffer()[4];
  int pattern = map(p, 0, 255, 0, (gPatternCount - 1));
  gPatterns[pattern]();

    EVERY_N_SECONDS( 1 ) {
      Serial.print("pattern = ");
      Serial.println(pattern);
    }
    EVERY_N_SECONDS( 10 ) {
      Serial.println(LEDS.getFPS());
    }

}

// **********************************************************************************************************
// Helpers
// **********************************************************************************************************

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
//  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % gAutoPatternCount;
  gCurrentPatternNumber++;
  if(gCurrentPatternNumber >= gAutoPatternCount) gCurrentPatternNumber = 0;
}

void autoRun() {

  gAutoPatterns[gCurrentPatternNumber]();
  
  
  // change patterns periodically
//  EVERY_N_SECONDS( map(STEPS, 0, 255, 10, 100 )) {
  EVERY_N_SECONDS(20) {
    nextPattern();
    Serial.print("Swapping to pattern ");
    Serial.println(gCurrentPatternNumber);
  } 

}
// **********************************************************************************************************
// Matrix Helpers
// **********************************************************************************************************

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if(x > kMatrixWidth) return 0;
  if(y > kMatrixHeight) return 0;

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

  if(i >= (NUM_LEDS - 1)) return 0;
  return i;
}



