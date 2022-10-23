#include <WiFiUdp.h>
#include <NTPClient.h>
uint8_t flipClock = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void drawHand(uint16_t angle, uint8_t endRing, CRGB color)
{
  // rotate 90 degrees counter-clockwise
  angle += 270;

  while (angle >= 360)
    angle -= 360;

  uint8_t index = 0;

  for (uint8_t ringIndex = 0; ringIndex <= endRing; ringIndex++)
  {
    index = angleToPixel360(angle, ringIndex);

    leds[index] |= color;
  }
}

void drawHandArc(uint16_t endAngle, uint8_t ringIndex, CRGB color)
{
  static const uint16_t startAngle = 270;

  // rotate 90 degrees counter-clockwise
  endAngle += 270;

  while (endAngle >= 360)
    endAngle -= 360;

  uint8_t startIndex = angleToPixel360(startAngle, ringIndex);
  uint8_t endIndex = angleToPixel360(endAngle, ringIndex);
  if (startIndex == endIndex)
  {
    leds[endIndex] = color;
  }
  else
  {
    fillRing360(ringIndex, color, startAngle, endAngle);
  }
}

void drawHandDot(uint16_t angle, uint8_t ringIndex, CRGB color)
{
  // rotate 90 degrees counter-clockwise
  angle += 270;

  while (angle >= 360)
    angle -= 360;

  uint8_t index = 0;

  index = angleToPixel360(angle, ringIndex);

  leds[index] |= color;
}

uint16_t hourAngle = 0;
uint16_t minuteAngle = 0;
uint16_t secondAngle = 0;

void updateClock()
{
  hourAngle = timeClient.getHours();
  if (hourAngle >= 12)
    hourAngle -= 12;
  hourAngle *= 30;

  minuteAngle = timeClient.getMinutes() * 6;
  secondAngle = timeClient.getSeconds() * 6;
}

void handClock()
{
//  dimAll(240);

  // hour
  drawHand(hourAngle, 4, CRGB::Red);

  // minute
  drawHand(minuteAngle, 6, CRGB::Green);

  // second
  drawHand(secondAngle, 8, CRGB::Blue);

  updateClock();
}

void arcClock()
{
  EVERY_N_SECONDS( 1 ) { 
  
    // hour
    drawHandArc(hourAngle, 6, CRGB::Red);
  
    // minute
    drawHandArc(minuteAngle, 7, CRGB::Green);
  
    // second
    drawHandArc(secondAngle, 8, CRGB::Blue);
  
    updateClock();

    FastLED.show();
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }
}

void rimClock()
{
  EVERY_N_SECONDS( 1 ) { 
  //  dimAll(240);
  
    // hour
    drawHandDot(hourAngle, 8, CRGB::Red);
  
    // minute
    drawHandDot(minuteAngle, 8, CRGB::Green);
  
    // second
    drawHandDot(secondAngle, 8, CRGB::Blue);

    updateClock();

    FastLED.show();
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }
}
