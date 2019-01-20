// SIN WAVE SETUP

// SINWAVE VARIABLES -- change these numbers to modify Sinwave mode
uint8_t wavebright = 255;                                     // You can change the brightness of the waves/bars rolling across the screen.
uint8_t thishue = 30;                                          // You can change the starting hue value for the first wave.
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. 0= color stays the same
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
bool thisdirSine = 0;                                             // You can change direction.
int8_t thisspeed = 4;                                         // You can change the speed, and use negative values.
uint8_t allfreq = 25;                                         // You can change the frequency, thus overall width of bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
int loopdelay = 4;                                           // You can change the delay. Also you can change the allspeed variable above.
uint8_t bgclr = 0;                                            // A rotating background colour.
uint8_t bgbright = 55;                                        // Background color brightness


void one_sin() {                                                                // This is the heart of this program. Sure is short.
  if (thisdirSine == 0) thisphase += thisspeed; else thisphase -= thisspeed;        // You can change direction and speed individually.
  thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  for (int k = 0; k < NUM_LEDS; k++) {
    int thisbright = qsubd(cubicwave8((k * allfreq) + thisphase), thiscutoff);  // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, bgbright);
    leds[k] += CHSV(thishue, allsat, thisbright);                               // Assigning hues and brightness to the led array.
  }
  bgclr++;
} 

void sinwave_1() {
  one_sin();
  FastLED.delay(map(SPEEDO,0,255, 50, 0));  // TWEAK ME
}

