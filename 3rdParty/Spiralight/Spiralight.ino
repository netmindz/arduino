//Spiralight
//Ed's Code
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <FastLED.h>





//------------------CUSTOMS---------------
//custom colours
#define DEFAULT1R 0
#define DEFAULT1G 0
#define DEFAULT1B 255

#define DEFAULT2R 255
#define DEFAULT2G 0
#define DEFAULT2B 0


// ---------------------------------------

const uint16_t kRecvPin = 14;

IRrecv irrecv(kRecvPin);

decode_results results;
CRGB default_col_1 = CRGB(DEFAULT1R, DEFAULT1G, DEFAULT1B );
CRGB default_col_2 = CRGB(DEFAULT2R, DEFAULT2G, DEFAULT2B);
DEFINE_GRADIENT_PALETTE( redblue_gp ) {
  0,     DEFAULT1R,  DEFAULT1G,  DEFAULT1B,   //blue
  128,   DEFAULT2R, DEFAULT2G,  DEFAULT2B,
  255,   DEFAULT1R,  DEFAULT1G,  DEFAULT1B
}; //red
CRGBPalette16 MC_p = redblue_gp;
DEFINE_GRADIENT_PALETTE( redblue1_gp ) {
  0,     DEFAULT1R,  DEFAULT1G,  DEFAULT1B,   //blue
  255,   DEFAULT2R, DEFAULT2G,  DEFAULT2B

}; //red
CRGBPalette16 MC1_p = redblue1_gp;

#define NUM_LEDS 61
#define DATA_PIN 3
int static_r = 255;
int static_g = 255;
int static_b = 255;
int brightness = 100;
int anispeed = 50;
long mil;
int r2b = 1;
int b2g;
int g2r;
int rainbow_r;
int rainbow_g;
int rainbow_b;
int rainbow_sp = 2;
int P2S = 1;
int S2P;
int MCFadePos;
float startIndex;
float colorIndex;
int spos;
float pi = 3.141592654;
int randomcols[] = {128, 0, 255};
int strpos;
int vispos;
int sbouncephase = 1;
int sbouncepos;
int Bouncepos;
int Bouncephase = 1;
CRGB leds[NUM_LEDS];
int pulsepos;
int pulsephase = 1;
CRGB raindrops[30];
int rainspeeds[30];
int rainposs[30];
int rainloc[30];
int sparklephase[30];
int shiftspeeds[61];
int shiftposs[61];
int shiftloc[61];
int shiftphase[61];
CRGB shiftcol1[61];
CRGB shiftcol2[61];

int Variation = 3;
int mode_Solid = 0;
int mode_RainbowFade = 0;
int mode_RainbowWave = 1;
int mode_IndivJump = 0;
int mode_AllJump = 0;
int mode_Strobe = 0;
int mode_Marquee = 0;
int mode_Segment = 0;
int mode_Visor = 0;
int mode_Bounce = 0;
int mode_Ripple = 0;
int mode_Pulse = 0;
int mode_Rain = 0;
int mode_Special = 0;
int mode_Sparkle = 0;
int mode_Shift = 0;


