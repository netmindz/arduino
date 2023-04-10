

void drawPixel(int x, int y, CRGB color) {
  leds[XY(x, y)] = color;
}


// Funky plank  -----------------------------------------------------------------------------------

void FunkyPlank() {
  int barWidth = (kMatrixHeight / 8);

  bool newReading = MSGEQ7read();

  // Led strip output
  if (newReading) {

    // display values of left channel on DMD
    for (int band = 0; band < 8; band++ )
    {
      int hue = MSGEQ7get(band, 0);
      int v = map(MSGEQ7get(band, 0), 0, MSGEQ7_OUT_MAX, 10, 255);
      for (int b = 0; b < barWidth; b++) {
        int  pos = (barWidth * band) + b;
        drawPixel(0, pos, CHSV(hue, 255, v));
        Serial.printf("pos: %u band: %u hue: %u v: %u\n", pos, band, hue, v);
        //      drawPixel((offset + band + 1), 0, CHSV(hue, 255, 255));
      }
    }

    // Update the display:
    for (int i = (kMatrixHeight - 1); i >= 0; i--) {
      for (int j = (kMatrixWidth - 1); j >= 0; j--) {
        int src = XY((j - 1), i);
        int dst = XY(j, i);
        leds[dst] = leds[src];
      }
    }
    FastLED.show(); // TODO: needs non-blocking delay

  }
}


// DJ Light  -----------------------------------------------------------------------------------


void DJLight() {

  bool newReading = MSGEQ7read();

//  int offset = 8;

  int mid = NUM_LEDS / 2;

  // Led strip output
  if (newReading) {

    int bands[8];
    for (int band = 0; band < 8; band++ ) {
      bands[band] = MSGEQ7get(band, 1) / 2;
    }


//    leds[mid] = CRGB(bands[6], bands[5] / 8, bands[1] / 2);
//    leds[mid].fadeToBlackBy(bands[3] / 12);


    leds[mid] = CRGB(bands[5], bands[2], bands[0]);
    leds[mid].fadeToBlackBy((map(bands[1], 0, MSGEQ7_OUT_MAX, 255, 10)));

   //  Serial.printf("RGB: %u %u %u\n", bands[5], bands[2], bands[0]);

    
    //move to the left
    for (int i = NUM_LEDS - 1; i > mid; i--) {
      leds[i] = leds[i - 1];
    }
    // move to the right
    for (int i = 0; i < mid; i++) {
      leds[i] = leds[i + 1];
    }

   EVERY_N_MILLISECONDS_I(DJLight_fade, map(FADE, 0, 255, 1000, 10)) {     // TODO: tweak fade rate
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].fadeToBlackBy(10);
    }
   }
   //showSegments();
   FastLED.show(); // TODO: needs non-blocking delay

  }
}

// EQ ------------------------------------------------------------------------------------
int eq_hue_wait = 10;
int eq_hue = 0;


void EQ() {

  int xpos;

  int barWidth = (kMatrixHeight / 14);
  int blockWidth = barWidth * 7;

  // analyze without delay
  bool newReading = MSGEQ7read();

  // Led strip output
  if (newReading) {
    eq_hue_wait++;
    if (eq_hue_wait > 20) {
      eq_hue += 15;
      if (eq_hue > 255) eq_hue = 0;
      eq_hue_wait = 0;
    }
    // display values of left channel on DMD
    for ( int band = 0; band < 7; band++ ) {
      int count = map(mapNoise(MSGEQ7get(band, 0)), 0, 255, 0, kMatrixWidth);
      //int count = map(band, 0, 6, 1, HEIGHT);
      for (int b = 0; b < barWidth; b++) {
        xpos = blockWidth - (barWidth * band) - b;
        if (b == 0 && band == 0 && xpos != 7) {
          Serial.print("Wrong xpos for band0 = ");
          Serial.println(xpos);
        }
        for (int i = 0; i <= kMatrixWidth; i++) {
          if (i <= count) {
            // unsigned int g = map(band, 0, 6, 254, 0);
            // CRGB color = CRGB(map(i , 1, WIDTH, 254, 0), g, 0);
            CRGB color = CHSV(eq_hue + (band * 15), 255,255);
            //CRGB color = CHSV((band * 35), 255, 255);
            safeSetPixel(xytopixel(i, xpos), color);
          }
          else {
//            safeSetPixel(xytopixel(i, xpos) , CRGB::Black);
            leds[xytopixel(i, xpos)].fadeToBlackBy(map(FADE, 0, 255, 100, 5));
          }
        }
      }
    }

    // display values of left channel on DMD
    for ( int band = 0; band < 7; band++ )
    {
      int count = map(mapNoise(MSGEQ7get(band, 1)), 0, 255, 0, kMatrixWidth);
      //int count = map(band, 0, 6, 1, HEIGHT);

      for (int b = 0; b < barWidth; b++) {
        xpos = blockWidth + 1 + (barWidth * band) + b;
        for (int i = 0; i <= kMatrixWidth; i++) {
          if (i <= count) {
            //          unsigned int g = map(band, 0, 6, 254, 0);
            //          CRGB color = CRGB(map(i , 1, WIDTH, 254, 0), g, 0);
            //          CRGB color = CHSV(map(count,0, HEIGHT, 0, 255) , 255,255);
            //CRGB color = CHSV(eq_hue + (band * 15), 255,255);
            CRGB color = CHSV((band * 35), 255, 255);
            safeSetPixel(xytopixel(i, xpos), color);
          }
          else {
//            safeSetPixel(xytopixel(i, xpos), CRGB::Black);
            leds[xytopixel(i, xpos)].fadeToBlackBy(map(FADE, 0, 255, 100, 5));
          }
        }
      }
    }

   FastLED.show();
  }
}
int xytopixel(int x, int y) {
  // return ((y - 1) * WIDTH) + (x - 1);
  return XY(x, y);
}

void safeSetPixel(int pos, CRGB value) {
  if (pos > (NUM_LEDS - 1)) {
    Serial.print("IXP ");
    Serial.println(pos);
  }
  else {
    leds[pos] = value;
  }
}
