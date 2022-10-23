/*
  Rings pattern, controlled over E1.31
*/
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <ESPAsyncE131.h>
#include <WLED-sync.h>
#include <ArduinoOTA.h>

#define UNIVERSE 1        // First DMX Universe to listen for
#define UNIVERSE_COUNT 1  // Total number of Universes to listen for, starting at UNIVERSE

#define CHANNEL_START 1 /* Channel to start listening at */

#ifdef ESP32
#define LED_PIN 26
#else
#define LED_PIN 2
#endif

#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define RINGS 9
#define NUM_LEDS 241

int JUMP = 15;
int SPEED = 100;
boolean INWARD = true;
int BRIGHTNESS = 10;
int autoPalette = 0;

#include "wifi.h"
// Create file with the following
// *************************************************************************
// #define SECRET_SSID "";  /* Replace with your SSID */
// #define SECRET_PSK "";   /* Replace with your WPA2 passphrase */
// *************************************************************************
const char ssid[] = SECRET_SSID;         /* Replace with your SSID */
const char passphrase[] = SECRET_PSK;   /* Replace with your WPA2 passphrase */

ESPAsyncE131 e131(UNIVERSE_COUNT);
WLEDSync sync;

CRGB leds[NUM_LEDS];  // main data
// Crossfade based off code by Preyy - https://www.reddit.com/r/FastLED/comments/fa0p0i/i_made_a_more_versatile_crossfader_video_code/
CRGB leds2[NUM_LEDS]; // temp store used during crossfade

int hue[RINGS];

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

CRGBPalette16 currentPalette = RainbowColors_p;

typedef void (*SimplePatternList[])();
void  autoRun();
void rings();
void audioRings();
void simpleRings();
void randomFlow();

bool newReading;

#include "audio.h"
#include "Spiralight.h"
#include "F_lying_circular.h"
#include "Disk.h"
#include "Clock.h"

CRGBPalette16 palettes[] = {RainbowColors_p, RainbowStripeColors_p, RainbowStripeColors_p, CloudColors_p, PartyColors_p, redblue_gp, redblue1_gp };
int gPaletteCount = ARRAY_SIZE(palettes);

TBlendType    currentBlending =  LINEARBLEND;

#define DEFAULT_DURATION 20

typedef void (*Pattern)();
typedef Pattern PatternList[];
typedef struct {
  Pattern pattern;
  String name;
  int duration;
} PatternAndName;
typedef PatternAndName PatternAndNameList[];

void autoRun();
void ringsx();
void simpleRings();
void randomFlow();
void audioRings();

