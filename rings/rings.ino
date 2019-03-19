#include <FastLED.h>

// Basic test of Rings

#define LED_PIN  D4

#define COLOR_ORDER GBR
#define CHIPSET     WS2811

#define RINGS 9

#define JUMP 20
#define SPEED 80

#define NUM_LEDS 241

CRGB leds[NUM_LEDS];      //naming our LED array
int hue[RINGS];

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( 15 );
  int h = 0;
  for(int r=0; r < RINGS; r++) {
    hue[r] = h;
    h += JUMP;
  }
}

void loop() {
  rings();
}

void rings() {
  for(int r=1; r<= RINGS; r++) {
    setRing(r, CHSV(hue[(r - 1)], 255,255));
  }
  FastLED.delay(SPEED);
  if(false) {
    for(int r=2; r < RINGS; r++) {
      hue[(r - 1)] = hue[r];
    }
    hue[(RINGS - 1)] += JUMP;
  }
  else {
    for(int r=(RINGS - 1); r > 1; r--) {
      hue[r] = hue[(r - 1)];
    }
//    if(random(0,10) > 2) {
      hue[1] += JUMP;
//    }
//    else {
//      hue[1] += random(0,255);
//    }
  }
}

void setRing(int ring, CRGB colour) {
  int offset = 0;
  int count = 0;
  switch (ring) {
    case 1:
      count = 1;
      break;
    case 2:
      offset = 1;
      count = 8;
      break;
    case 3:
      offset = 9;
      count = 12;
      break;
    case 4:
      offset = 21;
      count = 16;
      break;
    case 5:
      offset = 37;
      count = 24;
      break;
    case 6:
      offset = 61;
      count = 32;
      break;
    case 7:
      offset = 93;
      count = 40;
      break;
    case 8:
      offset =133;
      count = 48;
      break;
    case 9:
      offset = 181;
      count = 60;
      break;
  }
  for (int i = 0; i < count; i++) {
    leds[(offset + i)] = colour;
  }
}

