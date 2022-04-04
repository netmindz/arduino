#pragma message Teensy 4.0
#define LED_PIN 7
#define CLOCK_PIN 14

#include <TeensyDMX.h>

namespace teensydmx = qindesign::teensydmx;

teensydmx::Receiver dmxRx{Serial1};
uint8_t dmxRxBuf[513];  // Buffer up to 513 channels, including the start code

void InitMSGEQ7() { 
 
}

void controlSetup() {
  dmxRx.begin();
  pgm = 0;

  
//  delay(1000);
  Serial.println("controlSetup done");
}

int getValue(int chan, int minV, int maxV) {
  return map(dmxRxBuf[chan], 0, 255, minV, maxV);
}

unsigned long samplems = 0;
boolean MSGEQ7read() {
  if(fft1024.available() && (millis() - samplems) > 30) {
    samplems = millis();
    return true;
  }
  return false;
}

int MSGEQ7get(int band) {
  int a = map(spectrumValue[(band * 2)], 0, 100, 0, 255); // TODO check range
  int b = map(spectrumValue[(band * 2) + 1], 0, 100, 0, 255); // TODO check range
  return max(a, b);
}

int MSGEQ7get(int band, int channel) {
  return MSGEQ7get(band);
}

int led = 0;

// storage of the 7 10Bit (0-1023) audio band values
int left[7];
int right[7];

//// Checks if there's a new DMX frame and returns the frame size.
static int newFrame(teensydmx::Receiver &dmxRx) {
  return dmxRx.readPacket(dmxRxBuf, 0, 513);
      // Note: It's less efficient to read bytes you don't need;
      // this is only here because it was requested to make the
      // code look better. Ideally, you should call
      // `readPacket(buf, 0, size_needed)` instead.
}

void controlLoop() {
  // Read at least 5 bytes (4 channels) starting from channel 0 (start code)
  int read = newFrame(dmxRx);
  if (read >= 5 && dmxRxBuf[0] == 0) {  // Ensure start code is zero
    EVERY_N_SECONDS( 2 ) {
      Serial.printf("Brighness: %u\n", getValue(1, 0, 255)); // Dimmer data for Channel 1
    }
    led = !led;
    // digitalWrite(LED_BUILTIN, led); - breaks audio - move pin
    FastLED.setBrightness(getValue(1, 0, 100));
    pgm = getValue(2, 0, (getPatternCount() - 1));
    SPEED = getValue(3, 0, 255); // speed = 3
    FADE = getValue(4, 0, 255);  // fade = 4

  }
}
