// Polar coordinate mapping code for FastLED by Adam Haile, Maniacal Labs:
// http://maniacallabs.com/2015/05/04/review-code-adafruit-dotstar-disk/
// https://gist.github.com/adammhaile/a769f3ff87ff61f22ace

#define ringCount 9 // total Number of Rings

//Map rings on disk to indicies.
//This is where all the magic happens.
//Each represents one of the concentric rings.
uint8_t ringMap[ringCount][2] = {
  {0, 0},     //0 Center Point
  {1, 8},     //1
  {9, 20},   //2
  {21, 36},   //3
  {37, 60},   //4
  {61, 92},   //5
  {93, 132},  //6
  {133, 180}, //7
  {181, 240}, //8 Outer Ring
};

//For convenience, last ring index
uint8_t lastRing = ringCount - 1;

//Arrays containing degrees between each pixel for each ring.
//This is to speed up later calculations by doing these ahead of time.
//I've given everything with the option of using 360 degres per cirlce
//or 256. The latter fits nicer into a single byte and *should* be a bit
//faster since it's only doing single byte math and not having to deal with
//negative angles since uint8_t already rolls around to a positive value.
//In the 256 degree per circle methods, 64 is equivalent to 90, 128 to 180,
//and 192 to 270
float * ringSteps360;
float * ringSteps256;

//360 Degree Helper function to map angle and ring index to pixel
uint16_t angleToPixel360(int16_t angle, uint8_t ring)
{
  if (ring >= ringCount) return 0;
  angle = angle % 360;
  if (angle < 0) angle = 360 + angle;
  return ringMap[ring][0] + int(angle / ringSteps360[ring]);
}

//256 Degree Helper function to map angle and ring index to pixel
uint16_t angleToPixel256(uint8_t angle, uint8_t ring)
{
  if (ring >= ringCount) return 0;
  return ringMap[ring][0] + int(angle / ringSteps256[ring]);
}

//Fill in the ringSteps arrays for later use.
inline void setupRings()
{
  ringSteps360 = (float*)malloc(ringCount * sizeof(float));
  uint8_t count = 0;
  for (int r = 0; r < ringCount; r++)
  {
    count = (ringMap[r][1] - ringMap[r][0] + 1);
    ringSteps360[r] = (360.0 / float(count));
  }

  ringSteps256 = (float*)malloc(ringCount * sizeof(float));
  count = 0;
  for (int r = 0; r < ringCount; r++)
  {
    count = (ringMap[r][1] - ringMap[r][0] + 1);
    ringSteps256[r] = (256.0 / float(count));
  }
}

//360 Degree helper to set a pixel, given angle and ring index
void setPixel360(int16_t angle, uint8_t ring, CRGB color)
{
  uint16_t pixel = angleToPixel360(angle, ring);
  if (pixel >= NUM_LEDS) return;
  leds[pixel] = color;
}

//256 Degree helper to set a pixel, given angle and ring index
void setPixel256(uint8_t angle, uint8_t ring, CRGB color)
{
  uint16_t pixel = angleToPixel256(angle, ring);
  leds[pixel] = color;
}

//360 Degree function to draw a line along a given angle from one ring to another
void drawRadius360(int16_t angle, CRGB color, uint8_t startRing, uint8_t endRing)
{
  if (startRing > lastRing) startRing = 0;
  if (endRing > lastRing) endRing = lastRing;
  for (uint8_t r = startRing; r <= endRing; r++)
  {
    setPixel360(angle, r, color);
  }
}

//256 Degree function to draw a line along a given angle from one ring to another
void drawRadius256(uint8_t angle, CRGB color, uint8_t startRing, uint8_t endRing)
{
  if (startRing > lastRing) startRing = 0;
  if (endRing > lastRing) endRing = lastRing;
  for (uint8_t r = startRing; r <= endRing; r++)
  {
    setPixel256(angle, r, color);
  }
}

