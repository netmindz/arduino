#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncE131.h>

#define UNIVERSE 1        // First DMX Universe to listen for
#define UNIVERSE_COUNT 1  // Total number of Universes to listen for, starting at UNIVERSE

#define CHANNEL_START 1 /* Channel to start listening at */

// How many leds in your strip?
#define NUM_LEDS 30

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN D4
#define CLOCK_PIN 13

int JUMP = 1;
int SPEED = 30;
int BRIGHTNESS = 15;
int FADE = 220;

#include "wifi.h"
// Create file with the following
// *************************************************************************
// #define SECRET_SSID "";  /* Replace with your SSID */
// #define SECRET_PSK "";   /* Replace with your WPA2 passphrase */
// *************************************************************************
const char ssid[] = SECRET_SSID;         /* Replace with your SSID */
const char passphrase[] = SECRET_PSK;   /* Replace with your WPA2 passphrase */

ESPAsyncE131 e131(UNIVERSE_COUNT);
// Define the array of leds
CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette = PartyColors_p;
TBlendType    currentBlending = LINEARBLEND;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
CRGBPalette16 palettes[] = {RainbowColors_p, OceanColors_p, RainbowStripeColors_p, CloudColors_p, LavaColors_p, ForestColors_p, PartyColors_p};

void setup() {
  Serial.begin(115200);
  Serial.println("resetting");
  LEDS.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  LEDS.setBrightness(BRIGHTNESS);
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
}

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(FADE);
  }
}

void loop() {
  readDMX();
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    hue += JUMP;
    leds[i] = ColorFromPalette(currentPalette, hue, 255, currentBlending);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    readDMX();
    delay(SPEED);
  }
  Serial.print("x");

  // Now go in the other direction.
  for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
    // Set the i'th led to red
    hue += JUMP;
    leds[i] = ColorFromPalette(currentPalette, hue, 255, currentBlending);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    readDMX();
    delay(SPEED);
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

    /* Parse a packet and update pixels */
    BRIGHTNESS = getValue(packet, 1, 0, 255);
    JUMP = getValue(packet, 2, 1, 40);
    SPEED = getValue(packet, 3, 200, 0);
    FADE = getValue(packet, 4, 70, 255);
    currentPalette = palettes[getValue(packet, 5, 0, (ARRAY_SIZE(palettes) - 1))];

    FastLED.setBrightness(BRIGHTNESS);
  }
}

int getValue(e131_packet_t packet, int chan, int minV, int maxV) {
  return map(packet.property_values[(CHANNEL_START + (chan - 1))], 0, 255, minV, maxV);
}

