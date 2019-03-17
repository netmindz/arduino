#include<FastLED.h>

#define CHIPSET WS2811
#define LED_PIN D4
#define COLOR_ORDER BGR
#define NUM_LEDS 200

CRGB leds[NUM_LEDS];

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <ESPAsyncE131.h>

#define UNIVERSE 1        // First DMX Universe to listen for
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

int brightness  = 50;
int pattern = 0;

void autoRun();
void rainbow();

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { autoRun, rainbow };

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

  e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT);

  int sanity = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    sanity++;
    if (sanity > 10) break;
  }
  Serial.println(F("Connected to wifi "));
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
}

void loop() {
  readDMX();
  gPatterns[pattern]();
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

    brightness = getValue(packet, 1, 0, 255);
    FastLED.setBrightness(brightness);

    pattern = getValue(packet, 2, 0, (gPatternCount - 1));

  }
}

int getValue(e131_packet_t packet, int chan, int minV, int maxV) {
  return map(packet.property_values[(CHANNEL_START + (chan - 1))], 0, 255, minV, maxV);
}

int autoPattern = 1;
void autoRun() {

  gPatterns[autoPattern]();

  EVERY_N_SECONDS(60) {
    nextPattern();
    Serial.print("Swapping to pattern ");
    Serial.println(autoPattern);
  }

}

void nextPattern() {
  autoPattern++;
  if (autoPattern >= gPatternCount) autoPattern = 1;
}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, 0, 7);
  FastLED.show();
}

