/*
   DMX Required Connections
   --------------------
   pin 0: RO
   DE + RE = LOW

*/

// **********************************************************************************************************

#define FASTLED_ALLOW_INTERRUPTS   1 // setting 0 fixes flutter, causes crash
//#define FASTLED_INTERRUPT_RETRY_COUNT 0

const uint8_t kMatrixWidth = 50; // length of string
const uint8_t kMatrixHeight = 16; // number of strings
#define NUM_LEDS_PER_STRIP 100
#define NUM_STRIPS 8


#define NUM_AUDIO_LEDS NUM_LEDS_PER_STRIP // should same as length of string

#define NUM_LEDS (NUM_STRIPS * NUM_LEDS_PER_STRIP)

const bool    kMatrixSerpentineLayout = true;

#define FRAMES_PER_SECOND  120

// **********************************************************************************************************
#define USE_OCTOWS2811
#include<OctoWS2811.h>

#include <TeensyDMX.h>
#include <FastLED.h>
#include <Audio.h>

// **********************************************************************************************************

namespace teensydmx = qindesign::teensydmx;

teensydmx::Receiver dmxRx{Serial1};
uint8_t dmxRxBuf[513];  // Buffer up to 513 channels, including the start code

CRGB leds[NUM_LEDS];
CRGB ledsAudio[NUM_AUDIO_LEDS];

// **********************************************************************************************************

uint8_t STEPS = 150;
uint8_t BRIGHTNESS = 255;
uint8_t SPEEDO = 180;
uint8_t FADE = 180;

uint8_t RED = 0;
uint8_t GREEN = 0;
uint8_t BLUE = 0;


uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

CRGBPalette16 currentPalette;
TBlendType currentBlending;

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

#include "cylon.h"
#include "colours.h"

// *************
// End audio
// *************


typedef void (*Pattern)();
typedef Pattern PatternList[];
typedef struct {
  Pattern pattern;
  String name;
} PatternAndName;
typedef PatternAndName PatternAndNameList[];

PatternAndNameList gPatterns = { 
  { autoRun, "auotrun"}, 
  { pixels,"pixels"},         // Long line of colours
  { fillnoise,"fillnoise"},      // Center to edges with base color and twinkle
  { jugglep,"jugglep"},        // Long line of sinewaves
  { ripple,"ripple"},         // Juggle with twinkles 
  { pixel,"pixel"},          // Long line of colours
  { matrix,"matrix"},         // Start to end with twinkles
  { onesine,"onesine"},        // Long line of shortlines
  { noisefire,"noisefire"},      // Start to end
  { rainbowbit,"rainbowbit"},     // Long line of short lines with twinkles
  { noisefiretest,"noisefiretest"},  // Center to edges
  { rainbowg,"rainbowg"},       // Long line with flashes
  { noisewide,"noisewide"},      // Center to edges
  { plasma,"plasma"},         // Long line of short lines
  { besin,"besin"},          // center to edges with black
  { noisepal,"noisepal"},       // Long line
  { cylon,"cylon"},
  { rainbowSweep,"rainbowSweep"},
  { Rainbow,"Rainbow"},
  { dsnake,"dsnake"},
  { RainbowWash,"RainbowWash"},
  { Ripple,"Ripple"},
  { sinwave_1,"sinwave_1"},
  { confetti,"confetti"},
  { sinelon,"sinelon"},
  { juggle,"juggle"}
  };
// soundmems_demo
// pixels, fillnoise, jugglep, ripple, pixel, matrix, onesine, noisefire, rainbowbit, noisefiretest, rainbowg, noisewide, plasma, besin, noisepal

// shimmer, confetti, sinelon,
PatternAndNameList gAutoPatterns = {
//  { pixels, "pixels"},         // Long line of colours
//  { fillnoise, "fillnoise"},      // Center to edges with base color and twinkle
//  { jugglep, "jugglep"},        // Long line of sinewaves
//  { ripple, "ripple"},         // Juggle with twinkles
//  { pixel, "pixel"},          // Long line of colours
//  { matrix, "matrix"},         // Start to end with twinkles
//  { onesine, "onesine"},        // Long line of shortlines
//  { noisefire, "noisefire"},      // Start to end
//  { rainbowbit, "rainbowbit"},     // Long line of short lines with twinkles
//  { noisefiretest, "noisefiretest"},  // Center to edges
//  { rainbowg, "rainbowg"},       // Long line with flashes
//  { noisewide, "noisewide"},      // Center to edges
//  { plasma, "plasma"},         // Long line of short lines
//  { besin, "besin"},          // center to edges with black
//  { noisepal, "noisepal"},       // Long line
  { rainbowSweep, "rainbowSweep"},
  { Rainbow, "Rainbow"},
  { dsnake, "dsnake"},
  { RainbowWash, "RainbowWash"},
  { Ripple, "Ripple"},
  { sinwave_1, "sinwave_1"},
  { rainbow, "rainbow"},
  { bpm, "bpm"}
 };



