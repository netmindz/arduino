#if defined(ESP8266)
#include <ESP8266WiFi.h>

#define LED_PIN D4
#define CLOCK_PIN D3

// MSGEQ7 wiring on spectrum analyser shield
#define MSGEQ7_STROBE_PIN D6
#define MSGEQ7_RESET_PIN  D1
#define AUDIO_LEFT_PIN    A0
#define AUDIO_RIGHT_PIN   A0

#else
#include <WiFi.h>

#define LED_PIN 27
#define CLOCK_PIN 33

// MSGEQ7 wiring on spectrum analyser shield
#define MSGEQ7_STROBE_PIN 32
#define MSGEQ7_RESET_PIN  15
#define AUDIO_LEFT_PIN    A0
#define AUDIO_RIGHT_PIN   A1

#endif

#include <ESPAsyncE131.h>

#include "ota.h"

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


void controlSetup() {
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
    if(sanity > 10) break;
  }
  Serial.println(F("Connected to wifi "));
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  setupOTA();

  pgm = 0;

}


int getValue(e131_packet_t packet, int chan, int minV, int maxV) {
  return map(packet.property_values[(CHANNEL_START + (chan - 1))], 0, 255, minV, maxV);
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

    int gPatternCount = 32; // FIXME

    BRIGHTNESS = getValue(packet, 1, 0, 255);
    FastLED.setBrightness(BRIGHTNESS);

    pgm = getValue(packet, 2, 0, (gPatternCount - 1)); // FIXME // pattern = 2
    SPEED = getValue(packet, 3, 0, 255); // speed = 3
    FADE = getValue(packet, 4, 0, 255);  // fade = 4 

  }
}


void controlLoop() {
  readDMX();
  ArduinoOTA.handle();
}
