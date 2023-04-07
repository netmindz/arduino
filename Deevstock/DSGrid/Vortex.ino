class Vortex {
    int x = 0;
    int y = 0;
    int right = 1;
    int up = 0;
    int w = kMatrixWidth - 1;
    int h = kMatrixHeight - 1;
    int margin = 0;
    int hue;

  public:
    Vortex() {

    }

    void vortex() {
      leds[ XY(x, y)]  = ColorFromPalette(currentPalette, hue, 255, LINEARBLEND);
      // End of right
      if ((x >= (w - margin)) && (up == 0)) {
        up = 1;
        right = 0;
          //    vortexHue += 15;
      }
      // End of up
      else if ((y >= (h - margin)) && (up == 1)) {
        up = 0;
        right = -1;
        //    vortexHue += 15;
      }
      // End of left
      else if ((x == (0 + margin)) && (up == 0 && right == -1)) {
        up = -1;
        right = 0;
        //    vortexHue += 15;
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
Vortex vortexs[NUM_SNAKES];

int vortexsStarted = 1;
//int const vortexDelay = ((kMatrixWidth * 2) + (kMatrixHeight * 2) - 1);
int const vortexDelay = SNAKE_GAP;
int vortexGap = 0;

void vortex() {

  if (vortexsStarted < NUM_SNAKES) {
    if (vortexGap == vortexDelay)  {
      vortexsStarted++;
      vortexGap = 0;
    }
    else {
      vortexGap++;
    }
  }

  for (int  i = 0; i < vortexsStarted; i++) {
    vortexs[i].vortex();
  }
  fadeToBlackBy(leds, NUM_LEDS, map(FADE, 0, 255, 100, 0));
  FastLED.delay(map(SPEED, 0, 255, 70, 0));
  EVERY_N_SECONDS(10) {
    Serial.printf("FPS: %u\n", FastLED.getFPS());
  }
}


