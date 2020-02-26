// Based off code by Preyy - https://www.reddit.com/r/FastLED/comments/fa0p0i/i_made_a_more_versatile_crossfader_video_code/

#include "FastLED.h"
#define LEDstrips 28
#define LEDper 36
#define LEDtype WS2813
#define LEDcorr TypicalSMD5050
#define LEDdither 255     //try 0 to reduce flickering
uint8_t LEDbright = 11;
int LEDamps = 3000;

uint8_t hue[2];       // These are arrays because two patterns will overlap
int rowbucket[2];     // Counters for effects that expand over time
int colbucket[2];
int countbucket[2];

uint8_t deffade = 5;      // Default fade value for the fader function
uint8_t currfade = 0;

uint8_t patternum = 0;    // For keeping track of what patterns are running
uint8_t oldpattern = 1;
uint16_t crossct = 255;

// have 3 independent CRGBs - 2 for the sources and one for the output
CRGB leds[LEDstrips][LEDper];                                 // There is probably a way to do this with only 2 CRGB objects
CRGB leds2[LEDstrips][LEDper];
//CRGB leds3[LEDstrips][LEDper];

DEFINE_GRADIENT_PALETTE( startup ){ //RYGCB
  0,   255,  0,  0,      //red
  51,   255,  0,  0,
  52,   255,  255,  0,    //yellow
  102,  255,  255,  0,     
  103,  0,  255,  0,      //green
  153,  0,  255,  0,
  154,  0,  255,  255,    //cyan
  205,  0,  255,  255,
  206,  0,  0,  255,      //blue
  255,  0,  0,  255};

DEFINE_GRADIENT_PALETTE( tropicana ){
    0,    0, 0, 0,
  71, 66, 245, 230,
  145, 255, 255, 255,
  255,  245, 66, 203};

