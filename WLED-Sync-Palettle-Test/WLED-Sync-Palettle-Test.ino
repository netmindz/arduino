/*
   Read audio data from MSGEQ7 chip and send as UDP data in the same format as used in https://github.com/atuline/WLED
   This allows for sending FFT data from either ESP8266 or ESP32 to other devices running the audio responsive WLED
*/

#if defined(ESP8266) // ESP8266
#include <ESP8266WiFi.h>
#else // ESP32
#include <WiFi.h>
#endif
#include <WLED-sync.h> // https://github.com/netmindz/WLED-sync
#include "FastLED.h"

CRGBPalette256 currentPalette;
CRGB rgb;
#define LED_TYPE WS2812B
#define DATA_PIN 16
#define NUM_LEDS 20
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define BRIGHTNESS  125

#include "wifi.h"
// Create file with the following
// *************************************************************************
// #define SECRET_SSID "";  /* Replace with your SSID */
// #define SECRET_PSK "";   /* Replace with your WPA2 passphrase */
// *************************************************************************
const char ssid[] = SECRET_SSID;
const char passphrase[] = SECRET_PSK;

WLEDSync sync;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passphrase);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("\nConnected! IP address: ");
  Serial.println(WiFi.localIP());
  
  sync.begin();

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.println("Setup complete");
}

void loop() {

  if (sync.read()) {
    for (int b = 0; b < NUM_GEQ_CHANNELS; b++) {
      uint8_t val = sync.fftResult[b];
      Serial.printf("%u ", val);
    }
    Serial.println();
    newGradientPalette();
    FastLED.show();
  }

}

//---------------------------------------------------------------
void newGradientPalette(){
  uint8_t xyz[12];  // Needs to be 4 times however many colors are being used.
                    // 3 colors = 12, 4 colors = 16, etc.

  rgb = getCRGBForBand(0);
  
  xyz[0] = 0;  // anchor of first color - must be zero
  xyz[1] = rgb.r;
  xyz[2] = rgb.g;
  xyz[3] = rgb.b;

  rgb = getCRGBForBand(4);
  xyz[4] = 128;
  xyz[5] = rgb.r;
  xyz[6] = rgb.g;
  xyz[7] = rgb.b;
  
  rgb = getCRGBForBand(8);
  xyz[8] = 255;  // anchor of last color - must be 255
  xyz[9] = rgb.r;
  xyz[10] = rgb.g;
  xyz[11] = rgb.b;

  currentPalette = xyz;

  for (int p = 0; p < NUM_LEDS; p++) {
    leds[p] = ColorFromPalette(currentPalette, p*255/(NUM_LEDS-1), BRIGHTNESS, NOBLEND);
  }

}


// CRGB getCRGBForBand(int b) {
//   CRGB value;
//   CHSV hsv = CHSV(sync.fftResult[b], 255, map(sync.fftResult[b], 0, 255, 50, 255));  // pick hue
//   hsv2rgb_rainbow(hsv, value);  // convert to R,G,B
//   return value;
// }

CRGB getCRGBForBand(int x) {
  CRGB value;
  CHSV hsv;
  if(x == 0) {
    value = CRGB(sync.fftResult[10]/2, sync.fftResult[4]/2, sync.fftResult[0]/2);
  }
  else if(x == 255) {
    value = CRGB(sync.fftResult[10]/2, sync.fftResult[0]/2, sync.fftResult[4]/2);
  } 
  else {
    value = CRGB(sync.fftResult[0]/2, sync.fftResult[4]/2, sync.fftResult[10]/2);
  } 
  return value;
}
