/*
TODO:
 
 example show oscis+p
 document caleidoscopes better
 write better caleidoscopes...
 improve and document emitters and oszillators
 explaining one example step by step:
 goal? what? how? why?
 gridmapping for rotation + zoom
 good interpolation for other matrix dimensions than 15*15
 more move & stream functions
 layers
 palettescoo
 link effects to areas
 1D examples
 2d example with more than 2 sines
 speed up MSGEQ7 readings
 
 
 DONE:
 25.6.      creating basic structure 
 setting up basic examples 
 26.6.      MSGEQ7 Support
 wrote more examples
 27.6.      improved documentation
 added Move
 added AutoRun
 TODO list
 Copy
 29.6.      rotate+mirror triangle
 more examples
 30.6.      RenderCustomMatrix
 added more comments
 alpha version released
 
 
/*
 
/*
 Funky Clouds Compendium (alpha version)
 by Stefan Petrick. Extended by Will Tatam
 
 An ever growing list of examples, tools and toys 
 for creating one- and twodimensional LED effects.
 
 Dedicated to the users of the FastLED v2.1 library
 by Daniel Garcia and Mark Kriegsmann.
 
 Provides basic and advanced helper functions.
 Contains many examples how to creatively combine them.
 
 Tested @ATmega2560 (runs propably NOT on an Uno or
 anything else with less than 4kB RAM)

 WT: Does run now on Uno after trimming a bit
 */


#define LOOP_COUNT 3000


// all 2D effects will be calculated in this matrix size
// do not touch
const uint8_t WIDTH  = kMatrixWidth;
const uint8_t HEIGHT = kMatrixHeight;

// the oscillators: linear ramps 0-255
// modified only by MoveOscillators()
byte osci[4]; 

// sin8(osci) swinging between 0 - 15
// modified only by MoveOscillators()
byte p[4];



// noise stuff
uint16_t speed = 10;
uint16_t scale = 50;
uint16_t scale2 = 30;
#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
uint8_t fnoise[MAX_DIMENSION][MAX_DIMENSION];
uint8_t noise2[MAX_DIMENSION][MAX_DIMENSION];
static uint16_t fx;
static uint16_t fy;
static uint16_t fz;
static uint16_t x2;
static uint16_t y2;
static uint16_t z2;
//palette stuff

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

/*
-------------------------------------------------------------------
 Init Inputs and Outputs: LEDs and MSGEQ7
 -------------------------------------------------------------------
 */

void ReadAudio() {
  if(MSGEQ7read()) {
    for (int b = 0; b < 7; b++) {
      left[b] = map(MSGEQ7get(b, 0), 0, MSGEQ7_OUT_MAX, 0, 1023);
      right[b] = map(MSGEQ7get(b, 1), 0, MSGEQ7_OUT_MAX, 0, 1023);
    }
  }
}



/*
-------------------------------------------------------------------
 Basic Helper functions:
 
 XY                  translate 2 dimensional coordinates into an index
 Line                draw a line
 Pixel               draw a pixel
 ClearAll            empty the screenbuffer
 MoveOscillators     increment osci[] and calculate p[]=sin8(osci)
 InitMSGEQ7          activate the MSGEQ7 
 ReadAudio           get data from MSGEQ7 into left[7] and right[7]
 
 -------------------------------------------------------------------
 */

// Bresenham line algorythm based on 2 coordinates
void Line(int x0, int y0, int x1, int y1, byte color) {
  int dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1-y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2; 
  for(;;) {  
    leds[XY(x0, y0)] = CHSV(color, 255, 255);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 > dy) { 
      err += dy; 
      x0 += sx; 
    } 
    if (e2 < dx) { 
      err += dx; 
      y0 += sy; 
    } 
  }
}

// write one pixel with HSV color to coordinates
void Pixel(int x, int y, byte color) {
  leds[XY(x, y)] = CHSV(color, 255, 255);
}

// delete the screenbuffer
void ClearAll()  
{
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = 0;
  }
}

