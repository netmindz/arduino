class Snake {
    int x = 0;
    int y = 0;
    int right = 1;
    int up = 0;
    int w = kMatrixWidth - 1;
    int h = kMatrixHeight - 1;
    int margin = 0;
    int hue;

  public:
    Snake() {

    }

    void snake() {
      leds[ XY(x, y)]  = ColorFromPalette(currentPalette, hue, 255, currentBlending);
      // End of right
      if ((x >= (w - margin)) && (up == 0)) {
        up = 1;
        right = 0;
        //    snakeHue += 15;
      }
      // End of up
      else if ((y >= (h - margin)) && (up == 1)) {
        up = 0;
        right = -1;
        //    snakeHue += 15;
      }
      // End of left
      else if ((x == (0 + margin)) && (up == 0 && right == -1)) {
        up = -1;
        right = 0;
        //    snakeHue += 15;
      }
      // End of down
      else if ((x == (0 + margin) && y == (1 + margin)) && (up == -1 && right == 0)) {
        y += up;
        up = 0;
        right = 1;
        hue += 15;
        margin++;
        y++;
      }

      // Final LED
      if (right == 0 && up == -1 && margin == 3) {
        //delay(1500);
        hue = random(0, 255);
        margin = 0;
        x = -1;
        y = 0;
        right = 1;
        up = 0;
      }

     // Option toggle?
      fadeToBlackBy(leds, (kMatrixWidth * kMatrixHeight), map(SPEEDO,0,255, 0, 100)); // TWEAK ME
      //  fadeLightBy(leds, (kMatrixWidth * kMatrixHeight), 20); 
      x += right;
      y += up;
    }
};

#define NUM_SNAKES 3
Snake snakes[NUM_SNAKES];

int snakesStarted = 1;
// int const snakeDelay = ((kMatrixWidth * 2) + (kMatrixHeight * 2) - 1);
int const snakeDelay = 10;
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

  for (i = 0; i < snakesStarted; i++) {
    snakes[i].snake();
  }
  FastLED.delay(map(SPEEDO,0,255, 0, 1000));  // TWEAK ME was 155
}


