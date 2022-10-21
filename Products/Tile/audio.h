/*
   Adapted from https://github.com/atuline/WLED

   Take audio data from UDP data sent by WLED rather than MSGEQ7
*/

//#define FFT_SAMPLING_LOG
//#define MIC_SAMPLING_LOG

#define UDP_SYNC_HEADER "00002"

long lastTime = 0;
int delayMs = 10;                                   // I don't want to sample too often and overload WLED.

uint8_t myVals[32];                                 // Used to store a pile of samples as WLED frame rate and WLED sample rate are not synchronized

struct audioSyncPacket {
  char    header[6];      //  06 Bytes
  float   sampleRaw;      //  04 Bytes  - either "sampleRaw" or "rawSampleAgc" depending on soundAgc setting
  float   sampleSmth;     //  04 Bytes  - either "sampleAvg" or "sampleAgc" depending on soundAgc setting
  uint8_t samplePeak;     //  01 Bytes  - 0 no peak; >=1 peak detected. In future, this will also provide peak Magnitude
  uint8_t reserved1;      //  01 Bytes  - for future extensions - not used yet
  uint8_t fftResult[16];  //  16 Bytes
  float  FFT_Magnitude;   //  04 Bytes
  float  FFT_MajorPeak;   //  04 Bytes
};

bool isValidUdpSyncVersion(char header[6]) {
  return (header == UDP_SYNC_HEADER);
}


const uint16_t samples = 512;                     // This value MUST ALWAYS be a power of 2

unsigned int sampling_period_us;
unsigned long microseconds;

float FFT_MajorPeak = 0;
float FFT_Magnitude = 0;
uint16_t mAvg = 0;

uint8_t fftResult[16];

void logAudio() {

#ifdef MIC_SAMPLING_LOG
  //------------ Oscilloscope output ---------------------------
  Serial.print(targetAgc); Serial.print(" ");
  Serial.print(multAgc); Serial.print(" ");
  Serial.print(sampleAgc); Serial.print(" ");

  Serial.print(sample); Serial.print(" ");
  Serial.print(sampleAvg); Serial.print(" ");
  Serial.print(micLev); Serial.print(" ");
  Serial.print(samplePeak); Serial.print(" ");    //samplePeak = 0;
  Serial.print(micIn); Serial.print(" ");
  Serial.print(100); Serial.print(" ");
  Serial.print(0); Serial.print(" ");
  Serial.println(" ");
#ifdef ESP32                                   // if we are on a ESP32
  Serial.print("running on core ");               // identify core
  Serial.println(xPortGetCoreID());
#endif
#endif

#ifdef FFT_SAMPLING_LOG
  for (int i = 0; i < 16; i++) {
    Serial.print((int)constrain(fftResult[i], 0, 254));
    Serial.print(" ");
  }
  Serial.println("");
#endif
}  // logAudio()
