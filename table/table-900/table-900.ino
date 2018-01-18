//Code includes samples written by Mark Kriegsman and Andrew Tuline.  Thanks so much for all your help guys!

#include <FastLED.h>
#include "SPI.h" // SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include "MSGEQ7.h"

//---LED SETUP STUFF
#define LED_PIN 6
#define BUTTON_PIN 2 
#define COLOR_ORDER GRB

#define WIDTH 30
#define HEIGHT 30

int kMatrixWidth = WIDTH;
int kMatrixHeight = HEIGHT;

#define NUM_LEDS (WIDTH * HEIGHT)     // Change to reflect the number of LEDs you have

CRGB leds[NUM_LEDS];      //naming our LED array

//BUTTON SETUP STUFF\w43fed
byte prevKeyState = HIGH;     

boolean kMatrixSerpentineLayout = true;

// EQ STUFF


#define pinAnalogLeft A0
#define pinAnalogRight A1
#define pinReset 5
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(60)
#define MSGEQ7_SMOOTH true

int band;
CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

int ledMode = 5;  //FIRST ACTIVE MODE

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

// MODE VARIABLES -- Change these numbers to create new interesting modes

int BRIGHTNESS = 55;    //0-255.  Lower number saves battery life, higher number is screamingly bright
int SATURATION = 255;    //0 is white (no color) and 255 is fully saturated with color
int HUE = 0;             //0-255, around the color wheel
int STEPS = 4;           //Wider or narrower bands of color
int SPEEDO = 10;         //The speed of the animation

// SIN WAVE SETUP

#define qsubd(x, b)  ((x>b)?wavebright:0)                   // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                          // Analog Unsigned subtraction macro. if result <0, then => 0


// SINWAVE VARIABLES -- change these numbers to modify Sinwave mode
uint8_t wavebright = 255;                                     // You can change the brightness of the waves/bars rolling across the screen.
uint8_t thishue = 30;                                          // You can change the starting hue value for the first wave.
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. 0= color stays the same
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
bool thisdir = 0;                                             // You can change direction.
int8_t thisspeed = 4;                                         // You can change the speed, and use negative values.
uint8_t allfreq = 25;                                         // You can change the frequency, thus overall width of bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
int loopdelay = 4;                                           // You can change the delay. Also you can change the allspeed variable above. 
uint8_t bgclr = 0;                                            // A rotating background colour.
uint8_t bgbright = 55;                                        // Background color brightness

//RIPPLE VARIABLES

uint8_t colour;                                               // Ripple colour is randomized.
int center = 0;                                               // Center of the current ripple.
int step = -1;                                                // -1 is the initializing step.
uint8_t myfade = 255;                                         // Starting brightness.
#define maxsteps 16                                           // Case statement wouldn't allow a variable.


// EQ vars 

int eq_hue = 0;
int eq_hue_wait = 10;

// audio check
int scount =  0;
int smillis = 0;
int slastmillis = 0;

// Change pattern
uint16_t plastmillis = 0;



  int barWidth = (WIDTH / 7) / 2;
  int blockWidth = barWidth * 7;


//------------------SETUP------------------
void setup()  
{
  Serial.begin(38400);

  Serial.print("***************** Setup *****************");
  Serial.print("ledmode=");
  Serial.println(ledMode); 


  delay( 2000 ); // power-up safety delay
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  FastLED.addLeds<WS2811, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  currentBlending = LINEARBLEND;
  
  MSGEQ7.begin();
  
}

