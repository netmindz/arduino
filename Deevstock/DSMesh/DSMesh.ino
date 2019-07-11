#include<FastLED.h>

#define CHIPSET WS2811
#define LED_PIN D4
#define COLOR_ORDER RGB
// the size of your matrix
#define kMatrixWidth  10
#define kMatrixHeight 10
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
const bool    kMatrixSerpentineLayout = true;

// used in FillNoise for central zooming
byte CentreX =  (kMatrixWidth / 2) - 1;
byte CentreY = (kMatrixHeight / 2) - 1;

uint8_t STEPS = 150;
uint8_t BRIGHTNESS = 250;
uint8_t SPEEDO = 180;
uint8_t FADE = 180;

// a place to store the color palette

CRGB leds[NUM_LEDS];

CRGBPalette16 palettes[] = {RainbowColors_p, RainbowStripeColors_p, RainbowStripeColors_p, CloudColors_p, PartyColors_p };

CRGBPalette16 currentPalette = palettes[0];
TBlendType currentBlending = LINEARBLEND;

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <ESPAsyncE131.h>

#define UNIVERSE 2        // First DMX Universe to listen for
#define UNIVERSE_COUNT 1  // Total number of Universes to listen for, starting at UNIVERSE
#define CHANNEL_START 1   // Channel to start listening at


#include "wifi.h"
// Create file with the following
// *************************************************************************
// #define SECRET_SSID "";  /* Replace with your SSID */
// #define SECRET_PSK "";   /* Replace with your WPA2 passphrase */
// *************************************************************************
const char ssid[] = SECRET_SSID;
const char passphrase[] = SECRET_PSK;

ESPAsyncE131 e131(UNIVERSE_COUNT);

int pattern = 0;

#include "demo.h";
#include "chase.h";
#include "colorWaves.h";
#include "pride.h"
#include "datchet.h";
#include "funkynoise.h";
#include "stars.h"
void autoRun();
void fill();

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { autoRun, 
showStars,
//fill,
colorWaves,
chase,
datchet,
pride,
rainbow,
rainbowWithGlitter,
confetti,
sinelon,
juggle,
bpm,
MirroredNoise,
RedClouds,
Lavalamp1,
Lavalamp2,
Lavalamp3,
Lavalamp4,
Lavalamp5,
Constrained1,
RelativeMotion1,
Water,
Bubbles1,
TripleMotion,
CrossNoise,
CrossNoise2,
Caleido1,
Caleido2,
Caleido3,
Caleido4,
Caleido5,
Caleido6,
Caleido7
};

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
int gPatternCount = ARRAY_SIZE(gPatterns);

void setup() {
  Serial.begin(115200);
  // Make sure you're in station mode
  WiFi.mode(WIFI_STA);

  Serial.println("");
  Serial.print(F("Connecting to "));
  Serial.print(ssid);

  if (passphrase != NULL)
    WiFi.begin(ssid, passphrase);
  else
    WiFi.begin(ssid);

  int sanity = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    sanity++;
    if (sanity > 20) break;
  }
  Serial.println(F("Connected to wifi "));
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT);

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.printf("gPatternCount = %u\n", gPatternCount);
}

void loop() {
  readDMX();
  gPatterns[pattern]();
  FastLED.show();
  EVERY_N_SECONDS( 5 ) {
    Serial.printf("Pattern : %u\n", pattern);
  }

}


void readDMX() {
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

    BRIGHTNESS = getValue(packet, 1, 0, 255);
    FastLED.setBrightness(BRIGHTNESS);

    pattern = getValue(packet, 2, 0, (gPatternCount - 1));

    STEPS =   getValue(packet, 3, 0, 255);
    SPEEDO =  getValue(packet, 4, 0, 255);
    FADE =    getValue(packet, 5, 100, 0);

    currentPalette = palettes[getValue(packet, 6,  0, (ARRAY_SIZE(palettes) - 1))]; // channel 6
  }
}

int getValue(e131_packet_t packet, int chan, int minV, int maxV) {
  return map(packet.property_values[(CHANNEL_START + (chan - 1))], 0, 255, minV, maxV);
}

int autoPattern = 1; // never set to 0
void autoRun() {

  gPatterns[autoPattern]();

  // FIXME: only needed in some patterns
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }

  EVERY_N_SECONDS(90) {
    nextPattern();
    //    autoPattern = random(1, gPatternCount);
    Serial.printf("Next Auto pattern %u\n", autoPattern);
  }

}

void nextPattern() {
  autoPattern++;
  if (autoPattern >= gPatternCount) autoPattern = 1;
}

void fill() {
//  int colorIndex = 0;
//  uint8_t brightness = 255;
//  for (int i = 0; i < NUM_LEDS; i++) {
//    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
//    colorIndex += 3;
//  }
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.delay(50);
}

