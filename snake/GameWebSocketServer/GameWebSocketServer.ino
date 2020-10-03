
/*
   WebSocketServer.ino

    Created on: 22.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <SoftwareSerial.h>

ESP8266WiFiMulti WiFiMulti;

WebSocketsServer webSocket = WebSocketsServer(81);

#define SERIAL_DEBUG Serial

SoftwareSerial swSer(D1, D2, false);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      SERIAL_DEBUG.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        SERIAL_DEBUG.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      SERIAL_DEBUG.printf("[%u] get Text: %s\n", num, payload);
      swSer.printf("%s", payload);

      // send message to client
      webSocket.sendTXT(num, "Sent command");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      SERIAL_DEBUG.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);

      // send message to client
      // webSocket.sendBIN(num, payload, length);
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

  WiFiMulti.addAP("X", "Y");

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
    SERIAL_DEBUG.println("w");
  }

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