// **********************************************************************************************************
// Setup
// **********************************************************************************************************

void setup() {
  // Audio requires memory to work.
  AudioMemory(12);

  /* USB serial */
  Serial.begin(115200);

  dmxRx.begin();

//  pinMode(LED_BUILTIN, OUTPUT); --- BREAKS AUDIO ?!

  // FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  LEDS.addLeds<OCTOWS2811, RGB>(leds, NUM_LEDS_PER_STRIP);
  
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("Setup");

  currentPalette = palettes[0];

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.volume(0.5);
  
  ledtest();
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
int gPatternCount = ARRAY_SIZE(gPatterns);
int gAutoPatternCount = ARRAY_SIZE(gAutoPatterns);
int paletteCount = ARRAY_SIZE(palettes);

int led = 0;
elapsedMillis elapsed;

// **********************************************************************************************************
// Main
// **********************************************************************************************************

// Checks if there's a new DMX frame and returns the frame size.
static int newFrame(teensydmx::Receiver dmxRx) {
  return dmxRx.readPacket(dmxRxBuf, 0, 513);
      // Note: It's less efficient to read bytes you don't need;
      // this is only here because it was requested to make the
      // code look better. Ideally, you should call
      // `readPacket(buf, 0, size_needed)` instead.
}

int pattern = 0;
void loop()
{
  // Read at least to 7 bytes (6 channels) starting from channel 0 (start code)
  int read = newFrame(dmxRx);
  if (read >= 7 && dmxRxBuf[0] == 0) {  // Ensure start code is zero
    led = !led;
    digitalWrite(LED_BUILTIN, led);
    int b = dmxRxBuf[1]; // brightness = 1
    if (b != BRIGHTNESS) {
      BRIGHTNESS = b;
      FastLED.setBrightness(BRIGHTNESS);
      Serial.printf("Brightness: %u\n", BRIGHTNESS);
    }
    STEPS = dmxRxBuf[2]; // steps = 2
    SPEEDO = dmxRxBuf[3]; //speed = 3
    FADE = dmxRxBuf[4]; // fade = 4
    int p = dmxRxBuf[5]; // pattern = 5
    pattern = map(p, 0, 255, 0, (gPatternCount - 1));
    if(p > (gPatternCount - 1)) { 
      p = 0;
    }
    else {
      pattern = p;
    }
    currentPalette = palettes[map(dmxRxBuf[6], 0, 255, 0, (paletteCount - 1))]; // channel 6

    RED = dmxRxBuf[7];
    GREEN = dmxRxBuf[8];
    BLUE = dmxRxBuf[9];

//    EVERY_N_SECONDS( 2 ) {
//      Serial.println(p);
//      Serial.print("b=");
//      Serial.println(b);
//    }
  }

    EVERY_N_SECONDS( 2 ) {
      if(pattern == 0) {
        Serial.print("auto pattern = ");
        Serial.println(gAutoPatterns[gCurrentPatternNumber].name);        
      }
      else {
        Serial.print("pattern = ");
        Serial.println(gPatterns[pattern].name);
      }
    }
    EVERY_N_SECONDS( 10 ) {
      Serial.printf("FPS: %u\n", LEDS.getFPS());
    }
  soundmems();
  gPatterns[pattern].pattern();

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
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

  gAutoPatterns[gCurrentPatternNumber].pattern();
  // change patterns periodically
//  EVERY_N_SECONDS( map(STEPS, 0, 255, 10, 100 )) {
  EVERY_N_SECONDS(60) {
    nextPattern();
    Serial.print("Swapping to pattern ");
    Serial.println(gAutoPatterns[gCurrentPatternNumber].name);
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