/*
Oscillators and Emitters
 */

// set the speeds (and by that ratios) of the oscillators here
void MoveOscillators() {
  osci[0] = osci[0] + 5;
  osci[1] = osci[1] + 2;
  osci[2] = osci[2] + 3;
  osci[3] = osci[3] + 4;
  for(int i = 0; i < 4; i++) { 
    p[i] = sin8(osci[i]) / 17;  //why 17? to keep the result in the range of 0-15 (matrix 15)
  }
}





/*
-------------------------------------------------------------------
 Functions for manipulating existing data within the screenbuffer:
 
 DimAll           scales the brightness of the screenbuffer down
 Caleidoscope1    mirror one quarter to the other 3 (and overwrite them)
 Caleidoscope2    rotate one quarter to the other 3 (and overwrite them)
 Caleidoscope3    useless bullshit?!
 Caleidoscope4    rotate and add the complete screenbuffer 3 times
 Caleidoscope5    copy a triangle from the first quadrant to the other half
 Caleidoscope6
 SpiralStream     stream = give it a nice fading tail
 HorizontalStream
 VerticalStream
 VerticalMove     move = just move it as it is one line down
 Copy             copy a rectangle
 RotateTriangle   copy + rotate a triangle (in 8*8)
 MirrorTriangle   copy + mirror a triangle (in 8*8)
 RainbowTriangle  static draw for debugging
 
 -------------------------------------------------------------------
 */

void Caleidoscope6() {
  for(int x = 1; x < 8 ; x++) {
    leds[XY(7 - x, 7 )] = leds[XY(x, 0)]; 
  } //a
  for(int x = 2; x < 8 ; x++) {
    leds[XY(7 - x, 6 )] = leds[XY(x, 1)]; 
  } //b
  for(int x = 3; x < 8 ; x++) {
    leds[XY(7 - x, 5 )] = leds[XY(x, 2)]; 
  } //c
  for(int x = 4; x < 8 ; x++) {
    leds[XY(7 - x, 4 )] = leds[XY(x, 3)]; 
  } //d
  for(int x = 5; x < 8 ; x++) {
    leds[XY(7 - x, 3 )] = leds[XY(x, 4)]; 
  } //e
  for(int x = 6; x < 8 ; x++) {
    leds[XY(7 - x, 2 )] = leds[XY(x, 5)]; 
  } //f
  for(int x = 7; x < 8 ; x++) {
    leds[XY(7 - x, 1 )] = leds[XY(x, 6)]; 
  } //g
}

// create a square twister
// x and y for center, r for radius
void SpiralStream(int x,int y, int r, byte dim) {  
  for(int d = r; d >= 0; d--) {                // from the outside to the inside
    for(int i = x-d; i <= x+d; i++) {
      leds[XY(i,y-d)] += leds[XY(i+1,y-d)];   // lowest row to the right
      leds[XY(i,y-d)].nscale8( dim );
    }
    for(int i = y-d; i <= y+d; i++) {
      leds[XY(x+d,i)] += leds[XY(x+d,i+1)];   // right colum up
      leds[XY(x+d,i)].nscale8( dim );
    }
    for(int i = x+d; i >= x-d; i--) {
      leds[XY(i,y+d)] += leds[XY(i-1,y+d)];   // upper row to the left
      leds[XY(i,y+d)].nscale8( dim );
    }
    for(int i = y+d; i >= y-d; i--) {
      leds[XY(x-d,i)] += leds[XY(x-d,i-1)];   // left colum down
      leds[XY(x-d,i)].nscale8( dim );
    }
  }
}

// give it a linear tail to the side
void HorizontalStream(byte scale)  
{
  for(int x = 1; x < WIDTH ; x++) {
    for(int y = 0; y < HEIGHT; y++) {
      leds[XY(x,y)] += leds[XY(x-1,y)];
      leds[XY(x,y)].nscale8( scale );
    }
  }
  for(int y = 0; y < HEIGHT; y++) 
    leds[XY(0,y)].nscale8(scale);
}