#define NUM_MODES 5
//------------------MAIN LOOP------------------
void loop() {
  
//  if(ledMode > 2 && isSilence()) ledMode = 0;

    if((((millis()/1000) - plastmillis)) > 120) {
      Serial.println("Next pattern");
      plastmillis = millis() / 1000;
      Serial.print("plastmillis=");
      Serial.println(plastmillis);
      ledMode++;
      if(ledMode > NUM_MODES) ledMode = 0;
    }
/*
  while(true) {
    snake();
  }
*/
    switch (ledMode) {
       case 0:  Rainbow(); break;              //Rainbow -- Change STEPS and SPEED to modify
       case 1: sinwave_1(); break;            //Sin Wave -- Change sinwave variables to modify
       case 2: ripple(); break;               //Ripple -- Change 
 //      case 3: Solid(); break;  //all off -- change BRIGHTNESS to 1-255 for a solid color
       case 3: EQ(); break;                  // Show EQ
       case 4: VU(); break;                  // Show scrolling VU
       case 5: FunkyPlank(); break;          // "Funky Plank" - https://www.youtube.com/watch?v=HhP0erBrPlI
       case 6: DJLight(); break;             // https://plus.google.com/115124694226931502095/posts/iMQiXg3tTH3
      // case 7: Beat(); break;
       case 8: snake(); break; 
}  

 // button management section
   
        byte currKeyState = digitalRead(BUTTON_PIN);
     
        if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
            keyRelease();
        }
        
        prevKeyState = currKeyState;
    }
       

boolean isSilence() {
    smillis += (millis() - slastmillis);
    slastmillis = millis();
    //Only check once per second
    if(smillis > 1000) {
      smillis = 0;
      int t = 0;
      for(int b = 0; b < 7; b++) {
        int displayPeak = MSGEQ7.get(b, 0);
        if(displayPeak > t) t = displayPeak;
      }
      if(t < 50) {
        scount++;
      }
      else {
        scount = 0;
      }
      Serial.print("scount=");
      Serial.println(scount);
    }
    boolean silence = (scount > 15);
    if(silence) scount = 0;
//    return silence;
    return true;
 }

// SOLID COLOR -------------------------------------
void Solid()
{
   fill_solid(leds, NUM_LEDS, CHSV(HUE, SATURATION, BRIGHTNESS));  
   FastLED.show(); 
}


//SIN WAVE
void sinwave_1() {
   one_sin();  
   show_at_max_brightness_for_power();
    delay_at_max_brightness_for_power(loopdelay*2.5);
    Serial.println(LEDS.getFPS());   
}

void one_sin() {                                                                // This is the heart of this program. Sure is short.
  if (thisdir == 0) thisphase+=thisspeed; else thisphase-=thisspeed;            // You can change direction and speed individually.
    thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  for (int k=0; k<NUM_LEDS; k++) {
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[numToPixel(k)] = CHSV(bgclr, 255, bgbright);
    leds[numToPixel(k)] += CHSV(thishue, allsat, thisbright);                               // Assigning hues and brightness to the led array.
  }
  bgclr++;
} // one_sin()



// RAINBOW --------------------------------------------------
void Rainbow()
{ 
  FastLED.setBrightness(  BRIGHTNESS );
  currentPalette = RainbowColors_p;
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; 

  FillLEDsFromPaletteColors( startIndex);
    
  FastLED.show();
  FastLED.delay(SPEEDO);  
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    colorIndex += STEPS;
  }
}

//RIPPLE --------------------------------------------------------------------------------
void ripple() {
  HUE = 140;HUE++; 
    if (HUE > 220) {HUE = 140;}   // constrain BG hue to blues and purples
  for (int i = 0; i < NUM_LEDS; i++) leds[numToPixel(i)] = CHSV(HUE++, 255, 50);  // Rotate background colour.

  switch (step) {

    case -1:                                                          // Initialize ripple variables.
      center = random(NUM_LEDS);
      colour = random16(0,256);
      step = 0;
      break;

    case 0:
      leds[numToPixel(center)] = CHSV(colour, 255, 255);                          // Display the first pixel of the ripple.
      step ++;
      break;

    case maxsteps:                                                    // At the end of the ripples.
      step = -1;
      break;

    default:                                                          // Middle of the ripples.
        leds[numToPixel(wrap(center + step))] += CHSV(colour, 255, myfade/step*2);   // Display the next pixels in the range for one side.
        leds[numToPixel(wrap(center - step))] += CHSV(colour, 255, myfade/step*2);   // Display the next pixels in the range for the other side.
        step ++;                                                      // Next step.
        break;  
  } // switch step
  
 show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(SPEEDO*2.5);  
} // ripple()
 
 
 
