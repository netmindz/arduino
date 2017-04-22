int hue = 0;
void rainbowSweep() {
  int jump = random(0, map(STEPS, 0, 255, 0, 25));
  int hue = random(0, 255);
  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      leds[ XY(x, y)]  = CHSV( hue, 255, 255);
    }
    FastLED.delay(70);
    hue += jump;
    if (hue > 255) hue = 0;
    fadeToBlackBy(leds, (kMatrixWidth * kMatrixHeight), 90);
  }
}
