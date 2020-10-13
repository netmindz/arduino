#include <FastLED.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <SoftwareSerial.h>
#include <ESP8266mDNS.h>

// Must copy and paste from main game
#define MAX_SNAKES 4
CRGB playerColors[MAX_SNAKES] = {CRGB::Blue, CRGB::DarkMagenta, CRGB::Yellow, CRGB::OrangeRed };


#include "wifi.h"
// Create file with the following
// *************************************************************************
// #define SECRET_SSID "";
// #define SECRET_PSK "";
// *************************************************************************
const char ssid[] = SECRET_SSID;
const char passphrase[] = SECRET_PSK;

WebSocketsServer webSocket = WebSocketsServer(81);

#define SERIAL_DEBUG Serial

SoftwareSerial swSer(D3, D4); //, false);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      SERIAL_DEBUG.printf("[%u] Disconnected!\n", num);
      swSer.printf("%u:e\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        SERIAL_DEBUG.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        if (num > (MAX_SNAKES - 1)) {
          Serial.println("MAX SNAKES!!");
          webSocket.sendTXT(num, "Sorry, player count exceeded");
          //webSocket.disconnect(num);
          return;
        }
        CRGB color = playerColors[num];
        char rgbTxt[8];
        sprintf(rgbTxt, "#%02X%02X%02X", color.r, color.g, color.b);
        String msg = "Connected player = " + (String) (num + 1) + " <span style=\"background: " + rgbTxt +  "\">&nbsp;&nbsp;</span>";
        Serial.println(msg);
        webSocket.sendTXT(num, msg);

        swSer.printf("%u:s\n", num);

        // send message to client
        //        webSocket.sendTXT(num, "Connected as " + (String) num);
      }
      break;
    case WStype_TEXT:
      SERIAL_DEBUG.printf("[%u] get Text: %s\n", num, payload);
      swSer.printf("%u:%s\n", num, payload);

      // send message to client
//      webSocket.sendTXT(num, "Sent command");
      break;
    case WStype_BIN:
      SERIAL_DEBUG.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);
      break;
  }

}

void setup() {
  // SERIAL_DEBUG.begin(921600);
  SERIAL_DEBUG.begin(115200);
  swSer.begin(115200);

  //Serial.setDebugOutput(true);
  SERIAL_DEBUG.setDebugOutput(true);

  SERIAL_DEBUG.println();
  SERIAL_DEBUG.println();
  SERIAL_DEBUG.println();

  for (uint8_t t = 4; t > 0; t--) {
    SERIAL_DEBUG.printf("[SETUP] BOOT WAIT %d...\n", t);
    SERIAL_DEBUG.flush();
    delay(1000);
  }

  // Make sure you're in station mode
  WiFi.mode(WIFI_STA);

  Serial.println("");
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

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
    if (sanity > 50) {
      Serial.println("ERROR: failed to connect to wifi");
      break;
    }
  }
  Serial.println("\nDone");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("snake")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
  if(swSer.available() > 0) {
    String input = Serial1.readStringUntil('\n');
    int index = input.indexOf(':');
    int s = input.substring(0, index).toInt();
    String msg = input.substring((index + 1));
    webSocket.sendTXT(s, msg);
  }
}