int wrap(int step) {
  if(step < 0) return NUM_LEDS + step;
  if(step > NUM_LEDS - 1) return step - NUM_LEDS;
  return step;
} // wrap()

// EQ ------------------------------------------------------------------------------------

void EQ() {

  int xpos;

  
  // analyze without delay
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Led strip output
  if (newReading) {
    eq_hue_wait++;
    if(eq_hue_wait > 20) {
      eq_hue += 15;
      if(eq_hue > 255) eq_hue = 0;
      eq_hue_wait = 0;
    }
    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      int count = map(MSGEQ7.get(band), 0, 255, 0, HEIGHT);
      //int count = map(band, 0, 6, 1, HEIGHT);
      for(int b=0; b < barWidth; b++) {
        xpos = blockWidth - (barWidth * band) - b; 
        if(b == 0 && band == 0 && xpos != 14) {
          Serial.print("Wrong xpos for band0 = ");
          Serial.println(xpos);
        }
        for (int i = 0; i <= WIDTH; i++) {
          if (i <= count) {
            // unsigned int g = map(band, 0, 6, 254, 0);
            // CRGB color = CRGB(map(i , 1, WIDTH, 254, 0), g, 0);
            //CRGB color = CHSV(eq_hue + (band * 15), 255,255);
            CRGB color = CHSV((band * 25), 255,255);
            safeSetPixel(xytopixel(xpos, i), color);
          }
          else {
            safeSetPixel(xytopixel(xpos, i) ,CRGB::Black);
          }
        }
      }
    }

    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      int count = map(MSGEQ7.get(band), 0, 255, 0, HEIGHT);
      //int count = map(band, 0, 6, 1, HEIGHT);
      
      for(int b=0; b < barWidth; b++) {
        xpos = blockWidth + 1 + (barWidth * band) + b; 
        for (int i = 0; i <= WIDTH; i++) {
          if (i <= count) {
  //          unsigned int g = map(band, 0, 6, 254, 0);
  //          CRGB color = CRGB(map(i , 1, WIDTH, 254, 0), g, 0);
  //          CRGB color = CHSV(map(count,0, HEIGHT, 0, 255) , 255,255);
            //CRGB color = CHSV(eq_hue + (band * 15), 255,255);
            CRGB color = CHSV((band * 25), 255,255);
            safeSetPixel(xytopixel(xpos, i), color);
          }
          else {
            safeSetPixel(xytopixel(xpos, i), CRGB::Black);
          }
        }
      }
    }

    FastLED.show();
  }
}
int xytopixel(int x, int y) {
  // return ((y - 1) * WIDTH) + (x - 1);
  return XY(x,y);
}

// VU -----------------------------------------------------------------------------------

int getTotal(int input) {
  int t = 0;
  for(int i=0; i < 7; i++) {
    t += MSGEQ7.get(i, input);
  }
  return t / 7;
}


