/*
A bunch of animations demonstrating how to creatively combine
 simple functions for nice effects.
 
 MirroredNoise         a symmetric noise pattern
 RedClouds             upstreaming clouds
 Lavalamp1 - Lavlamp 5
 Constrained1
 RelativeMotion1
 Water
 Bubbles1
 ShowPalette
 
 (list is uncomplete)
 -----------------------------------------------------------------
 */


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

  for(int i = 0; i < kMatrixWidth; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {

      // map the noise values down
      uint16_t index = ( noise[0][i][j] + noise[0][kMatrixWidth - 1 - i][j] ) / 2;
      uint16_t   bri = 255;
      // assign a color from the HSV space
      CRGB color = ColorFromPalette( currentPalette, index, bri);

      leds[XY(i,j)] = color;
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
  x[0] = beatsin16(1)*10;
  y[0] += 2000;
  z[0] += 100;
  scale_x[0] = 6000;
  scale_x[0] = 6000;

  // calculate the noise array
  FillNoise(0);

  for(int i = 0; i < kMatrixWidth; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {

      // map the noise values down to a byte range
      uint16_t index = noise[0][i][j];
      uint16_t   bri = 255;
      // assign a color depending on the actual palette
      CRGB color = ColorFromPalette( currentPalette, index + colorshift, bri);

      // draw only the part lower than the threshold
      if (index < 128) { 
        leds[XY(i,j)] = color;
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
  
  FastLED.delay(30);
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


// just shows the color gradient

void ShowRedPalette() {
  PaletteRed();
  ShowPalette();
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
  scale_x[0] = beatsin16(3,1000,10000);
  scale_y[0] = beatsin16(2,1000,10000);
  FillNoise(0);

  byte border = beatsin8(8);

  CrossMapping(1, border);
}


void RandomAnimation() {

  noisesmoothing = 100;

  // danger: dirty + crappy code!
  // use EVERY_N_MILLIS instead!
  byte ping=beatsin8(10); 
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



void MilliTimer() {

  currentPalette = RainbowStripeColors_p;

  //EVERY_N_SECONDS(10) {colorshift += random8();}

  //colorshift= beatsin88(12, 0,255, 0,0);

  y[0] = millis()*spd;
  x[0] = 2*beatsin88(spd*256);
  scale_x[0] = 4000;
  scale_y[0] = 4000;
  y[1] = millis()*spd*2;
  x[1] = 2*beatsin88(spd*256);
  scale_x[1] = 4000;
  scale_y[1] = 4000;
  FillNoise(0);
  FillNoise(1);
  MergeMethod3(1);
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

