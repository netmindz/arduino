#ifdef __IMXRT1062__ // Teensy 4
  #define  SMARTMATRIX
#else
  #define  APA1023030
#endif

#include "neomatrix_config.h"


#include<FastLED.h>

//---LED SETUP STUFF

#define COLOR_ORDER BGR

// the size of your matrix
#define kMatrixWidth  mw
#define kMatrixHeight mh

// used in FillNoise for central zooming
byte CentreX =  (kMatrixWidth / 2) - 1;
byte CentreY = (kMatrixHeight / 2) - 1;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
#define leds matrixleds

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

int BRIGHTNESS = 15;
int SPEED = 50;
int FADE = 50;


#if defined(CORE_TEENSY)
#include "control_tdmx.h"
#else
// #include "control_null.h"
#include "control_esp.h"
#endif

#include "stars.h"

void setup() {
  // enable debugging info output
  Serial.begin(115200);
  
  Serial.println("Setup");
  controlSetup();

  matrix_setup();
  
  //  leds[XY(0,0)] = CRGB::White;
  //  leds[XY(29,0)] = CRGB::Blue;
  //  leds[XY(29,8)] = CRGB::Yellow;
  //  leds[XY(0, 9)] = CRGB::Green;
  //  leds[XY(0, 28)] = CRGB::Green;
  //  leds[XY(0, 29)] = CRGB::Red;
  //  leds[XY(29, 29)] = CRGB::Blue;
  //  FastLED.delay(10000);

  //  ledtest();

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

typedef void (*Pattern)();
typedef Pattern PatternList[];
typedef struct {
  Pattern pattern;
  String name;
} PatternAndName;
typedef PatternAndName PatternAndNameList[];

PatternAndNameList gPatterns = {
  { autoRun, "autoRun"}, // must be first
  { showStars, "showStars"},
  { EQ, "EQ"},
  { VU, "VU"},
  { FunkyPlank, "FunkyPlank"},
  { DJLight, "DJLight"},
  { MirroredNoise, "MirroredNoise"},
//  RedClouds,
//  Lavalamp1,
  { Lavalamp2, "Lavalamp2"},
  { Lavalamp3, "Lavalamp3"},
  { Lavalamp4, "Lavalamp4"},
  { Lavalamp5, "Lavalamp5"},
  { Constrained1, "Constrained1"},
  { RelativeMotion1, "RelativeMotion1"},
  { Water, "Water"},
  //    Bubbles,
  { TripleMotion, "TripleMotion"},
  { CrossNoise, "CrossNoise"},
  { CrossNoise2, "CrossNoise2"},
  { RandomAnimation, "RandomAnimation"},
  { MilliTimer, "MilliTimer"},
  { Caleido1, "Caleido1"},
  { Caleido2, "Caleido2"},
  { Caleido3, "Caleido3"},
  { Caleido5, "Caleido5"},
  { vortex, "vortex"},
  { squares, "squares"},

  //      // Audio
  { MSGEQtest, "MSGEQtest"},
  { MSGEQtest2, "MSGEQtest2"},
  //   MSGEQtest3,
  { MSGEQtest4, "MSGEQtest4"},
  // AudioSpiral, // TODO: resize
  //     MSGEQtest5,// TODO: resize
  //     MSGEQtest6, //boring
  { MSGEQtest7, "MSGEQtest7"}, // nice but resize?
  { MSGEQtest8, "MSGEQtest8"},
  //   MSGEQtest9,
  //     CopyTest,
  { Audio1, "Audio1"},// TODO: resize
  { Audio2, "Audio2"}, // cool wave - move
  //     Audio3, // boring
  //     Audio4,// TODO: move
  //   CaleidoTest1,
  //  CaleidoTest2,// TODO: move
  { Audio5, "Audio5"}, // cool wave - move
  //     Audio6,

};

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
int gPatternCount = ARRAY_SIZE(gPatterns);

void loop() {

  controlLoop();

  EVERY_N_SECONDS(10) {
    Serial.println(gPatterns[pgm].name);
  }
  gPatterns[pgm].pattern();
  ShowFrame();
}

int autopgm = 1; // random(1, (gPatternCount - 1));
void autoRun() {
  EVERY_N_SECONDS(90) {
    autopgm = random(1, (gPatternCount - 1));
    // autopgm++;
    if (autopgm >= gPatternCount) autopgm = 1;
    Serial.print("Next Auto pattern: ");
    Serial.println(gPatterns[autopgm].name);
  }

  gPatterns[autopgm].pattern();

}
