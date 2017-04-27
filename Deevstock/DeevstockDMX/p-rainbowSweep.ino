int hue = 0;
int rsX = 0;
int jump = 0; 
void rainbowSweep() {
  
  
    for (int y = 0; y < kMatrixHeight; y++) {
      leds[ XY(rsX, y)]  = CHSV( hue, 255, 255);
    }
    FastLED.delay(map(SPEEDO,0,255, 200, 1));  // TWEAK ME was 70
    fadeToBlackBy(leds, NUM_LEDS, map(FADE,0,255, 100, 5));
    if(rsX < kMatrixWidth) {
      rsX++;
      hue += jump;
      if (hue > 255) hue = 0;
    }
    else {
      rsX = 0;
      hue = random(0, 255); // Audio ?
      jump = random(0, map(STEPS, 0, 255, 0, 100)); // TWEAK ME
    }
}
