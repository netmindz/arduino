boolean audioRec = false;
double fftResult[16];

// Read the UDP audio data sent by WLED-Audio
void readAudioUDP() {

  if (sync.read()) {

    audioRec = true;

      for (int i = 0; i < 16; i++) {
        fftResult[i] = sync.fftResult[i];
      }
      // Serial.println("Finished parsing UDP Sync Packet");
      newReading = true;
  }
  else {
    newReading = false;
  }
}
