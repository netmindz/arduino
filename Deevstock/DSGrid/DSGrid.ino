#ifdef __IMXRT1062__ // Teensy 4
  #pragma message "Use SMARTMATRIX"
  #define  SMARTMATRIX
  #define TEENSY4
#elif defined(__MK20DX256__) // Teensy 3.2
  #pragma message "Use APA1023030"
  #undef SMARTMATRIX
  #define  APA1023030
#else
  #pragma message "Use WS1616"
  #undef SMARTMATRIX
  #define  WS1616
#endif

#define DISABLE_MATRIX_TEST // No grey on start
#include "neomatrix_config.h"


#include<FastLED.h>

//---LED SETUP STUFF

#define COLOR_ORDER BGR

// the size of your matrix - now defined in neomatrix_config
#define kMatrixWidth  mw
#define kMatrixHeight mh

// used in FillNoise for central zooming
byte CentreX =  (kMatrixWidth / 2) - 1;
byte CentreY = (kMatrixHeight / 2) - 1;

int barWidth = (kMatrixWidth / 7) / 2;
int blockWidth = barWidth * 7;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
#define leds matrixleds

// a place to store the color palette
CRGBPalette16 currentPalette;
TBlendType    currentBlending = LINEARBLEND;

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
byte red_level;
byte green_level;
byte blue_level;

int BRIGHTNESS = 150;
int SPEED = 50;
int FADE = 50;


#include "stars.h"


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
  { autoRunAudio, "autoRunAudio"}, // must be second
#ifndef ESP32
  { showStars, "showStars"},
#endif 
  { vortex, "vortex"},
  { squares, "squares"},
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
#ifndef TEENSY4
  { EQ, "EQ"}, 
  { VU, "VU"},
  { FunkyPlank, "FunkyPlank"},
  { DJLight, "DJLight"},

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
 // crashing { Audio5, "Audio5"}, // cool wave - move
  //     Audio6,
#endif

};

PatternAndNameList gAudioPatterns = {
  { EQ, "EQ"}, 
  { VU, "VU"},
  { FunkyPlank, "FunkyPlank"},
  { DJLight, "DJLight"},

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
 // crashing { Audio5, "Audio5"}, // cool wave - move
  //     Audio6,
};

#include "colours.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
int gPatternCount = ARRAY_SIZE(gPatterns);
int gAudioPatternCount = ARRAY_SIZE(gAudioPatterns);
int gPaletteCount = ARRAY_SIZE(palettes);
int autopgm = random(1, (gPatternCount - 1));
int autoAudiopgm = 0;


#if defined(CORE_TEENSY)
#ifdef TEENSY4
#include "control_null.h" // Teensy 4.0 - no input
#else 
//#include "control_tdmx.h" // DMX and MSGEQ7 with Teensy 3.2
#include "control_tdmx_audio.h" // DMX inc audio with Teensy 3.2
#endif
#else
#include "control_esp.h" // ESP32/ESP8266 - E1.31 and audio from WLED sender
#endif

#include "audio-palettes.h"

CRGBPalette16 getPalette(int pNumber) {
  if(pNumber >= 1 && pNumber < 4) {
    return getAudioPalette(pNumber);    
  }
  return palettes[pNumber];
}

#include "qlc.h"

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

    ledtest();

  //  // Initialize our noise coordinates to some random values
  //  fx = random16();
  //  fy = random16();
  //  fz = random16();

  //  x2 = random16();
  //  y2 = random16();
  //  z2 = random16();

  //AutoRunAudio();
  Serial.printf("There are %u patterns\n", gPatternCount);

  qlcDumpDef();
}

void loop() {

  controlLoop();

  EVERY_N_SECONDS(10) {
    if(pgm == 0) {
     Serial.print("Auto: ");
      Serial.println(gPatterns[autopgm].name);
    }
    else if(pgm == 1) {
     Serial.print("AutoAudio: ");
      Serial.println(gAudioPatterns[autoAudiopgm].name);
    }
    else {

      Serial.println(gPatterns[pgm].name);
    }
  }
  gPatterns[pgm].pattern();
  ShowFrame();
}

int autoPalette = 0;
void autoRun() {
  EVERY_N_SECONDS(90) {
    autopgm = random(2, (gPatternCount - 1));
//     autopgm++;
    if (autopgm >= gPatternCount) autopgm = 1;
    Serial.print("Next Auto pattern: ");
    Serial.println(gPatterns[autopgm].name);
  }

  EVERY_N_SECONDS(145) {
    autoPalette = random(0, (gPaletteCount - 1));
    // autoPalette++;
    if (autoPalette > (gPaletteCount - 1)) autoPalette = 0;
    Serial.println("Next Auto pallette");
    currentPalette = palettes[autoPalette];
  }

  gPatterns[autopgm].pattern();

}

void autoRunAudio() {
  EVERY_N_SECONDS(30) {
    autoAudiopgm = random(0, (gAudioPatternCount - 1));
//     autoAudiopgm++;
    if (autoAudiopgm >= gAudioPatternCount) autoAudiopgm = 1;
    Serial.print("Next Auto pattern: ");
    Serial.println(gPatterns[autopgm].name);
  }
  gAudioPatterns[autoAudiopgm].pattern();
}
