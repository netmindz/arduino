int hue = 0;
void rainbowSweep() {
  int jump = random(0, map(STEPS, 0, 255, 0, 25)); // TWEAK ME
  int hue = random(0, 255); // Audio ?
  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      leds[ XY(x, y)]  = CHSV( hue, 255, 255);
    }
    FastLED.delay(map(SPEEDO,0,255, 0, 1000));  // TWEAK ME was 70
    hue += jump;
    if (hue > 255) hue = 0;
    fadeToBlackBy(leds, NUM_LEDS, map(SPEEDO,0,255, 0, 100));
  }
}
