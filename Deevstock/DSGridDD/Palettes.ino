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






