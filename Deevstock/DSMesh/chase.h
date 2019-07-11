int p = 0;
void chase() {
  leds[p]  = ColorFromPalette(currentPalette, gHue, 255, currentBlending);
  // FastLED.show();
  gHue += random(0, STEPS);
  if (gHue > 255) gHue = 0;
  p++;
  if (p > ( NUM_LEDS - 1)) p = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(FADE);
    // leds[i].fadeLightBy(10);
  }
  FastLED.delay(map(SPEEDO, 0, 255, 200, 0));
}
