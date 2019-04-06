#include <FastLED.h>

#define LED_PIN  6

#define COLOR_ORDER RGB
#define CHIPSET     WS2811

#define NUM_LEDS 900

CRGB leds[NUM_LEDS];      //naming our LED array

class Star {
    int p = 0;
    int dir = 0;
    uint8_t hue = random(0, 255);
    int v = 0;
    int vary = random(1, 5);
    int r = 15;

  public:
    Star() {

    }

    void setPixel(int pixel) {
      p = pixel;
    }

    void sparkle() {
      // should we change colour?
      if (random(1, r) == 1) {
        if (dir > 0) {
          hue += vary;
        }
        else {
          hue -= vary;
        }
        // should we change direction?
        if (random(1, 25) == 1) {
          dir = random(0, 1);
          if(v == 0) v = 255;
        }
      }
      if(v >= 1) v--;
      leds[p]  = CHSV(hue, 255, v);
    }
};



Star stars[NUM_LEDS];

void setup() {
//  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<APA102, 7, 14, RGB, DATA_RATE_MHZ(8)>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  FastLED.setBrightness( 130 );
  FastLED.setDither(0);

  for (int  i = 0; i < NUM_LEDS; i++) {
    stars[i].setPixel(i);
  }
}

void loop() {
  sparkle2();
}

int p = 0;
int hue = 0;


void sparkle2() {

  for (int  i = 0; i < NUM_LEDS; i++) {
    stars[i].sparkle();
  }
  FastLED.delay(0);
}


