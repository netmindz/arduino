/*
  https://github.com/StefanPetrick/FunkyNoise

  A bunch of animations demonstrating how to creatively combine
  simple functions for nice effects.

  MirroredNoise         a symmetric noise pattern
  RedClouds             upstreaming clouds
  Lavalamp1 - Lavlamp 5
  Constrained1
  RelativeMotion1
  Water
  Bubbles1// Fill the x/y array with 16-bit noise values

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
  ShowPalette

  (list is uncomplete)
  -----------------------------------------------------------------
*/
// can be used for palette rotation
// "colorshift"
byte colorshift;

// The coordinates for 3 16-bit noise spaces.
#define NUM_LAYERS 3

uint32_t x[NUM_LAYERS];
uint32_t y[NUM_LAYERS];
uint32_t z[NUM_LAYERS];
uint32_t scale_x[NUM_LAYERS];
uint32_t scale_y[NUM_LAYERS];

// used for the random based animations
int16_t dx;
int16_t dy;
int16_t dz;
int16_t dsx;
int16_t dsy;

// a 3dimensional array used to store the calculated
// values of the different noise planes
uint8_t noise[NUM_LAYERS][kMatrixWidth][kMatrixHeight];

// used for the color histogramm
uint16_t values[256];

uint8_t noisesmoothing;
/////////////////////////////////////////////////////////////////////////////////////
// Palettes
/////////////////////////////////////////////////////////////////////////////////////
/*
  Some color palettes.
  Includes the predifined FastLED palettes and custom ones.

  -----------------------------------------------------------------
*/


// A red-black palette.

void PaletteRed() {
  currentPalette = CRGBPalette16(
                     CHSV( 0, 255, 255 ),
                     CHSV( 0, 255, 0   ),
                     CHSV( 0, 255, 0   ),
                     CHSV( 0, 255, 255));
}


void PaletteCustom() {
  currentPalette = CRGBPalette16(
                     CHSV( 40, 255, 255),
                     CHSV( 40, 255, 255),
                     CHSV( 0, 255, 0   ),
                     CHSV( 0, 255, 255));
}

// Set here a global color palette.
// All the the predifined FastLED palettes:

void Pal() {
  //PaletteRed();
  //PaletteCustom();
  //currentPalette = CloudColors_p;
  //currentPalette = LavaColors_p;
  //currentPalette = OceanColors_p;
  currentPalette = ForestColors_p;
  //currentPalette = RainbowColors_p;
  //currentPalette = RainbowStripeColors_p;
  //currentPalette = PartyColors_p;
  //currentPalette = HeatColors_p;
}


void SetupRandomPalette() {
  currentPalette = CRGBPalette16(
                     CHSV( random8(), 255, 32 ),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 128, 255),
                     CHSV( random8(), 255, 255));
}


void Palette16() {
  currentPalette = CRGBPalette16(
                     0x000000, 0xFF0000, 0xFF0000, 0x000000,
                     0x000000, 0x00FF00, 0x00FF00, 0x000000,
                     0x000000, 0x0000FF, 0x0000FF, 0x000000,
                     0x000000, 0xFF0000, 0xFF0000, 0x000000);
}


void SetupRandomPalette2() {
  currentPalette = CRGBPalette16(
                     CHSV( random8(), 255, 0 ),
                     CHSV( random8(), 255, 0),
                     CHSV( random8(), 255, 0),
                     CHSV( random8(), 255, 0),

                     CHSV( random8(), 255, random8() ),
                     CHSV( random8(), random8(), 255),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 255, 255),

                     CHSV( random8(), 255, 0 ),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), random8(), 255),

                     CHSV( random8(), 255, 0 ),
                     CHSV( random8(), 255, 0),
                     CHSV( random8(), 255, 0),
                     CHSV( random8(), 255, 0));
}