void setup() {
  irrecv.enableIRIn();  // Start the receiver
  Serial.begin(9600);
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  FastLED.setBrightness(255);
  Serial.print("now running  IR Pin ");
  Serial.println(kRecvPin);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mode_Solid)
    do_Static();
  if (mode_RainbowFade) {
    if (Variation == 0)
      do_Rainbow_Fade();
    if (Variation == 1)
      do_MC_Fade();
    if (Variation >= 2)
      Variation = 0;
  }
  if (mode_RainbowWave) {
    if (Variation == 0)
      do_Spiral_Rainbow_Wave_1();
    if (Variation == 1)
      do_Spiral_Rainbow_Wave_2();
    if (Variation == 2)
      do_Spiral_Rainbow_Wave_3();
    if (Variation == 3)
      do_Spiral_MC_Wave_1();
    if (Variation == 4)
      do_Spiral_MC_Wave_2();
    if (Variation == 5)
      do_Spiral_MC_Wave_3();
    if (Variation == 6)
      do_Linear_Rainbow_Gradient_1();
    if (Variation == 7)
      do_Linear_Rainbow_Gradient_2();
    if (Variation == 8)
      do_Linear_Rainbow_Gradient_3();
    if (Variation == 9)
      do_Linear_MC_Gradient_1();
    if (Variation == 10)
      do_Linear_MC_Gradient_2();
    if (Variation == 11)
      do_Linear_MC_Gradient_3();
    if (Variation >= 12)
      Variation = 0;
  }

  if (mode_IndivJump) {
    if (Variation == 0)
      do_Indiv_Jump_Rainbow();
    if (Variation == 1)
      do_Indiv_Jump_MC();
    if (Variation >= 2)
      Variation = 0;
  }

  if (mode_AllJump)
    do_All_Jump_Rainbow();
  if (mode_Strobe) {
    if (Variation == 0)
      do_Strobe_Static();
    if (Variation == 1)
      do_Strobe_MC();
    if (Variation == 2)
      do_Strobe_Rainbow();
    if (Variation >= 3)
      Variation = 0;
  }
  if (mode_Marquee) {
    if (Variation == 0)
      do_Marquee_MC();
    if (Variation == 1)
      do_Marquee_Rainbow();
    if (Variation == 2)
      do_Marquee_Static();
    if (Variation >= 3)
      Variation = 0;
  }
  if (mode_Segment) {
    if (Variation == 0)
      do_Segment_Rainbow();
    if (Variation == 1)
      do_Segment_MC();
    if (Variation == 2)
      do_Segment_Static();
    if (Variation >= 3)
      Variation = 0;
  }
  if (mode_Visor) {
    if (Variation == 0)
      do_Visor_MC();
    if (Variation == 1)
      do_Visor_Rainbow();
    if (Variation == 2)
      do_Visor_Static();
    if (Variation >= 3)
      Variation = 0;
  }
  if (mode_Bounce) {
    if (Variation == 0)
      do_Bounce_Linear_MC();
    if (Variation == 1)
      do_Bounce_Spiral_Static();
    if (Variation == 2)
      do_Bounce_Spiral_Rainbow();
    if (Variation == 3)
      do_Bounce_Spiral_MC();
    if (Variation == 4)
      do_Bounce_Linear_Rainbow();
    if (Variation == 5)
      do_Bounce_Linear_Static();
    if (Variation >= 6)
      Variation = 0;
  }
  if (mode_Ripple) {
    if (Variation == 0)
      do_Ripple_Rainbow();
    if (Variation == 1)
      do_Ripple_MC();
    if (Variation == 2)
      do_Ripple_Static();
    if (Variation >= 3)
      Variation = 0;
  }

  if (mode_Pulse) {
    if (Variation == 0)
      do_Pulse_Rainbow();
    if (Variation == 1)
      do_Pulse_MC();
    if (Variation == 2)
      do_Pulse_Static();
    if (Variation >= 3)
      Variation = 0;

  }
  if (mode_Rain) {
    if (Variation == 0)
      do_Rain_Rainbow();
    if (Variation == 1)
      do_Rain_MC();
    if (Variation == 2)
      do_Rain_Static();
    if (Variation >= 3)
      Variation = 0;
  }
  if (mode_Special) {
    if (Variation == 0)
      do_Special_Xmas();
    if (Variation == 1)
      do_Special_Special();
    if (Variation >= 2)
      Variation = 0;
  }
  if (mode_Sparkle)
    do_Sparkle();
  if (mode_Shift) {
    if (Variation == 1)
      do_Shift_MC();
    if (Variation == 0)
      do_Shift_Rainbow();
    if (Variation >= 2)
      Variation = 0;
  }

  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    int code = results.value;
    Serial.print(code);
    Serial.println("");
    if (code == 16726725) {
      Serial.println("BRIGHT UP");
      brightness += 20;
      if (brightness > 100)
        brightness = 100;
      if (brightness < 0)
        brightness = 0;
      FastLED.setBrightness(map(brightness, 0, 100, 0, 255));
    }
    if (code == 16759365) {
      Serial.println("BRIGHT DOWN");
      if (brightness > 20)
        brightness -= 20;
      else
        brightness = 5;
      if (brightness > 100)
        brightness = 100;
      if (brightness < 1)
        brightness = 5;
      Serial.println(brightness);
      FastLED.setBrightness(map(brightness, 0, 100, 0, 255));
    }


    if (code == 16771095) {
      Serial.println("SPEED UP");
      anispeed += 10;
      if (anispeed > 100)
        anispeed = 100;
      if (anispeed < 0)
        anispeed = 0;
    }
    if (code == 16762935) {
      Serial.println("SPEED DOWN");
      anispeed -= 10;
      if (anispeed > 100)
        anispeed = 100;
      if (anispeed < 0)
        anispeed = 0;
    }

    if (code == 16745085) {
      Serial.println("VARITATION ++");
      soft_Zero();
      Variation++;
    }
    if (code == 16712445) {
      Serial.println("OFF");
      all_Zero();
      FastLED.clear();
      FastLED.show();
    }
    if (code == 16751205) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 0;
      static_g = 255;
      static_b = 0;

    }

    if (code == 16718565) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 255;
      static_g = 0;
      static_b = 0;

    }
    if (code == 16753245) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 0;
      static_g = 0;
      static_b = 255;

    }
    if (code == 16720605) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 255;
      static_g = 255;
      static_b = 255;

    }
    if (code == 16722645) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 200;
      static_g = 10;
      static_b = 0;

    }

    if (code == 16718055) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 255;
      static_g = 200;
      static_b = 0;

    }
    if (code == 16714485) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 255;
      static_g = 30;
      static_b = 0;

    }
    if (code == 16726215) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 255;
      static_g = 100;
      static_b = 0;

    }


    if (code == 16755285) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 10;
      static_g = 255;
      static_b = 20;

    }

    if (code == 16747125) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 0;
      static_g = 255;
      static_b = 100;

    }
    if (code == 16758855) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 0;
      static_g = 80;
      static_b = 30;

    }
    if (code == 16750695) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 0;
      static_g = 20;
      static_b = 2000;

    }


    if (code == 16749165) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 0;
      static_g = 20;
      static_b = 255;

    }

    if (code == 16757325) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 3;
      static_g = 0;
      static_b = 40;

    }
    if (code == 16742535) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 200;
      static_g = 0;
      static_b = 100;

    }
    if (code == 16734375) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 255;
      static_g = 0;
      static_b = 40;

    }


    if (code == 16716525) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 255;
      static_g = 100;
      static_b = 80;

    }

    if (code == 16724685) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 255;
      static_g = 70;
      static_b = 40;

    }
    if (code == 16767015) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 230;
      static_g = 245;
      static_b = 255;

    }
    if (code == 16775175) {
      Serial.println("COLOUR ");
      all_Zero();
      mode_Solid = 1;
      static_r = 255;
      static_g = 200;
      static_b = 255;

    }


    if (code == 16769055) {
      Serial.println("Fade ");
      all_Zero();
      mode_RainbowFade = 1;
    }
    if (code == 16736415) {
      Serial.println("Wave ");
      all_Zero();
      mode_RainbowWave = 1;
    }
    if (code == 16752735) {
      Serial.println("Indiv Jump ");
      all_Zero();
      mode_IndivJump = 1;
    }
    if (code == 16720095) {
      Serial.println("All Jump ");
      all_Zero();
      mode_AllJump = 1;
    }

    if (code == 16764975) {
      Serial.println("Strobe ");
      all_Zero();
      mode_Strobe = 1;
    }
    if (code == 16732335) {
      Serial.println("Marquee ");
      all_Zero();
      mode_Marquee = 1;
    }
    if (code == 16748655) {
      Serial.println("Segment ");
      all_Zero();
      mode_Segment = 1;
    }
    if (code == 16716015) {
      Serial.println("Visor ");
      all_Zero();
      mode_Visor = 1;
    }

    if (code == 16773135) {
      Serial.println("Special ");
      all_Zero();
      mode_Special = 1;
    }
    if (code == 16740495) {
      Serial.println("Bounce ");
      all_Zero();
      mode_Bounce = 1;
    }
    if (code == 16756815) {
      Serial.println("Ripple ");
      all_Zero();
      mode_Ripple = 1;
    }
    if (code == 16724175) {
      Serial.println("Pulse ");
      all_Zero();
      mode_Pulse = 1;
    }


    if (code == 16730295) {
      Serial.println("Rain ");
      all_Zero();
      mode_Rain = 1;
    }
    if (code == 16746615) {
      Serial.println("Sparkle ");
      all_Zero();
      mode_Sparkle = 1;
    }
    if (code == 16713975) {
      Serial.println("Shift ");
      all_Zero();
      mode_Shift = 1;
    }

    irrecv.resume();  // Receive the next value

  }
}
void soft_Zero() {

  for (int x = 0; x < 61; x++)
  {
    shiftspeeds[x] = 0;
    shiftposs[x] = 0;
    shiftloc[x] = 0;
    shiftphase[x] = 0;
    shiftcol1[x] =  CRGB(0, 0, 0);
    shiftcol2[x] =  CRGB(0, 0, 0);
  }
  for (int x = 0; x < 30; x++)
  {
    rainposs[x] = 0;
    raindrops[x] = CRGB(0, 0, 0);
    rainspeeds[x] = 0;
    rainloc[x] = 0;
  }
  FastLED.clear();
}
void all_Zero() {
  Variation = 0;
  mode_Solid = 0;
  mode_RainbowFade = 0;
  mode_RainbowWave = 0;
  mode_IndivJump = 0;
  mode_AllJump = 0;
  mode_Strobe = 0;
  mode_Marquee = 0;
  mode_Segment = 0;
  mode_Visor = 0;
  mode_Bounce = 0;
  mode_Ripple = 0;
  mode_Pulse = 0;
  mode_Rain = 0;
  mode_Special = 0;
  mode_Sparkle = 0;
  mode_Shift = 0;
  soft_Zero();
}
void do_Shift_Rainbow() {

  if (millis() - mil >= map(anispeed, 0, 100, 40, 10)) { // speedy 20   slow 40map(anispeed, 0, 100, 40, 10)
    mil = millis();

    //fill_solid(leds, 61, CRGB(static_r*lowm,static_g*lowm,static_b*lowm));

    //generate
    for ( int i = 0; i < 61; i++) {

      if (shiftposs[i] <= 0) {

        if (shiftphase[i] == 0) {
          shiftcol1[i] = CRGB(randomcols[random(3)], randomcols[random(3)], randomcols[random(3)]);

        }
        else {
          shiftcol1[i] = shiftcol2[i];
        }
        shiftcol2[i] = CRGB(randomcols[random(3)], randomcols[random(3)], randomcols[random(3)]);
        shiftposs[i] = 1;
        shiftphase[i] = 1;
        shiftspeeds[i] = random(1, 5);


        //Serial.println("Good");

      }
      else {
        if (shiftphase[i] == 1)
          shiftposs[i]  += shiftspeeds[i];

        if (shiftposs[i] >= 100) {
          shiftposs[i] = 0;
          shiftphase[i] = 1;


          shiftcol1[i] = shiftcol2[i];

          shiftcol2[i] = CRGB(randomcols[random(3)], randomcols[random(3)], randomcols[random(3)]);
          shiftposs[i] = 1;
          shiftphase[i] = 1;
          shiftspeeds[i] = random(1, 2);




        }
        // if (shiftposs[i] <= 0) {
        //   shiftposs[i] = 2;
        //  shiftphase[i] = 1;
        // }
        fade(i, shiftcol1[i], shiftcol2[i], shiftposs[i]);


      }

    }

    FastLED.show();
  }
}

void do_Shift_MC() {

  if (millis() - mil >= map(anispeed, 0, 100, 40, 10)) { // speedy 20   slow 40map(anispeed, 0, 100, 40, 10)
    mil = millis();
    //fill_solid(leds, 61, CRGB(static_r*lowm,static_g*lowm,static_b*lowm));

    //generate
    for ( int i = 0; i < 61; i++) {

      if (shiftposs[i] <= 0) {


        shiftposs[i] = random(2, 100);
        shiftphase[i] = random(1, 3);
        shiftspeeds[i] = random(1, 5);
        shiftloc[i] = random(61);
        int count = 0;
        // Serial.println(i);
        while (count != 1) {
          count = 0;
          for ( int t = 0; t < 61; t++) {
            if (shiftloc[i] == shiftloc[t])
              count++;
          }
          if (count != 1) {
            shiftloc[i] = random(61);
            // Serial.println("BAD");
          }

        }
        //Serial.println("Good");

      }
      else {
        if (shiftphase[i] == 1)
          shiftposs[i]  += shiftspeeds[i];
        else if (shiftphase[i] == 2)
          shiftposs[i] -= shiftspeeds[i];
        if (shiftposs[i] >= 100) {
          shiftposs[i] = 98;
          shiftphase[i] = 2;
        }
        if (shiftposs[i] <= 0) {
          shiftposs[i] = 2;
          shiftphase[i] = 1;
        }
        fade(shiftloc[i], default_col_1, default_col_2, shiftposs[i]);


      }

    }

    FastLED.show();
  }
}

