#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>

// Params for width and height
const uint8_t kMatrixWidth = 30; // length of string
const uint8_t kMatrixHeight = 30;
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

CRGB leds[NUM_LEDS];

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


class GameSnake {

    char dir;
    int x;
    int y;

  public:
    GameSnake() {
      init();
    }

    void init() {
      x = kMatrixWidth / 2;
      y = kMatrixHeight / 2;
      dir = 'L';
      Serial.println("Go!");
    }

    void frame() {
      switch (dir) {
        case 'U':
          y++;
          break;
        case 'D':
          y--;
          break;
        case 'L':
          x--;
          break;
        case 'R':
          x++;
          break;
      }

      if (x > (kMatrixWidth - 1)) {
        die();
      }
      if (x < 0) {
        die();
      }

      if (y > (kMatrixHeight - 1)) {
        die();
      }
      if (y < 0) {
        die();
      }
      Serial.print(x);
      Serial.print(",");
      Serial.println(y);

      FastLED.clear();
      leds[XY(x, y)] = CRGB::White;
      FastLED.delay(600);
    }

    void die() {
      Serial.println("Dead!");
      for (int i = 0; i < 10; i++) {
        fill_solid (leds, NUM_LEDS, CRGB::Red);
        FastLED.delay(100);
        fill_solid (leds, NUM_LEDS, CRGB::Black);
        FastLED.delay(100);
      }
      init();
    }

    void input(int c) {
      switch (c) {
        case 117:
          dir = 'U';
          break;
        case 100:
          dir = 'D';
          break;
        case 108:
          dir = 'L';
          break;
        case 114:
          dir = 'R';
          break;
        default:
          Serial.print("Unknown input ");
          Serial.println(c);
      }
    }
};

GameSnake snake;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812SERIAL, 1, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
}

int incomingByte = 0;
void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte);
    snake.input(incomingByte);
  }
  snake.frame();
}


