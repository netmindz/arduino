int p = 0;
void chase() {
  leds[p]  = CHSV( gHue, 255, 255);
  // FastLED.show();
  gHue += random(0, 20);
  if (gHue > 255) gHue = 0;
  p++;
  if (p > ( NUM_LEDS - 1)) p = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(10);
    // leds[i].fadeLightBy(10);
  }
  FastLED.delay(50);
}
