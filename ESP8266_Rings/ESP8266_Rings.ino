/*
  Rings pattern, controlled over E1.31
*/
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncE131.h>

#define UNIVERSE 1        // First DMX Universe to listen for
#define UNIVERSE_COUNT 1  // Total number of Universes to listen for, starting at UNIVERSE

#define CHANNEL_START 1 /* Channel to start listening at */
#define LED_PIN 12

#define COLOR_ORDER GBR
#define CHIPSET     WS2811

#define RINGS 6
#define NUM_LEDS 100

int JUMP = 15;
int SPEED = 200;
boolean INWARD = true;
int BRIGHTNESS = 15;

const char ssid[] = "";         /* Replace with your SSID */
const char passphrase[] = "";   /* Replace with your WPA2 passphrase */

ESPAsyncE131 e131(UNIVERSE_COUNT);

CRGB leds[NUM_LEDS];      //naming our LED array
int hue[RINGS];



void setup() {
  Serial.begin(115200);
  delay(10);

  // Make sure you're in station mode
  WiFi.mode(WIFI_STA);

  Serial.println("");
  Serial.print(F("Connecting to "));
  Serial.print(ssid);

  if (passphrase != NULL)
    WiFi.begin(ssid, passphrase);
  else
    WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Choose one to begin listening for E1.31 data
  //if (e131.begin(E131_UNICAST)) {
  if (e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT)) {  // Listen via Multicast
    Serial.println(F("Listening for data..."));
  }
  else {
    Serial.println(F("*** e131.begin failed ***"));
  }
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS); //.setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );

  int h = 0;
  for (int r = 0; r < RINGS; r++) {
    hue[r] = h;
    h += JUMP;
  }
}

void loop() {
  if (!e131.isEmpty()) {
    e131_packet_t packet;
    e131.pull(&packet);     // Pull packet from ring buffer

//    Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH1: %u\n",
//                  htons(packet.universe),                 // The Universe for this packet
//                  htons(packet.property_value_count) - 1, // Start code is ignored, we're interested in dimmer data
//                  e131.stats.num_packets,                 // Packet counter
//                  e131.stats.packet_errors,               // Packet error counter
//                  packet.property_values[1]);             // Dimmer data for Channel 1

    /* Parse a packet and update pixels */
    BRIGHTNESS = map(packet.property_values[(CHANNEL_START  + 0)], 0, 255, 0, 255);
    JUMP = map(packet.property_values[(CHANNEL_START  + 1)], 0, 255, 5, 40);
    SPEED = map(packet.property_values[(CHANNEL_START  + 2)], 0, 255, 1000, 20);
    if (packet.property_values[(CHANNEL_START  + 3)] < 125) {
      INWARD = true;
    }
    else {
      INWARD = false;
    }
    FastLED.setBrightness(BRIGHTNESS);
  }
  EVERY_N_SECONDS( 2 ) {
    Serial.print("BRIGHTNESS = ");
    Serial.print(BRIGHTNESS);

    Serial.print("  JUMP = ");
    Serial.print(JUMP);

    Serial.print("  SPEED = ");
    Serial.print(SPEED);

    Serial.print("  INWARD = ");
    Serial.print(INWARD);

    Serial.println();
  }

  rings();
}

void rings() {
  for (int r = 2; r <= RINGS; r++) {
    setRing(r, CHSV(hue[(r - 1)], 255, 255));
  }
  FastLED.delay(SPEED);
  if (INWARD) {
    for (int r = 2; r < RINGS; r++) {
      hue[(r - 1)] = hue[r];
    }
    hue[(RINGS - 1)] += JUMP;
  }
  else {
    for (int r = (RINGS - 1); r > 1; r--) {
      hue[r] = hue[(r - 1)];
    }
    //    if(random(0,10) > 2) {
    hue[1] += JUMP;
    //    }
    //    else {
    //      hue[1] += random(0,255);
    //    }
  }
}

void setRing(int ring, CRGB colour) {
  int offset = 1;
  int count = 0;
  switch (ring) {
    case 1:
      count = 0;
      break;
    case 2:
      offset = 1;
      count = 8;
      break;
    case 3:
      offset = 9;
      count = 12;
      break;
    case 4:
      offset = 21;
      count = 16;
      break;
    case 5:
      offset = 37;
      count = 24;
      break;
    case 6:
      offset = 61;
      count = 32;
      break;
  }
  for (int i = 0; i < count; i++) {
    leds[(offset + i - 1)] = colour;
  }
}