// give it a linear tail downwards
void VerticalStream(byte scale)  
{
  for(int x = 0; x < WIDTH ; x++) {
    for(int y = 1; y < HEIGHT; y++) {
      leds[XY(x,y)] += leds[XY(x,y-1)];
      leds[XY(x,y)].nscale8( scale );
    }
  }
  for(int x = 0; x < WIDTH; x++) 
    leds[XY(x,0)].nscale8(scale);
}

// just move everything one line down
void VerticalMove() {
  for(int y = 15; y > 0; y--) {
    for(int x = 0; x < 15; x++) {
      leds[XY(x, y)] = leds[XY(x, y-1)];
    }
  }
}

// copy the rectangle defined with 2 points x0, y0, x1, y1
// to the rectangle beginning at x2, x3
void Copy(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2) {
  for(int y = y0; y < y1+1; y++) {
    for(int x = x0; x < x1+1; x++) {
      leds[XY(x+x2-x0, y + y2-y0)] = leds[XY(x, y)];
    }
  }
}

// rotate + copy triangle (8*8)
void RotateTriangle() {
  for(int x = 1; x < 8; x++) {
    for(int y = 0; y<x; y++) {
      leds[XY(x, 7-y)] = leds[XY(7-x, y)];
    }
  }
}

// mirror + copy triangle (8*8)
void MirrorTriangle() {
  for(int x = 1; x < 8; x++) {
    for(int y = 0; y<x; y++) {
      leds[XY(7-y, x)] = leds[XY(7-x, y)];
    }
  }
}

// draw static rainbow triangle pattern (8x8)
// (just for debugging)
void RainbowTriangle() {
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j<=i; j++) {
      Pixel(7-i, j, i*j*4); 
    }
  }
}

/*
-------------------------------------------------------------------
 Examples how to combine functions in order to create an effect
 
 ...or: how to visualize some of the following data
 osci[0] ... osci[3]   (0-255) triangle
 p[0] ... p[3]         (0-15)  sinus
 left[0] ... left[6]   (0-1023) values of 63Hz, 160Hz, ...
 right[0] ... right[6] (0-1023)
 
 effects based only on oscillators (triangle/sine waves)
 
 AutoRun        shows everything that follows
 SlowMandala    red slow
 Dots1          2 arround one
 Dots2          stacking sines
 SlowMandala2   just nice and soft
 SlowMandala3   just nice and soft
 Mandala8       copy one triangle all over
 
 effects based on audio data (could be also linked to oscillators)
 
 MSGEQtest      colorfull 2 chanel 7 band analyzer
 MSGEQtest2     2 frequencies linked to dot emitters in a spiral mandala
 MSGEQtest3     analyzer 2 bars
 MSGEQtest4     analyzer x 4 (as showed on youtube)
 AudioSpiral    basedrum/snare linked to red/green emitters
 MSGEQtest5     one channel 7 band spectrum analyzer (spiral fadeout)
 MSGEQtest6     classic analyzer, slow falldown
 MSGEQtest7     spectrum mandala, color linked to low frequencies
 MSGEQtest8     spectrum mandala, color linked to osci
 MSGEQtest9     falling spectogram
 CopyTest
 Audio1
 Audio2
 Audio3
 Audio4
 CaleidoTest1
 Caleidotest2
 Audio5
 Audio6
 -------------------------------------------------------------------
 */

