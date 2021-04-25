//F_lying circular
//Fastled circular 241 rgb led matrix demo
//Yaroslaw Turbin, 22.01.2020
//https://vk.com/ldirko
// https://www.reddit.com/user/ldirko/
// https://wokwi.com/arduino/projects/288441747546046984


#define NUM_COLS 17
#define NUM_ROWS 17

byte XY (byte x, byte y) {
  static byte PlanarLookTable [] = {             //i made this lookup table in Excel ))))
    241, 53, 54, 55, 56, 57, 58, 59, 0, 1, 2, 3, 4, 5, 6, 7, 241,
    52, 241, 241, 103, 104, 105, 106, 107, 60, 61, 62, 63, 64, 65, 241, 241, 8,
    51, 241, 102, 241, 144, 145, 146, 147, 108, 109, 110, 111, 112, 241, 66, 241, 9,
    50, 101, 241, 143, 241, 177, 178, 179, 148, 149, 150, 151, 241, 113, 241, 67, 10,
    49, 100, 142, 241, 176, 241, 202, 203, 180, 181, 182, 241, 152, 241, 114, 68, 11,
    48, 99, 141, 175, 241, 201, 241, 219, 204, 205, 241, 183, 241, 153, 115, 69, 12,
    47, 98, 140, 174, 200, 241, 218, 231, 220, 221, 206, 241, 184, 154, 116, 70, 13,
    46, 97, 139, 173, 199, 217, 230, 239, 232, 233, 222, 207, 185, 155, 117, 71, 14,
    45, 96, 138, 172, 198, 216, 229, 238, 241, 234, 223, 208, 186, 156, 118, 72, 15,
    44, 95, 137, 171, 197, 215, 228, 237, 236, 235, 224, 209, 187, 157, 119, 73, 16,
    43, 94, 136, 170, 196, 241, 214, 227, 226, 225, 210, 241, 188, 158, 120, 74, 17,
    42, 93, 135, 169, 241, 195, 241, 213, 212, 211, 241, 189, 241, 159, 121, 75, 18,
    41, 92, 134, 241, 168, 241, 194, 193, 192, 191, 190, 241, 160, 241, 122, 76, 19,
    40, 91, 241, 133, 241, 167, 166, 165, 164, 163, 162, 161, 241, 123, 241, 77, 20,
    39, 241, 90, 241, 132, 131, 130, 129, 128, 127, 126, 125, 124, 241, 78, 241, 21,
    38, 241, 241, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 241, 241, 22,
    241, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 241
  };
  return (PlanarLookTable[y * NUM_COLS + x]);
}

void mydrawLine (byte x, byte y, byte x1, byte y1, CRGB color, bool dot) {  // my ugly line draw function )))

  byte xsteps = abs8(x - x1) + 1;
  byte ysteps = abs8(y - y1) + 1;
  byte steps =  xsteps >= ysteps ? xsteps : ysteps;

  for (byte i = 1; i <= steps; i++) {
    byte dx = lerp8by8 (x, x1, i * 255 / steps);
    byte dy = lerp8by8 (y, y1, i * 255 / steps);
    leds[XY(dx, dy)] += color;    // change to += for brightness look
  }

  if (dot) {     //add white point at the ends of line
    leds[XY(x, y)] += CRGB ::White;
    leds[XY(x1, y1)] += CRGB ::White;
  }
}

void F_lying_circular() {
  static byte bhue = 0;
  EVERY_N_MILLISECONDS(30) {
    bhue++;  //30 - speed of hue change
  }

  byte x1 = beatsin8 (18, 0, (NUM_COLS - 1));
  byte x2 = beatsin8 (23, 0, (NUM_COLS - 1));
  byte x3 = beatsin8 (27, 0, (NUM_COLS - 1));
  byte x4 = beatsin8 (31, 0, (NUM_COLS - 1));
  byte x5 = beatsin8 (33, 0, (NUM_COLS - 1));

  byte y1 = beatsin8 (20, 0, (NUM_ROWS - 1));
  byte y2 = beatsin8 (26, 0, (NUM_ROWS - 1));
  byte y3 = beatsin8 (15, 0, (NUM_ROWS - 1));
  byte y4 = beatsin8 (27, 0, (NUM_ROWS - 1));
  byte y5 = beatsin8 (30, 0, (NUM_ROWS - 1));

  //CRGB color = CHSV (bhue, 255, BRIGHTNESS);
  CRGB color = ColorFromPalette(currentPalette, bhue, 255, LINEARBLEND);

  fadeToBlackBy (leds, NUM_LEDS, 80 ); // TODO: tie to controls

  mydrawLine(x1, y1,  x2, y2, color, 1);
  mydrawLine(x2, y2,  x3, y3, color, 1);
  mydrawLine(x2, y2,  x4, y4, color, 1);
  mydrawLine(x3, y3,  x4, y4, color, 1);
  mydrawLine(x3, y3,  x1, y1, color, 1);
  mydrawLine(x4, y4,  x1, y1, color, 1);

  blur2d (leds, NUM_COLS, NUM_ROWS, 10 ); // TODO: tie to controls
  FastLED.delay((SPEED / 2));// TODO: might need map

} //loop
