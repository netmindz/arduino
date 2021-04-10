/*
  Rings pattern, controlled over E1.31
*/
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncE131.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define UNIVERSE 1        // First DMX Universe to listen for
#define UNIVERSE_COUNT 1  // Total number of Universes to listen for, starting at UNIVERSE

#define CHANNEL_START 1 /* Channel to start listening at */
#define LED_PIN 2

#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define RINGS 9
#define NUM_LEDS 241

int JUMP = 15;
int SPEED = 100;
boolean INWARD = true;
int BRIGHTNESS = 15;

#include "wifi.h"
// Create file with the following
// *************************************************************************
// #define SECRET_SSID "";  /* Replace with your SSID */
// #define SECRET_PSK "";   /* Replace with your WPA2 passphrase */
// *************************************************************************
const char ssid[] = SECRET_SSID;         /* Replace with your SSID */
const char passphrase[] = SECRET_PSK;   /* Replace with your WPA2 passphrase */

ESPAsyncE131 e131(UNIVERSE_COUNT);
WiFiUDP fftUdp;

CRGB leds[NUM_LEDS];      //naming our LED array
int hue[RINGS];

CRGBPalette16 palettes[] = {RainbowColors_p, RainbowStripeColors_p, RainbowStripeColors_p, CloudColors_p, PartyColors_p };

CRGBPalette16 currentPalette = palettes[0];
TBlendType    currentBlending =  LINEARBLEND;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint16_t audioSyncPort = 20000;
bool newReading;

#include "audio.h"
#include "Spiralight.h"

typedef void (*Pattern)();
typedef Pattern PatternList[];
typedef struct {
  Pattern pattern;
  String name;
} PatternAndName;
typedef PatternAndName PatternAndNameList[];

void autoRun();
void rings();
void simpleRings();
void randomFlow();
void audioRings();

PatternAndNameList gPatterns = {
  { autoRun, "autoRun"}, // must be first
  { rings, "rings"},
  { simpleRings, "simpleRings" },
  { randomFlow, "randomFlow" },
  { audioRings, "audioRings"},
  { do_Static, "do_Static"},
  { do_Rainbow_Fade, "do_Rainbow_Fade"},
  { do_MC_Fade, "do_MC_Fade"},
  { do_Spiral_Rainbow_Wave_1, "do_Spiral_Rainbow_Wave_1"},
  { do_Spiral_Rainbow_Wave_2, "do_Spiral_Rainbow_Wave_2"},
  { do_Spiral_Rainbow_Wave_3, "do_Spiral_Rainbow_Wave_3"},
  { do_Spiral_MC_Wave_1, "do_Spiral_MC_Wave_1"},
  { do_Spiral_MC_Wave_2, "do_Spiral_MC_Wave_2"},
  { do_Spiral_MC_Wave_3, "do_Spiral_MC_Wave_3"},
  { do_Linear_Rainbow_Gradient_1, "do_Linear_Rainbow_Gradient_1"},
  { do_Linear_Rainbow_Gradient_2, "do_Linear_Rainbow_Gradient_2"},
  { do_Linear_Rainbow_Gradient_3, "do_Linear_Rainbow_Gradient_3"},
  { do_Linear_MC_Gradient_1, "do_Linear_MC_Gradient_1"},
  { do_Linear_MC_Gradient_2, "do_Linear_MC_Gradient_2"},
  { do_Linear_MC_Gradient_3, "do_Linear_MC_Gradient_3"},
  { do_Indiv_Jump_Rainbow, "do_Indiv_Jump_Rainbow"},
  { do_Indiv_Jump_MC, "do_Indiv_Jump_MC"},
  { do_All_Jump_Rainbow, "do_All_Jump_Rainbow"},
  { do_Strobe_Static, "do_Strobe_Static"},
  { do_Strobe_MC, "do_Strobe_MC"},
  { do_Strobe_Rainbow, "do_Strobe_Rainbow"},
  { do_Marquee_MC, "do_Marquee_MC"},
  { do_Marquee_Rainbow, "do_Marquee_Rainbow"},
  { do_Marquee_Static, "do_Marquee_Static"},
  { do_Segment_Rainbow, "do_Segment_Rainbow"},
  { do_Segment_MC, "do_Segment_MC"},
  { do_Segment_Static, "do_Segment_Static"},
  { do_Visor_MC, "do_Visor_MC"},
  { do_Visor_Rainbow, "do_Visor_Rainbow"}, 
  { do_Visor_Static, "do_Visor_Static"},
  { do_Bounce_Linear_MC, "do_Bounce_Linear_MC"},
  { do_Bounce_Spiral_Static, "do_Bounce_Spiral_Static"},
  { do_Bounce_Spiral_Rainbow, "do_Bounce_Spiral_Rainbow"},
  { do_Bounce_Spiral_MC, "do_Bounce_Spiral_MC"},
  { do_Bounce_Linear_Rainbow, "do_Bounce_Linear_Rainbow"},
  { do_Bounce_Linear_Static, "do_Bounce_Linear_Static"},
  { do_Ripple_Rainbow, "do_Ripple_Rainbow"},
  { do_Ripple_MC, "do_Ripple_MC"},
  { do_Ripple_Static, "do_Ripple_Static"},
  { do_Pulse_Rainbow, "do_Pulse_Rainbow"},
  { do_Pulse_MC, "do_Pulse_MC"},
  { do_Pulse_Static, "do_Pulse_Static"},
  { do_Rain_Rainbow, "do_Rain_Rainbow"},
  { do_Rain_MC, "do_Rain_MC"},
  { do_Rain_Static, "do_Rain_Static"},
  { do_Special_Xmas, "do_Special_Xmas"},
  { do_Special_Special, "do_Special_Special"},
  { do_Sparkle, "do_Sparkle"},
  { do_Shift_MC, "do_Shift_MC"},
  { do_Shift_Rainbow, "do_Shift_Rainbow"},

};

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
int gPatternCount = ARRAY_SIZE(gPatterns);
int pgm = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS); //.setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );

  leds[0] = CRGB::Blue;
  FastLED.show();
  
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
  int sanity = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("w");
    sanity++;
    if (sanity > 20) break;
  }
  Serial.println("\nDone");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  leds[0] = CRGB::Green;
  FastLED.show();
  // Choose one to begin listening for E1.31 data
  //if (e131.begin(E131_UNICAST)) {
  if (e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT)) {  // Listen via Multicast
    Serial.println(F("Listening for data..."));
  }
  else {
    Serial.println(F("*** e131.begin failed ***"));
  }

  int h = 0;
  for (int r = 0; r < RINGS; r++) {
    hue[r] = ColorFromPalette(currentPalette, h, 255, currentBlending);
    h += JUMP;
  }

  fftUdp.beginMulticast(WiFi.localIP(), IPAddress(239, 0, 0, 1), audioSyncPort);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
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
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

}

