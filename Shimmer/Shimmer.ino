#include "FastLED.h"
#define NUM_LEDS 64
CRGB leds[NUM_LEDS];
void setup() {
  Serial.begin(9600);
  FastLED.setBrightness(30);   
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
}
void loop() { 
    int i = 1;
    int x = 1;
    
 boolean xrand = true;
 boolean irand = true;
 
 unsigned int l = 0;
 
 while(true) {
   
   /*
   if(l > 500) {
     l = 0;
     if(random(1,2) == 1) {
       xrand = true;
     }
     else {
       xrand = false;
     }
     if(random(1,2) == 1) {
       irand = true;
     }
     else {
       irand = false;
     }
     Serial.print("Mode change xrand=");
     Serial.print(xrand);
     Serial.print(" irand=");
     Serial.println(irand);
   }
   l++;
   /*/
   
   
    for(int dot = -1; dot < NUM_LEDS; dot += i) { 
            leds[dot].setHue(x);
            // leds[dot] = CRGB::Blue;
            FastLED.show();
            // clear this led for the next time around the loop
            // leds[dot] = CRGB::Black;
          delay(10);
   }
   
   if(irand) {
     i = random(2, 40);
   }
   else {
     if(i < 40) {  i++;  } else { i = 1;  }
   }

  if(xrand) {
    x = random(1,255);
  }
  else {
   if(x < 255) { x++; } else { x = 1;  }

  }
}
}
  

