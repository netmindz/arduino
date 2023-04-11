CRGB getCRGBForBand(int x, int pal) {
  CRGB value;
  CHSV hsv; 
  if(pal == 1) { // bit hacky to use palette id here, but don't want to litter the code with lots of different methods. TODO: add enum for palette creation type
    if(x == 1) {
      value = CRGB(MSGEQ7get(MSGEQ7_HIGH)/2, MSGEQ7get(MSGEQ7_MID)/2, MSGEQ7get(MSGEQ7_LOW)/2);
    }
    else if(x == 255) {
      value = CRGB(MSGEQ7get(MSGEQ7_HIGH)/2, MSGEQ7get(MSGEQ7_LOW)/2, MSGEQ7get(MSGEQ7_MID)/2);
    }
    else {
      value = CRGB(MSGEQ7get(MSGEQ7_LOW)/2, MSGEQ7get(MSGEQ7_MID)/2, MSGEQ7get(MSGEQ7_HIGH)/2);
    }
  }
  else if(pal == 2) {
    int b = map(x, 0, 255, 0, (NUM_GEQ_CHANNELS - 1)); // convert palette position to lower half of freq band
    hsv = CHSV(MSGEQ7get(b), 255, 255); // map(MSGEQ7get(b), 0, 255, 30, 255));  // pick hue
    hsv2rgb_rainbow(hsv, value);  // convert to R,G,B
  }
  return value;
}

CRGBPalette16 getAudioPalette(int pal) {
  // https://forum.makerforums.info/t/hi-is-it-possible-to-define-a-gradient-palette-at-runtime-the-define-gradient-palette-uses-the/63339
  
  static uint8_t xyz[16];  // Needs to be 4 times however many colors are being used.
                           // 3 colors = 12, 4 colors = 16, etc.

  xyz[0] = 0;  // anchor of first color - must be zero
  xyz[1] = 0;
  xyz[2] = 0;
  xyz[3] = 0;
  
  CRGB rgb = getCRGBForBand(1, pal);
  xyz[4] = 1;  // anchor of first color
  xyz[5] = rgb.r;
  xyz[6] = rgb.g;
  xyz[7] = rgb.b;
  
  rgb = getCRGBForBand(128, pal);
  xyz[8] = 128;
  xyz[9] = rgb.r;
  xyz[10] = rgb.g;
  xyz[11] = rgb.b;
  
  rgb = getCRGBForBand(255, pal);
  xyz[12] = 255;  // anchor of last color - must be 255
  xyz[13] = rgb.r;
  xyz[14] = rgb.g;
  xyz[15] = rgb.b;

  CRGBPalette16 p;
  return p.loadDynamicGradientPalette(xyz);
}