//360 Degree function to fill a ring from one angle to another (draw an arc)
void fillRing360(uint8_t ring, CRGB color, int16_t startAngle, int16_t endAngle)
{
  uint8_t start = angleToPixel360(startAngle, ring);
  uint8_t end = angleToPixel360(endAngle, ring);
  if (start > end)
  {
    for (int i = start; i <= ringMap[ring][1]; i++)
    {
      leds[i] = color;
    }
    for (int i = ringMap[ring][0]; i <= end; i++)
    {
      leds[i] = color;
    }
  }
  else if (start == end)
  {
    for (int i = ringMap[ring][0]; i <= ringMap[ring][1]; i++)
    {
      leds[i] = color;
    }
  }
  else
  {
    for (int i = start; i <= end; i++)
    {
      leds[i] = color;
    }
  }
}

//256 Degree function to fill a ring from one angle to another (draw an arc)
void fillRing256(uint8_t ring, CRGB color, uint8_t startAngle, uint8_t endAngle)
{
  uint8_t start = angleToPixel256(startAngle, ring);
  uint8_t end = angleToPixel256(endAngle, ring);
  if (start > end)
  {
    for (int i = start; i <= ringMap[ring][1]; i++)
    {
      leds[i] = color;
    }
    for (int i = ringMap[ring][0]; i <= end; i++)
    {
      leds[i] = color;
    }
  }
  else if (start == end)
  {
    for (int i = ringMap[ring][0]; i <= ringMap[ring][1]; i++)
    {
      leds[i] = color;
    }
  }
  else
  {
    for (int i = start; i <= end; i++)
    {
      leds[i] = color;
    }
  }
}

uint8_t chaseSinglePixelAroundOuterRing360() {
  static int angle360 = 0;
  FastLED.clear();
  setPixel360(angle360, lastRing, CRGB::Red);
  angle360++;
  if (angle360 >= 360)
    angle360 = 0;
  return 4;
}

uint8_t chaseSinglePixelAroundOuterRing256() {
  static uint8_t angle256 = 0;
  FastLED.clear();
  setPixel256(angle256, lastRing, CRGB::Green);
  angle256++;
  return 4;
}

uint8_t chaseRadiusLine360() {
  static int angle360 = 0;
  FastLED.clear();
  drawRadius360(angle360, CRGB::Blue, 0, lastRing);
  angle360++;
  if (angle360 >= 360)
    angle360 = 0;
  return 4;
}

uint8_t chaseRadiusLine256() {
  static uint8_t angle256 = 0;
  FastLED.clear();
  drawRadius256(angle256, CRGB::Purple, 0, lastRing);
  angle256++;
  return 4;
}

void halfCircleRainbow360() {
  static int angle360 = 0;
  FastLED.clear();
  fillRing360(8, CRGB::Red, angle360 - 90, angle360 + 90);
  fillRing360(7, CRGB::Orange, angle360 - 90, angle360 + 90);
  fillRing360(6, CRGB::Yellow, angle360 - 90, angle360 + 90);
  fillRing360(5, CRGB::Green, angle360 - 90, angle360 + 90);
  fillRing360(4, CRGB::Blue, angle360 - 90, angle360 + 90);
  fillRing360(3, CRGB::Purple, angle360 - 90, angle360 + 90);
  angle360++;
  if (angle360 >= 360)
    angle360 = 0;
//  return 4;

}

void halfCircleRainbow256() {
  static uint8_t angle256 = 0;
  FastLED.clear();
  fillRing256(8, CRGB::Purple, angle256 - 64, angle256 + 64);
  fillRing256(7, CRGB::Blue, angle256 - 64, angle256 + 64);
  fillRing256(6, CRGB::Green, angle256 - 64, angle256 + 64);
  fillRing256(5, CRGB::Yellow, angle256 - 64, angle256 + 64);
  fillRing256(4, CRGB::Orange, angle256 - 64, angle256 + 64);
  fillRing256(3, CRGB::Red, angle256 - 64, angle256 + 64);
  angle256++;
//  return 4;
}
