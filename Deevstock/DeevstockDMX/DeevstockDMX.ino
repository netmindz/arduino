/*
   Required Connections
   --------------------
   pin 0: RO

   DE + RE = LOW

*/
#define FASTLED_ALLOW_INTERRUPTS 1 // fixes flutter, possibly causes crash
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#include <DmxReceiver.h>
#include <FastLED.h>

DmxReceiver dmx;
IntervalTimer dmxTimer;

#define LED_PIN  7

#define CHIPSET NEOPIXEL

const uint8_t kMatrixWidth = 8; // length of string
const uint8_t kMatrixHeight = 5;

const bool    kMatrixSerpentineLayout = false;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

CRGB leds[NUM_LEDS];

int STEPS = 4;
int BRIGHTNESS = 5;
int SPEEDO = 0;

#define FRAMES_PER_SECOND  120

void autoRun();
void rainbow();
void rainbowWithGlitter();
void confetti();
void sinelon();
void juggle();
void bpm();
void rainbowSweep();
void Rainbow();
void RainbowWash();
void Ripple();
void shimmer();
void one_sin();
void snake();

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { autoRun, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, rainbowSweep, Rainbow, RainbowWash, Ripple, shimmer, one_sin, snake  };
uint8_t gCurrentPatternNumber = 1; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

//void dmxTimerISR(void)
//{
//  dmx.bufferService();
//} 

void setup() {
  /* USB serial */
  Serial.begin(115200);

  /* DMX */
  dmx.begin();

  /* Use a timer to service DMX buffers every 1ms */
  // dmxTimer.begin(dmxTimerISR, 1000);

  pinMode(LED_BUILTIN, OUTPUT);

  FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println("Setup");
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

int led = 0;
elapsedMillis elapsed;
void loop()
{
  dmx.bufferService();
  /* Toggle LED on every new frame */
  if (dmx.newFrame())
  {
    led = !led;
    digitalWrite(LED_BUILTIN, led);
    
    int b = dmx.getDimmer(1);
    if (b != BRIGHTNESS) {
      BRIGHTNESS = b;
      FastLED.setBrightness(BRIGHTNESS);
    }
    STEPS = dmx.getDimmer(2);
    SPEEDO = dmx.getDimmer(3);
  }

  int p = dmx.getDimmer(4);
  int pattern = map(p,0,255,0, (ARRAY_SIZE(gPatterns) - 1));
  Serial.print("pattern = ");
  Serial.println(pattern);
  gPatterns[pattern]();
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}



void autoRun() {
  
  gPatterns[gCurrentPatternNumber]();
  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { 
    Serial.println("hue");
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
  EVERY_N_SECONDS( 10 ) { 
    Serial.println("Next pattern");
    nextPattern();
  } // change patterns periodically

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