void AutoRunAudio() {
     // all MSGEQ7 based:
  Serial.println("MSGEQtest");
   for(int i = 0; i < LOOP_COUNT; i++) {MSGEQtest();}
   
  Serial.println("MSGEQtest2");
   for(int i = 0; i < LOOP_COUNT; i++) {MSGEQtest2();}
   
//  Serial.println("MSGEQtest3");
//   for(int i = 0; i < LOOP_COUNT; i++) {MSGEQtest3();}
   
  Serial.println("MSGEQtest4");
   for(int i = 0; i < LOOP_COUNT; i++) {MSGEQtest4();}
   
  Serial.println("AudioSpiral");
   for(int i = 0; i < LOOP_COUNT; i++) {AudioSpiral();}
   
  Serial.println("MSGEQtest5");
   for(int i = 0; i < LOOP_COUNT; i++) {MSGEQtest5();}
   
  Serial.println("MSGEQtest6");
   for(int i = 0; i < LOOP_COUNT; i++) {MSGEQtest6();}
   
  Serial.println("MSGEQtest7");
   for(int i = 0; i < LOOP_COUNT; i++) {MSGEQtest7();}
   
  Serial.println("MSGEQtest8");
   for(int i = 0; i < LOOP_COUNT; i++) {MSGEQtest8();}
  
// Just showing black 
//  Serial.println("MSGEQtest9");
//   for(int i = 0; i < LOOP_COUNT; i++) {MSGEQtest9();}
   
  Serial.println("CopyTest");
   for(int i = 0; i < LOOP_COUNT; i++) {CopyTest();}
   
  Serial.println("Audio1");
   for(int i = 0; i < LOOP_COUNT; i++) {Audio1();}
   
  Serial.println("Audio2");
   for(int i = 0; i < LOOP_COUNT; i++) {Audio2();}
   
  Serial.println("Audio3");
   for(int i = 0; i < (LOOP_COUNT * 2); i++) {Audio3();}
   
  Serial.println("Audio4");
   for(int i = 0; i < (LOOP_COUNT * 2); i++) {Audio4();}
   
  Serial.println("CaleidoTest1");
   for(int i = 0; i < LOOP_COUNT; i++) {CaleidoTest1();}
   
  Serial.println("CaleidoTest2");
   for(int i = 0; i < LOOP_COUNT; i++) {CaleidoTest2();}
   
  Serial.println("Audio5");
   for(int i = 0; i < (LOOP_COUNT * 2); i++) {Audio5();}
   
  Serial.println("Audio6");
   for(int i = 0; i < (LOOP_COUNT * 2); i++) {Audio6();}
}

// red, 4 spirals, one dot emitter
// demonstrates SpiralStream and Caleidoscope
// (psychedelic)
void SlowMandala() {
  for(int i = 0; i < 15; i++) {
    for(int j = 0; j < 15; j++) {
      Pixel(i,j,1);
      SpiralStream(4, 4, 4, 127);
      Caleidoscope1();
      ShowFrame();
      FastLED.delay(50);
    }
  }
}

// 2 oscillators flying arround one ;)
void Dots1() {
  MoveOscillators();
  //2 lissajous dots red
  leds[XY(p[0],p[1])] = CHSV (1 , 255, 255);
  leds[XY(p[2],p[3])] = CHSV (1 , 255, 150); 
  //average of the coordinates in yellow
  Pixel((p[2]+p[0])/2, (p[1]+p[3])/2, 50);
  ShowFrame();
  FastLED.delay(20);
  HorizontalStream(125);
}

// x and y based on 3 sine waves
void Dots2() {
  MoveOscillators();
  Pixel((p[2]+p[0]+p[1])/3, (p[1]+p[3]+p[2])/3, osci[3]);
  ShowFrame();
  FastLED.delay(20);
  HorizontalStream(125);
}

// beautifull but periodic
void SlowMandala2() {
  for(int i = 1; i < 8; i++) {
    for(int j = 0; j < 15; j++) {
      MoveOscillators();
      Pixel(j,i, (osci[0]+osci[1])/2);
      SpiralStream(4, 4, 4, 127);
      Caleidoscope2();
      ShowFrame();
      FastLED.delay(20);
    }
  }
}