PatternAndNameList gPatterns = {
  { autoRun, "autoRun",DEFAULT_DURATION}, // must be first
  { halfCircleRainbow360, "halfCircleRainbow360",DEFAULT_DURATION},
  { halfCircleRainbow256, "halfCircleRainbow256",DEFAULT_DURATION},
//  { rimClock, "rimClock" },
//  { arcClock, "arcClock" },
  { audioRings, "audioRings", 300},
  { F_lying_circular, "Flying Circular", DEFAULT_DURATION},
  { ringsx, "ringsx",DEFAULT_DURATION},
  { simpleRings, "simpleRings", 300 },
  { randomFlow, "randomFlow", 300 },
  { do_Spiral_Rainbow_Wave_1, "Spiral_Rainbow_Wave_1",DEFAULT_DURATION},
  { do_Spiral_Rainbow_Wave_2, "Spiral_Rainbow_Wave_2",DEFAULT_DURATION},
//  { do_Spiral_Rainbow_Wave_3, " _3",DEFAULT_DURATION},
  { do_Spiral_MC_Wave_1, "Spiral_MC_Wave_1",DEFAULT_DURATION},
  { do_Spiral_MC_Wave_2, "Spiral_MC_Wave_2",DEFAULT_DURATION},
//  { do_Spiral_MC_Wave_3, "Spiral_MC_Wave_3",DEFAULT_DURATION},
  { do_Linear_Rainbow_Gradient_1, "Linear_Rainbow_Gradient_1",DEFAULT_DURATION},
//  { do_Linear_Rainbow_Gradient_2, "Linear_Rainbow_Gradient_2",DEFAULT_DURATION},
  { do_Linear_Rainbow_Gradient_3, "Linear_Rainbow_Gradient_3",DEFAULT_DURATION},
  { do_Linear_MC_Gradient_1, "Linear_MC_Gradient_1",DEFAULT_DURATION},
//  { do_Linear_MC_Gradient_2, "Linear_MC_Gradient_2",DEFAULT_DURATION},
  { do_Linear_MC_Gradient_3, "Linear_MC_Gradient_3",DEFAULT_DURATION},
//  { do_Indiv_Jump_Rainbow, "Indiv_Jump_Rainbow",DEFAULT_DURATION},
//  { do_Indiv_Jump_MC, "Indiv_Jump_MC",DEFAULT_DURATION},
//  { do_All_Jump_Rainbow, "All_Jump_Rainbow",DEFAULT_DURATION},
//  { do_Strobe_Static, "Strobe_Static",DEFAULT_DURATION},
//  { do_Strobe_MC, "Strobe_MC",DEFAULT_DURATION},
//  { do_Strobe_Rainbow, "Strobe_Rainbow",DEFAULT_DURATION},
//  { do_Marquee_MC, "Marquee_MC",DEFAULT_DURATION},
//  { do_Marquee_Rainbow, "Marquee_Rainbow",DEFAULT_DURATION},
//  { do_Marquee_Static, "Marquee_Static",DEFAULT_DURATION},
  { do_Segment_Rainbow, "Segment_Rainbow",DEFAULT_DURATION},
  { do_Segment_MC, "Segment_MC",DEFAULT_DURATION},
//  { do_Segment_Static, "Segment_Static",DEFAULT_DURATION},
  { do_Visor_MC, "Visor_MC",DEFAULT_DURATION},
  { do_Visor_Rainbow, "Visor_Rainbow",DEFAULT_DURATION}, 
//  { do_Visor_Static, "Visor_Static",DEFAULT_DURATION},
  { do_Bounce_Linear_MC, "Bounce_Linear_MC",DEFAULT_DURATION},
//  { do_Bounce_Spiral_Static, "Bounce_Spiral_Static",DEFAULT_DURATION},
  { do_Bounce_Spiral_Rainbow, "Bounce_Spiral_Rainbow",DEFAULT_DURATION},
  { do_Bounce_Spiral_MC, "Bounce_Spiral_MC",DEFAULT_DURATION},
  { do_Bounce_Linear_MC, "Bounce_Linear_MC",DEFAULT_DURATION},
  { do_Ripple_Rainbow, "Ripple_Rainbow",DEFAULT_DURATION},
  { do_Ripple_MC, "Ripple_MC",DEFAULT_DURATION},
//  { do_Ripple_Static, "Ripple_Static",DEFAULT_DURATION},
//  { do_Pulse_Rainbow, "Pulse_Rainbow",DEFAULT_DURATION}, bit dull
//  { do_Pulse_MC, "Pulse_MC",DEFAULT_DURATION},
//  { do_Pulse_Static, "Pulse_Static",DEFAULT_DURATION},
  { do_Rain_Rainbow, "Rain_Rainbow",DEFAULT_DURATION},
  { do_Rain_MC, "Rain_MC",DEFAULT_DURATION},
//  { do_Rain_Static, "Rain_Static",DEFAULT_DURATION},
//  { do_Special_Xmas, "Special_Xmas",DEFAULT_DURATION},
//  { do_Special_Special, "Special_Special",DEFAULT_DURATION},
//  { do_Sparkle, "Sparkle",DEFAULT_DURATION},
  { do_Shift_MC, "Shift_MC",DEFAULT_DURATION},
  { do_Shift_Rainbow, "Shift_Rainbow",DEFAULT_DURATION},

};

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
int gPatternCount = ARRAY_SIZE(gPatterns);
int gPalletteCount = ARRAY_SIZE(palettes);
int pgm = 0;
int autopgm = 2; // random(1, (gPatternCount - 1));


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

  timeClient.begin();
  
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

  Serial.printf("There are %u patterns\n", gPatternCount);

  sync.begin();

  setupRings();

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

    EVERY_N_SECONDS( 10 ) { 
      Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH1: %u\n",
                  htons(packet.universe),                 // The Universe for this packet
                  htons(packet.property_value_count) - 1, // Start code is ignored, we're interested in dimmer data
                  e131.stats.num_packets,                 // Packet counter
                  e131.stats.packet_errors,               // Packet error counter
                  packet.property_values[1]);             // Dimmer data for Channel 1
    }

    /* Parse a packet and update pixels */
    BRIGHTNESS = getValue(packet, 1, 0, 255);
    pgm = getValue(packet, 2, 0, (gPatternCount - 1));
    JUMP = getValue(packet, 3, 5, 40);
    SPEED = getValue(packet, 4, 200, 0);
    if (packet.property_values[(CHANNEL_START  + 4)] < 125) {
      INWARD = true;
    }
    else {
      INWARD = false;
    }
    currentPalette = palettes[getValue(packet, 6,  0, (ARRAY_SIZE(palettes) - 1))]; // channel 6
    FastLED.setBrightness(BRIGHTNESS);
  }
  EVERY_N_SECONDS( 10 ) {
    Serial.printf("PG=%u BRIGHTNESS = %u   JUMP = %u   SPEED = %u", pgm, BRIGHTNESS, JUMP, SPEED);

    Serial.print("  INWARD = ");
    Serial.print(INWARD);

    Serial.println();
  }
  readAudioUDP();
  EVERY_N_SECONDS(10) {
    Serial.print("Pattern: ");
    if(pgm != 0) {
      Serial.println(gPatterns[pgm].name);
    }
    else {
      Serial.println(gPatterns[autopgm].name);
    }
  }
  gPatterns[pgm].pattern();
  // Update Leds
  FastLED.show();

  if(WiFi.status() == WL_CONNECTED) {
    timeClient.update();
  }
}

