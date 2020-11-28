#include <TeensyDmx.h>

#define LED_PIN 7
#define CLOCK_PIN 14

// MSGEQ7 wiring on spectrum analyser shield
#define MSGEQ7_RESET_PIN  5
#define MSGEQ7_STROBE_PIN 6
#define AUDIO_LEFT_PIN    A2
#define AUDIO_RIGHT_PIN   A1


TeensyDmx Dmx(Serial1);

void controlSetup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Dmx.setMode(TeensyDmx::DMX_IN);
  pgm = 0;

  InitMSGEQ7();
  
  delay(1000);
  Serial.println("Startup");
}

int getValue(int chan, int minV, int maxV) {
  return map(Dmx.getBuffer()[(chan - 1)], 0, 255, minV, maxV);
}

int led = 0;
void controlLoop() {
  int gPatternCount = 32; // FIXME
  Dmx.loop();
  if (Dmx.newFrame()) {
    EVERY_N_SECONDS( 2 ) {
      Serial.printf("Brighness: %u\n", getValue(1, 0, 255)); // Dimmer data for Channel 1
    }
    led = !led;
    digitalWrite(LED_BUILTIN, led);
    int b =  getValue(1, 0, 255); // brightness = 1
    if (b != BRIGHTNESS) {
      BRIGHTNESS = b;
      FastLED.setBrightness(BRIGHTNESS);
    }
    pgm = getValue(2, 0, (gPatternCount - 1)); // FIXME // pattern = 2
    SPEED = getValue(3, 0, 255); // speed = 3
    FADE = getValue(4, 0, 255);  // fade = 4

  }

}


// EQ STUFF
#define MSGEQ7_INTERVAL ReadsPerSecond(30)
#define MSGEQ7_SMOOTH true
CMSGEQ7<MSGEQ7_SMOOTH, MSGEQ7_RESET_PIN, MSGEQ7_STROBE_PIN, AUDIO_LEFT_PIN, AUDIO_RIGHT_PIN> MSGEQ7;

// wake up the MSGEQ7
void InitMSGEQ7() {
  pinMode(MSGEQ7_RESET_PIN, OUTPUT);      
  pinMode(MSGEQ7_STROBE_PIN, OUTPUT);   
  digitalWrite(MSGEQ7_RESET_PIN, LOW);     
  digitalWrite(MSGEQ7_STROBE_PIN, HIGH); 
}

// get the data from the MSGEQ7
// (still fucking slow...)
void ReadAudio() {
  digitalWrite(MSGEQ7_RESET_PIN, HIGH);
  digitalWrite(MSGEQ7_RESET_PIN, LOW);
  for(byte band = 0; band < 7; band++) {
    digitalWrite(MSGEQ7_STROBE_PIN, LOW); 
    delayMicroseconds(30); 
    left[band] = analogRead(AUDIO_LEFT_PIN); 
    right[band] = analogRead(AUDIO_RIGHT_PIN); 
    digitalWrite(MSGEQ7_STROBE_PIN, HIGH);
  }
}