// same with a different timing
void SlowMandala3() {
  for(int i = 0; i < 15; i++) {
    for(int j = 0; j < 15; j++) {
      MoveOscillators();
      Pixel(j,j, (osci[0]+osci[1])/2);
      SpiralStream(4, 4, 4, 127);
      Caleidoscope2();
      ShowFrame();
      FastLED.delay(20);
    }
  }
}

// 2 lissajou dots *2 *4
void Mandala8() {
  MoveOscillators();
  Pixel(p[0]/2, p[1]/2, osci[2]);
  Pixel(p[2]/2, p[3]/2, osci[3]);
  Caleidoscope5();
  Caleidoscope2();
  HorizontalStream(110);
  ShowFrame();
}

// colorfull 2 chanel 7 band analyzer
void MSGEQtest() {
  ReadAudio();
  for(int i = 0; i < 7; i++) {
    Pixel(i, 15-left[i]/64, left[i]/4);
  }
  for(int i = 0; i < 7; i++) {
    int xpos = map((i  + 1), 1, 7, 7, 1) + 8;
    Pixel(xpos, 15-right[i]/64, right[i]/4);
  }  
  ShowFrame();
  VerticalStream(120);
}

// 2 frequencies linked to dot emitters in a spiral mandala
void MSGEQtest2() {
  ReadAudio();
  if (left[0]>500) {
    Pixel(0,0,1);
    Pixel(1,1,1);
  }
  if (left[2]>200) {
    Pixel(2,2,100);
  }
  if (left[6]>200) {
    Pixel(5,0,200);
  }
  SpiralStream(4, 4, 4, 127);
  Caleidoscope1();
  ShowFrame();
}

// analyzer 2 bars
void MSGEQtest3() {
  ReadAudio();
  for(int i = 0; i < 8; i++) {
    Pixel(i, 15-left[0]/64, 1);
  }
  for(int i=8; i < 15; i++) {
    Pixel(i, 15-left[4]/64, 100);
  }
  ShowFrame();
  VerticalStream(120);
}

// analyzer x 4 (as showed on youtube)
void MSGEQtest4() {
  ReadAudio();
  for(int i = 0; i < 7; i++) {
    Pixel(7-i, 8-right[i]/128, i*10);
  } 
  Caleidoscope2();
  ShowFrame();
  DimAll(240);
}

// basedrum/snare linked to red/green emitters
void AudioSpiral() {
  MoveOscillators();
  SpiralStream(7, 7, 7, 130); 
  SpiralStream(4, 4, 4, 122);
  SpiralStream(11, 11, 3, 122);
  ReadAudio();
  if (left[1] > 500) {
    leds[XY(2,1)] = CHSV (1 , 255, 255);
  }  
  if (left[4] > 500) {
    leds[XY(random(15),random(15))] = CHSV (100 , 255, 255);
  }  
  ShowFrame();
  DimAll(250);
} 

// one channel 7 band spectrum analyzer (spiral fadeout)
void MSGEQtest5() {
  ReadAudio();
  for(int i = 0; i < 7; i++) {
    Line(2*i, 15-left[i]/64, 2*i, 15, i*10);
    Line(1+2*i, 15-left[i]/64, 1+2*i, 15, i*10);  
  }
  ShowFrame();
  SpiralStream(7, 7, 7, 120);
}

// classic analyzer, slow falldown
void MSGEQtest6() {
  ReadAudio();
  for(int i = 0; i < 7; i++) {
    Line(2*i, 15-left[i]/64, 2*i, 15, i*10);
    Line(1+2*i, 15-left[i]/64, 1+2*i, 15, i*10);  
  }
  ShowFrame();
  VerticalStream(170);
}

// geile Scheiße
// spectrum mandala, color linked to 160Hz band
void MSGEQtest7() {
  MoveOscillators();
  ReadAudio();
  for(int i = 0; i < 7; i++) {
    Pixel(7-i, 8-right[i]/128, i*10+right[1]/8);
  } 
  Caleidoscope5();
  Caleidoscope1();
  ShowFrame();
  DimAll(240);
}

