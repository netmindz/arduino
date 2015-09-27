/****************************************
 * Scrolling Sound Meter Sketch for the
 * Adafruit Microphone Amplifier
 ****************************************/

#include <FastLED.h>

#include "MSGEQ7.h"
#define pinAnalogLeft A0
#define pinAnalogRight A1
#define pinReset 5
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(20)
#define MSGEQ7_SMOOTH true

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

#define LED_PIN 2
#define CLOCK_PIN 6

#define COLOR_ORDER BGR
#define CHIPSET     DOTSTAR

#define BRIGHTNESS 50
#define DITHER 255 // try 0 to disable flickering

#define WIDTH 8
#define HEIGHT 15

CRGB leds[(WIDTH * HEIGHT)];

CRGB color;

const boolean gay = false;

void setup() {
  Serial.begin(9600);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setDither(DITHER);
  FastLED.addLeds<DOTSTAR, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, (WIDTH * HEIGHT)).setCorrection( TypicalLEDStrip );
}


void loop() {
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Led strip output
  if (newReading) {

    int displayPeakL = map(MSGEQ7.get(MSGEQ7_LOW, 0), 0, 255, 0, round(HEIGHT / 2));
    int displayPeakR = map(MSGEQ7.get(MSGEQ7_LOW, 1), 0, 255, 0, round(HEIGHT / 2));
    Serial.print("Display peak: ");
    Serial.println(displayPeakL);

    moveRight();
    int offset = round(HEIGHT / 2);

    for (int i = 1; i <= HEIGHT; i++){
        drawPixel(1, i, CRGB::Black);
    }
    
    for (int i = 1; i <= HEIGHT; i++){
        drawPixel(1, offset, CRGB::Blue);
    }

    for(int i=1; i <= displayPeakL; i++) {
      if(gay) {
         color = CHSV(map(i, 1, (HEIGHT/2), 0, 230), 255, 255);
      }
      else {
          unsigned int g = map(i, 1, HEIGHT, 254, 0);
          color = CRGB(map(i , 1, HEIGHT, 0, 254), g, 0);
      }
      drawPixel(1, (offset - i), color);
    }
    for(int i=1; i <= displayPeakR; i++) {
      if(gay) {
         color = CHSV(map(i, 1, (HEIGHT/2), 230, 0), 255, 255);
      }
      else {
          unsigned int g = map(i, 1, (HEIGHT/2), 254, 0);
          color = CRGB(map(i , 1, (HEIGHT/2), 0, 254), g, 0);
      }
      drawPixel(1, (offset + i), color);
    }
      
    FastLED.show();
  }
}

/*
1,1 = 0
1,2 = 1
2,1 = 15
*/

int xytopixel(int x, int y) {
  int p = ((x - 1) * HEIGHT) + (y - 1);
  return p;
}

void drawPixel(int x, int y, CRGB color) {
  leds[xytopixel(x, y)] = color;
}

void moveRight() {
  // Update the display:
  for (int i = HEIGHT; i >= 1; i--) {
    for (int j = WIDTH; j >= 1; j--) {
      int src = xytopixel((j - 1), i);
      int dst = xytopixel(j, i);
      leds[dst] = leds[src];
    }
  }
}


