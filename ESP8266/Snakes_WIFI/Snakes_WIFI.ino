#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncE131.h>

#define UNIVERSE 1        // First DMX Universe to listen for
#define UNIVERSE_COUNT 1  // Total number of Universes to listen for, starting at UNIVERSE

#define CHANNEL_START 1 /* Channel to start listening at */


#define LED_PIN  D4

#define COLOR_ORDER RGB
#define CHIPSET     WS2811

int kMatrixWidth = 30;
int kMatrixHeight = 30;

#define NUM_LEDS 900

#define NUM_SNAKES 300

// ********************
// DMX values
int BRIGHTNESS = 5;   // CH1
int SNAKES = 5;       // CH2
int SPEED = 10;       // CH3
int FADE = 40;        // CH4
// ********************


#include "wifi.h"
// Create file with the following
// *************************************************************************
// #define SECRET_SSID "";  /* Replace with your SSID */
// #define SECRET_PSK "";   /* Replace with your WPA2 passphrase */
// *************************************************************************
const char ssid[] = SECRET_SSID;
const char passphrase[] = SECRET_PSK;

ESPAsyncE131 e131(UNIVERSE_COUNT);

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
    int r = 1; // random(1, 20);
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
          // Serial.println("x > kMatrixWidth");
          x -= moveX;
          moveX = -1;
          // if(random(0,1) == 1) {
          moveY = random(-1, 1);
          // }
        }
        if (y > kMatrixHeight) {
          // Serial.println("y > kMatrixHeight");
          y -= moveY;
          moveY = -1;
          // if(random(0,1) == 1) {
          moveX = random(-1, 1);
          // }
        }


        if (x < 0) {
          // Serial.println("x < 1");
          x += moveX;
          moveX = 1;
          //if(random(0,1) == 1) {
          moveY = random(-1, 1);
          // }
        }
        if (y < 0) {
          // Serial.println("y < 1");
          y += moveY;
          moveY = 1;
          //if(random(0,1) == 1) {
          moveX = random(-1, 1);
          // }
        }
      }
      leds[XY(x, y)]  = CHSV(hue, 255, v);
    }

    void setV(int newV) {
      v = newV;
    }
};



Snake snakes[NUM_SNAKES];

void setup() {
  // Make sure you're in station mode
  WiFi.mode(WIFI_STA);

  Serial.println("");
  Serial.print(F("Connecting to "));
  Serial.print(ssid);

  if (passphrase != NULL)
    WiFi.begin(ssid, passphrase);
  else
    WiFi.begin(ssid);

  Serial.print("Waiting on wifi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("w");
  }
  Serial.println("\nDone");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // Choose one to begin listening for E1.31 data
  //if (e131.begin(E131_UNICAST)) {
  if (e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT)) {  // Listen via Multicast
    Serial.println(F("Listening for data..."));
  }
  else {
    Serial.println(F("*** e131.begin failed ***"));
  }


  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setDither(0);

  randomSeed(analogRead(0));
  Serial.begin(115200);

  for (int  i = 0; i < NUM_SNAKES; i++) {
    snakes[i].setXY(random(1, kMatrixWidth), random(1, kMatrixHeight));
  }
}

void loop() {

  if (!e131.isEmpty()) {
    e131_packet_t packet;
    e131.pull(&packet);     // Pull packet from ring buffer

    EVERY_N_SECONDS( 2 ) {

      Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH1: %u\n",
                    htons(packet.universe),                 // The Universe for this packet
                    htons(packet.property_value_count) - 1, // Start code is ignored, we're interested in dimmer data
                    e131.stats.num_packets,                 // Packet counter
                    e131.stats.packet_errors,               // Packet error counter
                    packet.property_values[1]);             // Dimmer data for Channel 1
    }

    /* Parse a packet and update pixels */
    BRIGHTNESS = map(packet.property_values[(CHANNEL_START  + 0)], 0, 255, 0, 255);
    SNAKES = map(packet.property_values[(CHANNEL_START  + 1)], 0, 255, 1, NUM_SNAKES);
    SPEED = map(packet.property_values[(CHANNEL_START  + 2)], 0, 255, 200, 0);
    FADE = map(packet.property_values[(CHANNEL_START  + 3)], 0, 255, 100, 0);
    //    if (packet.property_values[(CHANNEL_START  + 3)] < 125) {
    //      INWARD = true;
    //    }
    //    else {
    //      INWARD = false;
    //    }
    FastLED.setBrightness(BRIGHTNESS);
  }
  EVERY_N_SECONDS( 2 ) {
    Serial.print("BRIGHTNESS = ");
    Serial.print(BRIGHTNESS);

    Serial.print("  SNAKES = ");
    Serial.print(SNAKES);

    Serial.print("  SPEED = ");
    Serial.print(SPEED);

    Serial.print("  FADE = ");
    Serial.print(FADE);

    Serial.println();
  }


  snake();
}

void snake() {

  for (int  i = 0; i < NUM_SNAKES; i++) {
    if (i <= SNAKES) {
      snakes[i].setV(255);
      snakes[i].moveSnake();
    }
    else {
      snakes[i].setV(0);
    }

  }
  FastLED.delay(SPEED);
  if (FADE == 100) {
    FastLED.clear();
  }
  else {
    fadeToBlackBy( leds, NUM_LEDS, FADE);
  }
}


