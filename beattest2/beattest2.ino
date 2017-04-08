// Arduino Beat Detector By Damian Peckett 2015
// http://damian.pecke.tt/beat-detection-on-the-arduino
// License: Public Domain.

// Our Global Sample Rate, 5000hz
#define SAMPLEPERIODUS 200

#define LED 13

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup() {
  Serial.begin(9600);
  //The pin with the LED
  pinMode(LED, OUTPUT);
}

// 20 - 200hz Single Pole Bandpass IIR Filter
float bassFilter(float sample) {
  static float xv[3] = {0, 0, 0}, yv[3] = {0, 0, 0};
  xv[0] = xv[1]; xv[1] = xv[2];
  xv[2] = sample / 9.1f;
  yv[0] = yv[1]; yv[1] = yv[2];
  yv[2] = (xv[2] - xv[0])
          + (-0.7960060012f * yv[0]) + (1.7903124146f * yv[1]);
  return yv[2];
}

// 10hz Single Pole Lowpass IIR Filter
float envelopeFilter(float sample) { //10hz low pass
  static float xv[2] = {0, 0}, yv[2] = {0, 0};
  xv[0] = xv[1];
  xv[1] = sample / 160.f;
  yv[0] = yv[1];
  yv[1] = (xv[0] + xv[1]) + (0.9875119299f * yv[0]);
  return yv[1];
}

// 1.7 - 3.0hz Single Pole Bandpass IIR Filter
float beatFilter(float sample) {
  static float xv[3] = {0, 0, 0}, yv[3] = {0, 0, 0};
  xv[0] = xv[1]; xv[1] = xv[2];
  xv[2] = sample / 7.015f;
  yv[0] = yv[1]; yv[1] = yv[2];
  yv[2] = (xv[2] - xv[0])
          + (-0.7169861741f * yv[0]) + (1.4453653501f * yv[1]);
  return yv[2];
}

void loop() {
  unsigned long time = micros(); // Used to track rate
  float sample, value, envelope, beat, thresh;
  unsigned char i;

  for (i = 0;; ++i) {
    // Read ADC and center so +-512
    sample = (float)analogRead(0) - 503.f;
    //        Serial.print("sample = ");
    //        Serial.println(sample);
    // Filter only bass component
    value = bassFilter(sample);

    // Take signal amplitude and filter
    if (value < 0)value = -value;
    envelope = envelopeFilter(value);

    // Every 200 samples (25hz) filter the envelope
    if (i == 200) {
      // Filter out repeating bass sounds 100 - 180bpm
      beat = beatFilter(envelope);

      // Threshold it based on potentiometer on A0
      thresh = 0.02f * (float)analogRead(A0);

      // If we are above threshold, light up LED
      if (beat > thresh) {
        Serial.println("beat");
        digitalWrite(LED, HIGH);
      }
      else {
        digitalWrite(LED, LOW);
      }

      //Reset sample counter
      i = 0;
    }

    // Consume excess clock cycles, to keep at 5000 hz
    for (unsigned long up = time + SAMPLEPERIODUS; time > 20 && time < up; time = micros());
  }
}
