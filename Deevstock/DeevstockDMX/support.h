#ifndef SUPPORT_H
#define SUPPORT_H

// Supporting general function -------------------------------------------------------------------------------------------

void showfps() {                                                                // Show rames per seocond on the serial monitor.

  // Temporary local variable
  uint32_t currentMillis;

  // Persistent local variable
  static uint32_t lastMillis = 0;

  currentMillis = millis();
  loops++;                                                                      // Keep on incrementing this each time it's called until (currentMillis - lastMillis) > 1000

  if (currentMillis - lastMillis > 1000) {                                      // If 1 second has passed, then. .
    Serial.println(loops);                                                      // Print the value of loops (which is loops per second).
    lastMillis = currentMillis;                                                 // Reset everything and start counting all over again. By Andrew Tuline.
    loops = 0;
  }

} // showfps()



// Supporting visual functions ----------------------------------------------------------------------------------------------

void lineit() {                                                                 // Send the pixels one or the other direction down the line.

  if (thisdir == 0) {
    for (int i = NUM_AUDIO_LEDS - 1; i > 0 ; i-- ) ledsAudio[i] = ledsAudio[i - 1];
  } else {
    for (int i = 0; i < NUM_AUDIO_LEDS - 1 ; i++ ) ledsAudio[i] = ledsAudio[i + 1];
  }

} // waveit()



void waveit() {                                                                 // Shifting pixels from the center to the left and right.

  for (int i = NUM_AUDIO_LEDS - 1; i > NUM_AUDIO_LEDS / 2; i--) {                           // Move to the right.
    ledsAudio[i] = ledsAudio[i - 1];
  }

  for (int i = 0; i < NUM_AUDIO_LEDS / 2; i++) {                                      // Move to the left.
    ledsAudio[i] = ledsAudio[i + 1];
  }

} // waveit()



void addGlitter( fract8 chanceOfGlitter) {                                      // Let's add some glitter, thanks to Mark

  if ( random8() < chanceOfGlitter) {
    ledsAudio[random16(NUM_AUDIO_LEDS)] += CRGB::White;
  }

} // addGlitter()

#endif

uint16_t getPixel(int p)
{
  uint16_t i;

  if ( kMatrixSerpentineLayout == false) {
    i = p;
  }

  if ( kMatrixSerpentineLayout == true) {
    float f = ((float)(p + 1) / (float) NUM_LEDS_PER_STRIP);
    int r = ceil(f);

    if (r & 0x01)   {
      i = p;
    } else {
      i = ((NUM_LEDS_PER_STRIP * r) - (p % NUM_LEDS_PER_STRIP)) - 1;
    }
  }

  // if (i >= (NUM_LEDS - 1)) return 0;
  return i;
}

void showSegments() {
  for (int s = 0; s < NUM_STRIPS; s++) {
    for (int i = 0; i < NUM_AUDIO_LEDS; i++) {
      int p = (s * NUM_AUDIO_LEDS) + i;
      leds[getPixel(p)] = blend(ledsAudio[i], CRGB(RED, GREEN, BLUE), 125);
    }
  }
  FastLED.show();
}



