/****************************************
 * Scrolling Sound Meter Sketch for the
 * Adafruit Microphone Amplifier
 ****************************************/

#include <Wire.h>
#include <FastLED.h>
#include <Average.h>

#define WIDTH 8
#define HEIGHT 8

#include "MSGEQ7.h"
#define pinAnalogLeft A0
#define pinAnalogRight A1
#define pinReset 5
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(10)
#define MSGEQ7_SMOOTH true

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

#define LED_PIN 2
#define CLOCK_PIN 6

#define COLOR_ORDER BGR
#define CHIPSET     DOTSTAR

#define BRIGHTNESS 20

#define WIDTH 15
#define HEIGHT 7


#define BUFFER_INTERVAL 10

CRGB leds[(WIDTH * HEIGHT)];
Average<unsigned int> peaks(200);

CRGB color;

unsigned int sample;
unsigned int maxPeak = 0;
const boolean gay = true;
const boolean runningGain = true;

unsigned int bufferPos = 0;
unsigned int runningMax = 0;

void setup() {
  Serial.begin(9600);
  FastLED.setBrightness(100);
  FastLED.addLeds<DOTSTAR, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, (WIDTH * HEIGHT)).setCorrection( TypicalLEDStrip );
}


void loop() {
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Led strip output
  if (newReading) {


    // map 1v p-p level to the max scale of the display
    int displayPeak = map(MSGEQ7.get(MSGEQ7_BASS), 0, 255, 0, HEIGHT);
    //  Serial.print("Display peak: ");
    //  Serial.println(displayPeak);

    moveLeft();

    // draw the new sample
    for (int i = 1; i <= HEIGHT; i++)
    {
      if (i > displayPeak) // blank these pixels
      {
        drawPixel(i, 1, CRGB::Black);
      }
      else
      {

        if (gay) {
          unsigned int v = 254;
          //        unsigned int v = map(displayPeak, 1, HEIGHT, 50, 254);
          color = CHSV(map(i, 1, HEIGHT, 230, 0), 255, v);
        }
        else {
          unsigned int g = map(i, 1, HEIGHT, 254, 0);
          color = CRGB(map(i , 1, HEIGHT, 0, 254), g, 0);
        }

        drawPixel(i, 1, color);
      }
    }
    FastLED.show();
  }
}


int xytopixel(int x, int y) {
  int p = ((y - 1) * WIDTH) + (x - 1);
  return p;
}

void drawPixel(int x, int y, CRGB color) {
  leds[xytopixel(x, y)] = color;
}

void moveLeft() {
  // Update the display:
  for (int i = HEIGHT; i > 1; i--) {
    for (int j = WIDTH; j >= 1; j--) {
      int src = xytopixel(j, (i - 1));
      int dst = xytopixel(j, i);
      leds[dst] = leds[src];
    }
  }
}


