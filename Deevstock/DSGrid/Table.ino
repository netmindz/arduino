#include "MSGEQ7.h"

//BUTTON SETUP STUFF\w43fed
byte prevKeyState = HIGH;

// EQ STUFF


#define pinAnalogLeft A0
#define pinAnalogRight A1
#define pinReset 5
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(30)
#define MSGEQ7_SMOOTH true

int band;
CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

// EQ vars

int eq_hue = 0;
int eq_hue_wait = 10;

// audio check
int scount =  0;
int smillis = 0;
int slastmillis = 0;


int barWidth = (WIDTH / 7) / 2;
int blockWidth = barWidth * 7;


// EQ ------------------------------------------------------------------------------------

void EQ() {

  int xpos;


  // analyze without delay
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Led strip output
  if (newReading) {
    eq_hue_wait++;
    if (eq_hue_wait > 20) {
      eq_hue += 15;
      if (eq_hue > 255) eq_hue = 0;
      eq_hue_wait = 0;
    }
    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      int count = map(MSGEQ7.get(band), 0, 255, 0, HEIGHT);
      //int count = map(band, 0, 6, 1, HEIGHT);
      for (int b = 0; b < barWidth; b++) {
        xpos = blockWidth - (barWidth * band) - b;
        if (b == 0 && band == 0 && xpos != 14) {
          Serial.print("Wrong xpos for band0 = ");
          Serial.println(xpos);
        }
        for (int i = 0; i <= WIDTH; i++) {
          if (i <= count) {
            // unsigned int g = map(band, 0, 6, 254, 0);
            // CRGB color = CRGB(map(i , 1, WIDTH, 254, 0), g, 0);
            //CRGB color = CHSV(eq_hue + (band * 15), 255,255);
            CRGB color = CHSV((band * 35), 255, 255);
            safeSetPixel(xytopixel(xpos, i), color);
          }
          else {
            safeSetPixel(xytopixel(xpos, i) , CRGB::Black);
          }
        }
      }
    }

    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      int count = map(MSGEQ7.get(band), 0, 255, 0, HEIGHT);
      //int count = map(band, 0, 6, 1, HEIGHT);

      for (int b = 0; b < barWidth; b++) {
        xpos = blockWidth + 1 + (barWidth * band) + b;
        for (int i = 0; i <= WIDTH; i++) {
          if (i <= count) {
            //          unsigned int g = map(band, 0, 6, 254, 0);
            //          CRGB color = CRGB(map(i , 1, WIDTH, 254, 0), g, 0);
            //          CRGB color = CHSV(map(count,0, HEIGHT, 0, 255) , 255,255);
            //CRGB color = CHSV(eq_hue + (band * 15), 255,255);
            CRGB color = CHSV((band * 35), 255, 255);
            safeSetPixel(xytopixel(xpos, i), color);
          }
          else {
            safeSetPixel(xytopixel(xpos, i), CRGB::Black);
          }
        }
      }
    }

   
  }
}
int xytopixel(int x, int y) {
  // return ((y - 1) * WIDTH) + (x - 1);
  return XY(x, y);
}

// VU -----------------------------------------------------------------------------------

int getTotal(int input) {
  int t = 0;
  for (int i = 0; i < 7; i++) {
    t += MSGEQ7.get(i, input);
  }
  return t / 7;
}


void VU() {
  CRGB color;

  const boolean gay = true;

  bool newReading = MSGEQ7.read(ReadsPerSecond(60));

  // Led strip output
  if (newReading) {

    int displayPeakL = map(MSGEQ7.get(MSGEQ7_LOW, 0) - 20, 0, 255, 0, round(HEIGHT / 2));
    int displayPeakR = map(MSGEQ7.get(MSGEQ7_LOW, 1) - 20, 0, 255, 0, round(HEIGHT / 2));
    Serial.print("Display peak: ");
    Serial.println(displayPeakL);

    moveUp();
    int offset = round(HEIGHT / 2);

    //    for (int i = 1; i <= HEIGHT; i++){
    //        drawPixel(0, i, CRGB::Black);
    //    }

    for (int i = 0; i < HEIGHT; i++) {
      drawPixel(offset, i, CRGB::Blue);
    }

    for (int i = 1; i <= displayPeakL; i++) {
      if (gay) {
        color = CHSV(map(i, 1, (HEIGHT / 2), 0, 230), 255, 255);
      }
      else {
        unsigned int g = map(i, 1, HEIGHT, 254, 0);
        color = CRGB(map(i , 1, HEIGHT, 0, 254), g, 0);
      }
      drawPixel((offset - i), 0, color);
    }
    for (int i = 1; i <= displayPeakR; i++) {
      if (gay) {
        color = CHSV(map(i, 1, (HEIGHT / 2), 230, 0), 255, 255);
      }
      else {
        unsigned int g = map(i, 1, (HEIGHT / 2), 254, 0);
        color = CRGB(map(i , 1, (HEIGHT / 2), 0, 254), g, 0);
      }
      drawPixel((offset + i), 0, color);
    }

   
  }
}

