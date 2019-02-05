#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include<FastLED.h>

//---LED SETUP STUFF

#define COLOR_ORDER BGR

// the size of your matrix
#define kMatrixWidth  30
#define kMatrixHeight 30

// used in FillNoise for central zooming
byte CentreX =  (kMatrixWidth / 2) - 1;
byte CentreY = (kMatrixHeight / 2) - 1;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[kMatrixWidth * kMatrixHeight];

// a place to store the color palette
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

// can be used for palette rotation
// "colorshift"
byte colorshift;

// The coordinates for 3 16-bit noise spaces.
#define NUM_LAYERS 3

uint32_t x[NUM_LAYERS];
uint32_t y[NUM_LAYERS];
uint32_t z[NUM_LAYERS];
uint32_t scale_x[NUM_LAYERS];
uint32_t scale_y[NUM_LAYERS];

// used for the random based animations
int16_t dx;
int16_t dy;
int16_t dz;
int16_t dsx;
int16_t dsy;

// a 3dimensional array used to store the calculated
// values of the different noise planes
uint8_t noise[NUM_LAYERS][kMatrixWidth][kMatrixHeight];

// used for the color histogramm
uint16_t values[256];

uint8_t noisesmoothing;

// everything for the button + menu handling
int button1;
int button2;
int button3;
byte mode;
int pgm = 0;
byte spd;
byte brightness;
byte red_level;
byte green_level;
byte blue_level;

int BRIGHTNESS;


#if defined(CORE_TEENSY)
#include "control_tdmx.h"
#else
//#include "control_null.h"
#include "control_esp.h"
#endif


void setup() {
  // enable debugging info output
  Serial.begin(115200);
  
  Serial.println("Setup");
  controlSetup();

  // add the SmartMatrix controller
  int firstSectionLEDCount = (kMatrixWidth * 8);
  FastLED.addLeds<WS2812, LED_PIN_ALT, GRB>(leds, 0, firstSectionLEDCount).setCorrection( TypicalLEDStrip );
  // should be (firstSectionLEDCount - 1) bit i'm skipping an LED
  FastLED.addLeds<WS2812SERIAL, LED_PIN, BRG>(leds, (firstSectionLEDCount + 1), (NUM_LEDS - firstSectionLEDCount)).setCorrection( TypicalLEDStrip );

  // switch dithering off to avoid flicking at low fps
  FastLED.setDither(0);

  FastLED.setBrightness(255);

  //  leds[XY(0,0)] = CRGB::White;
  //  leds[XY(29,0)] = CRGB::Blue;
  //  leds[XY(29,8)] = CRGB::Yellow;
  //  leds[XY(0, 9)] = CRGB::Green;
  //  leds[XY(0, 28)] = CRGB::Green;
  //  leds[XY(0, 29)] = CRGB::Red;
  //  leds[XY(29, 29)] = CRGB::Blue;
  //  FastLED.delay(10000);

  //  ledtest();

  InitMSGEQ7();

  //  // Initialize our noise coordinates to some random values
  //  fx = random16();
  //  fy = random16();
  //  fz = random16();

  //  x2 = random16();
  //  y2 = random16();
  //  z2 = random16();

  //AutoRunAudio();

}

// basically beatsin16 with an additional phase

uint16_t beatsin(accum88 beats_per_minute, uint16_t lowest = 0, uint16_t highest = 65535, byte phase = 0)
{
  uint16_t beat = beat16( beats_per_minute);
  uint16_t beatsin = (sin16( beat + (phase * 256)) + 32768);
  uint16_t rangewidth = highest - lowest;
  uint16_t scaledbeat = scale16( beatsin, rangewidth);
  uint16_t result = lowest + scaledbeat;
  return result;
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {
  autoRun,
  EQ,
  VU,
  FunkyPlank,
  DJLight,
  MirroredNoise,
  RedClouds,
  Lavalamp1,
  Lavalamp2,
  Lavalamp3,
  Lavalamp4,
  Lavalamp5,
  Constrained1,
  RelativeMotion1,
  Water,
  //    Bubbles,
  TripleMotion,
  CrossNoise,
  CrossNoise2,
  RandomAnimation,
  MilliTimer,
  Caleido1,
  Caleido2,
  Caleido3,
  Caleido5,
  vortex,
  squares,

//      // Audio
//         MSGEQtest,
//     MSGEQtest2,
//  //   MSGEQtest3,
//     MSGEQtest4,
//     AudioSpiral,
//     MSGEQtest5,
//     MSGEQtest6,
//     MSGEQtest7,
//     MSGEQtest8,
//  //   MSGEQtest9,
//     CopyTest,
//     Audio1,
//     Audio2,
//     Audio3,
//     Audio4,
//     CaleidoTest1,
//     CaleidoTest2,
//     Audio5,
//     Audio6,

};

SimplePatternList gAutoPatterns = {
  squares,
  DJLight,
  VU,
 FunkyPlank,
  EQ,
  vortex,
  MirroredNoise,
///  RedClouds,
//  Lavalamp1,
//  Lavalamp2,
//  Lavalamp3,
//  Lavalamp4,
//  Lavalamp5,
  Constrained1,
  RelativeMotion1,
  Water,
  //    Bubbles,
  TripleMotion,
  CrossNoise,
  CrossNoise2,
  RandomAnimation,
  MilliTimer,
  Caleido1,
  Caleido2,
  Caleido3,
  Caleido5,
  
  vortex,
  squares,
  vortex,
  squares,
};


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
int gPatternCount = ARRAY_SIZE(gAutoPatterns);

int BRIGHTNESS;
void loop() {

  Dmx.loop();
  if (Dmx.newFrame()) {
    Serial.println("d");
    int b = Dmx.getBuffer()[1]; // brightness = 1
    if (b != BRIGHTNESS) {
      BRIGHTNESS = b;
      FastLED.setBrightness(BRIGHTNESS);
    }
    int p = Dmx.getBuffer()[2]; // pattern = 5
    pgm = map(p, 0, 255, 0, (gPatternCount - 1));
    if (p > (gPatternCount - 1)) {
      p = 0;
    }
    else {
      pgm = p;
    }
  }

  //  Serial.println(gPatterns[pgm]);
  gPatterns[pgm]();
  ShowFrame();
}

int autopgm = 0;
void autoRun() {
  EVERY_N_SECONDS(90) {
    Serial.println("Next Auto pattern");
    autopgm = random(0, (gPatternCount - 1));
   // autopgm++;
    if (autopgm >= gPatternCount) autopgm = 0;
  }

  gAutoPatterns[autopgm]();

}

