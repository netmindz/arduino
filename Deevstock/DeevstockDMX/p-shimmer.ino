boolean xrand = false;
boolean irand = true;

unsigned int l = 0;
int i = 1;
int x = 1;

void shimmer() {
  for (int dot = -1; dot < NUM_LEDS; dot += i) {
    leds[dot].setHue(x);
    // leds[dot] = CRGB::Blue;
    FastLED.delay(map(SPEEDO,0,255, 200, 5));  // TWEAK ME was 10
    // clear this led for the next time around the loop
    // leds[dot] = CRGB::Black;
  }

  if (irand) {
    i = random(2, 40);
  }
  else {
    if (i < 40) {
      i++;
    } else {
      i = 1;
    }
  }

  if (xrand) {
    x = random(1, 255);
  }
  else {
    if (x < 255) {
      x++;
    } else {
      x = 1;
    }

  }
}


