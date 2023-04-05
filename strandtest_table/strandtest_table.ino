#include <FastLED.h>

#define LED_PIN 2
#define CLOCK_PIN 6

#define COLOR_ORDER GBR
#define CHIPSET     DOTSTAR

#define BRIGHTNESS 100

#define NUM_LEDS 150

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  LEDS.setBrightness(150);
  FastLED.addLeds<DOTSTAR, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
}

void loop() {
  // Some example procedures showing how to display to the pixels:
/*    colorWipe(strip.Color(255, 0, 0), 170); // Red
  theaterChase(strip.Color(  0,   0, 127), 170); // Blue
  colorWipe(strip.Color(0, 255, 0), 170); // Green
  theaterChase(strip.Color(127,   0,   0), 170); // Red
  colorWipe(strip.Color(0, 0, 255), 70); // BlueC
  theaterChase(strip.Color(127, 127, 127), 170); // White
  // Send a theater pixel chase in...
//  brightness();
/*
  int button = digitalRead(2);
  if(button == HIGH) {
      rainbowCycle(20);
  }
  else { 
      rainbow(20,3);
  }
*/
//    rainbowCycle(20);

/*
      rainbow(50,3);
      rainbowCycle(50);
      rainbow(50,5);
      rainbowCycle(150);
      rainbow(20,1);
      */

//    theaterChaseRainbowOld(50, 5);
  //    rainbowCycle(50);
    for(int x=1; x <= 20; x++) {
      theaterChaseRainbowOld(70, x);
      Serial.print("\ntheaterChaseRainbow=");
      Serial.print(x);
      Serial.print("\n");
    }
    for(int x=1; x <= 20; x++) {
      theaterChaseRainbow(70, x);
      Serial.print("\ntheaterChaseRainbow=");
      Serial.print(x);
      Serial.print("\n");
    }

/*    rainbow(30,18);
    rainbow(40,9);
    rainbow(50,3); */
//    test();
//  theaterChase(strip.Color(255,   125,   0), 2); 
}

void fadeRange(int current, int total) {
//  Serial.print(" current=");
//  Serial.print(current);
  
  int brightness = BRIGHTNESS;
  
 LEDS.setBrightness(brightness);

  Serial.print(" current=");
  Serial.print(current);
  
  int dec = brightness / current;
   
  Serial.print(" dec=");
  Serial.print(dec);
  
  fade(dec);
} 

void fadeRangeExp(float current, float total) {
//  Serial.print(" current=");
//  Serial.print(current);
  int progress = (current / total) * 100;
  Serial.print(" progress=");
  Serial.print(progress);
  
  float decay = 0;
  if(progress < 20) {
    decay = 0.75;
  }
  else if(progress < 50) {
     decay = 0.85;
  }
  else {
    decay = 0.9999;
  }
  
  Serial.print(" decay=");
  Serial.print(decay);
  
  fadeExp(decay);
} 

// Esponential fade
void fadeExp(float fade) {
  for(uint16_t p=0; p < NUM_LEDS; p++) {
    CRGB current = leds[p];
    leds[p].r = current.r * fade;
    leds[p].g = current.g * fade;
    leds[p].b = current.b * fade;
 }
}
void fade(int fade) {
  for(uint16_t p=0; p < NUM_LEDS; p++) {
    CRGB current = leds[p];
    leds[p].r = current.r - fade;
    leds[p].g = current.g - fade;
    leds[p].b = current.b - fade;
 }
}


// Fill the dots one after the other with a color
void colorWipe(CRGB c, uint8_t wait) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      leds[i] = c;
      brightness();
      FastLED.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait, int inc) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<NUM_LEDS; i++) {
      leds[i].setHSV((((i+j) & 255) +  inc), 255, 255);
    }
    brightness();
    FastLED.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      leds[i].setHSV((((i * 256 / NUM_LEDS) + j) & 255), 255, 255);
    }
    brightness();
    FastLED.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(CRGB c, uint8_t wait) {
  for (int j=0; j<100; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 8; q++) {
      for (int i=0; i < NUM_LEDS; i=i+8) {
        leds[(i+q)] =  c;    //turn every third pixel on
      }
      fade(30);
      brightness();
      FastLED.show();
     
      delay(wait);
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait, int x) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < x; q++) {
        for (int i=0; i < NUM_LEDS; i=i+x) {
          leds[(i+q)].setHSV(( (i+j) % 255), 255, 255);    //turn every third pixel on
        }
//        fade(20);
        brightness();
        FastLED.show();
//        float fade = (float)x / (float)40;
        fadeRange(x, NUM_LEDS);
       
        delay(wait);
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbowOld(uint8_t wait, int x) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < x; q++) {
        for (int i=0; i < NUM_LEDS; i=i+x) {
            leds[(i+q)].setHSV(((i+j) % 255), 255, 255);    //turn every third pixel on
        }
//        fade(20);
        brightness();
        FastLED.show();
//        float fade = (float)x / (float)40;
        float fade = 0.7;
        if(x < 10) {
          fade = 0.5;
        }
        fadeExp(fade);
       
        delay(wait);
    }
  }
}

void brightness() {
/*
  int photo = analogRead(A0) / 5;
  int bright = 20 + photo;
  Serial.print("photo: ");
  Serial.print(photo);
  Serial.print("\n");
  if(bright > 150) bright = 150;
  if(bright < 0) bright = 10;
  strip.setBrightness(bright);
  Serial.print("brightness: ");
  Serial.print(bright);
  Serial.print("\n");
  */
  LEDS.setBrightness(BRIGHTNESS);
}

