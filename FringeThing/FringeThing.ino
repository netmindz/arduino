#include <ESP8266WiFi.h>
#include "FastLED.h"

//how many clients should be able to connect to this ESP8266
#define MAX_SRV_CLIENTS 1
const char* ssid = "Enterprise";
const char* password = "hardhouserules";

#define textBuffSize 30 //length of longest command string plus two spaces for CR + LF
char textBuff[textBuffSize]; //someplace to put received text
int charsReceived = 0;

#define LED_PIN 12
#define CHIPSET     NEOPIXEL

#define NUM_LEDS 40
CRGB leds[NUM_LEDS];


WiFiServer server(7200);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(250);
  leds[0] = CRGB::Blue;
  FastLED.show();

  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if (i == 21) {
    leds[0] = CRGB::Red;
    FastLED.show();
    Serial.print("Could not connect to"); Serial.println(ssid);
    while (1) delay(500);
  }
  server.begin();
  server.setNoDelay(true);


  Serial.print("Ready! Use 'nc ");
  Serial.print(WiFi.localIP());
  Serial.println(" 7200' to connect");

  leds[0] = CRGB::Green;
  FastLED.show();

}

void loop() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.println(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  //check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        //get data from the telnet client and push it to the UART
        while (serverClients[i].available()) {
          getReceivedText();
        }
      }
    }
  }

}

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}



void getReceivedText()
{
  char c;
  int charsWaiting;
  WiFiClient client = serverClients[0];

  // copy waiting characters into textBuff
  //until textBuff full, CR received, or no more characters
  charsWaiting = client.available();
  charsReceived = 0;
  do {
    c = client.read();
    textBuff[charsReceived] = c;
    charsReceived++;
    charsWaiting--;
  }
  while (charsReceived <= textBuffSize && (!(c == 0x0d || c == 0x0a)) && client.available() > 0);

  //if CR found go look at received text and execute command
  if (c == 0x0d || c == 0x0a) {
    parseReceivedText();
    for(int x = 0; x < textBuffSize; x++) {
      textBuff[charsReceived] = 0x00;
    }
  }

  // if textBuff full without reaching a CR, print an error message
  if (charsReceived >= textBuffSize) {
    client.println();
    Serial.println("Buffer full");
  }
  // if textBuff not full and no CR, do nothing else;
  // go back to loop until more characters are received

}

void parseReceivedText() {
  shuntUp();
  // scroll();
  String line(textBuff);
//  Serial.print("***");
//  Serial.print(line);
//    Serial.print("###");
  int cmd = getValue(line, ' ', 0).toInt();
  int hue = cmd;
  int server = getValue(line, ' ', 1).toInt();
  String tmp = getValue(line, ' ', 2);
  tmp.replace("s","");
  float duration = tmp.toFloat();
  long d = duration * 1000; // s -> ms
  uint8_t bright = map(d, 0, 60000, 30, 255);
  Serial.printf("CMD:%i  Server:%i  Duration:%f  Bright:%i\n", cmd, server, duration, bright);

  if (cmd == 10) hue = 0;
  if (cmd == 100) hue = 3;
  if (cmd == 10000) hue = 6;
  if (cmd == 10001) hue = 9;
  if (cmd == 10002) hue = 12;
  if (cmd == 1003) hue = 15;
  if (cmd == 1005) hue = 18;
  if (cmd == 101) hue = 21;
  if (cmd == 1010) hue = 24;
  if (cmd == 1011) hue = 27;
  if (cmd == 1012) hue = 30;
  if (cmd == 1014) hue = 33;
  if (cmd == 1015) hue = 36;
  if (cmd == 1020) hue = 39;
  if (cmd == 1021) hue = 42;
  if (cmd == 1027) hue = 45;
  if (cmd == 1028) hue = 48;
  if (cmd == 103) hue = 51;
  if (cmd == 104) hue = 54;
  if (cmd == 1080) hue = 57;
  if (cmd == 1081) hue = 60;
  if (cmd == 1082) hue = 63;
  if (cmd == 109) hue = 66;
  if (cmd == 1102) hue = 69;
  if (cmd == 1103) hue = 72;
  if (cmd == 1105) hue = 75;
  if (cmd == 112) hue = 78;
  if (cmd == 13000) hue = 81;
  if (cmd == 17) hue = 84;
  if (cmd == 18) hue = 87;
  if (cmd == 19) hue = 90;
  if (cmd == 2014) hue = 93;
  if (cmd == 2040) hue = 96;
  if (cmd == 2041) hue = 99;
  if (cmd == 2042) hue = 102;
  if (cmd == 2043) hue = 105;
  if (cmd == 2046) hue = 108;
  if (cmd == 21) hue = 111;
  if (cmd == 22) hue = 114;
  if (cmd == 25) hue = 117;
  if (cmd == 31) hue = 120;
  if (cmd == 35) hue = 123;
  if (cmd == 36) hue = 126;
  if (cmd == 38) hue = 129;
  if (cmd == 39) hue = 132;
  if (cmd == 4000) hue = 135;
  if (cmd == 45) hue = 138;
  if (cmd == 48) hue = 141;
  if (cmd == 50) hue = 144;
  if (cmd == 5000) hue = 147;
  if (cmd == 6) hue = 150;
  if (cmd == 60) hue = 153;
  if (cmd == 6003) hue = 156;
  if (cmd == 6009) hue = 159;
  if (cmd == 62) hue = 162;
  if (cmd == 65) hue = 165;
  if (cmd == 68) hue = 168;
  if (cmd == 69) hue = 171;
  if (cmd == 70) hue = 25;
  if (cmd == 7002) hue = 177;
  if (cmd == 71) hue = 80;
  if (cmd == 72) hue = 183;
  if (cmd == 76) hue = 45;
  if (cmd == 77) hue = 189;
  if (cmd == 78) hue = 192;
  if (cmd == 8) hue = 195;
  if (cmd == 80) hue = 198;
  if (cmd == 8003) hue = 201;
  if (cmd == 8004) hue = 204;
  if (cmd == 86) hue = 207;
  if (cmd == 9) hue = 210;
  if (cmd == 9000) hue = 213;
  if (cmd == 9002) hue = 216;
  if (cmd == 9005) hue = 219;
  if (cmd == 9015) hue = 222;
  if (cmd == 9018) hue = 225;
  if (cmd == 9020) hue = 228;
  if (cmd == 9022) hue = 231;
  if (cmd == 9023) hue = 234;


  Serial.print("hue=");
  Serial.println(hue);
  if (hue == 72) hue = 150;
  if (hue == 71) hue = 100;
  if (hue == 69) hue = 120;
  
  /*for(int n = 0; n > kMatrixWidth; n++) {
    leds[XY(0,n)] = CHSV(hue, 255, 255); 
  }*/
  leds[0] = CHSV(hue, 255, bright);
  FastLED.show();
}

void shuntUp() {
  for (int i = (NUM_LEDS - 1); i > 0; i--) {
    leds[i] = leds[(i - 1)];
  }
}

void scroll() {
  for(int i = (kMatrixHeight - 1); i > 0; i--) {
    for(int n = 0; n < 4; n++) {
      leds[XY(i,(n + 1))] =  leds[XY(i,n)];
    }
  }
}


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