int xytopixelP(int x, int y) {
  //  int p = ((x - 1) * HEIGHTP) + (y - 1);
  //  return p;
  return XY(y, x);
}

void drawPixelP(int x, int y, CRGB color) {
  leds[xytopixelP(x, y)] = color;
}

void drawPixel(int x, int y, CRGB color) {
  leds[xytopixel(x, y)] = color;
}

void moveUp() {
  // Update the display:
  for (int i = (HEIGHT - 1); i >= 0; i--) {
    for (int j = (WIDTH - 1); j >= 0; j--) {
      int src = XY(j, (i - 1));
      int dst = XY(j, i);
      leds[dst] = leds[src];
    }
  }
}


void safeSetPixel(int pos, CRGB value) {
  if (pos > (NUM_LEDS - 1)) {
    Serial.print("IXP ");
    Serial.println(pos);
  }
  else {
    leds[pos] = value;
  }
}


// Funky plank  -----------------------------------------------------------------------------------


void FunkyPlank() {

  bool newReading = MSGEQ7.read(ReadsPerSecond(30));

  int offset = 8;
  // Led strip output
  if (newReading) {

    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      int hue = MSGEQ7.get(band, 0);
      int v = map(MSGEQ7.get(band, 0), 0, 255, 10, 255);
      for (int b = 0; b < barWidth; b++) {
        int  xpos = blockWidth - (barWidth * band) - b;
        drawPixel(xpos, 0, CHSV(hue, 255, v));
        //      drawPixel((offset + band + 1), 0, CHSV(hue, 255, 255));
      }
    }

    // display values of left channel on DMD
    for ( band = 0; band < 7; band++ )
    {
      int hue = MSGEQ7.get(band, 1);
      int v = map(MSGEQ7.get(band, 1), 0, 255, 10, 255);
      for (int b = 0; b < barWidth; b++) {
        int xpos = blockWidth + 1 + (barWidth * band) + b;
        drawPixel(xpos, 0, CHSV(hue, 255, v));
      }
    }

   
    moveUp();

  }
}

// DJ Light  -----------------------------------------------------------------------------------


void DJLight() {

  bool newReading = MSGEQ7.read(ReadsPerSecond(30));

//  int offset = 8;

  int mid = NUM_LEDS / 2;

  // Led strip output
  if (newReading) {

    int bands[8];
    for ( band = 0; band < 7; band++ )
    {
      bands[band] = MSGEQ7.get(band, 1);
    }



//    leds[mid] = CRGB(bands[6], bands[5] / 8, bands[1] / 2);
//    leds[mid].fadeToBlackBy(bands[3] / 12);

    leds[mid] = CRGB(bands[5]/2, bands[2]/2, bands[0]/2);
    leds[mid].fadeToBlackBy((map(bands[1], 0, 255, 255, 10)));
    
    //move to the left
    for (int i = NUM_LEDS - 1; i > mid; i--) {
      leds[i] = leds[i - 1];
    }
    // move to the right
    for (int i = 0; i < mid; i++) {
      leds[i] = leds[i + 1];
    }

   EVERY_N_MILLISECONDS(300) {
    fade_down(10); // TODO: map to fade
   }
  }
}

void fade_down(uint8_t value) {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].fadeToBlackBy(value);
  }
}

/*
  void soften_spectrum_data()
  {
  for (byte i = 0; i < 7; i++)
  {
    uint8_t old = bands[i];
    uint16_t data = left[i] + right[i] + old + old;
    data = data / 4;
    bands[i] = data;
  }
  }
*/