void SetupRandomPalette3() {
  currentPalette = CRGBPalette16(
                     CHSV( random8(), 255, 0 ),
                     CHSV( random8(), 255, 0),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 255, 255),

                     CHSV( random8(), 255, 0 ),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 255, 0),

                     CHSV( random8(), 255, 0 ),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 255, 0),

                     CHSV( random8(), 255, 255 ),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 255, 0),
                     CHSV( random8(), 255, 0));
}


void SetupRandomPalette4() {
  currentPalette = CRGBPalette16(
                     CHSV( random8(), 255, random8() ),
                     CHSV( random8(), 255, random8()),
                     CHSV( random8(), 255, 0),
                     CHSV( random8(), 255, 255),

                     CHSV( random8(), 255, random8() ),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), random8(), random8()),

                     CHSV( random8(), 255, random8() ),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 255, 0),
                     CHSV( random8(), 255, random8()),

                     CHSV( random8(), 255, 255 ),
                     CHSV( random8(), 255, 0),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 255, random8()));
}

/////////////////////////////////////////////////////////////////////////////////////
// Helpers
/////////////////////////////////////////////////////////////////////////////////////
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}

// show just one layer

void ShowLayer(byte layer, byte colorrepeat) {
  for (uint8_t i = 0; i < kMatrixWidth; i++) {
    for (uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = noise[layer][i][j];

      uint8_t   bri = color;

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i, j)] = pixel;
    }
  }
}


// map a layer while ignoring the brightness information and replacing it by maximum

void ShowLayerBright(byte layer, byte colorrepeat) {
  for (uint8_t i = 0; i < kMatrixWidth; i++) {
    for (uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = noise[layer][i][j];

      uint8_t   bri = 255;

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i, j)] = pixel;
    }
  }
}


// Show 3 small 16x16 versions of the 3 noise planes
// to keep track what is going on where when.
// Useful to check before you start merging layers.
// Expects a 32x32 matrix to be the output device.

void Show3Layers() {
  for (uint8_t i = 0; i < 16; i++) {
    for (uint8_t j = 0; j < 16; j++) {
      leds[XY(i, j)] = ColorFromPalette( currentPalette, noise[0][i * 2][j * 2] * 2 , 255 );
    }
  }
  for (uint8_t i = 16; i < 32; i++) {
    for (uint8_t j = 0; j < 16; j++) {
      leds[XY(i, j)] = ColorFromPalette( currentPalette, noise[1][(i - 16) * 2][j * 2] * 2 , 255 );
    }
  }
  for (uint8_t i = 0; i < 16; i++) {
    for (uint8_t j = 16; j < 32; j++) {
      leds[XY(i, j)] = ColorFromPalette( currentPalette, noise[2][i * 2][(j - 16) * 2] * 2 , 255 );
    }
  }
}

// basically beatsin16 with an additional phase

uint16_t beatsin(accum88 beats_per_minute, uint16_t lowest = 0, uint16_t highest = 65535, byte phase = 0)
{
  uint16_t beat = beat16( beats_per_minute);
  uint16_t beatsin = (sin16( beat + (phase * 256)) + 32768);
  uint16_t rangewidth = highest - lowest;
  uint16_t scaledbeat = scale16( beatsin, rangewidth);
  uint16_t result = lowest + scaledbeat;
  return result;
}
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


// Fill the x/y array with 16-bit noise values

void FillNoise(byte layer) {

  for (uint8_t i = 0; i < kMatrixWidth; i++) {

    uint32_t ioffset = scale_x[layer] * (i - CentreX);

    for (uint8_t j = 0; j < kMatrixHeight; j++) {

      uint32_t joffset = scale_y[layer] * (j - CentreY);

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
  for (int i = 0; i < NUM_LAYERS; i++) {
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
}


// Update leds and show fps

void ShowFrame() {

  // update leds
  LEDS.show();

  // count and output the fps
  LEDS.countFPS();

}


// Dim everything in leds a bit down.

void DimAll(byte value)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(value);
  }
}


// Delete the leds array.

void CLS()
{
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = 0;
  }
}


