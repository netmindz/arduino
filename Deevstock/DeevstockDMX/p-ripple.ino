int HUE = 0;             //0-255, around the color wheel
// int STEPS = 4;           //Wider or narrower bands of color
// int SPEEDO = 10;         //The speed of the animation

//RIPPLE VARIABLES
uint8_t colour;                                               // Ripple colour is randomized.
int center = 0;                                               // Center of the current ripple.
int step = -1;                                                // -1 is the initializing step.
uint8_t myfade = 255;                                         // Starting brightness.
#define maxsteps 16                                           // Case statement wouldn't allow a variable.

//RIPPLE --------------------------------------------------------------------------------
int wrap(int step) {
  if (step < 0) return NUM_LEDS + step;
  if (step > NUM_LEDS - 1) return step - NUM_LEDS;
  return step;
}

void Ripple() {
  HUE = 140; HUE++;
  if (HUE > 220) {
    HUE = 140; // constrain BG hue to blues and purples
  }
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(HUE++, 255, map(FADE,0,255, 50, 150));  // Rotate background colour.

  switch (step) {

    case -1:                                                          // Initialize ripple variables.
      center = random(NUM_LEDS);
      colour = random16(0, 256);
      step = 0;
      break;

    case 0:
      leds[center] = CHSV(colour, 255, 255);                          // Display the first pixel of the ripple.
      step ++;
      break;

    case maxsteps:                                                    // At the end of the ripples.
      step = -1;
      break;

    default:                                                          // Middle of the ripples.
      leds[wrap(center + step)] += CHSV(colour, 255, myfade / step * 2); // Display the next pixels in the range for one side.
      leds[wrap(center - step)] += CHSV(colour, 255, myfade / step * 2); // Display the next pixels in the range for the other side.
      step ++;                                                      // Next step.
      break;
  } // switch step

  FastLED.delay(map(SPEEDO,0,255, 100, 20));  // TWEAK ME
}





