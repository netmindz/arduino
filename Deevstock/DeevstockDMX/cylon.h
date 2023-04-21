void fadeall() {
  for (int i = 0; i < NUM_AUDIO_LEDS; i++) {
    ledsAudio[i].nscale8(FADE);
  }
}
int cylonDir = 1;
void cylon() {
  static uint8_t hue = 0;
  static uint8_t i = 0;

  // First slide the led in one direction
  if(i > NUM_AUDIO_LEDS) {
    if(cylonDir == 1) {
      cylonDir = -1;
    }
    else {
      cylonDir = 1;
    }
  }
  i += cylonDir;
  
    // Set the i'th led to red
    hue += STEPS;
    ledsAudio[i] = ColorFromPalette(currentPalette, hue, 255, currentBlending);
    // Show the leds
    showSegments();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    FastLED.delay(map(SPEEDO, 0, 255, 100, 0));
  
  // Now go in the other direction.  
	for(int i = (NUM_AUDIO_LEDS)-1; i >= 0; i--) {
		// Set the i'th led to red 
    ledsAudio[i] = ColorFromPalette(currentPalette, hue, 255, currentBlending);
		// Show the leds
    showSegments();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
    FastLED.delay(map(SPEEDO, 0, 255, 100, 0));
	}

}