void VU() {
  CRGB color;

  const boolean gay = false;

  bool newReading = MSGEQ7.read(ReadsPerSecond(30));

  // Led strip output
  if (newReading) {

    int displayPeakL = map(MSGEQ7.get(MSGEQ7_LOW, 0), 0, 255, 0, round(HEIGHT / 2));
    int displayPeakR = map(MSGEQ7.get(MSGEQ7_LOW, 1), 0, 255, 0, round(HEIGHT / 2));
    Serial.print("Display peak: ");
    Serial.println(displayPeakL);

    moveUp();
    int offset = round(HEIGHT / 2);

//    for (int i = 1; i <= HEIGHT; i++){
//        drawPixel(0, i, CRGB::Black);
//    }
    
    for (int i = 0; i < HEIGHT; i++){
        drawPixel(offset, i, CRGB::Blue);
    }

    for(int i=1; i <= displayPeakL; i++) {
      if(gay) {
         color = CHSV(map(i, 1, (HEIGHT/2), 0, 230), 255, 255);
      }
      else {
          unsigned int g = map(i, 1, HEIGHT, 254, 0);
          color = CRGB(map(i , 1, HEIGHT, 0, 254), g, 0);
      }
      drawPixel((offset - i), 0, color);
    }
    for(int i=1; i <= displayPeakR; i++) {
      if(gay) {
         color = CHSV(map(i, 1, (HEIGHT/2), 230, 0), 255, 255);
      }
      else {
          unsigned int g = map(i, 1, (HEIGHT/2), 254, 0);
          color = CRGB(map(i , 1, (HEIGHT/2), 0, 254), g, 0);
      }
      drawPixel((offset + i), 0, color);
    }
      
    FastLED.show();
  }
}

/*
1,1 = 0
1,2 = 1
2,1 = 15
*/

int xytopixelP(int x, int y) {
//  int p = ((x - 1) * HEIGHTP) + (y - 1);
//  return p;
  return XY(y,x);
}

void drawPixelP(int x, int y, CRGB color) {
  leds[xytopixelP(x, y)] = color;
}

void drawPixel(int x, int y, CRGB color) {
  leds[xytopixel(x, y)] = color;
}

void moveUp() {
  // Update the display:
  for (int i = (HEIGHT - 1); i >= 0; i--) {
    for (int j = (WIDTH - 1); j >= 0; j--) {
      int src = XY(j, (i - 1));
      int dst = XY(j, i);
      leds[dst] = leds[src];
    }
  }
}



void safeSetPixel(int pos, CRGB value) {
  if(pos > (NUM_LEDS -1)) {
    Serial.print("IXP ");
    Serial.println(pos);
  }
  else {
    leds[pos] = value;
  }
}


// Funky plank  -----------------------------------------------------------------------------------


void FunkyPlank() {

  bool newReading = MSGEQ7.read(ReadsPerSecond(30));

  int offset = 8;
  // Led strip output
  if (newReading) {
    
    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      int hue = MSGEQ7.get(band, 0);
      for(int b=0; b < barWidth; b++) {
       int  xpos = blockWidth - (barWidth * band) - b; 
        drawPixel(xpos, 0, CHSV(hue, 255, 255));
  //      drawPixel((offset + band + 1), 0, CHSV(hue, 255, 255));
      }
    }

    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      int hue = MSGEQ7.get(band, 1);
      for(int b=0; b < barWidth; b++) {
        int xpos = blockWidth + 1 + (barWidth * band) + b; 
        drawPixel(xpos, 0, CHSV(hue, 255, 255));
      }
    }

    FastLED.show();
    moveUp();

  }
}

// DJ Light  -----------------------------------------------------------------------------------


void DJLight() {

  bool newReading = MSGEQ7.read(ReadsPerSecond(30));

  int offset = 8;
  
  int mid = NUM_LEDS / 2;
  
  // Led strip output
  if (newReading) {
    
    int bands[8];
    for ( band = 0; band < 7; band++ )
    {
      bands[band] = MSGEQ7.get(band, 1);
    }
    
    
    
    leds[mid] = CRGB(bands[6], bands[5] / 8, bands[1] / 2);
    leds[mid].fadeToBlackBy(bands[3] / 12);
    //move to the left
    for (int i = NUM_LEDS - 1; i > mid; i--) {
      leds[i] = leds[i - 1];
    }
    // move to the right
    for (int i = 0; i < mid; i++) {
      leds[i] = leds[i + 1];
    }
    FastLED.show();
    fade_down(1);
  }
}

void fade_down(uint8_t value){
for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].fadeToBlackBy(value);
  }
}

