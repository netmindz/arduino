#ifndef RAINBOWBIT_H
#define RAINBOWBIT_H

void rainbowbit() {

  timeval = 10;                                                                 // Our EVERY_N_MILLIS_I timer value.
  
  uint8_t beatA = beatsin8(17, 0, 255);                                         // Starting hue.
  
  if (samplepeak == 1) {                                                        // Trigger a rainbow with a peak.
    fill_rainbow(ledsAudio + random8(0,NUM_AUDIO_LEDS/2), random8(0,NUM_AUDIO_LEDS/2), beatA, 8);  // Use FastLED's fill_rainbow routine.
  }
  
  fadeToBlackBy(ledsAudio, NUM_AUDIO_LEDS, 40);                                            // Fade everything. By Andrew Tuline.

  addGlitter(sampleavg);                                                        // Add glitter based on sampleavg.
  showSegments();
} // rainbowbit()

#endif
