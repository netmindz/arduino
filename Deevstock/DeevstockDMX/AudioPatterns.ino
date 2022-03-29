boolean MSGEQ7read() {
  return fft_available;
}

#define MSGEQ7_OUT_MAX 255

int MSGEQ7get(int band, int channel) {
  int value = map((fftData[band] * 1000), 0, 100, 0, MSGEQ7_OUT_MAX); // TODO: should be 100, but testing at home
//  Serial.printf("Band: %u = %u\n", band, value);
  return value;
}

int barWidth = (kMatrixHeight / 8);

void drawPixel(int x, int y, CRGB color) {
  leds[XY(x, y)] = color;
}


// Funky plank  -----------------------------------------------------------------------------------

void FunkyPlank() {

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

  int mid = NUM_AUDIO_LEDS / 2;

  // Led strip output
  if (newReading) {

    int bands[8];
    for (int band = 0; band < 8; band++ ) {
      bands[band] = MSGEQ7get(band, 1) / 2;
    }


//    leds[mid] = CRGB(bands[6], bands[5] / 8, bands[1] / 2);
//    leds[mid].fadeToBlackBy(bands[3] / 12);


    ledsAudio[mid] = CRGB(bands[5], bands[2], bands[0]);
    ledsAudio[mid].fadeToBlackBy((map(bands[1], 0, MSGEQ7_OUT_MAX, 255, 10)));

     Serial.printf("RGB: %u %u %u\n", bands[5], bands[2], bands[0]);

    
    //move to the left
    for (int i = NUM_AUDIO_LEDS - 1; i > mid; i--) {
      ledsAudio[i] = ledsAudio[i - 1];
    }
    // move to the right
    for (int i = 0; i < mid; i++) {
      ledsAudio[i] = ledsAudio[i + 1];
    }

   EVERY_N_MILLISECONDS(300) {
    // TODO: map to fade
    for (int i = 0; i < NUM_AUDIO_LEDS; i++) {
      ledsAudio[i].fadeToBlackBy(10);
    }
   }
   showSegments();
  }
}
