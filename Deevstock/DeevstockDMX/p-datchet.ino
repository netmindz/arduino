//int p = 0;
//int hue = 0;
//void chase() {
//      leds[p]  = CHSV( hue, 255, 255);
//      FastLED.show();
//      delay(1);
//      hue += random(0,20);
//      if (hue > 255) hue = 0;
//      p++;
//      if(p > ( NUM_LEDS - 1)) p=0;
//      for(int i = 0; i < NUM_LEDS; i++) {
////        leds[i].fadeToBlackBy(10);
//        leds[i].fadeLightBy(10);
//      }
//}


class DSnake {
    int p = 0;
    int hue = random(0,255);
    int v = random(50,255);

  public:
    DSnake() {

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

#define NUM_DSNAKES 30
DSnake dsnakes[NUM_SNAKES];

int dsnakesStarted = 0;
int const dsnakeDelay = 20;
int dsnakeGap = 0;

void dsnake() {

  if (dsnakesStarted < NUM_DSNAKES) {
    if (dsnakeGap == dsnakeDelay)  {
      dsnakesStarted++;
      dsnakeGap = 0;
    }
    else {
      dsnakeGap++;
    }
  }

  for (int  i = 0; i < dsnakesStarted; i++) {
    dsnakes[i].snake();
  }
     fadeToBlackBy(leds, NUM_LEDS, map(STEPS,0,255, 0, 100));
//   fadeLightBy(leds, NUM_LEDS, map(STEPS,0,255, 0, 100));
   FastLED.delay(map(SPEEDO,0,255, 0, 1000));  // TWEAK ME (was 40)
}

