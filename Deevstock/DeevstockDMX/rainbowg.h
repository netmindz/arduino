#ifndef RAINBOWG_H
#define RAINBOWG_H


void rainbowg() {                                                               // Make a complex rainbow with glitter based on sampleavg.
  
  uint8_t beatA = beatsin8(17, 0, 255);                                         // Starting hue.
  uint8_t beatB = beatsin8(13, 0, 255);
  uint8_t beatC = beatsin8(11, 0, 255);

//  currentPalette = PartyColors_p;
  
  for (int i=0; i < NUM_AUDIO_LEDS; i++) {
    int colorIndex = (beatA+beatB+beatC)/3 * i * 4 / NUM_AUDIO_LEDS;
    ledsAudio[i] = ColorFromPalette( currentPalette, colorIndex, sampleavg, currentBlending); // Variable brightness 
  }

  addGlitter(sampleavg);                                                        // Add glitter baesd on sampleavg. By Andrew Tuline.
  showSegments();
} // rainbowg()

#endif

