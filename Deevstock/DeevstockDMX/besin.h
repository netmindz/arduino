#ifndef besin_H
#define besin_H

void besin() {                                                             // Add a Perlin noise soundbar. This looks really cool.

// Local definitions


// Persistent local variables


  timeval = 30;                                                                 // Our EVERY_N_MILLIS_I timer value.


// This works.
  ledsAudio[NUM_AUDIO_LEDS/2] = ColorFromPalette(currentPalette, millis(), sampleavg, NOBLEND);
  ledsAudio[NUM_AUDIO_LEDS/2-1] = ColorFromPalette(currentPalette, millis(), sampleavg, NOBLEND);


//  ledsAudio[NUM_AUDIO_LEDS/2] = ColorFromPalette(currentPalette, sampleavg, beatsin8(sampleavg,0,255), NOBLEND);
//  ledsAudio[NUM_AUDIO_LEDS/2-1] = ColorFromPalette(currentPalette, sampleavg, beatsin8(sampleavg,0,255), NOBLEND);


  waveit();                                                                     // Move the pixels to the left/right, but not too fast.

//  fadeToBlackBy(ledsAudio+NUM_AUDIO_LEDS/2-1, 2, 128);                                     // Fade the center, while waveit moves everything out to the edges.
  fadeToBlackBy(ledsAudio, NUM_AUDIO_LEDS, 2);                                                                                 

  showSegments();
} // besin()

#endif
