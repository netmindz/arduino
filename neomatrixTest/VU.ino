int HEIGHTP = kMatrixWidth;
int WIDTHP = kMatrixHeight;

void VU() {
  CRGB color;

  const boolean gay = false;

  bool newReading = MSGEQ7.read(ReadsPerSecond(20));

  // Led strip output
  if (newReading) {

    int displayPeakL = map(MSGEQ7.get(MSGEQ7_LOW, 0), 0, 255, 0, round(HEIGHTP / 2));
    int displayPeakR = map(MSGEQ7.get(MSGEQ7_LOW, 1), 0, 255, 0, round(HEIGHTP / 2));
    Serial.print("Display peak: ");
    Serial.println(displayPeakL);

    moveRight();
    int offset = round(HEIGHTP / 2);

    for (int i = 1; i <= HEIGHTP; i++){
        drawPixelP(1, i, CRGB::Black);
    }
    
    for (int i = 1; i <= HEIGHTP; i++){
        drawPixelP(1, offset, CRGB::Blue);
    }

    for(int i=1; i <= displayPeakL; i++) {
      if(gay) {
         color = CHSV(map(i, 1, (HEIGHTP/2), 0, 230), 255, 255);
      }
      else {
          unsigned int g = map(i, 1, HEIGHTP, 254, 0);
          color = CRGB(map(i , 1, HEIGHTP, 0, 254), g, 0);
      }
      drawPixelP(1, (offset - i), color);
    }
    for(int i=1; i <= displayPeakR; i++) {
      if(gay) {
         color = CHSV(map(i, 1, (HEIGHTP/2), 230, 0), 255, 255);
      }
      else {
          unsigned int g = map(i, 1, (HEIGHTP/2), 254, 0);
          color = CRGB(map(i , 1, (HEIGHTP/2), 0, 254), g, 0);
      }
      drawPixelP(1, (offset + i), color);
    }
      
    FastLED.show();
  }
}

void drawPixelP(int x, int y, CRGB color) {
  leds[xytopixelP(x, y)] = color;
}

int xytopixelP(int x, int y) {
  int p = ((x - 1) * HEIGHTP) + (y - 1);
  return p;
}


void moveRight() {
  // Update the display:
  for (int i = HEIGHTP; i >= 1; i--) {
    for (int j = WIDTHP; j >= 1; j--) {
      int src = xytopixelP((j - 1), i);
      int dst = xytopixelP(j, i);
      leds[dst] = leds[src];
    }
  }
}




