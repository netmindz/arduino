#include <FastLED.h>

#define LED_PIN  6

#define COLOR_ORDER RGB
#define CHIPSET     WS2811

#define NUM_LEDS 400

CRGB leds[NUM_LEDS];      //naming our LED array

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( 255 );
}

void loop() {
//  chase();
snake();
}

int p = 0;
int hue = 0;
void chase() {
      leds[p]  = CHSV( hue, 255, 255);
      FastLED.show();
      delay(1);
      hue += random(0,20);
      if (hue > 255) hue = 0;
      p++;
      if(p > ( NUM_LEDS - 1)) p=0;
      for(int i = 0; i < NUM_LEDS; i++) {
//        leds[i].fadeToBlackBy(10);
        leds[i].fadeLightBy(10);
      }
}


class Snake {
    int p = 0;
    int hue = random(0,255);
    int v = random(50,255);

  public:
    Snake() {

    }

    void snake() {
      leds[p]  = CHSV(hue, 255, v);
      p++;
      if(p > ( NUM_LEDS - 1)) {
        p=0;
        hue = random(0,255);
      }
    }
};

#define NUM_SNAKES 30
Snake snakes[NUM_SNAKES];

int snakesStarted = 0;
int const snakeDelay = 20;
int snakeGap = 0;

void snake() {

  if (snakesStarted < NUM_SNAKES) {
    if (snakeGap == snakeDelay)  {
      snakesStarted++;
      snakeGap = 0;
    }
    else {
      snakeGap++;
    }
  }

  for (int  i = 0; i < snakesStarted; i++) {
    snakes[i].snake();
  }
     fadeToBlackBy(leds, NUM_LEDS, 20);
//      fadeLightBy(leds, NUM_LEDS, 2);
  FastLED.delay(40);
}


