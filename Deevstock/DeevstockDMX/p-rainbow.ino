// int STEPS = 4;           //Wider or narrower bands of color
// int SPEEDO = 10;         //The speed of the animation

// RAINBOW --------------------------------------------------
void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, (BRIGHTNESS / 4), currentBlending);
    colorIndex += STEPS;
  }
}

void Rainbow()
{ 
  currentPalette = RainbowColors_p;
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; 

  FillLEDsFromPaletteColors( startIndex);
    
  FastLED.delay(map(SPEEDO,0,255, 200, 1));  // TWEAK ME (was 10)
}


