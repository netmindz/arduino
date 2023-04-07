int hue = 0;
int hueOffset = 10;
void squares() {
  FastLED.clear();
  for(int i = 0; i < (kMatrixWidth / 2); i++) {
    square(i, ColorFromPalette(currentPalette, (hue + (hueOffset * (i * 0.5))), 255, LINEARBLEND));
  }
  hue += 8;
  if(hue % 4 == 0) hueOffset += 1;
  delay(10);
}

void square(int s, CRGB color) {
  int p = (kMatrixWidth - 1) - s;
  for(int i = s; i <= p; i++) {
    leds[XY(i,s)] = color;
  }
  for(int i = s; i <= p; i++) {
    leds[XY(s,i)] = color;
  }
  for(int i = s; i <= p; i++) {
    leds[XY(p,i)] = color;
  }
  for(int i = s; i <= p; i++) {
    leds[XY(i,p)] = color;
  }
}

