boolean MSGEQ7read() {
  return fft_available;
}
#define NUM_GEQ_CHANNELS 16
#define MSGEQ7_OUT_MAX 255
#define MSGEQ7_BASS 0
#define MSGEQ7_LOW 0
#define MSGEQ7_MID (NUM_GEQ_CHANNELS / 2)
#define MSGEQ7_HIGH (NUM_GEQ_CHANNELS - 1)


int MSGEQ7get(int band) {
  int value = map((fftData[map(band, 0, 6, 0, (NUM_GEQ_CHANNELS - 1))] * 1000), 0, 100, 0, MSGEQ7_OUT_MAX); // TODO: should be 100, but testing at home
//  float value =  fftData[map(band, 0, 6, 0, (NUM_GEQ_CHANNELS - 1))] * 10000;// // map((fftData[map(band, 0, 6, 0, NUM_GEQ_CHANNELS)] * 100), 0, 100, 0, MSGEQ7_OUT_MAX); // TODO: should be 100, but testing at home
//  Serial.printf("Band: %u = %f\n", band, value);
  return (int) value;
}

int MSGEQ7get(int band, int channel) {
    return MSGEQ7get(band);
}

int mapNoise(int value) {
  return value; // TODO: fitler noise
}
