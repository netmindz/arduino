void ledtest() {
  int hue = 0;
  for (int y = 0; y < kMatrixHeight; y++) {
    for (int x = 0; x < kMatrixWidth; x++) {
      leds[ XY(x, y)]  = ColorFromPalette(currentPalette, hue, 255, currentBlending);
      FastLED.delay(50);
      leds[ XY(x, y)]  = CRGB::Black;
      hue += 5;
      if (hue > 255) hue = 0;
    }
  }
//  for (int i = 0; i < NUM_LEDS; i++) {
//      leds[i]  = CHSV( 100, 255, 255);
//      FastLED.delay(100);
//      leds[i]  = CRGB::Black;
//      hue += 5;
//      if (hue > 255) hue = 0;
//  }
}


