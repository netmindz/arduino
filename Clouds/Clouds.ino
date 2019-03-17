#include <FastLED.h>

// params - saturation, speed,

#define LED_PIN  D4

#define COLOR_ORDER RGB
#define CHIPSET     WS2811

#define NUM_LEDS 200

CRGB leds[NUM_LEDS];      //naming our LED array

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(255);
}

void loop() {
  addGlitter();
  fadeToBlackBy(leds, NUM_LEDS, 70);
  FastLED.delay(40);

}

void addGlitter() {
  // clusters of leds, souce of
  if (random(0, 20) == 0) {
    for (int i = 0; i < random(1, NUM_LEDS); i++) {
      int r = random16(NUM_LEDS);
      for (int j = 0; j < random(1, 15); j++) {
        if ((r + j) <  NUM_LEDS) {
          leds[(r + j)] = CHSV(random(0, 255), 100, 255);
        }
      }
    }
  }
}


