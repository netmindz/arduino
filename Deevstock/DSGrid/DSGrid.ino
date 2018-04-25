#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include<FastLED.h>

//---LED SETUP STUFF
#define LED_PIN 1
#define LED_PIN_ALT 8

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
byte pgm = 20;
byte spd;
byte brightness;
byte red_level;
byte green_level;
byte blue_level;

void setup() {
  // enable debugging info output
  Serial.begin(115200);

  // add the SmartMatrix controller
  int firstSectionLEDCount = (kMatrixWidth * 8);
  FastLED.addLeds<WS2812, LED_PIN_ALT, GRB>(leds, 0, firstSectionLEDCount).setCorrection( TypicalLEDStrip );
  // should be (firstSectionLEDCount - 1) bit i'm skipping an LED
  FastLED.addLeds<WS2812SERIAL, LED_PIN, BRG>(leds, (firstSectionLEDCount + 1), (NUM_LEDS - firstSectionLEDCount)).setCorrection( TypicalLEDStrip );

  // switch dithering off to avoid flicking at low fps
  FastLED.setDither(0);

  FastLED.setBrightness(20);

//  leds[XY(0,0)] = CRGB::White;
//  leds[XY(29,0)] = CRGB::Blue;
//  leds[XY(29,8)] = CRGB::Yellow;
//  leds[XY(0, 9)] = CRGB::Green;
//  leds[XY(0, 28)] = CRGB::Green;
//  leds[XY(0, 29)] = CRGB::Red;
//  leds[XY(29, 29)] = CRGB::Blue;
//  FastLED.delay(10000);

  //ledtest();
  
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


void loop() {
   EVERY_N_SECONDS(120) {
    Serial.println("Next pattern");
    //pgm = random(0, 19);
    pgm++;
    if (pgm > 19) pgm = 0;
  }

  RunAnimationDependingOnPgm();
  ShowFrame();
}
