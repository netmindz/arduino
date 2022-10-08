/*
Some useful functions for developing and debugging.
 Contains some magic numbers and is written specificly
 for the SmartMatrix.
 
 
 -----------------------------------------------------------------
 */


// Show the current palette.

void ShowPalette() {
  for(int i = 0; i < NUM_LEDS; i++) {
    byte color = i / (NUM_LEDS / 256);
    byte bri = 255; 
    leds[i] = ColorFromPalette( currentPalette, color, bri );
  }
}


// show just one layer

void ShowLayer(byte layer, byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = noise[layer][i][j]; 

      uint8_t   bri = color;

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
}


// map a layer while ignoring the brightness information and replacing it by maximum

void ShowLayerBright(byte layer, byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = noise[layer][i][j]; 

      uint8_t   bri = 255;

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
}


// Show 3 small 16x16 versions of the 3 noise planes
// to keep track what is going on where when.
// Useful to check before you start merging layers.
// Expects a 32x32 matrix to be the output device.

void Show3Layers() {
  for(uint8_t i = 0; i < 16; i++) {
    for(uint8_t j = 0; j < 16; j++) {
      leds[XY(i,j)] = ColorFromPalette( currentPalette, noise[0][i*2][j*2]*2 , 255 );
    }
  }
  for(uint8_t i = 16; i < 32; i++) {
    for(uint8_t j = 0; j < 16; j++) {
      leds[XY(i,j)] = ColorFromPalette( currentPalette, noise[1][(i-16)*2][j*2]*2 , 255 );
    }
  }
  for(uint8_t i = 0; i < 16; i++) {
    for(uint8_t j = 16; j < 32; j++) {
      leds[XY(i,j)] = ColorFromPalette( currentPalette, noise[2][i*2][(j-16)*2]*2 , 255 );
    }
  }
}

// serial output parameters for debugging

void ShowParameters(byte layer) {
  Serial.print("L");
  Serial.print(layer);
  Serial.print(" ");
  Serial.print(x[layer]); 
  Serial.print(" "); 
  Serial.print(y[layer]); 
  Serial.print(" "); 
  Serial.print(z[layer]); 
  Serial.print(" ");
  Serial.print(scale_x[layer]); 
  Serial.print(" ");
  Serial.print(scale_y[layer]); 
  Serial.print(" ");   
}

// serial output the noise value of noise[layer][0][0] for debugging

void SerialWriteNoiseValue(byte layer) {
  Serial.print("Layer");
  Serial.print(layer);
  Serial.print(": ");
  Serial.print(noise[layer][0][0]);
  Serial.print("  ");
}


// serial output menu values for debugging

void ShowMenuValues() {
  EVERY_N_SECONDS(10) {
  // serial print all relevant data
  /*
  Serial.print("Mode ");
  Serial.print(mode);
  */
  Serial.print(" PGM ");
  Serial.print(pgm);
  /*
  Serial.print(" SPD ");
  Serial.print(spd);
  Serial.print(" BRI ");
  Serial.print(brightness);
  Serial.print(" RED ");
  Serial.print(red_level);
  Serial.print(" GRN ");
  Serial.print(green_level);
  Serial.print(" BLU ");
  Serial.print(blue_level);
  */
  Serial.print(" FPS: ");
  Serial.println(LEDS.getFPS());
  }
}


// under construction!

void ShowNumberDistribution() {
  currentPalette = RainbowColors_p;
  x[0] += 1000;
  y[0] += 1000;
  z[0] += 1000;
  FillNoise(0);
  CLS();
  // clear array
  for(uint16_t i = 0; i < 256; i++) {
    values[i] = 0;
  }
  // count values
  for(uint16_t i = 0; i < 32; i++) {
    for(uint16_t j = 0; j < 32; j++) {
      //if (noise[0][i][j] == 133) values[0]++;
      //if (noise[0][i][j] == 129) values[1]++;
      values[noise[0][i][j]]++;
    }
  }
  // output a part of the result
  for(uint16_t i = 150; i < 170; i++) {
    Serial.print(" ");
    Serial.print(values[i]);
  }
  // draw chart
  for(uint8_t i = 100; i < 132; i++) {
    for(uint8_t j = 0; j < values[i]; j++) {
      leds[XY(i-100, 32-(j/4))] = 0xFF0000;
    }
  }
}





void ledtest() {
    int hue = 0;
  for (int y = 0; y < kMatrixHeight; y++) {
    for (int x = 0; x < kMatrixWidth; x++) {
      leds[ XY(x, y)]  = CHSV( hue, 255, 255);
      ShowFrame();
      leds[ XY(x, y)]  = CRGB::Black;
      hue += 5;
      if (hue > 255) hue = 0;
    }
  }

}




