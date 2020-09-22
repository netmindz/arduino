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
      leds[ XY(x, y)]  = CHSV(hue, 255, 255);
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
      if (right == 0 && up == -1 && margin == ( (kMatrixHeight / 2) - 1)) { // >=
        //delay(5000);
        hue = random(0, 255);
        margin = 0;
        x = -1;
        y = 0;
        right = 1;
        up = 0;
      }

     // Option toggle?
      //  fadeLightBy(leds, (kMatrixWidth * kMatrixHeight), 20); 
      x += right;
      y += up;
    }
};

#define SNAKE_GAP 30
#define NUM_SNAKES (NUM_LEDS / SNAKE_GAP) 
Snake snakes[NUM_SNAKES];

int snakesStarted = 1;
//int const snakeDelay = ((kMatrixWidth * 2) + (kMatrixHeight * 2) - 1);
int const snakeDelay = SNAKE_GAP;
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
  fadeToBlackBy(leds, (kMatrixWidth * kMatrixHeight), 25); // TWEAK ME
  FastLED.delay(100);
  EVERY_N_SECONDS(10) {
    Serial.println(FastLED.getFPS());
  }
}


