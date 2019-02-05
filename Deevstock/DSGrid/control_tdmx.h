#include <TeensyDmx.h>

#define LED_PIN 7
#define CLOCK_PIN 14

// MSGEQ7 wiring on spectrum analyser shield
#define MSGEQ7_STROBE_PIN 5
#define MSGEQ7_RESET_PIN  6
#define AUDIO_LEFT_PIN    A2
#define AUDIO_RIGHT_PIN   A1


TeensyDmx Dmx(Serial2);

void controlSetup() {
  Dmx.setMode(TeensyDmx::DMX_IN);
   pgm = 0;
}

void controlLoop() {
  int gPatternCount = 5; // FIXME
  Dmx.loop();
  if (Dmx.newFrame()) {
    Serial.println("d");
    int b = Dmx.getBuffer()[1]; // brightness = 1
    if (b != BRIGHTNESS) {
      BRIGHTNESS = b;
      FastLED.setBrightness(BRIGHTNESS);
    }
    int p = Dmx.getBuffer()[2]; // pattern = 5
    pgm = map(p, 0, 255, 0, (gPatternCount - 1)); // FIXME
    if (p > (gPatternCount - 1)) {
      p = 0;
    }
    else {
      pgm = p;
    }
  }

}

