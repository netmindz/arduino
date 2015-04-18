/****************************************
Scrolling Sound Meter Sketch for the
Adafruit Microphone Amplifier
****************************************/
     
#include <Wire.h>
#include <FastLED.h>

#define WIDTH 8
#define HEIGHT 8
#define DATA_PIN 6

CRGB leds[(WIDTH * HEIGHT)];

int LED_RED = CRGB::Red;
int LED_GREEN = CRGB::Green;


     
const int maxScale = 8;
const int redZone = 5;
     
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
     
void setup() {
  Serial.begin(9600);
  FastLED.setBrightness(10);   
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
  peakToPeak = signalMax - signalMin;
  
  // map 1v p-p level to the max scale of the display
  int displayPeak = map(peakToPeak, 0, 1023, 0, maxScale);
  
  // Update the display:
  for (int i = 0; i < 7; i++) // shift the display left
  {
    // matrix.displaybuffer[i] = matrix.displaybuffer[i+1];
  }
  
  // draw the new sample
  for (int i = 0; i <= maxScale; i++)
  {
    if (i >= displayPeak) // blank these pixels
    {
      drawPixel(i, 7, 0);
    }
    else if (i < redZone) // draw in green
    {
      drawPixel(i, 7, LED_GREEN);
    }
    else // Red Alert! Red Alert!
    {
      drawPixel(i, 7, LED_RED);
    }
  }
  FastLED.show();
}


int xytopixel(int x, int y) {
  int p = ((y -1) * WIDTH) + (x -1);
  /*
  Serial.print("xytopixel ");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(" = ");
  Serial.println(p);
  */
  return p;
}

void drawPixel(int x, int y, int color) {
  /*
  Serial.print("draw ");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.println(color);
  */
  leds[xytopixel(x, y)] = color;
}