void do_Sparkle() {
  float lowm = 0.3;
  if (millis() - mil >= map(anispeed, 0, 100, 40, 10)) { // speedy 20   slow 40
    mil = millis();
    fill_solid(leds, 61, CRGB(static_r * lowm, static_g * lowm, static_b * lowm));

    //generate
    for ( int i = 0; i < 30; i++) {

      if (rainposs[i] <= 0) {

        raindrops[i] = CRGB(static_r, static_g, static_b);


        rainposs[i] = 1;
        sparklephase[i] = 1;
        rainspeeds[i] = random(1, 5);
        rainloc[i] = random(61);
        int count = 0;
        while (count != 1) {
          count = 0;
          for ( int t = 0; t < 30; t++) {
            if (rainloc[i] == rainloc[t])
              count++;
          }
          if (count != 1) {
            rainloc[i] = random(61);
          }

        }

      }
      else {
        if (sparklephase[i] == 1)
          rainposs[i]  += rainspeeds[i];
        else if (sparklephase[i] == 2)
          rainposs[i] -= rainspeeds[i];
        if (rainposs[i] >= 100) {
          rainposs[i] = 99;
          sparklephase[i] = 2;
        }
        if (rainposs[i] <= 0) {
          rainposs[i] = 0;
          sparklephase[i] = 1;
        }
        fade(rainloc[i], CRGB(static_r * lowm, static_g * lowm, static_b * lowm), raindrops[i], rainposs[i]);


      }
    }

    FastLED.show();
  }
}
void do_Special_Special() {
  if (millis() - mil >= map(anispeed, 0, 100, 40, 10)) { // speedy 20   slow 40
    mil = millis();
    fill_solid(leds, 61, CRGB(150, 1, 50));
    //generate
    for ( int i = 0; i < 30; i++) {

      if (rainposs[i] <= 0) {
        if (random(2))
          raindrops[i] = CRGB::Red;
        else
          raindrops[i] = CRGB::Purple;

        rainposs[i] = 1;
        sparklephase[i] = 1;
        rainspeeds[i] = random(1, 5);
        rainloc[i] = random(61);
        int count = 0;
        while (count != 1) {
          count = 0;
          for ( int t = 0; t < 30; t++) {
            if (rainloc[i] == rainloc[t])
              count++;
          }
          if (count != 1) {
            rainloc[i] = random(61);
          }

        }

      }
      else {
        if (sparklephase[i] == 1)
          rainposs[i]  += rainspeeds[i];
        else if (sparklephase[i] == 2)
          rainposs[i] -= rainspeeds[i];
        if (rainposs[i] >= 100) {
          rainposs[i] = 99;
          sparklephase[i] = 2;
        }
        if (rainposs[i] <= 0) {
          rainposs[i] = 0;
          sparklephase[i] = 1;
        }
        fade(rainloc[i], CRGB(150, 1, 50), raindrops[i], rainposs[i]);


      }
    }

    FastLED.show();
  }
}
void do_Special_Xmas() {
  if (millis() - mil >= map(anispeed, 0, 100, 40, 10)) { // speedy 20   slow 40
    mil = millis();
    fill_solid(leds, 61, CRGB::Red);
    //generate
    for ( int i = 0; i < 30; i++) {

      if (rainposs[i] <= 0) {
        raindrops[i] = CRGB::Green;
        rainposs[i] = 1;
        sparklephase[i] = 1;
        rainspeeds[i] = random(1, 5);
        rainloc[i] = random(61);
        int count = 0;
        while (count != 1) {
          count = 0;
          for ( int t = 0; t < 30; t++) {
            if (rainloc[i] == rainloc[t])
              count++;
          }
          if (count != 1) {
            rainloc[i] = random(61);
          }

        }

      }
      else {
        if (sparklephase[i] == 1)
          rainposs[i]  += rainspeeds[i];
        else if (sparklephase[i] == 2)
          rainposs[i] -= rainspeeds[i];
        if (rainposs[i] >= 100) {
          rainposs[i] = 99;
          sparklephase[i] = 2;
        }
        if (rainposs[i] <= 0) {
          rainposs[i] = 0;
          sparklephase[i] = 1;
        }
        fade(rainloc[i], CRGB::Red, raindrops[i], rainposs[i]);


      }
    }

    FastLED.show();
  }
}

void do_Rain_Rainbow() {
  if (millis() - mil >= map(anispeed, 0, 100, 40, 10)) { // speedy 20   slow 40
    mil = millis();
    FastLED.clear();
    //generate
    for ( int i = 0; i < 20; i++) {

      if (rainposs[i] <= 0) {
        raindrops[i] = CRGB(randomcols[random(3)], randomcols[random(3)], randomcols[random(3)]);
        rainposs[i] = 100;
        rainspeeds[i] = random(1, 5);
        rainloc[i] = random(61);
        int count = 0;
        while (count != 1) {
          count = 0;
          for ( int t = 0; t < 20; t++) {
            if (rainloc[i] == rainloc[t])
              count++;
          }
          if (count != 1) {
            rainloc[i] = random(61);
          }

        }

      }
      else {
        rainposs[i] -= rainspeeds[i];
        if (rainposs[i] < 0)
          rainposs[i] = 0;
        fade(rainloc[i], CRGB::Black, raindrops[i], rainposs[i]);


      }
    }

    FastLED.show();
  }
}
void do_Rain_MC() {
  if (millis() - mil >= map(anispeed, 0, 100, 40, 10)) { // speedy 10   slow 40
    mil = millis();
    FastLED.clear();
    //generate
    for ( int i = 0; i < 20; i++) {

      if (rainposs[i] <= 0) {
        if (random(2))
          raindrops[i] = default_col_1;
        else
          raindrops[i] = default_col_2;
        rainposs[i] = 100;
        rainspeeds[i] = random(1, 5);
        rainloc[i] = random(61);
        int count = 0;
        while (count != 1) {
          count = 0;
          for ( int t = 0; t < 20; t++) {
            if (rainloc[i] == rainloc[t])
              count++;
          }
          if (count != 1) {
            rainloc[i] = random(61);
          }

        }

      }
      else {
        rainposs[i] -= rainspeeds[i];
        if (rainposs[i] < 0)
          rainposs[i] = 0;
        fade(rainloc[i], CRGB::Black, raindrops[i], rainposs[i]);


      }
    }

    FastLED.show();
  }
}
void do_Rain_Static() {
  if (millis() - mil >= map(anispeed, 0, 100, 40, 10)) { // speedy 10   slow 40
    mil = millis();
    FastLED.clear();
    //generate
    for ( int i = 0; i < 20; i++) {

      if (rainposs[i] <= 0) {

        raindrops[i] = CRGB(static_r, static_g, static_b);

        rainposs[i] = 100;
        rainspeeds[i] = random(1, 5);
        rainloc[i] = random(61);
        int count = 0;
        while (count != 1) {
          count = 0;
          for ( int t = 0; t < 20; t++) {
            if (rainloc[i] == rainloc[t])
              count++;
          }
          if (count != 1) {
            rainloc[i] = random(61);
          }

        }

      }
      else {
        rainposs[i] -= rainspeeds[i];
        if (rainposs[i] < 0)
          rainposs[i] = 0;
        fade(rainloc[i], CRGB::Black, raindrops[i], rainposs[i]);


      }
    }

    FastLED.show();
  }
}
void do_Pulse_MC() {
  //Serial.println(pulsepos);
  int pulsespeed = 2;
  if (millis() - mil >= map(anispeed, 0, 100, 40, 10)) { // speedy 20   slow 40
    mil = millis();
    if (pulsepos >= 0)
      FastLED.setBrightness(map(pulsepos, 0, 100, 0, 255));
    else
      FastLED.setBrightness(0);
    if (pulsephase == 1) {
      pulsepos += pulsespeed * 2;
      if (pulsepos >= 100) {
        pulsepos = 100;
        pulsephase = 2;
      }
    }

    if (pulsephase == 2) {
      pulsepos -= pulsespeed;
      if (pulsepos <= -30) {
        pulsepos = -30;
        pulsephase = 1;
      }
    }
    colorIndex = startIndex;
    for ( int i = 0; i < 1; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 1;  //how fast to advance through palette  3
    }
    colorIndex = startIndex;
    for ( int i = 1; i < 7; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 42;  //how fast to advance through palette  3
    }
    for ( int i = 7; i < 19; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 21;  //how fast to advance through palette  3
    }

    for ( int i = 19; i < 37; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 14;  //how fast to advance through palette  3
    }
    colorIndex = startIndex;
    for ( int i = 37; i < 61; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 11;  //how fast to advance through palette  3
    }
    FastLED.show();
    FastLED.setBrightness(brightness);

  }
}
void do_Pulse_Static() {
  // Serial.println(pulsepos);
  int pulsespeed = 2;
  if (millis() - mil >= map(anispeed, 0, 100, 40, 10)) { // speedy 20   slow 40
    mil = millis();
    if (pulsepos >= 0)
      FastLED.setBrightness(map(pulsepos, 0, 100, 0, 255));
    else
      FastLED.setBrightness(0);
    if (pulsephase == 1) {
      pulsepos += pulsespeed * 2;
      if (pulsepos >= 100) {
        pulsepos = 100;
        pulsephase = 2;
      }
    }

    if (pulsephase == 2) {
      pulsepos -= pulsespeed;
      if (pulsepos <= -30) {
        pulsepos = -30;
        pulsephase = 1;
      }
    }
    fill_solid(leds, 61, CRGB(static_r, static_g, static_b));
    FastLED.show();
    FastLED.setBrightness(brightness);

  }
}
void do_Pulse_Rainbow() {
  // Serial.println(pulsepos);
  int pulsespeed = 2;
  if (millis() - mil >= map(anispeed, 0, 100, 40, 10)) { // speedy 20   slow 40
    mil = millis();
    if (pulsepos >= 0)
      FastLED.setBrightness(map(pulsepos, 0, 100, 0, 255));
    else
      FastLED.setBrightness(0);
    if (pulsephase == 1) {
      pulsepos += pulsespeed * 2;
      if (pulsepos >= 100) {
        pulsepos = 100;
        pulsephase = 2;
      }
    }

    if (pulsephase == 2) {
      pulsepos -= pulsespeed;
      if (pulsepos <= -30) {
        pulsepos = -30;
        pulsephase = 1;
      }
    }
    colorIndex = startIndex;
    for ( int i = 0; i < 1; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 1;  //how fast to advance through palette  3
    }
    colorIndex = startIndex;
    for ( int i = 1; i < 7; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 42;  //how fast to advance through palette  3
    }
    for ( int i = 7; i < 19; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 21;  //how fast to advance through palette  3
    }

    for ( int i = 19; i < 37; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 14;  //how fast to advance through palette  3
    }
    colorIndex = startIndex;
    for ( int i = 37; i < 61; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 11;  //how fast to advance through palette  3
    }
    FastLED.show();
    FastLED.setBrightness(brightness);

  }
}