//Pattern Key
typedef void (*SimplePatternList[])();
uint8_t PATTERNnum = 0; // Index number of which pattern is current
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setup() {
  delay(500);
  // Initialize LEDs
   FastLED.setMaxPowerInVoltsAndMilliamps(5,LEDamps); //play with the order here
   //FastLED.setTemperature();
   FastLED.setBrightness(LEDbright);
   FastLED.setDither(LEDdither);
   FastLED.addLeds<LEDtype, PIN_C4, GRB>(leds[0], LEDper).setCorrection(LEDcorr);       // I'm switching to paralell output soon :)
   FastLED.addLeds<LEDtype, PIN_C3, GRB>(leds[1], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_C2, GRB>(leds[2], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_C1, GRB>(leds[3], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_C0, GRB>(leds[4], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_E1, GRB>(leds[5], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_E0, GRB>(leds[6], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_D7, GRB>(leds[7], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_D6, GRB>(leds[8], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_D5, GRB>(leds[9], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_D4, GRB>(leds[10], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_D1, GRB>(leds[11], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_D0, GRB>(leds[12], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_B7, GRB>(leds[13], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_B4, GRB>(leds[14], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_B3, GRB>(leds[15], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_B2, GRB>(leds[16], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_B1, GRB>(leds[17], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_B0, GRB>(leds[18], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_E7, GRB>(leds[19], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_E6, GRB>(leds[20], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_F0, GRB>(leds[21], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_F1, GRB>(leds[22], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_F2, GRB>(leds[23], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_F3, GRB>(leds[24], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_F4, GRB>(leds[25], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_F5, GRB>(leds[26], LEDper).setCorrection(LEDcorr);
   FastLED.addLeds<LEDtype, PIN_F6, GRB>(leds[27], LEDper).setCorrection(LEDcorr);
   delay(500);
   FastLED.clear();
   FastLED.show();
   Serial.begin(9600);
   delay(500);
}
SimplePatternList PATTERNlist = {diagonal, tropic, waterfall};    // PATTERNlist[PATTERNnum]();

void loop() {
  //uint8_t ratio = ease8InOutCubic(beatsin8(5));         fades in a straight line that looks pretty good right now, but could change that later
  
  EVERY_N_MILLIS(100){        // I'm frame rate limited, and havign this be low makes sure that some patterns don't run a ton faster than other patterns. Could definitely speed this up once I'm on a faster controller
    crossfader();
    hue[0]++;
    hue[1]++;
    FastLED.show();  
  } 
}

void diagonal(){
  fader(deffade);         // apply some fading, since this pattern is full screen it does not need to be too high
  uint8_t z = fetcher(0);     // get which version of the global variables to use based on what the current pattern is
  CRGBPalette16 palette = startup; 
  
  for (int r = 0; r < rowbucket[z]/2; r++){
    for (int c = 0; c < colbucket[z]/4; c++){
       leds[c][r] = ColorFromPalette( palette, hue[z]+(c+r)*10, 255, LINEARBLEND); // normal palette access
    }
  }

  if (colbucket[z]/4 < LEDstrips){      // expand width over time
    colbucket[z]++;
  }
  if (rowbucket[z]/2 < LEDper) {        // expand length over time
    rowbucket[z]++;
  }
}

void tropic(){
  fader(deffade);
  uint8_t z = fetcher(1);
  CRGBPalette16 palette = tropicana;      // I highly reccomend playing with palettes that have large sections of black in them. It creates great sections of discontinuity
  
  uint8_t BeatsPerMinute = 14;
  uint8_t beat = beatsin16( BeatsPerMinute, 25, 455);
  for(byte x = 0; x < LEDstrips; x++){
    for( int y = 0; y < LEDper; y++){
      leds[x][y] = ColorFromPalette(palette, hue[z]+(y*10)+x*2, beat-hue[z]+(y*10)-x*2, LINEARBLEND);   //leds[x][y] = ColorFromPalette(palette, hue+(y*10)+x*2, beat-hue+(y*10)-x*2, LINEARBLEND);
    }
  }
  hue[z] += 3;
}

void waterfall(){
  fader(deffade);
  uint8_t z = fetcher(2);  
  CRGBPalette16 palette = CloudColors_p;
  
  for (int row = 0; row < rowbucket[z]/2; row++){
    for (int col = 0; col < LEDstrips; col++){         // Write each row with start colour and a random saturation      
      leds[col][row] = ColorFromPalette(palette, hue[z] + row*15+sin8((row*7 + col*10+countbucket[z]*7))*(1+row)/80, 255, LINEARBLEND);          // This monster determines how much the wave twists, sin8 function is for base twist, and stuff to the left is for how much twist per row 
    }                                                                                                                                            // I'd love to make it twist back and forth, but I haven't come back to it yet, let me know if you can make it work
  }
  hue[z] = hue[z] - 25;
  if(rowbucket[z]/2 < LEDper){
    rowbucket[z]++;
  }  
   countbucket[z]++; 
}

void patcrossproc(){                                // Every time you switch patterns run this to begin crossfading
  oldpattern = patternum;                           // set the current pattern to be the old one so we can make it use the same variables
  crossct = 0;                                      // reset the blend amount
  patternum++;                                      // increase pattern number   -> this just goes in sequence, but the best part of this is that you can control this by remote or any other system

  rowbucket[0] = rowbucket[1];      // copy row status to default rows, and reset them for the new pattern
  rowbucket[1] = 0;
  colbucket[0] = colbucket[1];
  colbucket[1] = 0;
  countbucket[0] = countbucket[1];
  countbucket[1] = 0;
  hue[0] = hue[1];
  hue[1] = 0;
  
  if(patternum > 2){
    patternum = 0;
  }
}

void crossfader(){
 if(crossct >= 255){ 
    PATTERNlist[patternum]();   // run completed pattern only when fading is complete
    if (millis() > 15000){  
      EVERY_N_SECONDS(15){  // new pattern timer
        patcrossproc();
      } 
    }   
  } 
  else if(crossct < 255){
    crossct+=5;           // higher increase faster xfade
    if(crossct > 255){   // overflow prevention
      crossct = 255;
    }
    uint8_t blendamt = crossct;
 
   if (millis() > 15000){       // Run the old pattern and save to array
    PATTERNlist[oldpattern]();
      for(uint8_t x = 0; x < LEDstrips; x++){
        for(uint8_t y = 0; y < LEDper; y++){
          leds2[x][y] = leds[x][y];
        }
      }
    }
    
    PATTERNlist[patternum]();   // Run the new pattern and save to array 
    /*for(uint8_t x = 0; x < LEDstrips; x++){
      for(uint8_t y = 0; y < LEDper; y++){
        leds3[x][y] = leds[x][y];
      }
    }*/
    
    for(uint8_t x = 0; x < LEDstrips; x++){     // blend em
      for(uint8_t y = 0; y < LEDper; y++){
        leds[x][y] = blend( leds2[x][y], leds[x][y], blendamt);   // Blend arrays of LEDs, third value is blend %
      }
    }
  }
}

void fader(uint8_t targfade){
  if(currfade > targfade){
      currfade = targfade;
  }
  for(uint8_t x = 0; x < LEDstrips; x++){
    fadeToBlackBy( leds[x], LEDper, currfade);
  }
  EVERY_N_MILLIS(50){
    if(currfade < targfade){
      currfade++;
    }
    else if(currfade > targfade){
      currfade = targfade;
    }
  }
}

uint8_t fetcher(uint8_t oldcheck){        // Get which counters should be used
  if(oldcheck == oldpattern){
    return 0;
  } else {
    return 1;
  }
}
