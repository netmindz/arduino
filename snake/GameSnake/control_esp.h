// Use single controller setup where LEDs connected to ESP being used to run game as well as wifi control

#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#elif
#include <ESP8266WiFi.h>
#endif
#include <WebSocketsServer.h>
#include <ESPmDNS.h>
//#include <Hash.h>
#include <HashMap.h>


#include "wifi.h"
// Create file with the following
// *************************************************************************
// #define SECRET_SSID "";  /* Replace with your SSID */
// #define SECRET_PSK "";   /* Replace with your WPA2 passphrase */
// *************************************************************************
const char ssid[] = SECRET_SSID;         /* Replace with your SSID */
const char passphrase[] = SECRET_PSK;   /* Replace with your WPA2 passphrase */


WebSocketsServer webSocket = WebSocketsServer(81);

CreateHashMap (snakeMap, IPAddress, int, MAX_SNAKES);
int snakeIndex = 0;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  IPAddress ip = webSocket.remoteIP(num);
  int s = snakeMap[ip];
  switch (type) {

    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      snakes[s].exit();
      snakeIndex = s; // TODO: uses as next snake, but will fail if you had 3
      break;

    case WStype_CONNECTED:      {
        Serial.printf("[%u] Connection from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        if (snakeIndex > (MAX_SNAKES - 1)) {
          Serial.println("MAX SNAKES!!");
          webSocket.sendTXT(num, "Sorry, player count exceeded");
          webSocket.disconnect(num);
          return;
        }
        // send message to client
        CRGB color = playerColors[snakeIndex];
        char rgbTxt[8];
        sprintf(rgbTxt, "#%02X%02X%02X", color.r, color.g, color.b);
        String msg = "Connected player = " + (String) (snakeIndex + 1) + " <span style=\"background: " + rgbTxt +  "\">&nbsp;&nbsp;</span>";
        Serial.println(msg);
        webSocket.sendTXT(num, msg);
        snakes[snakeIndex].init(playerColors[snakeIndex]);
        snakeMap[ip] = snakeIndex;
        snakeIndex++;

        // TODO: remove this and replace with proper tracking oc active players
        leds[0] = CRGB::Black;
      }
      break;

    case WStype_TEXT: {
        Serial.printf("[%u] got Text: %s\n", num, payload);
        for ( size_t i = 0; i < length; i++ ) {
          snakes[s].input( payload[i] );
        }
        // send message to client
        //webSocket.sendTXT(num, "Sent command");
      }
      break;

    case WStype_BIN:
      Serial.printf("[%u] got binary length: %u\n", num, length);
      //      hexdump(payload, length);
      break;
  }

}

void controlSetup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

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

  leds[0] = CRGB::Blue;
  FastLED.show();

}

void controlLoop() {
  webSocket.loop();
}
