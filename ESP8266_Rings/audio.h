/*
   Adapted from https://github.com/atuline/WLED

   Take audio data from UDP data sent by WLED rather than MSGEQ7
*/

//#define FFT_SAMPLING_LOG
//#define MIC_SAMPLING_LOG

#define UDP_SYNC_HEADER "00001"


int sample;                                         // Current sample
float sampleAvg = 0;                                // Smoothed Average
bool samplePeak = 0;                                // Boolean flag for peak. Responding routine must reset this flag
//int sampleAdj;                                      // Gain adjusted sample value.
int sampleAgc;                                      // Our AGC sample

long lastTime = 0;
int delayMs = 10;                                   // I don't want to sample too often and overload WLED.

uint8_t myVals[32];                                 // Used to store a pile of samples as WLED frame rate and WLED sample rate are not synchronized

struct audioSyncPacket {
  char header[6] = UDP_SYNC_HEADER;
  uint8_t myVals[32];     //  32 Bytes
  int sampleAgc;          //  04 Bytes
  int sample;             //  04 Bytes
  float sampleAvg;        //  04 Bytes
  bool samplePeak;        //  01 Bytes
  uint8_t fftResult[16];  //  16 Bytes
  double FFT_Magnitude;   //  08 Bytes
  double FFT_MajorPeak;   //  08 Bytes
};

bool isValidUdpSyncVersion(char header[6]) {
  return (header == UDP_SYNC_HEADER);
}


const uint16_t samples = 512;                     // This value MUST ALWAYS be a power of 2

unsigned int sampling_period_us;
unsigned long microseconds;

double FFT_MajorPeak = 0;
double FFT_Magnitude = 0;
uint16_t mAvg = 0;

double fftResult[16];

//  // This is used for normalization of the result bins. It was created by sending the results of a signal generator to within 6" of a MAX9814 @ 40db gain.
//  // This is the maximum raw results for each of the result bins and is used for normalization of the results.
//  long maxChannel[] = {26000,  44000,  66000,  72000,  60000,  48000,  41000,  30000,  25000, 22000, 16000,  14000,  10000,  8000,  7000,  5000}; // Find maximum value for each bin with MAX9814 @ 40db gain.

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


// Read the UDP audio data sent by WLED-Audio
void readAudioUDP() {

  // Begin UDP Microphone Sync

  // Only run the audio listener code if we're in Receive mode
//  if (millis() - lastTime > delayMs) {
//    if (udpSyncConnected) {
      //      Serial.println("Checking for UDP Microphone Packet");
      int packetSize = fftUdp.parsePacket();
      if (packetSize) {
        //        Serial.println("Received UDP Sync Packet");
        uint8_t fftBuff[packetSize];
        fftUdp.read(fftBuff, packetSize);
        audioSyncPacket receivedPacket;
        memcpy(&receivedPacket, fftBuff, packetSize);
        for (int i = 0; i < 32; i++ ) {
          myVals[i] = receivedPacket.myVals[i];
        }
        sampleAgc = receivedPacket.sampleAgc;
        sample = receivedPacket.sample;
        sampleAvg = receivedPacket.sampleAvg;
        // VERIFY THAT THIS IS A COMPATIBLE PACKET
        char packetHeader[6];
        memcpy(&receivedPacket, packetHeader, 6);
        if (!(isValidUdpSyncVersion(packetHeader))) {
          memcpy(&receivedPacket, fftBuff, packetSize);
          for (int i = 0; i < 32; i++ ) {
            myVals[i] = receivedPacket.myVals[i];
          }
          sampleAgc = receivedPacket.sampleAgc;
          sample = receivedPacket.sample;
          sampleAvg = receivedPacket.sampleAvg;

          // Only change samplePeak IF it's currently false.  If it's true already, then the animation still needs to respond
          if (!samplePeak) {
            samplePeak = receivedPacket.samplePeak;
          }

          for (int i = 0; i < 16; i++) {
            fftResult[i] = receivedPacket.fftResult[i];
          }

          FFT_Magnitude = receivedPacket.FFT_Magnitude;
          FFT_MajorPeak = receivedPacket.FFT_MajorPeak;
          // Serial.println("Finished parsing UDP Sync Packet");

//          // "Legacy" - for MSGEQ7 patterns
//          for (int b = 0; b < 7; b++) {
//            left[b] = map(fftResult[(b * 2)], 0, 255, 0, 1023);
//            right[b] = map(fftResult[(b * 2)], 0, 255, 0, 1023);
//          }
//          newReading = true;
        }
      }
//    }
//  }

}
