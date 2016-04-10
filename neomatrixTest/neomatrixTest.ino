#include <FastLED.h>

#define LED_PIN  6

#define CHIPSET     NEOPIXEL

#define BRIGHTNESS 255

// Params for width and height
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 8;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = false;
// This function will return the right 'led index number' for
// a given set of X and Y coordinates on your matrix.
// IT DOES NOT CHECK THE COORDINATE BOUNDARIES.
// That's up to you.  Don't pass it bogus values.
//
// Use the "XY" function like this:
//
//    for( uint8_t x = 0; x < kMatrixWidth; x++) {
//      for( uint8_t y = 0; y < kMatrixHeight; y++) {
//
//        // Here's the x, y to 'led index' in action:
//        leds[ XY( x, y) ] = CHSV( random8(), 255, 255);
//
//      }
//    }
//
//
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

  return i;
}



#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if ( x >= kMatrixWidth) return -1;
  if ( y >= kMatrixHeight) return -1;
  return XY(x, y);
}

void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for ( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;
    for ( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}

int hue = 0;
void ledtest() {
  for (int y = 0; y < kMatrixHeight; y++) {
    for (int x = 0; x < kMatrixWidth; x++) {
      leds[ XY(x, y)]  = CHSV( hue, 255, 255);
      FastLED.show();
      delay(100);
      leds[ XY(x, y)]  = CRGB::Black;
      hue += 5;
      if (hue > 255) hue = 0;
    }
  }
}

int snakeX = 0;
int snakeY = 0;
int snakeRight = 1;
int snakeUp = 0;
int snakeW = kMatrixWidth - 1;
int snakeH = kMatrixHeight - 1;
int snakeMargin = 0;
void snake() {
  leds[ XY(snakeX, snakeY)]  = CHSV( hue, 255, 255);
  // End of right
  if ((snakeX >= (snakeW - snakeMargin)) && (snakeUp == 0)) {
    snakeUp = 1;
    snakeRight = 0;
    //    hue += 15;
  }
  // End of up
  else if ((snakeY >= (snakeH - snakeMargin)) && (snakeUp == 1)) {
    snakeUp = 0;
    snakeRight = -1;
    //    hue += 15;
  }
  // End of left
  else if ((snakeX == (0 + snakeMargin)) && (snakeUp == 0 && snakeRight == -1)) {
    snakeUp = -1;
    snakeRight = 0;
    //    hue += 15;
  }
  // End of down
  else if ((snakeX == (0 + snakeMargin) && snakeY == (1 + snakeMargin)) && (snakeUp == -1 && snakeRight == 0)) {
    snakeY += snakeUp;
    snakeUp = 0;
    snakeRight = 1;
    hue += 15;
    Serial.print("snakeMargin=");
    Serial.println(snakeMargin);
    snakeMargin++;
    if (snakeMargin > 2) {
      //delay(1500);
      hue = random(0,255);
      snakeMargin = 0;
      snakeX = -1;
      snakeY = 0;
      snakeRight = 1;
      snakeUp = 0;
    }
    else {
      snakeY++;
    }
  }
  FastLED.show();
  delay(15);
  // leds[ XY(snakeX, snakeY)]  = CRGB::Black;
  for(int i=0; i < (kMatrixWidth * kMatrixHeight); i++) {
    leds[i].fadeLightBy(20);
  }
  snakeX += snakeRight;
  snakeY += snakeUp;
}


// Demo that USES "XY" follows code below

void sample()
{
  uint32_t ms = millis();
  int32_t yHueDelta32 = ((int32_t)cos16( ms * (27 / 1) ) * (350 / kMatrixWidth));
  int32_t xHueDelta32 = ((int32_t)cos16( ms * (39 / 1) ) * (310 / kMatrixHeight));
  DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
  if ( ms < 5000 ) {
    FastLED.setBrightness( scale8( BRIGHTNESS, (ms * 256) / 5000));
  } else {
    FastLED.setBrightness(BRIGHTNESS);
  }
  FastLED.show();
}

void loop() {
  //ledtest();
  snake();
}



void setup() {
  //Serial.begin(38400);
  FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );

//  leds[ XY(0, 0)]  = CRGB::Red;
//  leds[ XY((kMatrixWidth - 1), (kMatrixHeight - 1))]  = CRGB::Blue;
//  FastLED.show();
//  delay(1000);
}
