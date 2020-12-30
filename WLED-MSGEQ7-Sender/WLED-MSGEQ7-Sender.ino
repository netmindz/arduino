/*
   Read audio data from MSGEQ7 chip and send as UDP data in the same format as used in https://github.com/atuline/WLED
   This allows for sending FFT data from either ESP8266 or ESP32 to other devices running the audio responsive WLED
*/

#if defined(ESP8266) // ESP8266
#include <ESP8266WiFi.h>
#else // ESP32
#include <WiFi.h>
#endif
#include <WiFiUdp.h>

#include "wifi.h"
// Create file with the following
// *************************************************************************
// #define SECRET_SSID "";  /* Replace with your SSID */
// #define SECRET_PSK "";   /* Replace with your WPA2 passphrase */
// *************************************************************************
const char ssid[] = SECRET_SSID;
const char passphrase[] = SECRET_PSK;


// MSGEQ7
#include "MSGEQ7.h"
#define pinAnalogLeft A0
#define pinAnalogRight A0
#define pinReset 6
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH false

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

WiFiUDP fftUdp;

#define UDP_SYNC_HEADER "00001"
uint16_t audioSyncPort = 20000;

struct audioSyncPacket {
  char header[6] = UDP_SYNC_HEADER;
  uint8_t myVals[32];     //  32 Bytes
  int sampleAgc;          //  04 Bytes
  int sample;             //  04 Bytes
  float sampleAvg;        //  04 Bytes
  bool samplePeak;        //  01 Bytes
  uint8_t fftResult[16];  //  16 Bytes
  double FFT_Magnitude;   //  08 Bytes
  double FFT_MajorPeak;   //  08 Bytes
};

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passphrase);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

#ifndef ESP8266
  fftUdp.beginMulticast(IPAddress(239, 0, 0, 1), audioSyncPort);
#else
  fftUdp.beginMulticast(WiFi.localIP(), IPAddress(239, 0, 0, 1), audioSyncPort);
#endif
  // This will set the IC ready for reading
  MSGEQ7.begin();
}

void loop() {
  // Analyze without delay every interval
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);
  if (newReading) {
    audioSyncPacket transmitData;

    for(int b=0; b < 16; b = b + 2) {
      transmitData.fftResult[b] = MSGEQ7.get((b/2));
      transmitData.fftResult[(b+1)] = MSGEQ7.get((b/2));
    }


    fftUdp.beginMulticastPacket();
    fftUdp.write(reinterpret_cast<uint8_t *>(&transmitData), sizeof(transmitData));
    fftUdp.endPacket();
  }

}
