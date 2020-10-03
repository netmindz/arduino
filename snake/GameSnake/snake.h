class Point {

  public:
    int x;
    int y;

  public:
    Point() {
    }

    void setXY(int newX, int newY) {
      x = newX;
      y = newY;
    }
};

class GameSnake {

    char dir;
    int x;
    int y;
    int l;
    int fruit;
    Point tail[200];
    int d; // TODO need to make speed per snake
    boolean started = false;
    CRGB colorH;
    CRGB colorT;

  public:
    GameSnake() {
    }

    void init(CRGB color) {
      started = true;
      colorH = color; // CHSV(hue,255,255);
      colorT = color.fadeToBlackBy(50);
      reset();
    }
    
    void reset() {
      x = random(1, (kMatrixWidth -1));
      y = random(1, (kMatrixHeight - 1));
      if(random(0,1) == 1) {
        dir = 'L';
      }
      else {
        dir = 'R';
      }
      l = 0;
      d = 600;
      Serial.println("Go!");
      newFruit();
    }

    void frame() {
      if(!started) {
        return;
      }

      // move tail values up by one
      for (int i = l; i >= 1; i--) {
        tail[i] = tail[(i - 1)];
      }
      
      switch (dir) {
        case 'U':
          y++;
          break;
        case 'D':
          y--;
          break;
        case 'R':
          x--;
          break;
        case 'L':
          x++;
          break;
      }

      if (x > (kMatrixWidth - 1)) {
        die();
      }
      if (x < 0) {
        die();
      }

      if (y > (kMatrixHeight - 1)) {
        die();
      }
      if (y < 0) {
        die();
      }
      if (leds[XY(x, y)].r != 0) {
        die();
      }
      if (leds[XY(x, y)].g != 0) {
        eat();
      }

      leds[XY(x, y)] = colorH;
      renderTail(colorT);
    }

    void frameClear() {
      if(!started) {
        return;
      }
      leds[XY(x, y)] = CRGB::Black;
      renderTail(CRGB::Black);

      Point h;
      h.setXY(x, y);
      tail[0] = h;
    }

    void die() {
      Serial.println("Dead");
      for (int i = 0; i < 10; i++) {
        renderTail(CRGB::Red);
        FastLED.delay(90);
        renderTail(CRGB::Black);
        FastLED.delay(90);
      }
      leds[fruit] = CRGB::Black;
      reset();
    }

    void exit() {
      Serial.println("Player exit");
      renderTail(CRGB::Black);
      leds[fruit] = CRGB::Black;
      started = false;
    }

    void input(int c) {
      switch (c) {
        case 117:
          Serial.println("Up");
          if (dir != 'D') dir = 'U';
          break;
        case 100:
          Serial.println("Down");
          if (dir != 'U') dir = 'D';
          break;
        case 108:
          Serial.println("Left");
          if (dir != 'R') dir = 'L';
          break;
        case 114:
          Serial.println("Right");
          if (dir != 'L') dir = 'R';
          break;
        default:
          Serial.print("Unknown input ");
          Serial.println(c);
      }
    }

    void renderTail(CRGB color) {
      for (int i = 0; i < l; i++) {
        Point t = tail[i];
        leds[XY(t.x, t.y)] = color;
      }
    }
    

    void newFruit() {
      int i = fruit = XY(random(0, (kMatrixWidth - 1)), random(0, (kMatrixHeight - 1)));
      if (leds[i].r != 0 ||leds[i].g != 0 || leds[i].b != 0) {
        Serial.println("Fruit inside, retry");
        newFruit();
      }
      else {
        Serial.println("Show new fruit");
        leds[i] = CRGB::Green;
        fruit = i;
      }
    }

    void eat() {
      l++;
      newFruit();
      d -= 15;
    }
};
