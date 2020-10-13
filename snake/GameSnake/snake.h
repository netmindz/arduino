#define MAX_SNAKES 4
CRGB playerColors[MAX_SNAKES] = {CRGB::Blue, CRGB::DarkMagenta, CRGB::Yellow, CRGB::OrangeRed};

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
  public: int d = 400; // TODO need to make speed per snake
    boolean started = false;
    boolean isDead  = false;
    int deathPulse = 0;
    CRGB colorH;
    CRGB colorT;

  public:
    GameSnake() {
    }

    void init(CRGB color) {
      started = true;
      colorH = color; // CHSV(hue,255,255);
      color.fadeToBlackBy(90);
      colorT = color; //.fadeToBlackBy(90);
      reset();
    }

    void reset() {
      isDead = false;
      x = random(1, (kMatrixWidth - 1));
      y = random(1, (kMatrixHeight - 1));
      if (random(0, 1) == 1) {
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
      if (!started) {
        return;
      }
      if (isDead) {
        if (deathPulse > 0) {
          if ( deathPulse & 0x01) {
            renderTail(CRGB::Red);
          }
          else {
            renderTail(CRGB::Black);
          }
          deathPulse--;
        }
        else {
          leds[fruit] = CRGB::Black; // TODO: not sure about this
          reset();
        }
        return;
      }

      renderTail(colorT);

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

      CRGB currentColor = leds[XY(x, y)];

      if (x > (kMatrixWidth - 1)) {
        die();
      }
      else if (x < 0) {
        die();
      }
      else if (y > (kMatrixHeight - 1)) {
        die();
      }
      else if (y < 0) {
        die();
      }
      else if (currentColor.g != 0 && currentColor.r == 0 && currentColor.b == 0) {
        //        Serial.printf("Current g,r value (%i,%i) = %i %i\n", x, y, currentColor.g, currentColor.r);
        eat();
      }
      else if (currentColor.r != 0 || currentColor.g != 0 || currentColor.b != 0) {
        die();
      }

      leds[XY(x, y)] = colorH;
    }

    void frameClear() {
      if (!started) {
        return;
      }

      leds[XY(x, y)] = CRGB::Black;
      renderTail(CRGB::Black);

      if(!isDead) {
        // move tail values up by one
        for (int i = l; i >= 1; i--) {
          tail[i] = tail[(i - 1)];
        }
  
        Point h;
        h.setXY(x, y);
        tail[0] = h;
      }
    }

    void die() {
      Serial.println("Dead");
      isDead = true;
      deathPulse = 5;
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
      if (leds[i].r != 0 || leds[i].g != 0 || leds[i].b != 0) {
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
