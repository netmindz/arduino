#pragma message Teensy 3.2 + MSGEQ7

#include <TeensyDmx.h>

#define NUM_GEQ_CHANNELS 16

#define MSGEQ7_OUT_MAX 255
#define MSGEQ7_BASS 0
#define MSGEQ7_LOW 0
#define MSGEQ7_MID (NUM_GEQ_CHANNELS / 2)
#define MSGEQ7_HIGH NUM_GEQ_CHANNELS


uint8_t fftResult[NUM_GEQ_CHANNELS];

TeensyDmx Dmx(Serial1);

void controlSetup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Dmx.setMode(TeensyDmx::DMX_IN);
  pgm = 0;
  
  delay(1000);
  Serial.println("Startup");
}

int getValue(int chan, int minV, int maxV) {
  return map(Dmx.getBuffer()[(chan - 1)], 0, 255, minV, maxV);
}

// storage of the 7 10Bit (0-1023) audio band values
int left[7];
int right[7];

bool newReading;
bool MSGEQ7read() {
  return newReading;
}

int MSGEQ7get(int band) {
  //  return left[band];
  return fftResult[map(band, 0, 6, 0, (NUM_GEQ_CHANNELS - 1))];
}

int MSGEQ7get(int band, int channel) {
  return MSGEQ7get(band);
}

int led = 0;

// storage of the 7 10Bit (0-1023) audio band values
int left[7];
int right[7];

void controlLoop() {
  Dmx.loop();
  if (Dmx.newFrame()) {
    EVERY_N_SECONDS( 2 ) {
      Serial.printf("Brighness: %u\n", getValue(1, 0, 255)); // Dimmer data for Channel 1
    }
    led = !led;
    digitalWrite(LED_BUILTIN, led);
    int b =  getValue(1); // brightness = 1
    if (b != BRIGHTNESS) {
      BRIGHTNESS = b;
      FastLED.setBrightness(BRIGHTNESS);
    }
    pgm = getValue(2, 0, (gPatternCount - 1)); // pattern = 2
    SPEED = getValue(3); // speed = 3
    FADE = getValue(4);  // fade = 4
    currentPalette = getPalette(getValue(5)); // palette = 5

    // Serial.println(getValue(400, 0, 255));
    if(getValue(400, 0, 255) > 0) {
      newReading = true;
      // Serial.println("T");
    }
    else {
      newReading = false;
      Serial.println("F");
    }
    for(int b = 0; b < NUM_GEQ_CHANNELS; b++) {
      fftResult[b] = getValue((401 + b), 0, 255);
      // Serial.printf("%03d ", fftResult[b]);
    }
    // Serial.println(" ");

    // "Legacy" - for MSGEQ7 patterns
    for (int b = 0; b < 7; b++) {
      left[b] = map(fftResult[(b * 2)], 0, 255, 0, 1023);
      right[b] = left[b];
    }
  }
}
