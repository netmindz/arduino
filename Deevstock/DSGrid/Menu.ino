// Some functions only used by my custom 3 button user interface.
// You might never need it.


void RunAnimationDependingOnPgm() {
  switch (pgm) {              

  case 0:
    MirroredNoise();     
    break;

  case 1:
    RedClouds();     
    break;

  case 2:
    Lavalamp1();     
    break;

  case 3:
    Lavalamp2();     
    break;

  case 4:
    Lavalamp3();     
    break;

  case 5:
    Lavalamp4();     
    break;

  case 6:
    Lavalamp5();     
    break;

  case 7:
    Constrained1();     
    break;

  case 8:
    RelativeMotion1();     
    break;

  case 9:
    Water();     
    break;

  case 10:
    Bubbles1();     
    break;

  case 11:
    TripleMotion();     
    break;  

  case 12:
    CrossNoise();     
    break; 

  case 13:
    CrossNoise2();     
    break;    

  case 14:
    RandomAnimation();     
    break;  

  case 15:
    MilliTimer();     
    break;   

  case 16:
    Caleido1();     
    break;      

  case 17:
    Caleido2();     
    break;     

  case 18:
    Caleido3();     
    break;   

  case 19:
    Caleido5();     
    break;      
    
  case 20:
    vortex();
    break;
  }
}

void ReadButtons () {
  byte NUM_PGM = 19;
  byte DEBOUNCE_DELAY = 200;
  byte STEP_WIDTH = 64;
  button1 = digitalRead(17);
  button2 = digitalRead(18);
  button3 = digitalRead(19);

  // if any button pressed
  if (button1 == LOW || button2 == LOW || button3 == LOW) {

    if (button1 == LOW) { // change (increment) mode
      mode ++;
      if (mode == 7) mode = 0;
      switch (mode) {

      case 0:
//        pSmartMatrix->scrollText("PROGAM + -", -1);
        break;

      case 1:
//        pSmartMatrix->scrollText("SPEED + -", -1);
        break;

      case 2:
//        pSmartMatrix->scrollText("BRIGHTNESS + -", -1);
        break;

      case 3:
//        pSmartMatrix->scrollText("RED + -", -1);
        break;

      case 4:
//        pSmartMatrix->scrollText("GREEN + -", -1);
        break;

      case 5:
//        pSmartMatrix->scrollText("BLUE + -", -1);
        break;

      case 6:
//        pSmartMatrix->scrollText("+ FOR RESET  - FOR TEXT OFF", -1);
        break;

      }

      delay(DEBOUNCE_DELAY);
      // 0 pgm
      // 1 spd
      // 2 bri
      // 3 red
      // 4 green
      // 5 blue
      // 6 reset

    }

    if (mode == 0 && button2 == LOW) { // pgm up
      pgm++;
      delay(DEBOUNCE_DELAY);
      if (pgm == NUM_PGM+1) pgm = 0;
    }
    if (mode == 0 && button3 == LOW) { // pgm down
      pgm--;
      delay(DEBOUNCE_DELAY);
      if (pgm == 255) pgm = NUM_PGM;
    }
    if (mode == 1 && button2 == LOW) { // spd up
      spd+=10;
      delay(DEBOUNCE_DELAY);
    }
    if (mode == 1 && button3 == LOW) { // spd down
      spd-=10;
      delay(DEBOUNCE_DELAY);
    }
    if (mode == 2 && button2 == LOW) { // bri up
      brightness = brightness + STEP_WIDTH;
      LEDS.setBrightness(brightness);
      delay(DEBOUNCE_DELAY);
    }
    if (mode == 2 && button3 == LOW) { // bri down
      brightness = brightness - STEP_WIDTH;
      LEDS.setBrightness(brightness);
      delay(DEBOUNCE_DELAY);
    }
    if (mode == 3 && button2 == LOW) { // red up
      red_level = red_level + STEP_WIDTH;
      delay(DEBOUNCE_DELAY);
    }
    if (mode == 3 && button3 == LOW) { // red down
      red_level = red_level - STEP_WIDTH;
      delay(DEBOUNCE_DELAY);
    }
    if (mode == 4 && button2 == LOW) { // green up
      green_level = green_level + STEP_WIDTH;
      delay(DEBOUNCE_DELAY);
    }
    if (mode == 4 && button3 == LOW) { // green down
      green_level = green_level - STEP_WIDTH;
      delay(DEBOUNCE_DELAY);
    }
    if (mode == 5 && button2 == LOW) { // blue up
      blue_level = blue_level + STEP_WIDTH;
      delay(DEBOUNCE_DELAY);
    }
    if (mode == 5 && button3 == LOW) { // blue down
      blue_level = blue_level - STEP_WIDTH;
      delay(DEBOUNCE_DELAY);
    }
    if (mode == 6 && button2 == LOW) { // reset all
      BasicVariablesSetup();
      delay(DEBOUNCE_DELAY);
    }
    if (mode == 6 && button3 == LOW) { // reset text
//      pSmartMatrix->scrollText(" ", -1);
      delay(DEBOUNCE_DELAY);
    }
  }
}

void SetupMatrixText() {
  /*
  pSmartMatrix->setScrollMode(wrapForward);
  //pSmartMatrix->setScrollMode(stopped);
  pSmartMatrix->setScrollColor({
    0xff, 0xff, 0xff            }
  );
  pSmartMatrix->setScrollSpeed(35);
  pSmartMatrix->setScrollFont(font5x7);
  //pSmartMatrix->scrollText("FunkyNoise", -1);
  //pSmartMatrix->setScrollOffsetFromEdge(10);
  */
}


//void RunButtonMenu() {
//  ReadButtons();
//  RunAnimationDependingOnPgm();
//  ColorCorrection();
//  ShowFrame();
//}







