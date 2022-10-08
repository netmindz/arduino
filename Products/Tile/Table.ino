
//BUTTON SETUP STUFF\w43fed
byte prevKeyState = HIGH;

int band;

// EQ vars

int eq_hue = 0;
int eq_hue_wait = 10;

// audio check
int scount =  0;
int smillis = 0;
int slastmillis = 0;


int bands = 7;
int barWidth = (WIDTH / bands) / 2;
int blockWidth = barWidth * bands;

int HQbands = 16;
int HQbarWidth = (WIDTH / HQbands) / 2;
int HQblockWidth = HQbarWidth * HQbands;


// EQ ------------------------------------------------------------------------------------

void EQ() {

  int xpos;

  // analyze without delay
  bool newReading = MSGEQ7read();

  int colourInc = 255 / HQbands;

  // Led strip output
  if (newReading) {
    eq_hue_wait++;
    if (eq_hue_wait > 20) {
      eq_hue += 15;
      if (eq_hue > 255) eq_hue = 0;
      eq_hue_wait = 0;
    }

    for ( band = 0; band < HQbands; band++ )
    {
      int count = map(mapNoise(MSGEQ16get(band, 0)), 0, MSGEQ7_OUT_MAX, 0, HEIGHT);
      //int count = map(band, 0, 6, 1, HEIGHT);
      for (int b = 0; b < HQbarWidth; b++) {
        xpos = HQblockWidth - (HQbarWidth * band) - b;
        for (int i = 0; i <= HEIGHT; i++) {
          if (i <= count) {
            // unsigned int g = map(band, 0, 6, 254, 0);
            // CRGB color = CRGB(map(i , 1, WIDTH, 254, 0), g, 0);
            //CRGB color = CHSV(eq_hue + (band * 15), 255,255);
            CRGB color = CHSV((band * colourInc), 255, 255);
            safeSetPixel(xytopixel(xpos, i), color);
          }
          else {
            safeSetPixel(xytopixel(xpos, i) , CRGB::Black);
          }
        }
      }
    }

    for ( band = 0; band < HQbands; band++ )
    {
      int count = map(mapNoise(MSGEQ16get(band, 1)), 0, MSGEQ7_OUT_MAX, 0, HEIGHT);
      //int count = map(band, 0, 6, 1, HEIGHT);

      for (int b = 0; b < HQbarWidth; b++) {
        xpos = HQblockWidth + 1 + (HQbarWidth * band) + b;
        for (int i = 0; i <= WIDTH; i++) {
          if (i <= count) {
            //          unsigned int g = map(band, 0, 6, 254, 0);
            //          CRGB color = CRGB(map(i , 1, WIDTH, 254, 0), g, 0);
            //          CRGB color = CHSV(map(count,0, HEIGHT, 0, 255) , 255,255);
            //CRGB color = CHSV(eq_hue + (band * 15), 255,255);
            CRGB color = CHSV((band * colourInc), 255, 255);
            safeSetPixel(xytopixel(xpos, i), color);
          }
          else {
            safeSetPixel(xytopixel(xpos, i), CRGB::Black);
          }
        }
      }
    }

   
  }
}
int xytopixel(int x, int y) {
  // return ((y - 1) * WIDTH) + (x - 1);
  return XY(x, y);
}

// VU -----------------------------------------------------------------------------------

int getTotal(int input) {
  int t = 0;
  for (int i = 0; i < 7; i++) {
    t += MSGEQ7get(i, input);
  }
  return t / 7;
}