// spectrum mandala, color linked to osci
void MSGEQtest8() {
  MoveOscillators();
  ReadAudio();
  for(int i = 0; i < 7; i++) {
    Pixel(7-i, 8-right[i]/128, i*10+osci[1]);
  } 
  Caleidoscope5();
  Caleidoscope2();
  ShowFrame();
  DimAll(240);
}

// falling spectogram
void MSGEQtest9() {
  ReadAudio();
  for(int i = 0; i < 7; i++) {
    leds[XY(i*2,0)] = CHSV(i*27, 255, right[i]/3); // brightness should be divided by 4
    leds[XY(1+i*2,0)] = CHSV(i*27, 255, left[i]/3);
  }
  leds[XY(14,0)] = 0;
  leds[XY(15,0)] = 0; 
  ShowFrame();
  VerticalMove();
}

// 9 analyzers
void CopyTest() {
  ReadAudio();
  for(int i = 0; i < 5; i++) {
    Line(i, 4- left[i]/256, i, 4, i*10);  
  }
  Copy(0,0, 4, 4, 5, 0);
  Copy(0,0, 4, 4, 10, 0);
  Copy(0,0,14,4,0,5);
  Copy(0,0, 14, 4, 0, 10);
  ShowFrame();
  DimAll(200);
}

// test scale
// NOT WORKING as intended YET!
void CopyTest2() {
  ReadAudio();
  for(int i = 0; i < 5; i++) {
    Line(i*2, 4- left[i]/128, i*2, 4, i*10);  
  }
  Scale(0,0, 4, 4, 
  7,7, 15, 15);
  ShowFrame();
  DimAll(200);

}

// rechtangle 0-1 -> 2-3
// NOT WORKING as intended YET!
void Scale(int x0, int y0, int x1, int y1, int x2, int y2 ,int x3, int y3) {
  for(int y = y2; y < y3+1; y++) {
    for(int x = x2; x < x3+1; x++) {
      leds[XY(x,y)] = leds[XY( 
      x0 + ( (x * (x1-x0)) / (x3-x1) ), 
      y0 + ( (y * (y1-y0)) / (y3-y1) ) )];
    }
  }
}

// line spectogram mandala
void Audio1() {
  ReadAudio();
  for(int i = 0; i < 5; i++) {
    Line(3*i, 15-left[i]/64, 3*(i+1), 15-left[i+1]/64, 255-i*15);
  }
  Caleidoscope4();
  ShowFrame();
  DimAll(10);
}

// line analyzer with stream
void Audio2() {
  ReadAudio();
  for(int i = 0; i < 5; i++) {
    Line(3*i, 15-left[i]/64, 3*(i+1), 15-left[i+1]/64, 255-i*15);
  }
  ShowFrame();
  HorizontalStream(120);
}

void Audio3() {
  ReadAudio();
  for(int i = 0; i < 7; i++) {
    leds[XY(6-i,right[i]/ 128)] = CHSV(i*27, 255, right[i]);
  } // brightness should be divided by 4
  Caleidoscope6();
  Caleidoscope2();
  ShowFrame();
  DimAll(255);
}

void Audio4() {
  ReadAudio();
  for(int i = 0; i < 5; i++) {
    Line(3*i, 8-left[i]/128, 3*(i+1), 8-left[i+1]/128, i*left[i]/32);
  }
  Caleidoscope4();
  ShowFrame();
  DimAll(12);
}

void CaleidoTest1() {
  ReadAudio();
  for(int i = 0; i < 7; i++) {
    Line(i, left[i]/256, i,0,left[i]/32);
  } 
  RotateTriangle();
  Caleidoscope2();  //copy + rotate
  ShowFrame();
  DimAll(240);
}

void CaleidoTest2() {
  MoveOscillators();
  ReadAudio();
  for(int i = 0; i < 7; i++) {
    Line(i, left[i]/200, i, 0, (left[i]/15)+150);
  } 
  MirrorTriangle();
  Caleidoscope1();  //mirror + rotate
  ShowFrame();
  DimAll(240);
}