void loop() {
  ArduinoOTA.handle();
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
    BRIGHTNESS = getValue(packet, 1, 0, 255);
    JUMP = getValue(packet, 2, 5, 40);
    SPEED = getValue(packet, 3, 200, 0);
    if (packet.property_values[(CHANNEL_START  + 3)] < 125) {
      INWARD = true;
    }
    else {
      INWARD = false;
    }
    currentPalette = palettes[getValue(packet, 5,  0, (ARRAY_SIZE(palettes) - 1))]; // channel 6
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
  readAudioUDP();
  EVERY_N_SECONDS(10) {
    Serial.println(gPatterns[pgm].name);
  }
  gPatterns[pgm].pattern();
}

int autopgm = 1; // random(1, (gPatternCount - 1));
void autoRun() {
  EVERY_N_SECONDS(90) {
    autopgm = random(1, (gPatternCount - 1));
    // autopgm++;
    if (autopgm >= gPatternCount) autopgm = 1;
    Serial.print("Next Auto pattern: ");
    Serial.println(gPatterns[autopgm].name);
  }

  gPatterns[autopgm].pattern();

}

void rings() {
  for (int r = 0; r < RINGS; r++) {
    setRing(r, ColorFromPalette(currentPalette, hue[r], 255, currentBlending));
  }
  FastLED.delay(SPEED);
  if (INWARD) {
    for (int r = 0; r < RINGS; r++) {
      hue[(r - 1)] = hue[r]; // set ring one less to that of the outer
    }
    hue[(RINGS - 1)] += JUMP;
  }
  else {
    for (int r = (RINGS - 1); r >= 1; r--) {
      hue[r] = hue[(r - 1)]; // set this ruing based on the inner
    }
    hue[0] += JUMP;
  }
}

int j;
void simpleRings() {
  for (int r = 0; r < RINGS; r++) {
    setRing(r, ColorFromPalette(currentPalette, j + (r * JUMP), 255, currentBlending));
  }
  j += JUMP;
  FastLED.delay(SPEED);
}


void randomFlow() {
  hue[0] = random(0, 255);
  for (int r = 0; r < RINGS; r++) {
    setRing(r, CHSV(hue[r], 255, 255));
  }
  for (int r = (RINGS - 1); r >= 1; r--) {
    hue[r] = hue[(r - 1)]; // set this ruing based on the inner
  }
  FastLED.delay(SPEED);
}

void audioRings() {
  if(newReading) {
    newReading = false;
    for (int i = 0; i < 7; i++) {
      // visualize the average bass of both channels
      uint8_t val = fftResult[(i*2)];
  
      // Visualize leds to the beat
      CRGB color = ColorFromPalette(currentPalette, val, 255, currentBlending);
      color.nscale8_video(val);
      setRing(i, color);
    }
  
    // Update Leds
    FastLED.show();
  }
}

void setRing(int ring, CRGB colour) {
  int offset = 0;
  int count = 0;
  switch (ring) {
    case 0:
      count = 1;
      break;
    case 1:
      offset = 1;
      count = 8;
      break;
    case 2:
      offset = 9;
      count = 12;
      break;
    case 3:
      offset = 21;
      count = 16;
      break;
    case 4:
      offset = 37;
      count = 24;
      break;
    case 5:
      offset = 61;
      count = 32;
      break;
    case 6:
      offset = 93;
      count = 40;
      break;
    case 7:
      offset = 133;
      count = 48;
      break;
    case 8:
      offset = 181;
      count = 60;
      break;
  }
  for (int i = 0; i < count; i++) {
    leds[(offset + i)] = colour;
  }
}


int getValue(e131_packet_t packet, int chan, int minV, int maxV) {
  return map(packet.property_values[(CHANNEL_START + (chan - 1))], 0, 255, minV, maxV);
}
