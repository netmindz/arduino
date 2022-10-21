#pragma message ESP platform - all in one

#define MSGEQ7_OUT_MAX 255
#define MSGEQ7_BASS 0
#define MSGEQ7_LOW 0
#define MSGEQ7_MID 3
#define MSGEQ7_HIGH 5

#if defined(ESP8266) // ESP8266
#include <ESP8266WiFi.h>

#define LED_PIN D4
#define CLOCK_PIN D3

//  // MSGEQ7 wiring on spectrum analyser shield
//  #define MSGEQ7_STROBE_PIN D6
//  #define MSGEQ7_RESET_PIN  D1
//  #define AUDIO_LEFT_PIN    A0
//  #define AUDIO_RIGHT_PIN   A0

#else // ESP32
#include <WiFi.h>

#define LED_PIN 2 // Huzzah! ESP 32 values
#define CLOCK_PIN 4

//  // MSGEQ7 wiring on spectrum analyser shield
//  #define MSGEQ7_STROBE_PIN 14
//  #define MSGEQ7_RESET_PIN  27
//  #define AUDIO_LEFT_PIN    A0 // pin 26
//  #define AUDIO_RIGHT_PIN   A1 // pin 25

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

WiFiUDP fftUdp;
boolean udpSyncConnected;
uint16_t audioSyncPort = 20000;

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
    if (sanity > 10) break;
  }
  Serial.println(F("Connected to wifi "));
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  delay(2000);
  
  pgm = 0; // 0 = autoRun

  setupOTA();

  udpSyncConnected = true; // TODO - sometimes the wifi starts but ip is still 0.0.0.0
  Serial.println("beginMulticast");
  fftUdp.beginMulticast(IPAddress(239, 0, 0, 1), audioSyncPort);

  Serial.println("End of ESP control setup");
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

#include "audio.h"

// FAKE MSGEQ for now
bool newReading;
bool MSGEQ7read() {
  return newReading;
}

int MSGEQ7get(int band) {
//  ReadAudio();
  return map(left[band], 0, 1023, 0, MSGEQ7_OUT_MAX);
}

int MSGEQ7get(int band, int channel) {
  return MSGEQ7get(band);
}

int MSGEQ16get(int band) {
//  ReadAudio();
  return map(fftResult[band], 0, 255, 0, MSGEQ7_OUT_MAX);
}

int MSGEQ16get(int band, int channel) {
  return MSGEQ16get(band);
}


// Read the UDP audio data sent by WLED-Audio
void readAudioUDP() {

  // Begin UDP Microphone Sync

  // Only run the audio listener code if we're in Receive mode
  if (millis() - lastTime > delayMs) {
    if (udpSyncConnected) {
      //      Serial.println("Checking for UDP Microphone Packet");
      int packetSize = fftUdp.parsePacket();
      if (packetSize) {
        //        Serial.println("Received UDP Sync Packet");
        uint8_t fftBuff[packetSize];
        fftUdp.read(fftBuff, packetSize);
        audioSyncPacket receivedPacket;
        memcpy(&receivedPacket, fftBuff, packetSize);

        // VERIFY THAT THIS IS A COMPATIBLE PACKET
        char packetHeader[6];
        memcpy(&receivedPacket, packetHeader, 6);
        if (!(isValidUdpSyncVersion(packetHeader))) {
          memcpy(&receivedPacket, fftBuff, packetSize);
          
          for (int i = 0; i < 16; i++) {
            fftResult[i] = receivedPacket.fftResult[i];
          }

          FFT_Magnitude = receivedPacket.FFT_Magnitude;
          FFT_MajorPeak = receivedPacket.FFT_MajorPeak;
          // Serial.println("Finished parsing UDP Sync Packet");

          // "Legacy" - for MSGEQ7 patterns
          for (int b = 0; b < 7; b++) {
            left[b] = map(fftResult[(b * 2)], 0, 255, 0, 1023);
            right[b] = map(fftResult[(b * 2)], 0, 255, 0, 1023);
          }
        }
      }
    }
  }

}

int mapNoise(int v) {
  return v;
}


void controlLoop() {
  readDMX();
  ArduinoOTA.handle();
  readAudioUDP();
  EVERY_N_MILLISECONDS(30) { // Possibly swap to when we actually get new UDP packet?
    if(newReading) {
      newReading = false;  
    }
    else {
      newReading = true;
    }
  }
}
