#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include "FastLED.h"

const char* ssid     = "";
const char* password = "";

WebSocketsServer webSocket = WebSocketsServer(81);
bool WSClients[10] = {false};

#define LED_PIN 4
#define NUM_LEDS 40
CRGB leds[NUM_LEDS];
CRGB value = CRGB::Red;

void setup() {
  Serial.begin(115200);
  delay(100);

  // We start by connecting to a WiFi network

  Serial.println("**************************************");
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
}


void loop() {
  webSocket.loop();
  fill_solid(leds,NUM_LEDS, value);
  FastLED.show();
  delay(100);
  /*
  delay(500);
  // Now turn the LED off, then pause
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
  */
}

#define DEBUGGING(...) Serial.println( __VA_ARGS__ )
#define DEBUGGING_L(...) Serial.print( __VA_ARGS__ )

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      WSClients[num] = false;
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        DEBUGGING("Connection from ");
        DEBUGGING(ip);
        WSClients[num] = true;
      }
      break;

    case WStype_TEXT:
      {
        String text = String((char *) &payload[0]);
        String chs = text.substring(1, 2);
        double var1, var2;
        String tmp;
        Serial.print("Message on websocket: ");
        Serial.println(text);
        if (text.startsWith("r")) {
          value = CRGB::Red;
        }
        if (text.startsWith("g")) {
          value = CRGB::Green;
        }
        if (text.startsWith("b")) {
          value = CRGB::Blue;
        }
        if (text.startsWith("#")) {
          int number = (int) strtol( &text[1], NULL, 16);
          
          // Split them up into r, g, b values
          int r = number >> 16;
          int g = number >> 8 & 0xFF;
          int b = number & 0xFF;
          /*
          Serial.print("red is ");
          Serial.println(r);
          Serial.print("green is ");
          Serial.println(g);
          Serial.print("blue is ");
          Serial.println(b);
          */
          value  =  CRGB(r, g, b);

        }
        webSocket.broadcastTXT(payload, lenght);
        /*for(uint8_t i = 0; i < 10; i++) {
                if (WSClients[i]) {
                    webSocket.broadcastTXT(payload, lenght);
                    delay(0);
                }

          }*/
      }
      break;

    case WStype_BIN:
      //hexdump(payload, lenght);
      // echo data back to browser
      //webSocket.sendBIN(num, payload, lenght);
      break;
  }
}