void do_Static() {
  if (millis() - mil >= 500) {// speedy
    mil = millis();
    fill_solid(leds, 61, CRGB(static_r, static_g, static_b));
    FastLED.show();
  }
}
void do_Bounce_Spiral_Static() {
  if (millis() - mil >= map(anispeed, 0, 100, 500, 80)) { // speedy
    mil = millis();

    if (sbouncephase == 1) {
      sbouncepos++;
      if (sbouncepos > 5) {
        sbouncepos = 5;
        sbouncephase = 2;
      }






    }

    if (sbouncephase == 2) {
      sbouncepos--;
      if (sbouncepos <= 0) {
        sbouncepos = 0;
        sbouncephase = 1;
      }
    }
    if (sbouncepos == 5)
      leds[0] = CRGB(static_r, static_g, static_b);
    else
      leds[0] = CRGB::Black;
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      if (sbouncepos >= 4)
        leds[i] = CRGB(static_r, static_g, static_b);
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos >= 3)
        leds[i] = CRGB(static_r, static_g, static_b);
      else
        leds[i] = CRGB::Black;
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos >= 2)
        leds[i] = CRGB(static_r, static_g, static_b);
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos >= 1)
        leds[i] = CRGB(static_r, static_g, static_b);
      else
        leds[i] = CRGB::Black;
    }
    FastLED.show();

  }
}
void do_Bounce_Spiral_MC() {
  if (millis() - mil >= map(anispeed, 0, 100, 500, 80)) { // speedy
    mil = millis();

    if (sbouncephase == 1) {
      sbouncepos++;
      if (sbouncepos > 5) {
        sbouncepos = 5;
        sbouncephase = 2;
      }






    }

    if (sbouncephase == 2) {
      sbouncepos--;
      if (sbouncepos <= 0) {
        sbouncepos = 0;
        sbouncephase = 1;
      }
    }
    if (sbouncepos == 5)
      fade(0, default_col_1, default_col_2, 50 );
    else
      leds[0] = CRGB::Black;
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24)    ;
      if (sbouncepos >= 4)
        fade(i, default_col_1, default_col_2, val );
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24)     ;
      if (sbouncepos >= 3)
        fade(i, default_col_1, default_col_2, val );
      else
        leds[i] = CRGB::Black;
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24)     ;
      if (sbouncepos >= 2)
        fade(i, default_col_1, default_col_2, val );
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24)    ;
      if (sbouncepos >= 1)
        fade(i, default_col_1, default_col_2, val );
      else
        leds[i] = CRGB::Black;
    }
    FastLED.show();

  }
}

void do_Bounce_Spiral_Rainbow() {
  if (millis() - mil >= map(anispeed, 0, 100, 500, 80)) { // speedy
    mil = millis();

    if (sbouncephase == 1) {
      sbouncepos++;
      if (sbouncepos > 5) {
        sbouncepos = 5;
        sbouncephase = 2;
      }






    }

    if (sbouncephase == 2) {
      sbouncepos--;
      if (sbouncepos <= 0) {
        sbouncepos = 0;
        sbouncephase = 1;
      }
    }
    if (sbouncepos == 5)
      leds[0] = ColorFromPalette( RainbowColors_p, 128, 255, LINEARBLEND);
    else
      leds[0] = CRGB::Black;
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      if (sbouncepos >= 4)
        leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos >= 3)
        leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      else
        leds[i] = CRGB::Black;
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos >= 2)
        leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos >= 1)
        leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      else
        leds[i] = CRGB::Black;
    }
    FastLED.show();

  }
}

