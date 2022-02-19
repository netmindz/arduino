#pragma message No control

#define MSGEQ7_OUT_MAX 255
#define MSGEQ7_BASS 0
#define MSGEQ7_LOW 0
#define MSGEQ7_MID 3
#define MSGEQ7_HIGH 5


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

int mapNoise(int v) {
  return v;
}
