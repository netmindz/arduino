#pragma message No control

void controlSetup() {
  
}

void controlLoop() {
  // placeholder for when no external controls
  pgm = 0;
}


boolean MSGEQ7read() {
  return true; // TODO: add delay
}

int MSGEQ7get(int band) {
  return random(0, 255);
}

int MSGEQ7get(int band, int channel) {
  return random(0, 255);
}
