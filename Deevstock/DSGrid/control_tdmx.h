#pragma message Teensy 3.2 + MSGEQ7
#include <TeensyDmx.h>
//#define MSGEQ7_10BIT
// MSGEQ7
#include "MSGEQ7.h"

// MSGEQ7 wiring on spectrum analyser shield
#define MSGEQ7_RESET_PIN  5
#define MSGEQ7_STROBE_PIN 6
#define AUDIO_LEFT_PIN    A2
#define AUDIO_RIGHT_PIN   A1
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH true

CMSGEQ7<true, MSGEQ7_RESET_PIN, MSGEQ7_STROBE_PIN, AUDIO_LEFT_PIN, AUDIO_RIGHT_PIN> MSGEQ7;

TeensyDmx Dmx(Serial1);

void InitMSGEQ7() { 
  MSGEQ7.begin();
}

void controlSetup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Dmx.setMode(TeensyDmx::DMX_IN);
  pgm = 0;

  InitMSGEQ7();
  
  delay(1000);
  Serial.println("Startup");
}

int getValue(int chan, int minV, int maxV) {
  return map(Dmx.getBuffer()[(chan - 1)], 0, 255, minV, maxV);
}


boolean MSGEQ7read() {
  return MSGEQ7.read(MSGEQ7_INTERVAL);
}

int MSGEQ7get(int band) {
  return mapNoise(MSGEQ7.get(band));
}

int MSGEQ7get(int band, int channel) {
  return mapNoise(MSGEQ7.get(band, channel));
}

int led = 0;

void controlLoop() {
  int gPatternCount = 32; // FIXME
  Dmx.loop();
  if (Dmx.newFrame()) {
    EVERY_N_SECONDS( 2 ) {
      Serial.printf("Brighness: %u\n", getValue(1, 0, 255)); // Dimmer data for Channel 1
    }
    led = !led;
    digitalWrite(LED_BUILTIN, led);
    int b =  getValue(1, 0, 255); // brightness = 1
    if (b != BRIGHTNESS) {
      BRIGHTNESS = b;
      FastLED.setBrightness(BRIGHTNESS);
    }
    pgm = getValue(2, 0, (gPatternCount - 1)); // FIXME // pattern = 2
    SPEED = getValue(3, 0, 255); // speed = 3
    FADE = getValue(4, 0, 255);  // fade = 4

  }
}