uint16_t crossct = 255;
int gOldPatternNumber;
void autoRun() {

  if (crossct >= 255) {
    // Run pattern as normal
    gPatterns[autopgm].pattern();

    EVERY_N_SECONDS_I(autoPattern, gPatterns[autopgm].duration) { // 90
      gOldPatternNumber = autopgm;
      autopgm = random(1, (gPatternCount - 1));
      //autopgm++;
      if (autopgm >= gPatternCount) autopgm = 1;
      Serial.print("Next Auto pattern: ");
      Serial.println(gPatterns[autopgm].name);
      crossct = 0;  // reset the blend amount
      FastLED.clear();
    }

  }
  else  {
    EVERY_N_MILLIS(100) {
      crossct += 10;         // higher increase faster xfade
      if (crossct > 255) { // overflow prevention
        crossct = 255;
      }
    }
    uint8_t blendamt = crossct;

    // Run the old pattern and save to array
    gPatterns[gOldPatternNumber].pattern();

    for (int i = 0; i < NUM_LEDS; i++) {
      leds2[i] = leds[i]; // Think there is a built in function for copy, but can't see in docs
    }

    gPatterns[autopgm].pattern();   // Run the new pattern and save to array

    for (int i = 0; i < NUM_LEDS; i++) {   // blend oldpattern into main output
      leds[i] = blend(leds2[i], leds[i], blendamt);   // Blend arrays of LEDs, third value is blend %
    }
  }

  EVERY_N_SECONDS(160) {
    autoPalette = random(0, (gPalletteCount - 1));
    //  autoPalette++;
    if (autoPalette >= gPalletteCount) autoPalette = 0;
    Serial.println("Next Auto pallette");
    currentPalette = palettes[autoPalette];
  }
  
}

void ringsx() {
  for (int r = 0; r < RINGS; r++) {
    setRing(r, ColorFromPalette(currentPalette, hue[r], 255, currentBlending));
  }
  delay(SPEED);
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
  delay(SPEED);
}


void randomFlow() {
  hue[0] = random(0, 255);
  for (int r = 0; r < RINGS; r++) {
    setRing(r, CHSV(hue[r], 255, 255));
  }
  for (int r = (RINGS - 1); r >= 1; r--) {
    hue[r] = hue[(r - 1)]; // set this ruing based on the inner
  }
  delay(SPEED);
}

void audioRings() {
  if(!audioRec && pgm == 0 && millis() > 5000) {
    Serial.println("Skip audioRings as no data");
//    autopgm++;
    autopgm = random(1, (gPatternCount - 1));
  }
  if(newReading) {
    newReading = false;
    for (int i = 0; i < 7; i++) {

      uint8_t val;
      if(INWARD) {
        val = fftResult[(i*2)];
      }
      else {
        int b = 14 -(i*2);
        val = fftResult[b];
      }
  
      // Visualize leds to the beat
      CRGB color = ColorFromPalette(currentPalette, val, val, currentBlending);
//      CRGB color = ColorFromPalette(currentPalette, val, 255, currentBlending);
//      color.nscale8_video(val);
      setRing(i, color);
//        setRingFromFtt((i * 2), i); 
    }

    setRingFromFtt(2, 7); // set outer ring to base
    setRingFromFtt(0, 8); // set outer ring to base

  }
}

void setRingFromFtt(int index, int ring) {
  uint8_t val = fftResult[index];
  // Visualize leds to the beat
  CRGB color = ColorFromPalette(currentPalette, val, 255, currentBlending);
  color.nscale8_video(val);
  setRing(ring, color);
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
