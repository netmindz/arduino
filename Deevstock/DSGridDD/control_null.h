#define LED_PIN 2 // Huzzah! ESP 32 values
#define CLOCK_PIN 4

// MSGEQ7 wiring on spectrum analyser shield
#define MSGEQ7_STROBE_PIN 14
#define MSGEQ7_RESET_PIN  27
#define AUDIO_LEFT_PIN    A0 // pin 26
#define AUDIO_RIGHT_PIN   A1 // pin 25

void controlSetup() {
  
}

void controlLoop() {
  // placeholder for when no external controls
  pgm = 3;
}