void do_Ripple_Rainbow() {
  if (millis() - mil >= map(anispeed, 0, 100, 500, 80)) { // speedy
    mil = millis();

    if (sbouncephase == 1) {
      sbouncepos++;
      if (sbouncepos > 5) {
        sbouncepos = 5;
        sbouncephase = 2;
      }






    }

    if (sbouncephase == 2) {
      sbouncepos--;
      if (sbouncepos <= 1) {
        sbouncepos = 1;
        sbouncephase = 1;
      }
    }
    if (sbouncepos == 5)
      leds[0] = ColorFromPalette( RainbowColors_p, 128, 255, LINEARBLEND);
    else
      leds[0] = CRGB::Black;
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      if (sbouncepos == 4)
        leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos == 3)
        leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      else
        leds[i] = CRGB::Black;
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos == 2)
        leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos == 1)
        leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      else
        leds[i] = CRGB::Black;
    }
    FastLED.show();

  }
}
void do_Ripple_MC() {
  if (millis() - mil >= map(anispeed, 0, 100, 500, 80)) { // speedy
    mil = millis();

    if (sbouncephase == 1) {
      sbouncepos++;
      if (sbouncepos > 5) {
        sbouncepos = 5;
        sbouncephase = 2;
      }






    }

    if (sbouncephase == 2) {
      sbouncepos--;
      if (sbouncepos <= 1) {
        sbouncepos = 1;
        sbouncephase = 1;
      }
    }
    if (sbouncepos == 5)
      fade(0, default_col_1, default_col_2, 50 );
    else
      leds[0] = CRGB::Black;
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24)    ;
      if (sbouncepos == 4)
        fade(i, default_col_1, default_col_2, val );
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24)     ;
      if (sbouncepos == 3)
        fade(i, default_col_1, default_col_2, val );
      else
        leds[i] = CRGB::Black;
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24)     ;
      if (sbouncepos == 2)
        fade(i, default_col_1, default_col_2, val );
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24)    ;
      if (sbouncepos == 1)
        fade(i, default_col_1, default_col_2, val );
      else
        leds[i] = CRGB::Black;
    }
    FastLED.show();

  }
}
void do_Ripple_Static() {
  if (millis() - mil >= map(anispeed, 0, 100, 500, 80)) { // speedy
    mil = millis();

    if (sbouncephase == 1) {
      sbouncepos++;
      if (sbouncepos > 5) {
        sbouncepos = 5;
        sbouncephase = 2;
      }






    }

    if (sbouncephase == 2) {
      sbouncepos--;
      if (sbouncepos <= 1) {
        sbouncepos = 1;
        sbouncephase = 1;
      }
    }
    if (sbouncepos == 5)
      leds[0] = CRGB(static_r, static_g, static_b);
    else
      leds[0] = CRGB::Black;
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      if (sbouncepos == 4)
        leds[i] = CRGB(static_r, static_g, static_b);
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos == 3)
        leds[i] = CRGB(static_r, static_g, static_b);
      else
        leds[i] = CRGB::Black;
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos == 2)
        leds[i] = CRGB(static_r, static_g, static_b);
      else
        leds[i] = CRGB::Black;
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      if (sbouncepos == 1)
        leds[i] = CRGB(static_r, static_g, static_b);
      else
        leds[i] = CRGB::Black;
    }
    FastLED.show();

  }
}
void do_Bounce_Linear_MC() {
  int Bouncespeed = 15;
  if (millis() - mil >= map(anispeed, 0, 100, 120, 30)) { // speedy
    mil = millis();



    if (Bouncephase == 1) {
      Bouncepos += Bouncespeed;
      if (Bouncepos >= 120) {
        Bouncepos = 100;
        Bouncephase = 2;
      }
      if (50 <= Bouncepos)
        fade(0, default_col_1, default_col_2, 50 );
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }
    }

    if (Bouncephase == 2) {
      Bouncepos -= Bouncespeed;
      if (Bouncepos <= -20) {
        Bouncepos = 100;
        Bouncephase = 3;
      }

      if (50 <= Bouncepos)
        fade(0, default_col_1, default_col_2, 50 );
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }


    }
    if (Bouncephase == 3) {
      Bouncepos -= Bouncespeed;
      if (Bouncepos <= -20) {
        Bouncepos = 0;
        Bouncephase = 4;
      }
      if (50 >= Bouncepos)
        fade(0, default_col_1, default_col_2, 50 );
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }
    }

    if (Bouncephase == 4) {
      Bouncepos += Bouncespeed;


      if (50 >= Bouncepos)
        fade(0, default_col_1, default_col_2, 50 );
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          fade(i, default_col_1, default_col_2, val );
        else
          leds[i] = CRGB::Black;
      }
      if (Bouncepos >= 120) {
        Bouncepos = 0;
        Bouncephase = 1;
        FastLED.clear();
        FastLED.show();
      }
    }

    FastLED.show();


  }
}
void do_Bounce_Linear_Rainbow() {
  int Bouncespeed = 15;
  if (millis() - mil >= map(anispeed, 0, 100, 120, 30)) { // speedy
    mil = millis();



    if (Bouncephase == 1) {
      Bouncepos += Bouncespeed;
      if (Bouncepos >= 120) {
        Bouncepos = 100;
        Bouncephase = 2;
      }
      if (50 <= Bouncepos)
        leds[0] = ColorFromPalette( RainbowColors_p, 128, 255, LINEARBLEND);
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }
    }

    if (Bouncephase == 2) {
      Bouncepos -= Bouncespeed;
      if (Bouncepos <= -20) {
        Bouncepos = 100;
        Bouncephase = 3;
      }

      if (50 <= Bouncepos)
        leds[0] = ColorFromPalette( RainbowColors_p, 128, 255, LINEARBLEND);
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }


    }
    if (Bouncephase == 3) {
      Bouncepos -= Bouncespeed;
      if (Bouncepos <= -20) {
        Bouncepos = 0;
        Bouncephase = 4;
      }
      if (50 >= Bouncepos)
        leds[0] = ColorFromPalette( RainbowColors_p, 128, 255, LINEARBLEND);
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }
    }

    if (Bouncephase == 4) {
      Bouncepos += Bouncespeed;


      if (50 >= Bouncepos)
        leds[0] = ColorFromPalette( RainbowColors_p, 128, 255, LINEARBLEND);
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = ColorFromPalette( RainbowColors_p, map(val, 0, 100, 0, 255), 255, LINEARBLEND);
        else
          leds[i] = CRGB::Black;
      }
      if (Bouncepos >= 120) {
        Bouncepos = 0;
        Bouncephase = 1;
        FastLED.clear();
        FastLED.show();
      }
    }

    FastLED.show();


  }
}

void do_Bounce_Linear_Static() {
  int Bouncespeed = 15;
  if (millis() - mil >= map(anispeed, 0, 100, 120, 30)) { // speedy
    mil = millis();



    if (Bouncephase == 1) {
      Bouncepos += Bouncespeed;
      if (Bouncepos >= 120) {
        Bouncepos = 100;
        Bouncephase = 2;
      }
      if (50 <= Bouncepos)
        leds[0] = CRGB(static_r, static_g, static_b);
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }
    }

    if (Bouncephase == 2) {
      Bouncepos -= Bouncespeed;
      if (Bouncepos <= -20) {
        Bouncepos = 100;
        Bouncephase = 3;
      }

      if (50 <= Bouncepos)
        leds[0] = CRGB(static_r, static_g, static_b);
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val <= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }


    }
    if (Bouncephase == 3) {
      Bouncepos -= Bouncespeed;
      if (Bouncepos <= -20) {
        Bouncepos = 0;
        Bouncephase = 4;
      }
      if (50 >= Bouncepos)
        leds[0] = CRGB(static_r, static_g, static_b);
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }
    }

    if (Bouncephase == 4) {
      Bouncepos += Bouncespeed;


      if (50 >= Bouncepos)
        leds[0] = CRGB(static_r, static_g, static_b);
      else
        leds[0] = CRGB::Black;
      for ( int i = 1; i < 7; i++) {

        float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 7; i < 19; i++) {

        float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }


      for ( int i = 19; i < 37; i++) {

        float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }
      for ( int i = 37; i < 61; i++) {

        float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
        if (val >= Bouncepos)
          leds[i] = CRGB(static_r, static_g, static_b);
        else
          leds[i] = CRGB::Black;
      }
      if (Bouncepos >= 120) {
        Bouncepos = 0;
        Bouncephase = 1;
        FastLED.clear();
        FastLED.show();
      }
    }

    FastLED.show();


  }
}
void do_Visor_MC() {
  int vislength = 2;
  if (millis() - mil >= map(anispeed, 0, 100, 200, 20)) {
    mil = millis();
    // set point
    fade(0, default_col_1, default_col_2, 50 );
    for ( int i = 1; i < 7; i++) {

      float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;

      fade(i, default_col_1, default_col_2, val );
    }
    for ( int i = 7; i < 19; i++) {

      float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
      fade(i, default_col_1, default_col_2, val );
    }


    for ( int i = 19; i < 37; i++) {

      float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
      fade(i, default_col_1, default_col_2, val );
    }
    for ( int i = 37; i < 61; i++) {

      float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
      fade(i, default_col_1, default_col_2, val );
    }


    vispos++;
    if (vispos > 61)
      vispos = 0;
    for ( int i = 0; i < 61; i++) {
      if (abs(i - vispos) > vislength)
        leds[i] = CRGB::Black;
      if (i - vispos == 0)
        leds[i] = CRGB(leds[i].r * 0.7, leds[i].g * 0.7, leds[i].b * 0.7);
      if (i - vispos == -1)
        leds[i] = CRGB(leds[i].r * 0.5, leds[i].g * 0.5, leds[i].b * 0.5);
      if (i - vispos == -2) {
        leds[i] = CRGB(leds[i].r * 0.2, leds[i].g * 0.2, leds[i].b * 0.2);

        Serial.println(vispos);
      }
    }

    FastLED.show();


  }
}

