
class Snake {
    int p = 0;
    int hue = random(0, 255);
    int v = random(50, 255);

  public:
    Snake() {
    }

    void snake() {
      leds[p]  = CHSV(hue, 255, v);
      p++;
      if (p > ( NUM_LEDS - 1)) {
        p = 0;
        hue = random(0, 255);
      }
    }
};

#define NUM_SNAKES 3
Snake snakes[NUM_SNAKES];

int snakesStarted = 0;
int const snakeDelay = 20;
int snakeGap = 0;

void datchet() {

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
  //     fadeToBlackBy(leds, NUM_LEDS, 30);
  fadeLightBy(leds, NUM_LEDS, 50);
  FastLED.delay(50);
}

