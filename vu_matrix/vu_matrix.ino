/****************************************
 * Scrolling Sound Meter Sketch for the
 * Adafruit Microphone Amplifier
 ****************************************/

#include <Wire.h>
#include <FastLED.h>

#define WIDTH 8
#define HEIGHT 8
#define DATA_PIN 6

CRGB leds[(WIDTH * HEIGHT)];

CRGB color;

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
unsigned int maxPeak = 0;
const boolean gay = true;

void setup() {
  Serial.begin(9600);
  FastLED.setBrightness(100);   
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, (WIDTH * HEIGHT));
}


void loop() {
  unsigned long startMillis= millis(); // Start of sample window
  unsigned int peakToPeak = 0; // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(0);
    if (sample < 1024) // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample; // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample; // save just the min levels
      }
    }
  }
  Serial.print("peakToPeak=");
  Serial.println(peakToPeak); 
  peakToPeak = signalMax - signalMin;

  if(peakToPeak > maxPeak) {
    maxPeak = peakToPeak;
  }

  // map 1v p-p level to the max scale of the display
  int displayPeak = map(peakToPeak, 0, maxPeak, 0, HEIGHT);
  Serial.print("Display peak: ");
  Serial.println(displayPeak);

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

      if(gay) {
        unsigned int v = map(displayPeak, 1, HEIGHT, 50, 254);
        color = CHSV(map(i, 1, HEIGHT, 230, 0), 255, v);
      }
      else {
        unsigned int g = map(i, 1, HEIGHT, 254, 0);
        color = CRGB(map(i ,1, HEIGHT, 0, 254), g, 0);
      }

      drawPixel(i, 1, color);
    }
  }
  FastLED.show();
}


int xytopixel(int x, int y) {
  int p = ((y -1) * WIDTH) + (x -1);
  return p;
}

void drawPixel(int x, int y, CRGB color) {
  leds[xytopixel(x, y)] = color;
}

void moveLeft() {
  // Update the display:
  for(int i=HEIGHT; i > 1; i--) {
    for(int j=WIDTH; j >= 1; j--) {
      int src = xytopixel(j,(i -1));
      int dst = xytopixel(j,i);
      leds[dst] = leds[src];
    }
  }
}