void Audio5() {
  ReadAudio();
  for(int i = 0; i < 5; i++) {
    Line(
    3 * i, 8 - left[i] / 128,        // from
    3 * (i+1), 8 - left[i+1] / 128,  // to
    i * 30); 
  }                       // color
  Caleidoscope4();
  ShowFrame();
  DimAll(9);
}

void Audio6() {
  ReadAudio();
  for(int i = 0; i < 5; i++) {
    Line(
    3 * i, 8 - left[i] / 128,        // from
    3 * (i+1), 8 - left[i+1] / 128,  // to
    i * 10);                         // lolor
    Line(
    15-(3 * i), 7 + left[i] / 128,        // from
    15-(3 * (i+1)), 7 + left[i+1] / 128,  // to
    i * 10);                              // color
  }  
  ShowFrame();
  DimAll(200);
  //ClearAll();
}


void fillnoise8() {
  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;
      fnoise[i][j] = inoise8(fx + ioffset,fy + joffset,fz);
    }
  }
  fy += speed;
  //z += 2;
}


void fillnoise82() {
  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale2 * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale2 * j;
      noise2[i][j] = inoise8(x2 + ioffset,y2 + joffset,z2);
    }
  }
  y2 += speed*3;
  //z += 2;
}

void NoiseExample1() {
  MoveOscillators();
  scale2 = 30+p[1]*3;
  fx = p[0]*15;
  fillnoise8();
  fillnoise82();
  for(int i = 0; i < kMatrixWidth; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {
      leds[XY(i,j)] = 
        CHSV(fnoise[i][j]<<1,255,(noise2[i][j]+fnoise[i][j])/2);
    }


  }


  ShowFrame();
}

void FillNoise(uint16_t x, uint16_t y,uint16_t z,uint16_t scale) {
  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;
      fnoise[i][j] = inoise8(fx + ioffset,fy + joffset,fz);
    }
  }
}

void NoiseExample2() { 
  MoveOscillators();
  FillNoise(2000-p[2]*100,100,100,100);
  for(int i = 0; i < p[2]; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {
      leds[XY(i,j)] = CRGB(fnoise[i][j],0,0);
    }
  }
  for(int i = 0; i < p[1]; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {
      leds[XY(j,i)] += CRGB(0, 0, fnoise[i][j]);
    }
  }
  ShowFrame();
  ClearAll();
}

void NoiseExample3() { 
  MoveOscillators();
  FillNoise(p[1]*100,p[2]*100,100,100);
  for(int i = 0; i < p[1]; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {
      leds[XY(i,j)] = CHSV(fnoise[i][j],255,200);
    }
  }

  for(int i = 0; i < p[3]; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {
      leds[XY(j,i)] += CHSV(128+fnoise[i][j],255,200);
    }
  }

  ShowFrame();
  ClearAll();
}

void SpeedTest() {
  ReadAudio();
  ShowFrame();
}

void NoiseExample4() { 
  MoveOscillators();
  FillNoise(100,100,100,100);
  for(int i = 0; i < p[0]+1; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {
      leds[XY(i,j)] += CHSV(fnoise[i][j+p[2]],255,255);
    }
  }
  ShowFrame();
  ClearAll();
}

void NoiseExample5() { 
  MoveOscillators();
  ReadAudio();
  FillNoise(100,100,100,300);

  for(int i = 0; i < kMatrixWidth; i++) {
    for(int j = 0; j < left[1]/64; j++) {
      leds[XY(i,15-j)] = CRGB(0,fnoise[i][left[1]/64-j],0);
    }
  }

  for(int i = 0; i < kMatrixWidth; i++) {
    for(int j = 0; j < left[5]/64; j++) {
      leds[XY(j,i)] += CRGB(fnoise[i][left[5]/64-j],0,0);
    }
  }
  ShowFrame();
  ClearAll();
}

