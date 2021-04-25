/*
   ESP8266 + FastLED + IR Remote: https://github.com/jasoncoon/esp8266-fastled-webserver
   Copyright (C) 2015-2017 Jason Coon

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <WiFiUdp.h>
uint8_t flipClock = 0;
int timeZone = 0;

unsigned long lastTimeSync = millis();

int oldSecTime = 0;
int oldSec = 0;

WiFiUDP udp;

IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

const int SECS_PER_HOUR = 3600;

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// send an NTP request to the time server at the given address
void sendNTPpacket()
{
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(timeServerIP, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

time_t getNtpTime()
{
  while (udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  sendNTPpacket();
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

void drawHand(uint16_t angle, uint8_t endRing, CRGB color)
{
  // rotate 90 degrees counter-clockwise
  angle += 270;

  while (angle >= 360)
    angle -= 360;

  uint8_t index = 0;

  for (uint8_t ringIndex = 0; ringIndex <= endRing; ringIndex++)
  {
    index = angleToPixel360(angle, ringIndex);

    leds[index] |= color;
  }
}

void drawHandArc(uint16_t endAngle, uint8_t ringIndex, CRGB color)
{
  static const uint16_t startAngle = 270;

  // rotate 90 degrees counter-clockwise
  endAngle += 270;

  while (endAngle >= 360)
    endAngle -= 360;

  uint8_t startIndex = angleToPixel360(startAngle, ringIndex);
  uint8_t endIndex = angleToPixel360(endAngle, ringIndex);
  if (startIndex == endIndex)
  {
    leds[endIndex] = color;
  }
  else
  {
    fillRing360(ringIndex, color, startAngle, endAngle);
  }
}

void drawHandDot(uint16_t angle, uint8_t ringIndex, CRGB color)
{
  // rotate 90 degrees counter-clockwise
  angle += 270;

  while (angle >= 360)
    angle -= 360;

  uint8_t index = 0;

  index = angleToPixel360(angle, ringIndex);

  leds[index] |= color;
}

uint16_t hourAngle = 0;
uint16_t minuteAngle = 0;
uint16_t secondAngle = 0;

void updateClock()
{
  hourAngle = hour();
  if (hourAngle >= 12)
    hourAngle -= 12;
  hourAngle *= 30;

  minuteAngle = minute() * 6;
  secondAngle = second() * 6;
}

void handClock()
{
  dimAll(240);
  // fill_solid(leds, NUM_LEDS, CRGB::Black);

  // hour
  drawHand(hourAngle, 4, CRGB::Red);

  // minute
  drawHand(minuteAngle, 6, CRGB::Green);

  // second
  drawHand(secondAngle, 8, CRGB::Blue);

  updateClock();
}

void arcClock()
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  // hour
  drawHandArc(hourAngle, 6, CRGB::Red);

  // minute
  drawHandArc(minuteAngle, 7, CRGB::Green);

  // second
  drawHandArc(secondAngle, 8, CRGB::Blue);

  updateClock();
}

void rimClock()
{
  dimAll(240);
  // fill_solid(leds, NUM_LEDS, CRGB::Black);

  // hour
  drawHandDot(hourAngle, 8, CRGB::Red);

  // minute
  drawHandDot(minuteAngle, 8, CRGB::Green);

  // second
  drawHandDot(secondAngle, 8, CRGB::Blue);

  updateClock();
}
