class DSnake {
    int p = 0;
    int hue = random(0,255);
    int v = random(50,255);

  public:
    DSnake() {

    }

    void snake() {
      leds[p]  = ColorFromPalette(currentPalette, hue, v, currentBlending);
      p++;
      if(p > ( NUM_LEDS - 1)) {
        p=0;
        hue = random(0,255);
      }
    }
};

#define NUM_DSNAKES 40
DSnake dsnakes[NUM_DSNAKES];

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
     fadeToBlackBy(leds, NUM_LEDS, map(FADE,0,255, 100, 1));
//   fadeLightBy(leds, NUM_LEDS, map(STEPS,0,255, 0, 100));
   FastLED.delay(map(SPEEDO,0,255, 100, 10));  // TWEAK ME (was 40)
}