// overlay layers 0&1&2 for color, layer 2 is brightness

void MergeMethod1(byte colorrepeat) {
  for (uint8_t i = 0; i < kMatrixWidth; i++) {
    for (uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = ( ( noise[0][i][j] )
                        + ( noise[1][i][j] )
                        + ( noise[2][i][j] ) )
                      / 3;

      // layer 2 gives the brightness
      uint8_t   bri = (noise[2][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i, j)] = pixel;
    }
  }
}


// overlay layers 0&1 for color, layer 2 is brightness

void MergeMethod2(byte colorrepeat) {
  for (uint8_t i = 0; i < kMatrixWidth; i++) {
    for (uint8_t j = 0; j < kMatrixHeight; j++) {

      // map the noise values down to a byte range
      // layer 0 and 2 interfere for the color
      uint8_t color = ( ( noise[0][i][j] )
                        + ( noise[1][i][j] ) )
                      / 2;

      // layer 2 gives the brightness
      uint8_t   bri = (noise[2][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i, j)] = pixel;
    }
  }
}


// overlay layers 0&1 for color, brightness is layer1

void MergeMethod3(byte colorrepeat) {
  for (uint8_t i = 0; i < kMatrixWidth; i++) {
    for (uint8_t j = 0; j < kMatrixHeight; j++) {

      // map the noise values down to a byte range
      // layer 0 and 2 interfere for the color
      uint8_t color = ( ( noise[0][i][j] )
                        + ( noise[1][i][j] ) )
                      / 2;

      // layer 1 gives the brightness
      uint8_t   bri = noise[1][i][j];

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i, j)] = pixel;
    }
  }
}


// overlay layers 0&1&2 for color, layer 0 is brightness

void MergeMethod4(byte colorrepeat) {
  for (uint8_t i = 0; i < kMatrixWidth; i++) {
    for (uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = ( ( noise[0][i][j] )
                        + ( noise[1][i][j] )
                        + ( noise[2][i][j] ) )
                      / 3;

      uint8_t   bri = (noise[0][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i, j)] = pixel;
    }
  }
}


// draw the part between lower and upper limit of one layer

void ConstrainedMapping(byte layer, byte lower_limit, byte upper_limit, byte colorrepeat) {

  for (uint8_t i = 0; i < kMatrixWidth; i++) {
    for (uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t data =  noise[layer][i][j] ;

      if ( data >= lower_limit  && data <= upper_limit) {

        CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (data + colorshift), data );

        leds[XY(i, j)] = pixel;
      }
    }
  }
}

// allows to dim colors down
// works on the screenbuffer, after the image is computed

void ColorCorrection() {
  //  for(uint16_t i = 0; i < NUM_LEDS; i++) {
  //    leds[i].r = scale8(leds[i].r, red_level);
  //    leds[i].g = scale8(leds[i].g, green_level);
  //    leds[i].b = scale8(leds[i].b, blue_level);
  //  }
}


// a constrained noise the fills the holes with a mirrored and recolored version of the same noise

void CrossMapping(byte colorrepeat, byte limit) {
  for (uint8_t i = 0; i < kMatrixWidth; i++) {
    for (uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color1 = noise[0][i][j];
      uint8_t color2 = noise[0][j][i];

      CRGB pixel;

      if (color1 > limit) {
        pixel = ColorFromPalette( currentPalette, colorrepeat * (color1 + colorshift), color2 );
      }
      else {
        pixel = ColorFromPalette( currentPalette, colorrepeat * (color2 + colorshift + 128), color1 );
      }
      leds[XY(i, j)] = pixel;
    }
  }
}


// a brightness mask based on layer 0 for the complete screenbuffer

