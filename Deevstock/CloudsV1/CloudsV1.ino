#define FASTLED_ALLOW_INTERRUPTS 0

// CLOUDS !!!

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

#include <ESPAsyncE131.h>

#define UNIVERSE 1        // First DMX Universe to listen for
#define UNIVERSE_COUNT 1  // Total number of Universes to listen for, starting at UNIVERSE

#define CHANNEL_START 1 /* Channel to start listening at */

ESPAsyncE131 e131(UNIVERSE_COUNT);

#include <FastLED.h>

#define LED_PIN  D4

#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define NUM_LEDS 200

CRGB leds[NUM_LEDS];      //naming our LED array

#include "wifi.h"
// Create file with the following
// *************************************************************************
// #define SECRET_SSID "";  /* Replace with your SSID */
// #define SECRET_PSK "";   /* Replace with your WPA2 passphrase */
// *************************************************************************
const char ssid[] = SECRET_SSID;
const char passphrase[] = SECRET_PSK;

int BRIGHTNESS = 255;
int SPEED = 1;

void setup() {
  Serial.begin(115200);
  
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setDither(0);
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

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  // Choose one to begin listening for E1.31 data
  //if (e131.begin(E131_UNICAST)) {
  if (e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT)) {  // Listen via Multicast
    Serial.println(F("Listening for data..."));
  }
  else {
    Serial.println(F("*** e131.begin failed ***"));
  }

}


int pmode = 0;
void loop() {
  ArduinoOTA.handle();
  if (!e131.isEmpty()) {
    e131_packet_t packet;
    e131.pull(&packet);     // Pull packet from ring buffer
EVERY_N_SECONDS(5) {
    Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH1: %u CH3: %u\n",
                  htons(packet.universe),                 // The Universe for this packet
                  htons(packet.property_value_count) - 1, // Start code is ignored, we're interested in dimmer data
                  e131.stats.num_packets,                 // Packet counter
                  e131.stats.packet_errors,               // Packet error counter
                  packet.property_values[1],             // Dimmer data for Channel 1
                  packet.property_values[3]);             // Dimmer data for Channel 3
}

    /* Parse a packet and update pixels */
    BRIGHTNESS = map(packet.property_values[(CHANNEL_START  + 0)], 0, 255, 0, 255);
    FastLED.setBrightness( BRIGHTNESS );
    // JUMP = map(packet.property_values[(CHANNEL_START  + 1)], 0, 255, 5, 40);
    SPEED = map(packet.property_values[(CHANNEL_START  + 2)], 0, 255, 200, 0);
    if (packet.property_values[(CHANNEL_START  + 3)] > 125) {
      pmode = 1;
    }
    else {
      pmode = 0;
    }
  }

  if (pmode == 0) {
    chase();
  }
  else {
    snake();
  }

}
int p = 0;
int hue = 0;
void chase() {
  // leds[p]  = CRGB::White;
  leds[p]  = CHSV( hue, 55, 255);
  FastLED.delay(SPEED);
  hue += random(0, 20);
  if (hue > 255) hue = 0;
  p++;
  if (p > ( NUM_LEDS - 1)) p = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(1);
    //leds[i].fadeLightBy(10);
  }
}
class Snake {
    int p = 0;
    int hue = random(0, 255);
    int v = random(50, 255);

  public:
    Snake() {

    }

    void snake() {
      leds[p]  = CHSV(hue, 255, v);
      p++;
      if (p > ( NUM_LEDS - 1)) {
        p = 0;
        hue = random(0, 255);
      }
    }
};

#define NUM_SNAKES 60
Snake snakes[NUM_SNAKES];

int snakesStarted = 0;
int const snakeDelay = 20;
int snakeGap = 0;

void snake() {

  if (snakesStarted < NUM_SNAKES) {
    if (snakeGap == snakeDelay)  {
      snakesStarted++;
      snakeGap = 0;
    }
    else {
      snakeGap++;
    }
  }

  for (int  i = 0; i < snakesStarted; i++) {
    snakes[i].snake();
  }
  fadeToBlackBy(leds, NUM_LEDS, 50);
  //      fadeLightBy(leds, NUM_LEDS, 50);
  //  FastLED.show();
  FastLED.delay(SPEED);
}