void do_Visor_Static() {
  int vislength = 2;
  if (millis() - mil >= map(anispeed, 0, 100, 200, 20)) {
    mil = millis();
    // set point
    fill_solid(leds, 61, CRGB(static_r, static_g, static_b));


    vispos++;
    if (vispos > 61)
      vispos = 0;
    for ( int i = 0; i < 61; i++) {
      if (abs(i - vispos) > vislength)
        leds[i] = CRGB::Black;
      if (i - vispos == 0)
        leds[i] = CRGB(leds[i].r * 0.7, leds[i].g * 0.7, leds[i].b * 0.7);
      if (i - vispos == -1)
        leds[i] = CRGB(leds[i].r * 0.5, leds[i].g * 0.5, leds[i].b * 0.5);
      if (i - vispos == -2) {
        leds[i] = CRGB(leds[i].r * 0.2, leds[i].g * 0.2, leds[i].b * 0.2);

        Serial.println(vispos);
      }
    }

    FastLED.show();


  }
}
void do_Visor_Rainbow() {
  int vislength = 2;
  if (millis() - mil >= map(anispeed, 0, 100, 200, 20)) {
    mil = millis();
    // set point
    leds[0] = ColorFromPalette( RainbowColors_p, 128, 255, LINEARBLEND);
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }


    vispos++;
    if (vispos > 61)
      vispos = 0;
    for ( int i = 0; i < 61; i++) {
      if (abs(i - vispos) > vislength)
        leds[i] = CRGB::Black;
      if (i - vispos == 0)
        leds[i] = CRGB(leds[i].r * 0.7, leds[i].g * 0.7, leds[i].b * 0.7);
      if (i - vispos == -1)
        leds[i] = CRGB(leds[i].r * 0.5, leds[i].g * 0.5, leds[i].b * 0.5);
      if (i - vispos == -2) {
        leds[i] = CRGB(leds[i].r * 0.2, leds[i].g * 0.2, leds[i].b * 0.2);

        Serial.println(vispos);
      }
    }

    FastLED.show();


  }
}
void do_Segment_Rainbow() {
  if (millis() - mil >= map(anispeed, 0, 100, 250, 0)) {
    mil = millis();
    spos++;
    if (spos >= 24)
      spos = 0;
    leds[0] = ColorFromPalette( RainbowColors_p, 128, 255, LINEARBLEND);
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      if  (((i - 1) * (360 / 6) >= 15 * spos and ((i - 1) * (360 / 6)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 1 == 0) == 0)
          if ((spos == 21 and i - 1 == 0) == 0)
            if ((spos == 22 and i - 1 == 0) == 0)
              if ((spos == 23 and i - 1 == 0) == 0)
                leds[i] = CRGB::Black;
      }
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      if  (((i - 7) * (360 / 12) >= 15 * spos and ((i - 7) * (360 / 12)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 7 == 0) == 0)
          if ((spos == 21 and i - 7 == 0) == 0)
            if ((spos == 22 and i - 7 == 0) == 0)
              if ((spos == 22 and i - 7 == 1) == 0)
                if ((spos == 23 and i - 7 == 0) == 0)
                  if ((spos == 23 and i - 7 == 1) == 0)
                    leds[i] = CRGB::Black;
      }
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      if  (((i - 19) * (360 / 18) >= 15 * spos and ((i - 19) * (360 / 18)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 19 == 0) == 0)
          if ((spos == 21 and i - 19 == 0) == 0)
            if ((spos == 21 and i - 19 == 1) == 0)
              if ((spos == 22 and i - 19 == 0) == 0)
                if ((spos == 22 and i - 19 == 1) == 0)
                  if ((spos == 23 and i - 19 == 0) == 0)
                    if ((spos == 23 and i - 19 == 1) == 0)
                      if ((spos == 23 and i - 19 == 2) == 0)
                        leds[i] = CRGB::Black;
      }
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
      if  (((i - 37) * (360 / 24) >= 15 * spos and ((i - 37) * (360 / 24)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 37 == 0) == 0)
          if ((spos == 21 and i - 37 == 0) == 0)
            if ((spos == 21 and i - 37 == 1) == 0)
              if ((spos == 22 and i - 37 == 0) == 0)
                if ((spos == 22 and i - 37 == 1) == 0)
                  if ((spos == 22 and i - 37 == 2) == 0)
                    if ((spos == 23 and i - 37 == 3) == 0)
                      if ((spos == 23 and i - 37 == 0) == 0)
                        if ((spos == 23 and i - 37 == 1) == 0)
                          if ((spos == 23 and i - 37 == 2) == 0)
                            leds[i] = CRGB::Black;
      }
    }

    FastLED.show();
  }
}
void do_Segment_MC() {
  if (millis() - mil >= map(anispeed, 0, 100, 250, 0)) {
    mil = millis();
    spos++;
    if (spos >= 24)
      spos = 0;
    leds[0] = ColorFromPalette( MC_p, 128, 255, LINEARBLEND);
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      leds[i] = ColorFromPalette( MC_p, val, 255, LINEARBLEND);
      if  (((i - 1) * (360 / 6) >= 15 * spos and ((i - 1) * (360 / 6)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 1 == 0) == 0)
          if ((spos == 21 and i - 1 == 0) == 0)
            if ((spos == 22 and i - 1 == 0) == 0)
              if ((spos == 23 and i - 1 == 0) == 0)
                leds[i] = CRGB::Black;
      }
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( MC_p, val, 255, LINEARBLEND);
      if  (((i - 7) * (360 / 12) >= 15 * spos and ((i - 7) * (360 / 12)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 7 == 0) == 0)
          if ((spos == 21 and i - 7 == 0) == 0)
            if ((spos == 22 and i - 7 == 0) == 0)
              if ((spos == 22 and i - 7 == 1) == 0)
                if ((spos == 23 and i - 7 == 0) == 0)
                  if ((spos == 23 and i - 7 == 1) == 0)
                    leds[i] = CRGB::Black;
      }
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( MC_p, val, 255, LINEARBLEND);
      if  (((i - 19) * (360 / 18) >= 15 * spos and ((i - 19) * (360 / 18)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 19 == 0) == 0)
          if ((spos == 21 and i - 19 == 0) == 0)
            if ((spos == 21 and i - 19 == 1) == 0)
              if ((spos == 22 and i - 19 == 0) == 0)
                if ((spos == 22 and i - 19 == 1) == 0)
                  if ((spos == 23 and i - 19 == 0) == 0)
                    if ((spos == 23 and i - 19 == 1) == 0)
                      if ((spos == 23 and i - 19 == 2) == 0)
                        leds[i] = CRGB::Black;
      }
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( MC_p, val, 255, LINEARBLEND);
      if  (((i - 37) * (360 / 24) >= 15 * spos and ((i - 37) * (360 / 24)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 37 == 0) == 0)
          if ((spos == 21 and i - 37 == 0) == 0)
            if ((spos == 21 and i - 37 == 1) == 0)
              if ((spos == 22 and i - 37 == 0) == 0)
                if ((spos == 22 and i - 37 == 1) == 0)
                  if ((spos == 22 and i - 37 == 2) == 0)
                    if ((spos == 23 and i - 37 == 3) == 0)
                      if ((spos == 23 and i - 37 == 0) == 0)
                        if ((spos == 23 and i - 37 == 1) == 0)
                          if ((spos == 23 and i - 37 == 2) == 0)
                            leds[i] = CRGB::Black;
      }
    }

    FastLED.show();
  }
}

void do_Segment_Static() {
  if (millis() - mil >= map(anispeed, 0, 100, 250, 0)) {
    mil = millis();
    spos++;
    if (spos >= 24)
      spos = 0;
    leds[0] = CRGB(static_r, static_g, static_b);
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      leds[i] = CRGB(static_r, static_g, static_b);
      if  (((i - 1) * (360 / 6) >= 15 * spos and ((i - 1) * (360 / 6)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 1 == 0) == 0)
          if ((spos == 21 and i - 1 == 0) == 0)
            if ((spos == 22 and i - 1 == 0) == 0)
              if ((spos == 23 and i - 1 == 0) == 0)
                leds[i] = CRGB::Black;
      }
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = CRGB(static_r, static_g, static_b);
      if  (((i - 7) * (360 / 12) >= 15 * spos and ((i - 7) * (360 / 12)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 7 == 0) == 0)
          if ((spos == 21 and i - 7 == 0) == 0)
            if ((spos == 22 and i - 7 == 0) == 0)
              if ((spos == 22 and i - 7 == 1) == 0)
                if ((spos == 23 and i - 7 == 0) == 0)
                  if ((spos == 23 and i - 7 == 1) == 0)
                    leds[i] = CRGB::Black;
      }
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = CRGB(static_r, static_g, static_b);
      if  (((i - 19) * (360 / 18) >= 15 * spos and ((i - 19) * (360 / 18)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 19 == 0) == 0)
          if ((spos == 21 and i - 19 == 0) == 0)
            if ((spos == 21 and i - 19 == 1) == 0)
              if ((spos == 22 and i - 19 == 0) == 0)
                if ((spos == 22 and i - 19 == 1) == 0)
                  if ((spos == 23 and i - 19 == 0) == 0)
                    if ((spos == 23 and i - 19 == 1) == 0)
                      if ((spos == 23 and i - 19 == 2) == 0)
                        leds[i] = CRGB::Black;
      }
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = CRGB(static_r, static_g, static_b);
      if  (((i - 37) * (360 / 24) >= 15 * spos and ((i - 37) * (360 / 24)) <= 15 * (spos + 4)) == 0) {
        if ((spos == 20 and i - 37 == 0) == 0)
          if ((spos == 21 and i - 37 == 0) == 0)
            if ((spos == 21 and i - 37 == 1) == 0)
              if ((spos == 22 and i - 37 == 0) == 0)
                if ((spos == 22 and i - 37 == 1) == 0)
                  if ((spos == 22 and i - 37 == 2) == 0)
                    if ((spos == 23 and i - 37 == 3) == 0)
                      if ((spos == 23 and i - 37 == 0) == 0)
                        if ((spos == 23 and i - 37 == 1) == 0)
                          if ((spos == 23 and i - 37 == 2) == 0)
                            leds[i] = CRGB::Black;
      }
    }

    FastLED.show();
  }
}


