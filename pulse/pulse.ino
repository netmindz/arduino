#include <FastLED.h>
#define LED_PIN     15
#define NUM_LEDS    32
#define BRIGHTNESS  10
#define LED_TYPE    NEOPIXEL
CRGB leds[NUM_LEDS];
class Pixel {
    int p = 0;
    int v = 0;
    int h = 0;
    bool locked = false;

  public:
    void set(int i) {
      p = i;
    }

    boolean isLocked() {
      return locked;
    }

    void hue(int hue) {
      if (locked) return;
      h = hue;
    }

    void fade(int value) {
      if (locked) return;
      v = v + value;
    }

    void setV(int value) {
      if (locked) return;
      v = value;
    }

    void render() {
      leds[p] = CHSV(h, 255, v);
    }

    void lock() {
      hue(random(0, 255));
      locked = true;
    }

    void rst() {
      v = 0;
      locked = false;
      h = 0;
    }
};
Pixel pixels[NUM_LEDS];

void setup() {
  FastLED.setBrightness( 55 );
  FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels[i].set(i);
  }
  Serial.begin(115200);
}

void loop() {
  pulse();
}

int frame = 0;
int v = 0;
void pulse() {
  frame++;
  if (frame < 50) {
    // fadeup
    fade(1);
  }
  else if (frame < 100) {
    // fadedown
    fade(-1);
  }
  else if (frame < 150) {
    // fadeup
    fade(1);
  }
  else {

    frame = 0;

    int sanity = 0;
    while (sanity < 10000) {
      sanity++;
      int p = random(0, NUM_LEDS);
      if (pixels[p].isLocked()) {
        continue;
      }
      else {
        pixels[p].lock();
        break;
      }
    }

    // Are we all locked
    boolean allLocked = true;
    for (int i = 0; i < NUM_LEDS; i++) {
      if (!pixels[i].isLocked()) {
        allLocked = false;
        break;
      }
    }
    Serial.println("End of loop");
    if (allLocked) {
      Serial.println("Reset");
      for (int i = 0; i < NUM_LEDS; i++) {
        pixels[i].rst();
      }
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels[i].setV(0);
      pixels[i].hue(random(0, 255));
    }

  }

  for (int i = 0; i < NUM_LEDS; i++) {
    pixels[i].render();
  }

  FastLED.delay(10);
}

void fade(int v) {
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels[i].fade(v);
  }
}