void NoiseExample6() { 
  //MoveOscillators();
  for(int size = 1; size < 200; size++) {
    fz++;
    FillNoise(size,size,fz,size);
    for(int i = 0; i < kMatrixWidth; i++) {
      for(int j = 0; j < kMatrixHeight; j++) {
        leds[XY(i,j)] = CHSV(50+fnoise[i][j],255,255);
      }
    }
    ShowFrame();
    //ClearAll();
  }  
  for(int size = 200; size > 1; size--) {
    fz++;
    FillNoise(15,15,fz,15);
    for(int i = 0; i < kMatrixWidth; i++) {
      for(int j = 0; j < kMatrixHeight; j++) {
        leds[XY(i,j)] = CHSV(50+fnoise[i][j],255,255);
      }
    }
    ShowFrame();
    //ClearAll();
  }  
}

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { 
      currentPalette = RainbowColors_p;         
      currentBlending = LINEARBLEND;
    }
    if( secondHand == 10)  { 
      currentPalette = RainbowStripeColors_p;   
      currentBlending = NOBLEND;  
    }
    if( secondHand == 15)  { 
      currentPalette = RainbowStripeColors_p;   
      currentBlending = LINEARBLEND;
    }
    if( secondHand == 20)  { 
      SetupPurpleAndGreenPalette();             
      currentBlending = LINEARBLEND;
    }
    if( secondHand == 25)  { 
      SetupTotallyRandomPalette();              
      currentBlending = LINEARBLEND;
    }
    if( secondHand == 30)  { 
      SetupBlackAndWhiteStripedPalette();       
      currentBlending = NOBLEND; 
    }
    if( secondHand == 35)  { 
      SetupBlackAndWhiteStripedPalette();       
      currentBlending = LINEARBLEND;
    }
    if( secondHand == 40)  { 
      currentPalette = CloudColors_p;           
      currentBlending = LINEARBLEND;
    }
    if( secondHand == 45)  { 
      currentPalette = PartyColors_p;           
      currentBlending = LINEARBLEND;
    }
    if( secondHand == 50)  { 
      currentPalette = myRedWhiteBluePalette_p; 
      currentBlending = NOBLEND;  
    }
    if( secondHand == 55)  { 
      currentPalette = myRedWhiteBluePalette_p; 
      currentBlending = LINEARBLEND;
    }
  }
}

void SetupTotallyRandomPalette()
{
  for( int i = 0; i < 15; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 15 palette entries...
  fill_solid( currentPalette, 15, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16( 
  green,  green,  black,  black,
  purple, purple, black,  black,
  green,  green,  black,  black,
  purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more 
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};
void NoiseExample7() { 
  ChangePalettePeriodically();
  for(int size = 1; size < 100; size++) {
    fz++;
    FillNoise(size*3,size*3,fz,size);
    for(int i = 0; i < kMatrixWidth; i++) {
      for(int j = 0; j < kMatrixHeight; j++) {
        leds[XY(i,j)] = ColorFromPalette( currentPalette, fnoise[i][j], 255, currentBlending);
      }
    }
    ShowFrame();
  }  
    for(int size = 100; size > 1; size--) {
    fz++;
    FillNoise(size*3,size*3,fz,size);
    for(int i = 0; i < kMatrixWidth; i++) {
      for(int j = 0; j < kMatrixHeight; j++) {
        leds[XY(i,j)] = ColorFromPalette( currentPalette, fnoise[i][j], 255, currentBlending);
      }
    }
    ShowFrame();
  }  
}

void NoiseExample8() { 
  ChangePalettePeriodically();
  fx++;
    fz++;
    FillNoise(fx*3,fx*3,fz,sin8(fx)>>1);
    for(int i = 0; i < kMatrixWidth; i++) {
      for(int j = 0; j < kMatrixHeight; j++) {
        leds[XY(i,j)] = ColorFromPalette( currentPalette, fnoise[i][j], 255, currentBlending);
      }
    }
    ShowFrame();
  }  
