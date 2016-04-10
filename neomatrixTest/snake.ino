int snakeX = 0;
int snakeY = 0;
int snakeRight = 1;
int snakeUp = 0;
int snakeW = kMatrixWidth - 1;
int snakeH = kMatrixHeight - 1;
int snakeMargin = 0;
int hue;

void snake() {
  leds[ XY(snakeX, snakeY)]  = CHSV( hue, 255, 255);
  FastLED.show();
  // End of right
  if ((snakeX >= (snakeW - snakeMargin)) && (snakeUp == 0)) {
    snakeUp = 1;
    snakeRight = 0;
    //    hue += 15;
  }
  // End of up
  else if ((snakeY >= (snakeH - snakeMargin)) && (snakeUp == 1)) {
    snakeUp = 0;
    snakeRight = -1;
    //    hue += 15;
  }
  // End of left
  else if ((snakeX == (0 + snakeMargin)) && (snakeUp == 0 && snakeRight == -1)) {
    snakeUp = -1;
    snakeRight = 0;
    //    hue += 15;
  }
  // End of down
  else if ((snakeX == (0 + snakeMargin) && snakeY == (1 + snakeMargin)) && (snakeUp == -1 && snakeRight == 0)) {
    snakeY += snakeUp;
    snakeUp = 0;
    snakeRight = 1;
    hue += 15;
    snakeMargin++;
    snakeY++;
  }

  // Final LED
  if (snakeRight == 0 && snakeUp == -1 && snakeMargin == 3) {
    //delay(1500);
    hue = random(0, 255);
    snakeMargin = 0;
    snakeX = -1;
    snakeY = 0;
    snakeRight = 1;
    snakeUp = 0;
  }

  delay(15);
  fadeToBlackBy(leds, (kMatrixWidth * kMatrixHeight), 10);
  //  fadeLightBy(leds, (kMatrixWidth * kMatrixHeight), 20);
  snakeX += snakeRight;
  snakeY += snakeUp;
}

