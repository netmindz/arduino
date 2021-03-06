#define FASTLED_ALLOW_INTERRUPTS 0

#include <ESP8266WiFi.h>
#include <ESPAsyncE131.h>

#define UNIVERSE 1        // First DMX Universe to listen for
#define UNIVERSE_COUNT 1  // Total number of Universes to listen for, starting at UNIVERSE

#define CHANNEL_START 1 /* Channel to start listening at */

ESPAsyncE131 e131(UNIVERSE_COUNT);

#include <FastLED.h>

#define LED_PIN  4

#define COLOR_ORDER RGB
#define CHIPSET     WS2811

#define NUM_LEDS 100

CRGB leds[NUM_LEDS];      //naming our LED array

const char ssid[] = "";         /* Replace with your SSID */
const char passphrase[] = "";   /* Replace with your WPA2 passphrase */

int BRIGHTNESS = 15;
int SPEED = 200;

void setup() {
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
  if (!e131.isEmpty()) {
    e131_packet_t packet;
    e131.pull(&packet);     // Pull packet from ring buffer

    Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH1: %u\n",
                  htons(packet.universe),                 // The Universe for this packet
                  htons(packet.property_value_count) - 1, // Start code is ignored, we're interested in dimmer data
                  e131.stats.num_packets,                 // Packet counter
                  e131.stats.packet_errors,               // Packet error counter
                  packet.property_values[1]);             // Dimmer data for Channel 1

    /* Parse a packet and update pixels */
    BRIGHTNESS = map(packet.property_values[(CHANNEL_START  + 0)], 0, 255, 0, 255);
    FastLED.setBrightness( BRIGHTNESS );
    //    JUMP = map(packet.property_values[(CHANNEL_START  + 1)], 0, 255, 5, 40);
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
  leds[p]  = CHSV( hue, 255, 255);
  FastLED.delay(SPEED);
  hue += random(0, 20);
  if (hue > 255) hue = 0;
  p++;
  if (p > ( NUM_LEDS - 1)) p = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    //        leds[i].fadeToBlackBy(10);
    leds[i].fadeLightBy(10);
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


