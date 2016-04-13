// EQ vars 

int eq_hue = 0;
int eq_hue_wait = 10;

void EQ() {

  int xpos;
  // analyze without delay
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Led strip output
  if (newReading) {
    eq_hue_wait++;
    if(eq_hue_wait > 1) {
      eq_hue += 10;
      if(eq_hue > 255) eq_hue = 0;
      eq_hue_wait = 0;
    }
    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      xpos = map((band  + 1), 1, 7, 7, 1);
      int count = map(MSGEQ7.get(band), 0, 255, 0, kMatrixHeight);
      for (int i = 1; i <= kMatrixWidth; i++) {
        if (i <= count) {
          // unsigned int g = map(band, 0, 6, 254, 0);
          // CRGB color = CRGB(map(i , 1, WIDTH, 254, 0), g, 0);
          CRGB color = CHSV(eq_hue + (band * 20), 255,255);
          safeSetPixel(XY(xpos, i), color);
        }
        else {
          safeSetPixel(XY(xpos, i) ,CRGB::Black);
        }
      }
    }

    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      xpos = 8 + band  + 1;
      int count = map(MSGEQ7.get(band), 0, 255, 0, kMatrixHeight);
      for (int i = 1; i <= kMatrixWidth; i++) {
        if (i <= count) {
//          unsigned int g = map(band, 0, 6, 254, 0);
//          CRGB color = CRGB(map(i , 1, WIDTH, 254, 0), g, 0);
          CRGB color = CHSV(eq_hue + (band * 20), 255,255);
          safeSetPixel(XY(xpos, i), color);
        }
        else {
          safeSetPixel(XY(xpos, i), CRGB::Black);
        }
      }
    }

    FastLED.show();
  }
}

void safeSetPixel(int pos, CRGB value) {
  if(pos > (NUM_LEDS -1)) {
    Serial.print("IXP ");
    Serial.println(pos);
  }
  else {
    leds[pos] = value;
  }
}


