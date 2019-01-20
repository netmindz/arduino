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

// #define CHIPSET NEOPIXEL

const uint8_t kMatrixWidth = 1; // numer of strings
const uint8_t kMatrixHeight = 50; // length of string

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

const bool    kMatrixSerpentineLayout = false;

#define FRAMES_PER_SECOND  120

// **********************************************************************************************************
#define USE_OCTOWS2811
#include<OctoWS2811.h>

#include <TeensyDmx.h>
#include <FastLED.h>

// **********************************************************************************************************

TeensyDmx Dmx(Serial1);

CRGB leds[NUM_LEDS];

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

typedef void (*SimplePatternList[])();
// snake, 
SimplePatternList gPatterns = { autoRun, rainbowSweep, Rainbow, dsnake, RainbowWash, Ripple, shimmer, sinwave_1, rainbow, confetti, sinelon, juggle, bpm, rainbowWithGlitter
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



