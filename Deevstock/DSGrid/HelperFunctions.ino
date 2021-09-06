/*
Helpfull functions to keep the actual animation code short. 
 Contains so far:
 
 XY()
 FillNoise(byte layer)
 BasicVariablesSetup
 ShowFrame
 DimAll(byte value)
 CLS
 MergeMethod1(byte colorrepeat)
 MergeMethod2(byte colorrepeat)
 MergeMethod3(byte colorrepeat)
 MergeMethod4(byte colorrepeat)
 ConstrainedMapping(byte layer, byte lower_limit, byte upper_limit, byte colorrepeat)
 ShowAll(uint16_t frames_per_animation)
 ColorCorrection
 
 (uncomplete list)
 
 -----------------------------------------------------------------
 */


// Translate the x/y coordinates into the right index in the
// framebuffer.
// The Smartmatrix has a simple line by line layout, no 
// serpentines. It safed 2 fps to keep this function short.
// The function is called (sometimes) over 200 000 times per second!
//
//uint16_t XY( uint8_t x, uint8_t y) {
//  uint16_t i;
//  i = (y * kMatrixWidth) + x;
//  return i;
//}

bool kMatrixSerpentineLayout = true;

// Fill the x/y array with 16-bit noise values 
void FillNoise(byte layer) {

  for(uint8_t i = 0; i < kMatrixWidth; i++) {

    uint32_t ioffset = scale_x[layer] * (i-CentreX);

    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint32_t joffset = scale_y[layer] * (j-CentreY);

      byte data = inoise16(x[layer] + ioffset, y[layer] + joffset, z[layer]) >> 8;

      uint8_t olddata = noise[layer][i][j];
      uint8_t newdata = scale8( olddata, noisesmoothing ) + scale8( data, 256 - noisesmoothing );
      data = newdata;


      noise[layer][i][j] = data;
    }
  }
}


// Initialise the coordinates of the noise space with random
// values for an altering starting point.
// Set the zoom factor to a moderate level.
// Fill the delta values with random stepwidths.

void BasicVariablesSetup() {

  // set to reasonable values to avoid a black out
  colorshift = 0;
  noisesmoothing = 200;

  currentPalette = RainbowStripeColors_p;


  // just any free input pin
  random16_add_entropy(analogRead(18));

  // fill coordinates with random values
  // set zoom levels
  for(int i = 0; i < NUM_LAYERS; i++) {
    x[i] = random16();
    y[i] = random16();
    z[i] = random16();
    scale_x[i] = 6000;
    scale_y[i] = 6000;
  }
  // for the random movement
  dx  = random8();
  dy  = random8();
  dz  = random8();
  dsx = random8();
  dsy = random8();

  // everything for the menu
  mode = 0;
  spd = 10;
  brightness = 25;
  red_level = 255;
  green_level = 255;
  blue_level = 255;

  LEDS.setBrightness(brightness);
}


// Update leds and show fps

void ShowFrame() {

  matrix->show();

  // output debugging infos
  ShowMenuValues();
}


// Dim everything in leds a bit down.

void DimAll(byte value)  
{
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(value);
  }
}


// Delete the leds array.

void CLS()  
{
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = 0;
  }
}


// overlay layers 0&1&2 for color, layer 2 is brightness

void MergeMethod1(byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] )
        + ( noise[2][i][j] ) )
        / 3; 

      // layer 2 gives the brightness  
      uint8_t   bri = (noise[2][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
} 


// overlay layers 0&1 for color, layer 2 is brightness

void MergeMethod2(byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      // map the noise values down to a byte range
      // layer 0 and 2 interfere for the color
      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] ) )
        / 2; 

      // layer 2 gives the brightness  
      uint8_t   bri = (noise[2][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
}


// overlay layers 0&1 for color, brightness is layer1

void MergeMethod3(byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      // map the noise values down to a byte range
      // layer 0 and 2 interfere for the color
      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] ) )
        / 2; 

      // layer 1 gives the brightness  
      uint8_t   bri = noise[1][i][j];

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
}


// overlay layers 0&1&2 for color, layer 0 is brightness

void MergeMethod4(byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] )
        + ( noise[2][i][j] ) )
        / 3; 

      uint8_t   bri = (noise[0][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
} 


// draw the part between lower and upper limit of one layer

void ConstrainedMapping(byte layer, byte lower_limit, byte upper_limit, byte colorrepeat) {

  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t data =  noise[layer][i][j] ;

      if ( data >= lower_limit  && data <= upper_limit) {

        CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (data + colorshift), data );

        leds[XY(i,j)] = pixel;
      }
    }
  }
}


// one possibility for a basic scripting / frame line composition

void ShowAll(uint16_t count) {
  for(uint16_t i = 0; i < count; i++) {
    MirroredNoise();
    ShowFrame();
  }  

  for(uint16_t i = 0; i < count; i++) {
    RedClouds();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Lavalamp1();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Lavalamp2();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Lavalamp3();
    ShowFrame();
  }


  for(uint16_t i = 0; i < count; i++) {
    Lavalamp4();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Lavalamp5();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Constrained1();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    RelativeMotion1();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Water();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    Bubbles1();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    TripleMotion();
    ShowFrame();
  }

  for(uint16_t i = 0; i < count; i++) {
    CrossNoise2();
    ShowFrame();
  }
}


// allows to dim colors down
// works on the screenbuffer, after the image is computed

void ColorCorrection() {
  for(uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].r = scale8(leds[i].r, red_level);
    leds[i].g = scale8(leds[i].g, green_level);
    leds[i].b = scale8(leds[i].b, blue_level);
  }
}


// a constrained noise the fills the holes with a mirrored and recolored version of the same noise

void CrossMapping(byte colorrepeat, byte limit) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color1 = noise[0][i][j];
      uint8_t color2 = noise[0][j][i];

      CRGB pixel;

      if (color1 > limit) {
        pixel = ColorFromPalette( currentPalette, colorrepeat * (color1 + colorshift), color2 );
      }
      else {
        pixel = ColorFromPalette( currentPalette, colorrepeat * (color2 + colorshift + 128), color1 );
      }
      leds[XY(i,j)] = pixel;
    }
  }
}


// a brightness mask based on layer 0 for the complete screenbuffer

void FilterAll() {
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {
      leds[XY(i,j)] %= noise[0][i][j];
    }
  }
}
