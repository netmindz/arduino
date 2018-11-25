#include <Adafruit_NeoPixel.h>

#define PIN 6

#define BRIGHTNESS 20

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  Serial.begin(9600);
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(BRIGHTNESS);
      Serial.print("Setup");
}

void loop() {
  // Some example procedures showing how to display to the pixels:
//    colorWipe(strip.Color(255, 0, 0), 70); // Red
//  theaterChase(strip.Color(  0,   0, 127), 70); // Blue
//  colorWipe(strip.Color(0, 255, 0), 70); // Green
//  theaterChase(strip.Color(127,   0,   0), 70); // Red
//  colorWipe(strip.Color(0, 0, 255), 70); // Blue
//  theaterChase(strip.Color(127, 127, 127), 70); // White
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


//    theaterChaseRainbowOld(50, 5);
 //     rainbowCycle(50);
  
    for(int x=4; x <= 20; x++) {
      theaterChaseRainbowOld(30, x);
      Serial.print("\ntheaterChaseRainbow=");
      Serial.print(x);
      Serial.print("\n");
    }
    
  //    rainbow(50,3);
  //    rainbowCycle(50);
    for(int x=3; x <= 20; x++) {
      theaterChaseRainbow(30, x);
      Serial.print("\ntheaterChaseRainbow=");
      Serial.print(x);
      Serial.print("\n");
    }
/*
      rainbow(50,5);
      rainbowCycle(150);
      rainbow(20,1);
      */

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
  
  strip.setBrightness(brightness);

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
  for(uint16_t p=0; p < strip.numPixels(); p++) {
    uint32_t current = strip.getPixelColor(p);
    uint8_t r = (uint8_t)(current >> 16);
    r = r * fade;
    uint8_t g = (uint8_t)(current >>  8);
    g = g * fade;
    uint8_t b = (uint8_t) current;
    b = b * fade;
    strip.setPixelColor(p, r, g ,b);
 }
}
void fade(int f) {
  int fade = fade / (BRIGHTNESS / 255);
  
  for(uint16_t p=0; p < strip.numPixels(); p++) {
    uint32_t current = strip.getPixelColor(p);
    uint8_t r = (uint8_t)(current >> 16);
    if(r <= fade) {
        r = 0;
    }
    else {
      r -= fade;
    }
    uint8_t g = (uint8_t)(current >>  8);
    if(g <= fade) {
        g = 0;
    }
    else {
      g -= fade;
    }
    uint8_t b = (uint8_t) current;
    if(b <= fade) {
        b = 0;
    }
    else {
      b -= fade;
    }
    strip.setPixelColor(p, r, g ,b);
 }
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      brightness();
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait, int inc) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i+j) & 255), inc));
    }
    brightness();
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    brightness();
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<100; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 8; q++) {
      for (int i=0; i < strip.numPixels(); i=i+8) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      fade(30);
      brightness();
      strip.show();
     
      delay(wait);
/*     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
*/
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait, int x) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < x; q++) {
        for (int i=0; i < strip.numPixels(); i=i+x) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
//          strip.setPixelColor(i+q, Wheel( j % 255));    //turn every third pixel on
        }
//        fade(20);
        brightness();
        strip.show();
//        float fade = (float)x / (float)40;
        fadeRange(x, strip.numPixels());
       
        delay(wait);
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbowOld(uint8_t wait, int x) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < x; q++) {
        for (int i=0; i < strip.numPixels(); i=i+x) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
//          strip.setPixelColor(i+q, Wheel( j % 255));    //turn every third pixel on
        }
//        fade(20);
        brightness();
        strip.show();
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


uint32_t Wheel(byte WheelPos) {
  return Wheel(WheelPos, 3);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, int inc) {
    
  if(WheelPos < 85) {
   return strip.Color(WheelPos * inc, 255 - WheelPos * inc, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * inc, 0, WheelPos * inc);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * inc, 255 - WheelPos * inc);
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
  strip.setBrightness(BRIGHTNESS);
}