void FilterAll() {
  for (uint8_t i = 0; i < kMatrixWidth; i++) {
    for (uint8_t j = 0; j < kMatrixHeight; j++) {
      leds[XY(i, j)] %= noise[0][i][j];
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////
// Patterns
/////////////////////////////////////////////////////////////////////////////////////
// MirroredNoise() draws a symmetric noise pattern
// 108 fps

void MirroredNoise() {

  // move within the noise space
  x[0] += 100;
  z[0] += 100;
  scale_x[0] = 4000;
  scale_y[0] = 4000;

  // calculate the noise array
  FillNoise(0);

  currentPalette = RainbowStripeColors_p;
  noisesmoothing = 10;

  for (int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {

      // map the noise values down
      uint16_t index = ( noise[0][i][j] + noise[0][kMatrixWidth - 1 - i][j] ) / 2;
      uint16_t   bri = 255;
      // assign a color from the HSV space
      CRGB color = ColorFromPalette( currentPalette, index, bri);

      leds[XY(i, j)] = color;
    }
  }
}


// RedClouds() draws a constrained noise space with a palette
// softening the edges.
// Upstreaming red clouds.
// 108 fps

void RedClouds() {

  // clear the screenbuffer
  CLS();

  PaletteRed();
  colorshift = 240;

  // move within the noise space
  x[0] = beatsin16(1) * 10;
  y[0] += 2000;
  z[0] += 100;
  scale_x[0] = 6000;
  scale_x[0] = 6000;

  // calculate the noise array
  FillNoise(0);

  for (int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {

      // map the noise values down to a byte range
      uint16_t index = noise[0][i][j];
      uint16_t   bri = 255;
      // assign a color depending on the actual palette
      CRGB color = ColorFromPalette( currentPalette, index + colorshift, bri);

      // draw only the part lower than the threshold
      if (index < 128) {
        leds[XY(i, j)] = color;
      }
    }
  }
}


// Lavalamp1
// works good with the RedBlack palette

void Lavalamp1() {

  PaletteRed();
  colorshift = 0;

  x[0] = beatsin16(3, 200, 64000);
  y[0] += 100;
  z[0] = 7000;
  scale_x[0] = 6000;
  scale_y[0] = 8000;
  FillNoise(0);

  x[1] = beatsin16(2, 200, 64000);
  y[1] += 130;
  z[1] = 7000;
  scale_x[1] = 6000;
  scale_y[1] = 8000;
  FillNoise(1);

  x[2] = beatsin16(4, 200, 6400);
  y[2] += 1000;
  z[2] = 3000;
  scale_x[2] = 7000;
  scale_y[2] = 8000;
  FillNoise(2);

  noisesmoothing = 200;

  MergeMethod1(2);
  //Show3Layers();
}


// with a scrolling palette

void Lavalamp2() {

  currentPalette = PartyColors_p;

  noisesmoothing = 200;

  x[0] = beatsin16(3, 200, 64000);
  y[0] = beatsin16(4, 200, 64000);
  z[0] = 7000;
  scale_x[0] = beatsin16(2, 6000, 8000);
  scale_y[0] = beatsin16(1, 4000, 12000);
  FillNoise(0);

  x[1] = beatsin16(5, 200, 64000);
  y[1] = beatsin16(6, 200, 64000);
  z[1] = 6000;
  scale_x[1] = 6000;
  scale_y[1] = 8000;
  FillNoise(1);

  x[2] = beatsin16(4, 200, 6400);
  y[2] += 1000;
  z[2] = 3000;
  scale_x[2] = 7000;
  scale_y[2] = 8000;
  FillNoise(2);

  colorshift++;

  MergeMethod1(2);
  //Show3Layers();
}


// a very slow one

void Lavalamp3() {

  noisesmoothing = 40;
  currentPalette = ForestColors_p;

  y[0] += 100;
  z[0] = 7000;
  scale_x[0] = 6000;
  scale_y[0] = 6000;
  FillNoise(0);

  y[1] += 200;
  z[1] = 3000;
  scale_x[1] = 7000;
  scale_y[1] = 8000;
  FillNoise(1);

  y[2] += 250;
  z[2] = 6000;
  scale_x[2] = 20000;
  scale_y[2] = 8000;
  FillNoise(2);

  MergeMethod1(1);
  //Show3Layers();
}


// the palette can also be defined within the animation

void Lavalamp4() {

  currentPalette = CRGBPalette16(
                     CHSV(   0, 255, 0    ),
                     CHSV(   0, 255, 255  ),
                     CHSV(   0, 255, 0    ),
                     CHSV( 160, 255, 255  ));

  noisesmoothing = 150;

  y[0] += 100;
  z[0] = 7000;
  scale_x[0] = 6000;
  scale_y[0] = 6000;
  FillNoise(0);

  y[1] += 200;
  z[1] = 3000;
  scale_x[1] = 7000;
  scale_y[1] = 8000;
  FillNoise(1);

  y[2] += 250;
  z[2] = 6000;
  scale_x[2] = 20000;
  scale_y[2] = 8000;
  FillNoise(2);

  MergeMethod1(2);
  //Show3Layers();
}


// lets play with the scaling of 2 layers

void Lavalamp5() {

  currentPalette = CRGBPalette16(
                     CHSV(   0, 255, 0   ),
                     CHSV(   0, 200, 255 ),
                     CHSV(  63, 150, 255 ),
                     CHSV( 160, 255, 0   ));

  noisesmoothing = 50;

  y[0] += 1000;
  z[0] = 7000;
  scale_x[0] = beatsin16(3, 1000, 20000);
  scale_y[0] = 6000;
  FillNoise(0);

  y[1] += 2000;
  z[1] = 3000;
  scale_x[1] = beatsin16(4, 1000, 20000);
  scale_y[1] = 8000;
  FillNoise(1);

  y[2] += 3000;
  z[2] = 6000;
  scale_x[2] = beatsin16(5, 1000, 20000);
  scale_y[2] = 8000;
  FillNoise(2);

  MergeMethod2(3);
  //Show3Layers();
}


// 2 layers of constrained noise using differnt palettes for color mapping

void Constrained1() {

  noisesmoothing = 100;
  colorshift = 0;

  x[0] += 2000;
  scale_x[0] = 6000;
  scale_y[0] = 6000;
  FillNoise(0);

  x[1] -= 2000;
  scale_x[1] = 6000;
  scale_y[1] = 6000;
  FillNoise(1);

  CLS();

  // define a palette used for layer 0
  currentPalette = CRGBPalette16(
                     CHSV(   0, 255, 0   ),
                     CHSV(   0, 255, 0   ),
                     CHSV(   0, 255, 255 ),
                     CHSV( 160, 255, 0   ));

  // map layer 0 (red) for noise values between 100 and 200
  ConstrainedMapping( 0, 100, 200, 1);

  // palette for the second layer
  currentPalette = CRGBPalette16(
                     CHSV( 0, 255, 0   ),
                     CHSV( 0, 255, 0 ),
                     CHSV( 160, 255, 255 ),
                     CHSV( 160, 255, 0   ));

  // map layer 1 (blue) for noise values between 100 and 200
  ConstrainedMapping( 1, 100, 200, 1);
}


// move 2 layers relative to each other

void RelativeMotion1() {

  currentPalette = CRGBPalette16(
                     CHSV(  0, 255, 0   ),
                     CHSV( 80, 255, 255 ),
                     CHSV( 60, 255, 255 ),
                     CHSV(  0, 255, 0   ));

  colorshift = beatsin8(10);
  noisesmoothing = 100;

  x[0] = 5 * beatsin16(2, 15000, 40000);
  y[0] = 5 * beatsin16(3, 15000, 40000);
  z[0] += 100;
  scale_x[0] = 6000 + beatsin16(30, 0, 4000);
  scale_y[0] = 8000 + beatsin16(27, 0, 4000);
  FillNoise(0);

  x[1] = x[0] + (5 * beatsin16(30, 0, 10000)) - 25000;
  y[1] = y[0] + (5 * beatsin16(40, 0, 10000)) - 25000;
  z[1] += 100;
  scale_x[1] = 6000 + beatsin16(30, 0, 3000);
  scale_y[1] = 8000 + beatsin16(27, 0, 3000);
  FillNoise(1);

  MergeMethod3(1);
}


// first approach of a water simulation
// uses a beatsin function with phase shift

void Water() {

  currentPalette = OceanColors_p;
  colorshift++;
  noisesmoothing = 200;

  // 2 sinewaves shiftet by 63 (90 degrees)
  // results in a circular motion
  x[0] = 10 * beatsin(10, 0, 10000, 0);
  y[0] = 10 * beatsin(10, 0, 10000, 63);
  z[0] += 1000;
  scale_x[0] = 6000;
  scale_y[0] = 8000;
  FillNoise(0);

  x[1] = x[0] + (10 * beatsin(60, 0, 10000, 0)) - 50000;
  y[1] = y[0] + (10 * beatsin(60, 0, 10000, 63)) - 50000;
  z[1] += 1000;
  scale_x[1] = 6000 ;
  scale_y[1] = 8000;
  FillNoise(1);

  MergeMethod3(3);
}


// outlined bubbles by constrained mapping + palette

void Bubbles1() {

  noisesmoothing = 200;
  PaletteRed();
  colorshift = 0;

  x[0] = beatsin16(7);
  y[0] += 2000;
  z[0] = 7000;
  scale_x[0] = 6000;
  scale_y[0] = 6000;
  FillNoise(0);

  x[1] = beatsin16(8);
  y[1] += 3000;
  z[1] = 10000;
  scale_x[1] = 6000;
  scale_y[1] = 6000;
  FillNoise(1);

  CLS();

  ConstrainedMapping(1, 0, 100, 3);
  ConstrainedMapping(0, 0, 100, 3);
}


// layer2 movving arround a layer1 moving arround a layer0

void TripleMotion() {

  currentPalette = RainbowColors_p;
  colorshift++;
  noisesmoothing = 200;

  x[0] = 10 * beatsin(10, 0, 10000, 0);
  y[0] = 10 * beatsin(9, 0, 10000, 63);
  z[0] += 1000;
  scale_x[0] = 6000;
  scale_y[0] = 8000;
  FillNoise(0);

  x[1] = x[0] + (10 * beatsin(13, 0, 10000, 0));
  y[1] = y[0] + (10 * beatsin(12, 0, 10000, 63));
  z[1] += 1000;
  scale_x[1] = 6000;
  scale_y[1] = 8000;
  FillNoise(1);

  x[2] = x[1] + (10 * beatsin(18, 0, 10000, 0));
  y[2] = y[1] + (10 * beatsin(17, 0, 10000, 63));
  z[2] += 1000;
  scale_x[2] = 6000;
  scale_y[2] = 8000;
  FillNoise(2);

  MergeMethod4(2);
  //Show3Layers();
}


void CrossNoise() {

  currentPalette = RainbowStripeColors_p;
  colorshift = 50;
  noisesmoothing = 20;

  //x[0] = 10 * beatsin(10, 0, 10000, 0);
  y[0] += 100;
  z[0] += 50;
  scale_x[0] = 4000;
  scale_y[0] = 4000;
  FillNoise(0);
  byte border = beatsin8(10, 20, 236);

  CrossMapping(1, border);
}


void CrossNoise2() {

  currentPalette = RainbowStripeColors_p;
  noisesmoothing = 20;

  y[0] += 100;
  z[0] += 50;
  scale_x[0] = beatsin16(3, 1000, 10000);
  scale_y[0] = beatsin16(2, 1000, 10000);
  FillNoise(0);

  byte border = beatsin8(8);

  CrossMapping(1, border);
}


void RandomAnimation() {

  noisesmoothing = 100;

  // danger: dirty + crappy code!
  // use EVERY_N_MILLIS instead!
  byte ping = beatsin8(10);
  if (ping < 1) {
    SetupRandomPalette();
    dy = random(2000) - 1000;
    dx = random(2000) - 1000;
    dz = random(2000) - 1000;
    scale_x[0] = random(8000) + 2000;
    scale_y[0] = random(8000) + 2000;
  }

  y[0] += dy;
  x[0] += dx;
  z[0] += dz;
  FillNoise(0);


  ShowLayer(0, 2);
}
// All the caleidoscope functions work directly within the screenbuffer (leds array).
// Draw whatever you like in the area x(0-15) and y (0-15) and then copy it arround.

// rotates the first 16x16 quadrant 3 times onto a 32x32 (+90 degrees rotation for each one)

void Caleidoscope1() {
  for (int x = 0; x < kMatrixWidth / 2 ; x++) {
    for (int y = 0; y < kMatrixHeight / 2; y++) {
      leds[XY( kMatrixWidth - 1 - x, y )] = leds[XY( y, x )];
      leds[XY( kMatrixWidth - 1 - x, kMatrixHeight - 1 - y )] = leds[XY( x, y )];
      leds[XY( x, kMatrixHeight - 1 - y )] = leds[XY( y, x )];
    }
  }
}

// mirror the first 16x16 quadrant 3 times onto a 32x32

void Caleidoscope2() {
  for (int x = 0; x < kMatrixWidth / 2 ; x++) {
    for (int y = 0; y < kMatrixHeight / 2; y++) {
      leds[XY( kMatrixWidth - 1 - x, y )] = leds[XY( x, y )];
      leds[XY( x, kMatrixHeight - 1 - y )] = leds[XY( x, y )];
      leds[XY( kMatrixWidth - 1 - x, kMatrixHeight - 1 - y )] = leds[XY( x, y )];
    }
  }
}


// copy one diagonal triangle into the other one within a 16x16

void Caleidoscope3() {
  for (int x = 0; x <= CentreX ; x++) {
    for (int y = 0; y <= x; y++) {
      leds[XY( x, y )] = leds[XY( y, x )];
    }
  }
}


// copy one diagonal triangle into the other one within a 16x16 (90 degrees rotated compared to Caleidoscope3)

void Caleidoscope4() {
  for (int x = 0; x <= CentreX ; x++) {
    for (int y = 0; y <= CentreY - x; y++) {
      leds[XY( CentreY - y, CentreX - x )] = leds[XY( x, y )];
    }
  }
}


// copy one diagonal triangle into the other one within a 8x8

void Caleidoscope5() {
  for (int x = 0; x < kMatrixWidth / 4 ; x++) {
    for (int y = 0; y <= x; y++) {
      leds[XY( x, y )] = leds[XY( y, x )];
    }
  }

  for (int x = kMatrixWidth / 4; x < kMatrixWidth / 2 ; x++) {
    for (int y = kMatrixHeight / 4; y >= 0; y--) {
      leds[XY( x, y )] = leds[XY( y, x )];
    }
  }
}






void Caleido1() {

  EVERY_N_SECONDS(4) {

    SetupRandomPalette2();
    dy = random16(4000) - 2000;
    dx = random16(4000) - 2000;
    dz = random16(4000) - 2000;
    scale_x[0] = random16(10000) + 2000;
    scale_y[0] = random16(10000) + 2000;
  }

  y[0] += dy;
  x[0] += dx;
  z[0] += dz;
  FillNoise(0);

  ShowLayer(0, 1);
  Caleidoscope2();
}


void Caleido2() {

  EVERY_N_SECONDS(4) {

    SetupRandomPalette3();
    dy = random16(4000) - 2000;
    dx = random16(4000) - 2000;
    dz = random16(4000) - 2000;
    scale_x[0] = random16(10000) + 2000;
    scale_y[0] = random16(10000) + 2000;
  }

  y[0] += dy;
  x[0] += dx;
  z[0] += dz;
  FillNoise(0);

  ShowLayer(0, 1);
  Caleidoscope1();
}


// the "Electric Mandala"

void Caleido3() {

  // a new parameter set every 15 seconds
  EVERY_N_SECONDS(15) {

    SetupRandomPalette3();
    dy = random16(500) - 250; // random16(2000) - 1000 is pretty fast but works fine, too
    dx = random16(500) - 250;
    dz = random16(500) - 250;
    scale_x[0] = random16(10000) + 2000;
    scale_y[0] = random16(10000) + 2000;
  }

  y[0] += dy;
  x[0] += dx;
  z[0] += dz;

  FillNoise(0);
  ShowLayer(0, 1);

  Caleidoscope3();
  Caleidoscope1();
}


void Caleido4() {

  EVERY_N_SECONDS(45) {

    SetupRandomPalette3();
    /*
      dy = random16(2000) - 1000; // random16(2000) - 1000 is pretty fast but works fine, too
      dx = random16(2000) - 1000;
      dz = random16(2000) - 1000;
    */

    dy = random16(500) - 250;
    dx = random16(500) - 250;
    dz = random16(500) - 250;

    scale_x[0] = random16(10000) + 2000;
    scale_y[0] = random16(10000) + 2000;
  }

  y[0] += dy;
  x[0] += dx;
  z[0] += dz;

  FillNoise(0);
  ShowLayer(0, 1);

  Caleidoscope4();
  Caleidoscope2();
}

void Caleido5() {

  // a new parameter set every 10 seconds
  EVERY_N_SECONDS(10) {

    SetupRandomPalette4();

    dy = random16(1000) - 500; // random16(2000) - 1000 is pretty fast but works fine, too
    dx = random16(1000) - 500;
    dz = random16(500);


    scale_x[0] = random16(7000) + 2000;
    scale_y[0] = random16(7000) + 2000;
  }

  y[0] += dy;
  x[0] += dx;
  z[0] += dz;

  EVERY_N_MILLIS(50) {
    colorshift++;
  }

  FillNoise(0);
  ShowLayer(0, 1);

  Caleidoscope5();
  Caleidoscope4();
  Caleidoscope2();
}


void Caleido6() {

  // a new parameter set every 10 seconds
  EVERY_N_SECONDS(10) {

    SetupRandomPalette4();

    dy = random16(1000) - 500; // random16(2000) - 1000 is pretty fast but works fine, too
    dx = random16(1000) - 500;
    dz = random16(500);


    scale_x[0] = random16(7000) + 2000;
    scale_y[0] = random16(7000) + 2000;
  }

  y[0] += dy;
  x[0] += dx;
  z[0] += dz;

  EVERY_N_MILLIS(50) {
    colorshift++;
  }

  FillNoise(0);
  ShowLayerBright(0, 1);

  //Caleidoscope5();
  Caleidoscope4();
  Caleidoscope1();
}


void Caleido7() {

  EVERY_N_SECONDS(10) {

    SetupRandomPalette4();

    dy = random16(1000) - 500;
    dx = random16(1000) - 500;
    dz = random16(500);


    scale_x[0] = random16(7000) + 2000;
    scale_y[0] = random16(7000) + 2000;
  }

  y[0] += dy;
  x[0] += dx;
  z[0] += dz;

  EVERY_N_MILLIS(50) {
    colorshift++;
  }

  FillNoise(0);
  ShowLayerBright(0, 1);

  Caleidoscope4();
  Caleidoscope1();
  FilterAll();
}


