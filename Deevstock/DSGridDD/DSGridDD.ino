#include<FastLED.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

int getPatternCount();


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

// leds2 is populated by non-beat-reactive patterns
CRGB leds2[kMatrixWidth * kMatrixHeight];
// leds3 is populated by beat-reactive patterns
CRGB leds3[kMatrixWidth * kMatrixHeight];

int coords[kMatrixWidth][kMatrixHeight]; // map of x,y
const int height = kMatrixHeight;
const int width = kMatrixWidth;

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

int BRIGHTNESS = 55;
int SPEED = 50;
int FADE = 50;

// Statistic.h library is used for music analytics
#include <Statistic.h>

// "mult" is the multiplier that can be adjusted over WiFi
double mult = 1.00;
// "DDAudio.h" is Diod.design's music-analytic code.
#include "DDAudio.h"
#include "DDGradients.h"
#include "DDDrawing.h" // Must be above Animatinos
#include "DDAnimations.h"
#include "DDPatternLists.h"

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

  // This function in 'DDAudio.h' sets the input (aux/mic), and configures the audio adapter
  audioSetup();
  
  // If there's a mapping function used to set (x,y) or (x,y,z) coordinates for each LED pixel, put here:
  // mapXY() is a function that sets an LED # to every x,y coordinate on a matrix.
  mapXY();

  FastLED.addLeds<APA102, LED_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(8)>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  // switch dithering off to avoid flicking at low fps
  FastLED.setDither(0);

  FastLED.setBrightness(55);

  //  leds[XY(0,0)] = CRGB::White;
  //  leds[XY(29,0)] = CRGB::Blue;
  //  leds[XY(29,8)] = CRGB::Yellow;
  //  leds[XY(0, 9)] = CRGB::Green;
  //  leds[XY(0, 28)] = CRGB::Green;
  //  leds[XY(0, 29)] = CRGB::Red;
  //  leds[XY(29, 29)] = CRGB::Blue;
  //  FastLED.delay(10000);

  ledtest();

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
  autoRun, // must be first
  showStars,
  EQ,
  VU,
  FunkyPlank,
  DJLight,
  MirroredNoise,
//  RedClouds,
//  Lavalamp1,
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
  MSGEQtest,
  MSGEQtest2,
  //   MSGEQtest3,
  MSGEQtest4,
  // AudioSpiral, // TODO: resize
  //     MSGEQtest5,// TODO: resize
  //     MSGEQtest6, //boring
  MSGEQtest7, // nice but resize?
  MSGEQtest8,
  //   MSGEQtest9,
  //     CopyTest,
  Audio1,// TODO: resize
  Audio2, // cool wave - move
  //     Audio3, // boring
  //     Audio4,// TODO: move
  //   CaleidoTest1,
  //  CaleidoTest2,// TODO: move
  Audio5, // cool wave - move
  //     Audio6,

};

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
int gPatternCount = ARRAY_SIZE(gPatterns);

void loop() {

  controlLoop();
  // Music Analysis code
  musicAnalytics();
  
//  Serial.println(gPatterns[pgm]);
  gPatterns[pgm]();
//  FastLED.setMaxRefreshRate(120);
//  fullSparkles();
//  blend2(255);
//  
  ShowFrame();
}

int autopgm = 1; //random(1, (gPatternCount - 1));
void autoRun() {
  EVERY_N_SECONDS(90) {
    autopgm = random(1, (gPatternCount - 1));
    // autopgm++;
    Serial.printf("Next Auto pattern %u\n", autopgm);
    if (autopgm >= gPatternCount) autopgm = 1;
  }

  gPatterns[autopgm]();

}

// Mapping function for an XY matrix where the LED path snakes back and fourth
void mapXY() {
  static int index = 0; // Keeps track of which LED is next

  for (int i = 0; i < height; i++) { // On every row...
    for (int j = 0; j < width; j++) { // For every column...
      if (i % 2 == 0) { // If it's going towards the right
        coords[j][i] = index; // The LED we're on is applied to that (x,y) coordinate
        index++; // move to next LED
      }
      else { // If it's going towards the left...
        coords[width - 1 - j][i] = index;
        index++;
      }
    }
  }
}


// This function automatically mixes leds2 and leds3 into leds as the song's beat comes in and drops out
void autoMusicVisualizing() {
  // A local variable to remember the last 
  static int mixAmountOld;
  mixAmountOld = mixAmount;

  // While a beat is present (mixAmount = 0), change the color palette used in non-beat pattern
  // "mixAmountOld != 255" is used so it only changes the color palette once
  if (mixAmount == 255 && mixAmountOld != 255) {
    currentPaletteNumber = random8(gGradientPaletteCount); // generate random number
    currentPalette = gGradientPalettes[currentPaletteNumber]; // Use that random number to assign a color palette

    nextMusicWithNoBeatPattern(); // chooses new non-beat pattern
  }

  // While no beat is present (mixAmount = 255), change the beat-dependent pattern
  // "mixAmountOld != 0" is used so it only chooses a new pattern once
  if (mixAmount == 0 && mixAmountOld != 0) 
    nextLowBeatPattern(); // chooses new beat-dependent pattern
  
  // When non-beat patterns are not color-specific, their look can be changed by having colors scroll through.
  // This randomly chooses whether to scroll the colors or keep them still
  //if (mixAmount == 255 && mixAmountOld != 255) changingHue = random8(2);
  
  // This is where the music-visualizing patterns are written to leds2 and leds3
  // Or if there is silence, populate leds with an ambient pattern

  if (silence) {
    ambientPatterns[currentAmbPatternNumber]();
  }
  else {
    // If there's a constant beat, populate leds3 with a constant beat pattern
    // TODO: make this another pattern list, and make it fade in
    if (constBeat)
      drawCircles();
    // else, populate leds3 with a non-constant beat pattern
    else
      lowBeatPatterns[currentLowBeatPatternNumber]();

    // Populate leds2 with non-beat pattern
    musicWithNoBeatPatterns[musicWithNoBeatPatternNumber]();
  }

  // blends leds3 in to leds. If a beat is present, mixAmount = 255 and leds = leds3.
  blend3(mixAmount);
  // blends leds2 in to leds. If a beat is NOT present, mixAmount = 0, and leds = leds2.
  blend2((mixAmount - 255) * -1);
  //blend2(255);
} // end autoMusicVisualizing()

int getPatternCount() {
  return gPatternCount;
}
