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
    Point tail[200];
    int d;

  public:
    GameSnake() {
      init();
    }

    void init() {
      x = kMatrixWidth / 2;
      y = kMatrixHeight / 2;
      dir = 'L';
      l = 0;
      d = 600;
      Serial.println("Go!");
      newFruit();
    }

    void frame() {
      Point t = tail[l];
      leds[XY(t.x, t.y)] = CRGB::Black;

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

      leds[XY(x, y)] = CRGB::White;
      Point h;
      h.setXY(x, y);
      tail[0] = h;
      FastLED.delay(d);
    }

    void die() {
      Serial.println("Dead");
      for (int i = 0; i < 10; i++) {
        fill_solid (leds, NUM_LEDS, CRGB::Red);
        FastLED.delay(90);
        fill_solid (leds, NUM_LEDS, CRGB::Black);
        FastLED.delay(90);
      }
      FastLED.clear();
      init();
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

    void newFruit() {
      int i = XY(random(0, (kMatrixWidth - 1)), random(0, (kMatrixHeight - 1)));
      if (leds[i].g != 0) {
        Serial.print("Fruit inside, retry");
        newFruit();
      }
      else {
        Serial.println("Show new fruit");
        leds[i] = CRGB::Green;
      }
    }

    void eat() {
      l++;
      newFruit();
      d -= 15;
    }
};