void VU() {
  CRGB color;

  const boolean gay = false;

  bool newReading = MSGEQ7read();

  // Led strip output
  if (newReading) {
// Serial.printf("max = %u, value=%u\n", MSGEQ7_OUT_MAX, MSGEQ7get(MSGEQ7_MID, 0));
    int displayPeakL = map(MSGEQ7get(MSGEQ7_MID, 0), 0, MSGEQ7_OUT_MAX, 0, round(HEIGHT / 2));
    int displayPeakR = map(MSGEQ7get(MSGEQ7_LOW, 1), 0, MSGEQ7_OUT_MAX, 0, round(HEIGHT / 2));
    Serial.print("Display peak: ");
    Serial.println(displayPeakL);

    moveUp();
    int offset = round(HEIGHT / 2);

    // reset first row to black
    for (int i = 0; i <= WIDTH; i++) {
        drawPixel(i, 0, CRGB::Black);
    }

    for (int i = 0; i < HEIGHT; i++) {
      drawPixel(offset, i, CRGB::Blue);
    }

    for (int i = 1; i <= displayPeakL; i++) {
      if (gay) {
        color = CHSV(map(i, 1, (HEIGHT / 2), 0, 230), 255, 255);
      }
      else {
        unsigned int g = map(i, 1, HEIGHT, 254, 0);
        color = CRGB(map(i , 1, HEIGHT, 0, 254), g, 0);
      }
      drawPixel((offset - i), 0, color);
    }
    for (int i = 1; i <= displayPeakR; i++) {
      if (gay) {
        color = CHSV(map(i, 1, (HEIGHT / 2), 230, 0), 255, 255);
      }
      else {
        unsigned int g = map(i, 1, (HEIGHT / 2), 254, 0);
        color = CRGB(map(i , 1, (HEIGHT / 2), 0, 254), g, 0);
      }
      drawPixel((offset + i), 0, color);
    }

   
  }
}

int xytopixelP(int x, int y) {
  //  int p = ((x - 1) * HEIGHTP) + (y - 1);
  //  return p;
  return XY(y, x);
}

void drawPixelP(int x, int y, CRGB color) {
  leds[xytopixelP(x, y)] = color;
}

void drawPixel(int x, int y, CRGB color) {
  leds[xytopixel(x, y)] = color;
}

void moveUp() {
  // Update the display:
  for (int i = (HEIGHT - 1); i >= 0; i--) {
    for (int j = (WIDTH - 1); j >= 0; j--) {
      int src = XY(j, (i - 1));
      int dst = XY(j, i);
      leds[dst] = leds[src];
    }
  }
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


// Funky plank  -----------------------------------------------------------------------------------


void FunkyPlank() {

  bool newReading = MSGEQ7read();

  int offset = HQblockWidth + HQbarWidth;

  // Led strip output
  if (newReading) {

    for ( band = 0; band < HQbands; band++ )
    {
      int hue = MSGEQ16get(band, 0);
      int v = map(MSGEQ16get(band, 0), 0, MSGEQ7_OUT_MAX, 10, 255);
      for (int b = 0; b < HQbarWidth; b++) {
        int  xpos = HQblockWidth - (HQbarWidth * band) - b;
        drawPixel(xpos, 0, CHSV(hue, 255, v));
        //      drawPixel((offset + band + 1), 0, CHSV(hue, 255, 255));
      }
    }

    for ( band = 0; band < HQbands; band++ )
    {
      int hue = MSGEQ16get(band, 1);
      int v = map(MSGEQ16get(band, 1), 0, MSGEQ7_OUT_MAX, 10, 255);
      for (int b = 0; b < HQbarWidth; b++) {
        int xpos = HQblockWidth + 1 + (HQbarWidth * band) + b;
        drawPixel(xpos, 0, CHSV(hue, 255, v));
      }
    }

   
    moveUp();

  }
}

// DJ Light  -----------------------------------------------------------------------------------


void DJLight() {

  // Would this be better rendered as 32x32 then scaled?
  
  bool newReading = MSGEQ7read();

  int mid = NUM_LEDS / 2;

  // Led strip output
  if (newReading) {

    int bands[8];
    for ( band = 0; band < 7; band++ ) {
      bands[band] = MSGEQ7get(band, 1);
    }

    leds[mid] = CRGB(bands[5]/2, bands[2]/2, bands[0]/2);
    leds[mid].fadeToBlackBy((map(bands[1], 0, MSGEQ7_OUT_MAX, 255, 10)));
    
    //move to the left
    for (int i = NUM_LEDS - 1; i > mid; i--) {
      leds[i] = leds[i - 1];
    }
    // move to the right
    for (int i = 0; i < mid; i++) {
      leds[i] = leds[i + 1];
    }

   EVERY_N_MILLISECONDS(1000) { // Two values depend on size of matrix (450 px to end vs 2000 30x30 vs 64x64)
    fade_down(6);
   }
  }
}

void fade_down(uint8_t value) {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].fadeToBlackBy(value);
  }
}

/*
  void soften_spectrum_data()
  {
  for (byte i = 0; i < 7; i++)
  {
    uint8_t old = bands[i];
    uint16_t data = left[i] + right[i] + old + old;
    data = data / 4;
    bands[i] = data;
  }
  }
*/