/*
void soften_spectrum_data()
{
  for (byte i = 0; i < 7; i++)
  {
    uint8_t old = bands[i];
    uint16_t data = left[i] + right[i] + old + old;
    data = data / 4;
    bands[i] = data;
  }
}
*/

// Beat   -----------------------------------------------------------------------------------
const int frames = 150;
int frame = 0;
int bass[frames];
//int snare[frames];
//int hat[frames];

int find_average(int ary[], int siz){
  double sum = 0;
  for (int i = 0; i < siz; i++){
    sum += ary[i];
  }
  Serial.print("find_average=");
  Serial.println(sum/siz);
  return sum/siz;
}

void Beat(){
  if(MSGEQ7.read(ReadsPerSecond(80))) {
    int new_bass = MSGEQ7.get(MSGEQ7_BASS, 0);
    int new_snare = MSGEQ7.get(MSGEQ7_MID, 0);
    int new_hat = MSGEQ7.get(MSGEQ7_HIGH, 0);

    int r = 0;  
    int g = 0;  
    int b = 0;  
//    if ((new_snare  > find_average(snare, frames))){
//      r = map((new_snare  - find_average(snare, frames)), 0, 255, 0 ,255);
//    }
//    if ((new_hat/find_average(hat, frames)) > 1){
//    if ((new_hat > find_average(hat, frames))){
//      g = map((new_hat  - find_average(hat, frames)), 0, 255, 0 ,255);
//    }
//    if ((new_bass/find_average(bass, frames)) > 1){
  //  if ((new_bass > find_average(bass, frames))){
/*        if((new_snare  - getLastFrame(snare, frame)) > 0) {
          r = (new_snare  - getLastFrame(snare, frame));
        }
        if((new_hat  - getLastFrame(hat, frame)) > 0) {
          g = (new_hat  - getLastFrame(hat, frame));
        }
        */
        if((new_bass  - getLastFrame(bass, frame)) > 0) {
//          b = (new_bass  - getLastFrame(bass, frame));
          b = (new_bass  - find_average(bass, frames));
        }
        Serial.println(b);

//    hat[frame] = new_hat;
//    snare[frame] = new_snare;
    bass[frame] = new_bass;
    frame++;
    if (frame >= frames) frame=0;
    fill_solid(leds, NUM_LEDS, CRGB(r,g,b));
    FastLED.show();
  }
}

int getLastFrame(int data[], int frame) {
  if((frame - 1) < 0) frame=frames;
  return data[(frame - 1)]; 
}
  

// BUTTON CONTROL STUFF   -----------------------------------------------------------------------------------

// called when button is pressed
void shortKeyPress() {
    Serial.println("short");
    ledMode++;
    if (ledMode > NUM_MODES) {
      ledMode=0;
    }
}

// called when key goes from pressed to not pressed
void keyRelease() {
    Serial.println("key release");
        shortKeyPress();
    }
    
    
    
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}

uint16_t numToPixel(uint16_t p)
{
  uint16_t i;

  if ( kMatrixSerpentineLayout == false) {
    i = p;
  }

  if ( kMatrixSerpentineLayout == true) {
    int y = p / kMatrixWidth;
    if ((y % 2) != 0) {
      int r = p % kMatrixWidth;
      i = (y * kMatrixWidth) + (kMatrixWidth - r) - 1;
    } else {
      // Even rows run forwards
      i = p;
    }
    if(p == 29 && i != 29) { // last of first row
      Serial.print("** y=");
      Serial.println(y);
      Serial.print("p(29)=");
      Serial.println(i);
    }
    if(p == 59 && i != 30) { // first of row 2
      Serial.print("# y=");
      Serial.println(y);
      Serial.print("p(59)=");
      Serial.println(i);
    }
    if(p == 30 && i != 59) { // last of row 2
      Serial.print("$ y=");
      Serial.println(y);
      Serial.print("p(30)=");
      Serial.println(i);
    }
  }

  return i;
}

