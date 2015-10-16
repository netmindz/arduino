//Code includes samples written by Mark Kriegsman and Andrew Tuline.  Thanks so much for all your help guys!

#include <FastLED.h>

//---LED SETUP STUFF
#define LED_PIN 2
#define CLOCK_PIN 6
#define BUTTON_PIN 7 
#define COLOR_ORDER GBR

#define NUM_LEDS 120       // Change to reflect the number of LEDs you have

CRGB leds[NUM_LEDS];      //naming our LED array

//BUTTON SETUP STUFF
byte prevKeyState = HIGH;        


int ledMode = 0;  //FIRST ACTIVE MODE

CRGBPalette16 currentPalette;
TBlendType    currentBlending;


// MODE VARIABLES -- Change these numbers to create new interesting modes

int BRIGHTNESS = 155;    //0-255.  Lower number saves battery life, higher number is screamingly bright
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



//------------------SETUP------------------
void setup()  
{
  delay( 2000 ); // power-up safety delay
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  FastLED.addLeds<DOTSTAR, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  currentBlending = BLEND;
  
}

#define NUM_MODES 3
//------------------MAIN LOOP------------------
void loop() {
    switch (ledMode) {
      // BRIGHTNESS = 255;
       case 0:  Rainbow(); break;              //Rainbow -- Change STEPS and SPEED to modify
       case 1: sinwave_1(); break;            //Sin Wave -- Change sinwave variables to modify
       case 2: ripple(); break;               //Ripple -- Change 
       // BRIGHTNESS=0;
       case 3: Solid(); break;  //all off -- change BRIGHTNESS to 1-255 for a solid color
}  

 // button management section
   
        byte currKeyState = digitalRead(BUTTON_PIN);
     
        if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
            keyRelease();
        }
        
        prevKeyState = currKeyState;
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
    leds[k] = CHSV(bgclr, 255, bgbright);
    leds[k] += CHSV(thishue, allsat, thisbright);                               // Assigning hues and brightness to the led array.
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
  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(HUE++, 255, 50);  // Rotate background colour.

  switch (step) {

    case -1:                                                          // Initialize ripple variables.
      center = random(NUM_LEDS);
      colour = random16(0,256);
      step = 0;
      break;

    case 0:
      leds[center] = CHSV(colour, 255, 255);                          // Display the first pixel of the ripple.
      step ++;
      break;

    case maxsteps:                                                    // At the end of the ripples.
      step = -1;
      break;

    default:                                                          // Middle of the ripples.
        leds[wrap(center + step)] += CHSV(colour, 255, myfade/step*2);   // Display the next pixels in the range for one side.
        leds[wrap(center - step)] += CHSV(colour, 255, myfade/step*2);   // Display the next pixels in the range for the other side.
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



//BUTTON CONTROL STUFF
// called when button is pressed
void shortKeyPress() {
    Serial.println("short");
    ledMode++;
    if (ledMode > NUM_MODES){
    ledMode=0; }
}

// called when key goes from pressed to not pressed
void keyRelease() {
    Serial.println("key release");
        shortKeyPress();
    }