void do_Marquee_Rainbow() {
  if (millis() - mil >= map(anispeed, 0, 100, 1250, 250)) {
    mil = millis();
    leds[0] = ColorFromPalette( RainbowColors_p, 128, 255, LINEARBLEND);
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }
    for ( int i = 0; i < 61; i++) {
      if (random(2))

        leds[i] = CRGB::Black;
    }

    FastLED.show();
  }
}
void do_Marquee_MC() {
  if (millis() - mil >= map(anispeed, 0, 100, 1250, 250)) {
    mil = millis();
    fade(0, default_col_1, default_col_2, 50 );
    for ( int i = 1; i < 7; i++) {

      float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;

      fade(i, default_col_1, default_col_2, val );
    }
    for ( int i = 7; i < 19; i++) {

      float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
      fade(i, default_col_1, default_col_2, val );
    }


    for ( int i = 19; i < 37; i++) {

      float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
      fade(i, default_col_1, default_col_2, val );
    }
    for ( int i = 37; i < 61; i++) {

      float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
      fade(i, default_col_1, default_col_2, val );
    }
    for ( int i = 0; i < 61; i++) {
      if (random(2))

        leds[i] = CRGB::Black;
    }

    FastLED.show();
  }
}
void do_Marquee_Static() {
  if (millis() - mil >= map(anispeed, 0, 100, 1250, 250)) {
    mil = millis();

    for ( int i = 0; i < 61; i++) {
      if (random(2))
        leds[i] = CRGB(static_r, static_g, static_b);
      else
        leds[i] = CRGB::Black;
    }

    FastLED.show();
  }
}
void do_Strobe_Rainbow() {
  if (millis() - mil >= map(anispeed, 0, 100, 1500, 50)) {
    mil = millis();
    if (strpos) {
      strpos = 0;
      fill_solid(leds, 61, CRGB::Black);
    }
    else {
      strpos = 1;
      fill_solid(leds, 61, CRGB(random(255), random(255), random(255)));
    }
    FastLED.show();
  }
}
void do_Strobe_MC() {
  if (millis() - mil >= map(anispeed, 0, 100, 1500, 50)) {
    mil = millis();
    if (strpos) {
      strpos = 0;
      fill_solid(leds, 61, CRGB::Black);
    }
    else {
      strpos = 1;
      if (random(2))
        fill_solid(leds, 61, default_col_1);
      else
        fill_solid(leds, 61, default_col_2);
    }
    FastLED.show();
  }
}
void do_Strobe_Static() {
  if (millis() - mil >= map(anispeed, 0, 100, 1500, 50)) {
    mil = millis();
    if (strpos) {
      strpos = 0;
      fill_solid(leds, 61, CRGB::Black);
    }
    else {
      strpos = 1;
      fill_solid(leds, 61, CRGB(static_r, static_g, static_b));
    }
    FastLED.show();
  }
}
void do_Indiv_Jump_MC() {
  if (millis() - mil >= map(anispeed, 0, 100, 1250, 250)) {
    mil = millis();
    for ( int i = 0; i < 61; i++) {
      int val = random(5);
      if (val == 5)
        leds[i] = CRGB::Black;
      else if (val % 2 == 0)
        leds[i] = default_col_1;
      else
        leds[i] = default_col_2;

    }
    FastLED.show();
  }
}
void do_Indiv_Jump_Rainbow() {
  if (millis() - mil >= map(anispeed, 0, 100, 1250, 150)) {
    mil = millis();
    for ( int i = 0; i < 61; i++) {
      leds[i] = CRGB(randomcols[random(3)], randomcols[random(3)], randomcols[random(3)]);
    }
    FastLED.show();
  }
}
void do_All_Jump_Rainbow() {
  if (millis() - mil >= map(anispeed, 0, 100, 1250, 150)) {
    mil = millis();

    fill_solid(leds, 61, CRGB(random(255), random(255), random(255)));

    FastLED.show();
  }
}
void do_Linear_MC_Gradient_2() {
  if (millis() - mil >= 10) {
    mil = millis();
    fade(0, default_col_1, default_col_2, 50 );
    for ( int i = 1; i < 7; i++) {

      float val = (((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24   ;

      fade(i, default_col_1, default_col_2, val );
    }
    for ( int i = 7; i < 19; i++) {

      float val = (((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
      fade(i, default_col_1, default_col_2, val );
    }


    for ( int i = 19; i < 37; i++) {

      float val = (((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
      fade(i, default_col_1, default_col_2, val );
    }
    for ( int i = 37; i < 61; i++) {

      float val = (((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24   ;
      fade(i, default_col_1, default_col_2, val );
    }
    FastLED.show();

  }

}
void do_Linear_MC_Gradient_1() {
  if (millis() - mil >= 10) {
    mil = millis();
    colorIndex = startIndex;
    leds[0] = ColorFromPalette( MC_p, 128 + (180 - colorIndex), 255, LINEARBLEND);
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      leds[i] = ColorFromPalette( MC_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( MC_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( MC_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( MC_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    colorIndex += 28;
    FastLED.show();
    int wavspeedint = map(anispeed, 0, 100, 2, 30);
    float wavspeed = wavspeedint / 10.0;
    FastLED.show();
    startIndex = startIndex + wavspeed;  //how fast to move the palette down the strip
    if (startIndex > 255) {
      startIndex = 0;
    }
  }

}
void do_Linear_MC_Gradient_3() {
  if (millis() - mil >= 10) {
    mil = millis();
    colorIndex = startIndex;
    leds[0] = ColorFromPalette( MC_p, 128 + (180 - colorIndex), 255, LINEARBLEND);
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((180 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      leds[i] = ColorFromPalette( MC_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((180 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( MC_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((180 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( MC_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((180 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( MC_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    colorIndex += 28;
    FastLED.show();
    int wavspeedint = map(anispeed, 0, 100, 2, 30);
    float wavspeed = wavspeedint / 10.0;
    FastLED.show();
    startIndex = startIndex + wavspeed;  //how fast to move the palette down the strip
    if (startIndex > 255) {
      startIndex = 0;
    }
  }

}
void do_Linear_Rainbow_Gradient_1() {
  if (millis() - mil >= 10) {
    mil = millis();
    colorIndex = startIndex;
    leds[0] = ColorFromPalette( RainbowColors_p, 128 + (180 - colorIndex), 255, LINEARBLEND);
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      leds[i] = ColorFromPalette( RainbowColors_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    colorIndex += 28;
    FastLED.show();
    int wavspeedint = map(anispeed, 0, 100, 2, 30);
    float wavspeed = wavspeedint / 10.0;
    FastLED.show();
    startIndex = startIndex + wavspeed;  //how fast to move the palette down the strip
    if (startIndex > 255) {
      startIndex = 0;
    }
  }

}

void do_Linear_Rainbow_Gradient_2() {
  if (millis() - mil >= 10) {
    mil = millis();
    leds[0] = ColorFromPalette( RainbowColors_p, 128, 255, LINEARBLEND);
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((90 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((90 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((90 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((90 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val, 255, LINEARBLEND);
    }
    FastLED.show();

  }

}
void do_Linear_Rainbow_Gradient_3() {
  if (millis() - mil >= 10) {
    mil = millis();
    colorIndex = startIndex;
    leds[0] = ColorFromPalette( RainbowColors_p, 128 + (180 - colorIndex), 255, LINEARBLEND);
    for ( int i = 1; i < 7; i++) {

      float val = ((((3 * cos(((180 + ((i - 1) * (360.0 / 6.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55   ;
      leds[i] = ColorFromPalette( RainbowColors_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    for ( int i = 7; i < 19; i++) {

      float val = ((((6 * cos(((180 + ((i - 7) * (360.0 / 12.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }


    for ( int i = 19; i < 37; i++) {

      float val = ((((9 * cos(((180 + ((i - 19) * (360.0 / 18.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    for ( int i = 37; i < 61; i++) {

      float val = ((((12 * cos(((180 + ((i - 37) * (360.0 / 24.0))) * 71) / 4068 )) + 12) * 100) / 24) * 2.55    ;
      leds[i] = ColorFromPalette( RainbowColors_p, val + (180 - colorIndex), 255, LINEARBLEND);
    }
    colorIndex += 28;
    FastLED.show();
    int wavspeedint = map(anispeed, 0, 100, 2, 30);
    float wavspeed = wavspeedint / 10.0;
    FastLED.show();
    startIndex = startIndex + wavspeed;  //how fast to move the palette down the strip
    if (startIndex > 255) {
      startIndex = 0;
    }
  }

}
void do_Spiral_MC_Wave_1() {
  if (millis() - mil >= 10) {
    mil = millis();
    colorIndex = startIndex;
    for ( int i = 0; i < 1; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 1;  //how fast to advance through palette  3
    }
    colorIndex = startIndex;
    for ( int i = 1; i < 7; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 42;  //how fast to advance through palette  3
    }
    for ( int i = 7; i < 19; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 21;  //how fast to advance through palette  3
    }

    for ( int i = 19; i < 37; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 14;  //how fast to advance through palette  3
    }
    colorIndex = startIndex;
    for ( int i = 37; i < 61; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 11;  //how fast to advance through palette  3
    }
    int wavspeedint = map(anispeed, 0, 100, 2, 30);
    float wavspeed = wavspeedint / 10.0;
    FastLED.show();
    startIndex = startIndex + wavspeed;  //how fast to move the palette down the strip
    if (startIndex > 255) {
      startIndex = 0;
    }
  }//end_every_n
}

void do_Spiral_MC_Wave_2() {
  if (millis() - mil >= 10) {
    mil = millis();
    colorIndex = startIndex;
    for ( int i = 0; i < NUM_LEDS; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 4
                   ;  //how fast to advance through palette  3
    }
    int wavspeedint = map(anispeed, 0, 100, 2, 30);
    float wavspeed = wavspeedint / 10.0;
    FastLED.show();
    startIndex = startIndex + wavspeed;  //how fast to move the palette down the strip
    if (startIndex > 255) {
      startIndex = 0;
    }
  } //end_every_n
}

void do_Spiral_MC_Wave_3() {
  if (millis() - mil >= 10) {
    mil = millis();
    colorIndex = startIndex;
    for ( int i = 0; i < 1; i++) {
      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);
    }
    colorIndex = colorIndex + 51;  //how fast to advance through palette  3
    for ( int i = 1; i < 7; i++) {
      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);
    }
    colorIndex = colorIndex + 51;  //how fast to advance through palette  3
    for ( int i = 7; i < 19; i++) {
      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);
    }
    colorIndex = colorIndex + 51;  //how fast to advance through palette  3
    for ( int i = 19; i < 37; i++) {
      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);
    }
    colorIndex = colorIndex + 51;  //how fast to advance through palette  3
    for ( int i = 37; i < 61; i++) {
      leds[i] = ColorFromPalette( MC_p, colorIndex, 255, LINEARBLEND);
    }
    colorIndex = colorIndex + 51;  //how fast to advance through palette  3
    int wavspeedint = map(anispeed, 0, 100, 2, 30);
    float wavspeed = wavspeedint / 10.0;
    FastLED.show();
    startIndex = startIndex + wavspeed;  //how fast to move the palette down the strip
    if (startIndex > 255) {
      startIndex = 0;
    }
  }//end_every_n
}
void do_Spiral_Rainbow_Wave_1() {
  if (millis() - mil >= 10) {
    mil = millis();
    colorIndex = startIndex;
    for ( int i = 0; i < 1; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 1;  //how fast to advance through palette  3
    }
    colorIndex = startIndex;
    for ( int i = 1; i < 7; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 42;  //how fast to advance through palette  3
    }
    for ( int i = 7; i < 19; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 21;  //how fast to advance through palette  3
    }

    for ( int i = 19; i < 37; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 14;  //how fast to advance through palette  3
    }
    colorIndex = startIndex;
    for ( int i = 37; i < 61; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 11;  //how fast to advance through palette  3
    }
    int wavspeedint = map(anispeed, 0, 100, 2, 30);
    float wavspeed = wavspeedint / 10.0;
    FastLED.show();
    startIndex = startIndex + wavspeed;  //how fast to move the palette down the strip
    if (startIndex > 255) {
      startIndex = 0;
    }
  }//end_every_n
}
void do_Spiral_Rainbow_Wave_2() {
  if (millis() - mil >= 10) {
    mil = millis();
    colorIndex = startIndex;
    for ( int i = 0; i < NUM_LEDS; i++) {
      //ColorFromPalette( paletteName, colorIndex[0-255], brightness[0-255], blendType[NOBLEND or LINEARBLEND])


      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);

      colorIndex = colorIndex + 4
                   ;  //how fast to advance through palette  3
    }
    int wavspeedint = map(anispeed, 0, 100, 2, 30);
    float wavspeed = wavspeedint / 10.0;
    FastLED.show();
    startIndex = startIndex + wavspeed;  //how fast to move the palette down the strip
    if (startIndex > 255) {
      startIndex = 0;
    }
  }//end_every_n
}
void do_Spiral_Rainbow_Wave_3() {
  if (millis() - mil >= 10) {
    mil = millis();
    colorIndex = startIndex;
    for ( int i = 0; i < 1; i++) {
      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);
    }
    colorIndex = colorIndex + 51;  //how fast to advance through palette  3
    for ( int i = 1; i < 7; i++) {
      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);
    }
    colorIndex = colorIndex + 51;  //how fast to advance through palette  3
    for ( int i = 7; i < 19; i++) {
      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);
    }
    colorIndex = colorIndex + 51;  //how fast to advance through palette  3
    for ( int i = 19; i < 37; i++) {
      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);
    }
    colorIndex = colorIndex + 51;  //how fast to advance through palette  3
    for ( int i = 37; i < 61; i++) {
      leds[i] = ColorFromPalette( RainbowColors_p, colorIndex, 255, LINEARBLEND);
    }
    colorIndex = colorIndex + 51;  //how fast to advance through palette  3
    int wavspeedint = map(anispeed, 0, 100, 2, 30);
    float wavspeed = wavspeedint / 10.0;
    FastLED.show();
    startIndex = startIndex + wavspeed;  //how fast to move the palette down the strip
    if (startIndex > 255) {
      startIndex = 0;
    }
  }//end_every_n
}
void do_MC_Fade() {
  if (millis() - mil >= map(anispeed, 0, 100, 24, 1)) {
    mil = millis();

    if (P2S) {
      MCFadePos++;
      if (MCFadePos >= 100) {
        MCFadePos = 100;
        P2S = 0;
        S2P = 1;
      }
    }
    if (S2P) {
      MCFadePos--;
      if (MCFadePos <= 0) {
        MCFadePos = 0;
        P2S = 1;
        S2P = 0;
      }
    }
    fill_solid( leds, NUM_LEDS, CRGB(default_col_1.r + ((default_col_2.r - default_col_1.r) * (MCFadePos / 100.0)), default_col_1.g + ((default_col_2.g - default_col_1.g) * (MCFadePos / 100.0)), default_col_1.b + ((default_col_2.b - default_col_1.b) * (MCFadePos / 100.0)) ));
    FastLED.show();

  }
}

void do_Rainbow_Fade() {
  if (millis() - mil >= map(anispeed, 0, 100, 24, 1)) {
    mil = millis();
    fill_solid( leds, NUM_LEDS, CRGB(rainbow_r, rainbow_g, rainbow_b));
    FastLED.show();
    if (r2b) {
      rainbow_r -= rainbow_sp;
      rainbow_b += rainbow_sp;
      if (rainbow_r <= 0) {
        r2b = 0;
        b2g = 1;
        rainbow_r = 0;
        rainbow_b = 255;
      }
    }
    if (b2g) {
      rainbow_b -= rainbow_sp;
      rainbow_g += rainbow_sp;
      if (rainbow_b <= 0) {
        g2r = 1;
        b2g = 0;
        rainbow_b = 0;
        rainbow_g = 255;
      }
    }
    if (g2r) {
      rainbow_g -= rainbow_sp;
      rainbow_r += rainbow_sp;
      if (rainbow_g <= 0) {
        g2r = 0;
        r2b = 1;
        rainbow_g = 0;
        rainbow_r = 255;
      }
    }
  }
}





void fade(int pix, CRGB fade_from, CRGB fade_to, int per) {


  leds[pix].r = fade_from.r + ((fade_to.r - fade_from.r) * (per / 100.0)) ;
  leds[pix].g = fade_from.g + ((fade_to.g - fade_from.g) * (per / 100.0));
  leds[pix].b = fade_from.b + ((fade_to.b - fade_from.b) * (per / 100.0));
}
