// some stuff which has to prove if its worth to become part of FunkyNoise


// All the caleidoscope functions work directly within the screenbuffer (leds array).
// Draw whatever you like in the area x(0-15) and y (0-15) and then copy it arround.

// rotates the first 16x16 quadrant 3 times onto a 32x32 (+90 degrees rotation for each one)

void Caleidoscope1() {
  for(int x = 0; x < kMatrixWidth / 2 ; x++) {
    for(int y = 0; y < kMatrixHeight / 2; y++) {
      leds[XY( kMatrixWidth - 1 - x, y )] = leds[XY( y, x )];    
      leds[XY( kMatrixWidth - 1 - x, kMatrixHeight - 1 - y )] = leds[XY( x, y )];    
      leds[XY( x, kMatrixHeight - 1 - y )] = leds[XY( y, x )];    
    }
  }
}


// mirror the first 16x16 quadrant 3 times onto a 32x32

void Caleidoscope2() {
  for(int x = 0; x < kMatrixWidth / 2 ; x++) {
    for(int y = 0; y < kMatrixHeight / 2; y++) {
      leds[XY( kMatrixWidth - 1 - x, y )] = leds[XY( x, y )];              
      leds[XY( x, kMatrixHeight - 1 - y )] = leds[XY( x, y )];             
      leds[XY( kMatrixWidth - 1 - x, kMatrixHeight - 1 - y )] = leds[XY( x, y )]; 
    }
  }
}


// copy one diagonal triangle into the other one within a 16x16

void Caleidoscope3() {
  for(int x = 0; x <= CentreX ; x++) {
    for(int y = 0; y <= x; y++) {
      leds[XY( x, y )] = leds[XY( y, x )]; 
    }
  }
}  


// copy one diagonal triangle into the other one within a 16x16 (90 degrees rotated compared to Caleidoscope3)

void Caleidoscope4() {
  for(int x = 0; x <= CentreX ; x++) {
    for(int y = 0; y <= CentreY-x; y++) {
      leds[XY( CentreY - y, CentreX - x )] = leds[XY( x, y )]; 
    }
  }
}  


// copy one diagonal triangle into the other one within a 8x8

void Caleidoscope5() {
  for(int x = 0; x < kMatrixWidth/4 ; x++) {
    for(int y = 0; y <= x; y++) {
      leds[XY( x, y )] = leds[XY( y, x )];
    }
  }

  for(int x = kMatrixWidth/4; x < kMatrixWidth/2 ; x++) {
    for(int y = kMatrixHeight/4; y >= 0; y--) {
      leds[XY( x, y )] = leds[XY( y, x )];
    }
  }
}
