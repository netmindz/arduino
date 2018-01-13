#include <FastLED.h>

#define LED_PIN  6

#define COLOR_ORDER RGB
#define CHIPSET     WS2811

int kMatrixWidth = 30;
int kMatrixHeight = 30;

#define NUM_LEDS 900

#define NUM_SNAKES 10


CRGB leds[NUM_LEDS];      //naming our LED array

bool kMatrixSerpentineLayout = true;
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if (x > kMatrixWidth) return 0;
  if (y > kMatrixHeight) return 0;

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

  if (i >= (NUM_LEDS - 1)) return 0;
  return i;
}



class Snake {
    int dir = 0;
    uint8_t hue = random(0, 255);
    int v = 255;
    int r = random(1,20);
    int rskip = r;

    int x = 0;
    int y = 0;

    int moveX = random(-1, 1);
    int moveY = random(-1, 1);

  public:
    Snake() {

    }

    void setXY(int newX, int newY) {
      x = newX;
      y = newY;
      if (moveX == 0 && moveY == 0) {
        moveX = 1;
        moveY = 1;
      }
    }

    void moveSnake() {

      rskip--;
      if (rskip == 0) {
        rskip = r;

        x += moveX;
        y += moveY;

        if (x > kMatrixWidth) {
          Serial.println("x > kMatrixWidth");
          x -= moveX;
          moveX = -1;
          // if(random(0,1) == 1) {
          moveY = random(-1, 1);
          // }
        }
        if (y > kMatrixHeight) {
          Serial.println("y > kMatrixHeight");
          y -= moveY;
          moveY = -1;
          // if(random(0,1) == 1) {
          moveX = random(-1, 1);
          // }
        }


        if (x < 0) {
          Serial.println("x < 1");
          x += moveX;
          moveX = 1;
          //if(random(0,1) == 1) {
          moveY = random(-1, 1);
          // }
        }
        if (y < 0) {
          Serial.println("y < 1");
          y += moveY;
          moveY = 1;
          //if(random(0,1) == 1) {
          moveX = random(-1, 1);
          // }
        }
      }
      leds[XY(x, y)]  = CHSV(hue, 255, v);
    }
};



Snake snakes[NUM_SNAKES];

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( 30 );
  FastLED.setDither(0);

  randomSeed(analogRead(0));
  Serial.begin(115200);

  for (int  i = 0; i < NUM_SNAKES; i++) {
    snakes[i].setXY(random(1, kMatrixWidth), random(1, kMatrixHeight));
  }
}

void loop() {
  snake();
}

void snake() {

  for (int  i = 0; i < NUM_SNAKES; i++) {
    snakes[i].moveSnake();
  }
  FastLED.delay(0);
  // FastLED.clear();
  fadeToBlackBy( leds, NUM_LEDS, 40);
}


