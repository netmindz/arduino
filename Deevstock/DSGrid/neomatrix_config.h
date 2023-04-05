#ifndef neomatrix_config_h
#define neomatrix_config_h

/* There are 2 major backends
1) SmartMatrix (via SmartMatrix::GFX)
2) Not SmartMatrix (via FastLED::NeoMatrix or FastLED_SPITFT::GFX)

All backends end up using the same Framebuffer::GFX as the base class
but SmartMatrix is sufficiently different to need its own exceptions and handling
(for one, with SmartMatrix, the buffer is actually handled by SmartMatrix whereas
the other libraries define their own FastLED CRGB buffer (RGB888) ).

Backends you should choose from (define 1):
- SMARTMATRIX (if you are using the old SMARTMATRIX3, also define SMARTMATRIXV3)
- ILI9341
- ST7735_128b160
- ST7735_128b128
- SSD1331 (96x64 TFT)
- Everything below is NeoMatrix in different patterns:
  M24BY24 M32BY8X3 M16BY16T4 M64BY64 are 4 examples of NEOMATRIX defines
  (3 tiled 32x8, 4 tiled 16x16, and a single zigzag 64x64 array)
- ARDUINOONPC is auto defined by https://github.com/marcmerlin/ArduinoOnPc-FastLED-GFX-LEDMatrix
  - On ARM, we assume rPi and define RPIRGBPANEL
  - Elsewhere, we assume rendering on linux/X11
    - LINUX_RENDERER_X11 is the default with ArduinoOnPc-FastLED-GFX-LEDMatrix
    - LINUX_RENDERER_SDL can be defined in ArduinoOnPc-FastLED-GFX-LEDMatrix's Makefile

LEDMATRIX is a separate define you'd set before including this file and
adds the LEDMatrix API if you need it.
The TL;DR is you shouldn't bother with it if you already have the GFX
and FastLED APIs, unless you can use fancy wavy scrolling colored fonts or the
sprite support in LEDMatrix.

You should not need to modify this file at all unless you are adding new matrix
definitions and/or changing pin mappings for TFT screens. To choose which backend
to use, set the define before you include the file.
*/

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// README README README README README README README README README README  README README README README
// If you have never used FastLED::NeoMatrix before, please try these 2 examples first
// https://github.com/marcmerlin/FastLED_NeoMatrix/tree/master/examples/matrixtest
// https://github.com/marcmerlin/FastLED_NeoMatrix/tree/master/examples/MatrixGFXDemo
// Then just uncomment this line below (define M24BY24) and fix the matrix definition
// or use one of the other ones if they are closer ot your setup (M32BY8X3 M16BY16T4 M64BY64)
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//#define M24BY24



// #if !defined(M24BY24) && !defined(M32BY8X3) && !defined(M16BY16T4) && !defined(M64BY64) && !defined(SMARTMATRIX) && !defined(SSD1331) && !defined(ST7735_128b128) && !defined(ST7735_128b160) && !defined(ILI9341) && !defined(ARDUINOONPC)
//     #ifdef ESP8266
//     //#define SSD1331
//     //#define SSD1331_ROTATE 1
//     // ESP8266 shirt with neopixel strips
//     #define M32BY8X3
//     //#define M16BY16T4
//     #endif

//     #ifdef ESP32
//     //#define ILI9341
//     //#define ST7735_128b160
//     //#define ST7735_128b128
//     //#define SSD1331
//     //#define SSD1331_ROTATE 1
//     // #define SMARTMATRIX
//     //#define M64BY64
//     #endif

//     // Teensy 3.6
//     #ifdef __MK66FX1M0__
//     #define ILI9341
//     #define ILI_ROTATE 1
//     // If instead you are using the old SmartMatrix V3, define those 2
//     //#define SMARTMATRIX3
//     // And with SmartMatrix (v4), only this define is needed.
//     //#define SMARTMATRIX
//     #endif

//     // Teensy v.4
//     #ifdef __IMXRT1062__
//     //#define SMARTMATRIX3
//     // #define SMARTMATRIX
//     #endif
// #endif


#include <Adafruit_GFX.h>
bool init_done = 0;
uint32_t tft_spi_speed;
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// min/max are complicated. Arduino and ESP32 layers try to be helpful by using
// templates that take specific kinds of arguments, but those do not always work
// with mixed types:
// error: no matching function for call to 'max(byte&, int16_t&)'
// These defines get around this problem.
#define mmin(a,b) ((a<b)?(a):(b))
#define mmax(a,b) ((a>b)?(a):(b))

// The ESP32 FastLED defines below must be defined before FastLED.h is loaded
// They are not relevant if you don't actually use FastLED pixel output but cause
// no harm if we only include FastLED for its CRGB struct.
#ifdef ESP32
    // Allow infrared for old FastLED versions
    #define FASTLED_ALLOW_INTERRUPTS 1
    // Newer Samguyver ESP32 FastLED has a new I2S implementation that can be
    // better (or worse) than then default RMT which only supports 8 channels.
    #define FASTLED_ESP32_I2S
    #pragma message "Please use https://github.com/samguyer/FastLED.git if stock FastLED is unstable with ESP32"
#endif
#include <FastLED.h>

#ifdef LEDMATRIX
// Please use https://github.com/marcmerlin/LEDMatrix/ at lesat as recent as
// https://github.com/marcmerlin/LEDMatrix/commit/597ce703e924d45b2e676d6558c4c74a8ebc6991
// or https://github.com/Jorgen-VikingGod/LEDMatrix/commit/a11e74c8cd5b933021b6e15eb067280a52691449
// zero copy/no malloc code to work.
#include <LEDMatrix.h>
#endif


//============================================================================
// Ok, if you're doing matrices of displays, there is also a reasonable chance
// you'll be using SPIFFS or FATFS on flash, or an sdcard, so let's define it
// here (NeoMatrix-FastLED-IR actually also uses this to read a config file)
//============================================================================

// control if we decode in 32x32 or 64x64, or something else
#ifdef ESP8266
#define gif_size 32
#else
#define gif_size 64
#endif

// Note, you can use an sdcard on ESP32 or ESP8266 if you really want,
// but if your data fits in built in flash, why not use it?
// Use built in flash via SPIFFS/FATFS
// esp8266com/esp8266/libraries/SD/src/File.cpp
// ESP8266: http://esp8266.github.io/Arduino/versions/2.3.0/doc/filesystem.html#uploading-files-to-file-system
// ESP32: https://github.com/me-no-dev/arduino-esp32fs-plugin
// https://github.com/marcmerlin/esp32_fatfsimage/blob/master/README.md
#if defined(ESP8266)
    #define FS_PREFIX ""
    #include <FS.h>
    #define FSO SPIFFS
    #define FSOSPIFFS
    #if gif_size == 64
        #define GIF_DIRECTORY FS_PREFIX "/gifs64/"
    #else
        #define GIF_DIRECTORY FS_PREFIX "/gifs/"
    #endif
    extern "C" {
        #include "user_interface.h"
    }
#elif defined(ESP32)
    #define FS_PREFIX ""
    //#include <SPIFFS.h>
    //#define FSOSPIFFS
    //#define FSO SPIFFS
    //#include "FFat.h"
    #define FSO FFat
    #define FSOFAT
    // Do NOT add a trailing slash, or things will fail
    #if gif_size == 64
        #define GIF_DIRECTORY FS_PREFIX "/gifs64"
    #else
        #define GIF_DIRECTORY FS_PREFIX "/gifs"
    #endif
#else
    #define FS_PREFIX ""
    #define FSO SD
    #define FSOSD
    #if defined (ARDUINO)
    #include <SD.h>
    #endif
    // Chip select for SD card on the SmartMatrix Shield or Photon
    // Teensy 3.5/3.6
    #if defined(__MK64FX512__) || defined(__MK66FX1M0__)
        #define SD_CS BUILTIN_SDCARD
    #elif defined(ESP32)
        #define SD_CS 5
    #elif defined (ARDUINO)
        #define SD_CS 15
        //#define SD_CS BUILTIN_SDCARD
    #elif defined (SPARK)
        #define SD_CS SS
    #endif

    // Teensy SD Library requires a trailing slash in the directory name
    #if gif_size == 64
        #define GIF_DIRECTORY FS_PREFIX "/gifs64/"
    #else
        #define GIF_DIRECTORY FS_PREFIX "/gifs/"
    #endif
#endif

//============================================================================
// Matrix defines (SMARTMATRIX vs NEOMATRIX and size)
// You should #define one and only one of them and if you need to edit it,
// edit both the block below and the 2nd block in setup() at the bottom of this file
//============================================================================
//
//----------------------------------------------------------------------------
#if defined(SMARTMATRIX)
    // CHANGEME for ESP32, see MatrixHardware_ESP32_V0.h in SmartMatrix/src
//    #define GPIOPINOUT 8
    // This is defined by you before including this file if you are using the old SmartMatrixv3
    #ifdef SMARTMATRIXV3
        #include <SmartLEDShieldV4.h>
        #include <SmartMatrix3.h>
    #else // As of 2020/11, SmartMatrix v4 has a new interface
        // https://community.pixelmatix.com/t/smartmatrix-library-4-0-changes-to-matrixhardware-includes/709/9
        #ifdef ESP32
            // This saves RAM but could make your code unstable if you do Flash + Wifi + PSRAM
            //#define SMARTMATRIX_USE_PSRAM
            #include <MatrixHardware_ESP32_V0.h> // ESP32
        #elif __IMXRT1062__ // Teensy 4.0/4.1
            #include <MatrixHardware_Teensy4_ShieldV4Adapter.h> // Teensy 4 Adapter attached to SmartLED Shield for Teensy 3 (V4)
            //#include <MatrixHardware_Teensy4_ShieldV5.h>        // SmartLED Shield for Teensy 4 (V5)
        #else
            #include <MatrixHardware_Teensy3_ShieldV4.h>        // SmartLED Shield for Teensy 3 (V4)
            //#include <MatrixHardware_Teensy3_ShieldV1toV3.h>    // SmartMatrix Shield for Teensy 3 V1-V3
        #endif
        #include <SmartMatrix.h>
    #endif
    #include <SmartMatrix_GFX.h>
    uint8_t matrix_brightness = 255;

    #ifdef ESP32
    #pragma message "SmartMatrix for ESP32 with SMARTMATRIX_HUB75_64ROW_MOD32SCAN"
    const uint8_t kPanelType = SMARTMATRIX_HUB75_64ROW_MOD32SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
    const uint16_t MATRIX_TILE_WIDTH = 64; // width of EACH NEOPIXEL MATRIX (not total display)
    const uint16_t MATRIX_TILE_HEIGHT= 64; // height of each matrix
    #elif defined(__MK66FX1M0__) // my teensy 3.6 is connected to a 64x64 panel
    #pragma message "SmartMatrix for Teensy with 64x64 32 scan panel"
    //const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
    const uint8_t kPanelType = SMARTMATRIX_HUB75_64ROW_MOD32SCAN;
    const uint16_t MATRIX_TILE_WIDTH = 64; // width of EACH NEOPIXEL MATRIX (not total display)
    const uint16_t MATRIX_TILE_HEIGHT= 64; // height of each matrix
    #elif defined(__IMXRT1062__) // teensy v.4
    const uint8_t kPanelType = SMARTMATRIX_HUB75_64ROW_MOD32SCAN;
    const uint16_t MATRIX_TILE_WIDTH = 64; // width of EACH NEOPIXEL MATRIX (not total display)
    const uint16_t MATRIX_TILE_HEIGHT= 64; // height of each matrix
    #pragma message "SmartMatrix for Teensy 4.0 with 64x64 32 scan panel"
    #else
    #error Unknown architecture (not ESP32 or teensy 3.5/6 or teensy 4.0, please write a panel config)
    #endif
    // Used by LEDMatrix
    const uint8_t MATRIX_TILE_H     = 1;  // number of matrices arranged horizontally
    const uint8_t MATRIX_TILE_V     = 1;  // number of matrices arranged vertically

    // Used by NeoMatrix
    const uint16_t mw = MATRIX_TILE_WIDTH *  MATRIX_TILE_H;
    const uint16_t mh = MATRIX_TILE_HEIGHT * MATRIX_TILE_V;

    /// SmartMatrix Defines
    #define COLOR_DEPTH 24         // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
    const uint8_t kMatrixWidth = mw;
    const uint8_t kMatrixHeight = mh;
    const uint8_t kRefreshDepth = 24;       // known working: 24, 36, 48
    const uint8_t kDmaBufferRows = 2;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
    const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_FM6126A_RESET_AT_START);      // see http://docs.pixelmatix.com/SmartMatrix for options
    //const uint8_t kMatrixOptions = 0;
    const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);

    SMARTMATRIX_ALLOCATE_BUFFERS(matrixLayer, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
    SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);

    #ifdef LEDMATRIX
    // cLEDMatrix defines
    cLEDMatrix<MATRIX_TILE_WIDTH, -MATRIX_TILE_HEIGHT, HORIZONTAL_MATRIX,
        MATRIX_TILE_H, MATRIX_TILE_V, HORIZONTAL_BLOCKS> ledmatrix(false);
    #endif
    CRGB *matrixleds;

    void show_callback();
    SmartMatrix_GFX *matrix = new SmartMatrix_GFX(matrixleds, mw, mh, show_callback);

    // Sadly this callback function must be copied around with this init code
    void show_callback() {
    //    memcpy(backgroundLayer.backBuffer(), matrixleds, kMatrixHeight*kMatrixWidth*3);
    //    backgroundLayer.swapBuffers(false);
        backgroundLayer.swapBuffers(true);
        //matrixleds = (CRGB *)backgroundLayer.getRealBackBuffer());
        matrixleds = (CRGB *)backgroundLayer.backBuffer();
        matrix->newLedsPtr(matrixleds);
    #ifdef LEDMATRIX
        ledmatrix.SetLEDArray(matrixleds);
    #endif
        matrix->showfps();
    }

//----------------------------------------------------------------------------
#elif defined(APA1023030)
    #include <FastLED_NeoMatrix.h>
    #define FASTLED_NEOMATRIX

    uint8_t matrix_brightness = 64;
    // Used by LEDMatrix
    const uint16_t MATRIX_TILE_WIDTH = 30; // width of EACH NEOPIXEL MATRIX (not total display)
    const uint16_t MATRIX_TILE_HEIGHT= 30; // height of each matrix
    const uint8_t MATRIX_TILE_H     = 1;  // number of matrices arranged horizontally
    const uint8_t MATRIX_TILE_V     = 1;  // number of matrices arranged vertically

    // Used by NeoMatrix
    const uint16_t mw = MATRIX_TILE_WIDTH *  MATRIX_TILE_H;
    const uint16_t mh = MATRIX_TILE_HEIGHT * MATRIX_TILE_V;

    CRGB *matrixleds;
    #ifdef LEDMATRIX
    // cLEDMatrix defines
    // Unfortunately LEDMatrix has its own matrix definition that isn't as well documented
    // and easy to use. Look for examples if you need to setup a matrix of matrices.
    cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, VERTICAL_ZIGZAG_MATRIX> ledmatrix(false);
    #endif

    // MATRIX DECLARATION:
    // Parameter 1 = width of EACH NEOPIXEL MATRIX (not total display)
    // Parameter 2 = height of each matrix
    // Parameter 3 = number of matrices arranged horizontally
    // Parameter 4 = number of matrices arranged vertically
    // Parameter 5 = pin number (most are valid)
    // Parameter 6 = matrix layout flags, add together as needed:
    //   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
    //     Position of the FIRST LED in the FIRST MATRIX; pick two, e.g.
    //     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
    //   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs WITHIN EACH MATRIX are
    //     arranged in horizontal rows or in vertical columns, respectively;
    //     pick one or the other.
    //   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns WITHIN
    //     EACH MATRIX proceed in the same order, or alternate lines reverse
    //     direction; pick one.
    //   NEO_TILE_TOP, NEO_TILE_BOTTOM, NEO_TILE_LEFT, NEO_TILE_RIGHT:
    //     Position of the FIRST MATRIX (tile) in the OVERALL DISPLAY; pick
    //     two, e.g. NEO_TILE_TOP + NEO_TILE_LEFT for the top-left corner.
    //   NEO_TILE_ROWS, NEO_TILE_COLUMNS: the matrices in the OVERALL DISPLAY
    //     are arranged in horizontal rows or in vertical columns, respectively;
    //     pick one or the other.
    //   NEO_TILE_PROGRESSIVE, NEO_TILE_ZIGZAG: the ROWS/COLUMS OF MATRICES
    //     (tiles) in the OVERALL DISPLAY proceed in the same order for every
    //     line, or alternate lines reverse direction; pick one.  When using
    //     zig-zag order, the orientation of the matrices in alternate rows
    //     will be rotated 180 degrees (this is normal -- simplifies wiring).
    //   See example below for these values in action.
    FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT,
        NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
        NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG );
//============================================================================
#elif defined(WS1616)
    #include <FastLED_NeoMatrix.h>
    #define FASTLED_NEOMATRIX

    uint8_t matrix_brightness = 64;
    // Used by LEDMatrix
    const uint16_t MATRIX_TILE_WIDTH = 16; // width of EACH NEOPIXEL MATRIX (not total display)
    const uint16_t MATRIX_TILE_HEIGHT= 16; // height of each matrix
    const uint8_t MATRIX_TILE_H     = 1;  // number of matrices arranged horizontally
    const uint8_t MATRIX_TILE_V     = 1;  // number of matrices arranged vertically

    // Used by NeoMatrix
    const uint16_t mw = MATRIX_TILE_WIDTH *  MATRIX_TILE_H;
    const uint16_t mh = MATRIX_TILE_HEIGHT * MATRIX_TILE_V;

    CRGB *matrixleds;
    #ifdef LEDMATRIX
    // cLEDMatrix defines
    // Unfortunately LEDMatrix has its own matrix definition that isn't as well documented
    // and easy to use. Look for examples if you need to setup a matrix of matrices.
    cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, VERTICAL_ZIGZAG_MATRIX> ledmatrix(false);
    #endif

    // MATRIX DECLARATION:
    // Parameter 1 = width of EACH NEOPIXEL MATRIX (not total display)
    // Parameter 2 = height of each matrix
    // Parameter 3 = number of matrices arranged horizontally
    // Parameter 4 = number of matrices arranged vertically
    // Parameter 5 = pin number (most are valid)
    // Parameter 6 = matrix layout flags, add together as needed:
    //   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
    //     Position of the FIRST LED in the FIRST MATRIX; pick two, e.g.
    //     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
    //   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs WITHIN EACH MATRIX are
    //     arranged in horizontal rows or in vertical columns, respectively;
    //     pick one or the other.
    //   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns WITHIN
    //     EACH MATRIX proceed in the same order, or alternate lines reverse
    //     direction; pick one.
    //   NEO_TILE_TOP, NEO_TILE_BOTTOM, NEO_TILE_LEFT, NEO_TILE_RIGHT:
    //     Position of the FIRST MATRIX (tile) in the OVERALL DISPLAY; pick
    //     two, e.g. NEO_TILE_TOP + NEO_TILE_LEFT for the top-left corner.
    //   NEO_TILE_ROWS, NEO_TILE_COLUMNS: the matrices in the OVERALL DISPLAY
    //     are arranged in horizontal rows or in vertical columns, respectively;
    //     pick one or the other.
    //   NEO_TILE_PROGRESSIVE, NEO_TILE_ZIGZAG: the ROWS/COLUMS OF MATRICES
    //     (tiles) in the OVERALL DISPLAY proceed in the same order for every
    //     line, or alternate lines reverse direction; pick one.  When using
    //     zig-zag order, the orientation of the matrices in alternate rows
    //     will be rotated 180 degrees (this is normal -- simplifies wiring).
    //   See example below for these values in action.
    FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT,
        NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
        NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG );
//============================================================================
#else
    #error "Please write a matrix config or choose one of the definitions above. If you have a FastLED matrix, define M24BY24 at the top of this file"
#endif

//============================================================================
// End Matrix defines (SMARTMATRIX vs NEOMATRIX and size)
//============================================================================

// Compat for some other demos
const uint32_t NUMMATRIX = mw*mh;
const uint32_t NUM_LEDS = NUMMATRIX;
const uint16_t MATRIX_HEIGHT = mh;
const uint16_t MATRIX_WIDTH = mw;

// Compat with SmartMatrix code that uses those variables
// (but don't redefine for SmartMatrix backend)
#ifndef SMARTMATRIX
const uint16_t kMatrixWidth = mw;
const uint16_t kMatrixHeight = mh;
#endif

#ifdef ESP8266
// Turn off Wifi in setup()
// https://www.hackster.io/rayburne/esp8266-turn-off-wifi-reduce-current-big-time-1df8ae
//
#include "ESP8266WiFi.h"
extern "C" {
#include "user_interface.h"
}
#endif // ESP8266

float matrix_gamma = 1; // higher number is darker, needed for Neomatrix more than SmartMatrix

// Like XY, but for a mirror image from the top (used by misconfigured code)
int XY2( int x, int y, bool wrap=false) {
    wrap = wrap; // squelch compiler warning
    return matrix->XY(x,MATRIX_HEIGHT-1-y);
}

// FastLED::colorutils needs a signature with uint8_t
uint16_t XY( uint8_t x, uint8_t y) {
    return matrix->XY(x,y);
}
// but x/y can be bigger than 256
uint16_t XY16( uint16_t x, uint16_t y) {
    return matrix->XY(x,y);
}

int wrapX(int x) {
    if (x < 0 ) return 0;
    if (x >= MATRIX_WIDTH) return (MATRIX_WIDTH-1);
    return x;
}

void show_free_mem(const char *pre=NULL) {
    Framebuffer_GFX::show_free_mem(pre);
}

void die(const char *mesg) {
    Serial.println(mesg);
    while(1) delay((uint32_t)1); // while 1 loop only triggers watchdog on ESP chips
}

void *mallocordie(const char *varname, uint32_t req, bool psram=true) {
    // If varname starts with @, show debug for the allocation
    void *mem;

#ifndef BOARD_HAS_PSRAM
    psram = false;
#endif
    if (varname[0] == '@') {
        if (psram) Serial.print("PS");
        Serial.print("Malloc ");
        Serial.print(varname);
        Serial.print(" . Requested bytes: ");
        Serial.println(req);
    }
#ifdef ESP32
    if (psram) {
        mem = ps_malloc(req);
    } else {
        mem = malloc(req);
    }
#else
    mem = malloc(req);
#endif

    if (mem) {
        return mem;
    } else {
        Serial.print("FATAL: ");
        if (psram) Serial.print("ps_");
        Serial.print("malloc failed for ");
        Serial.println(varname);
        show_free_mem();
        while (1); // delay(1);  Adding this seems to cause an ESP32 bug
    }
    return NULL;
}

void matrix_setup(bool initserial=true, int reservemem = 40000) {
    reservemem = reservemem; // squelch compiler warning if var is unused.

    if (init_done) {
        Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> BUG: matrix_setup called twice");
        return;
    }
    init_done = 1;

    // It's bad to call Serial.begin twice, if your calling script runs it first, make sure
    // you request that it isn't called a 2nd time here.
    if (initserial) {
        Serial.begin(115200);
        Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Serial.begin");
    }

    // Teensy takes a while to initialize serial port.
    // Teensy 3.0, 3.1/3.2, 3.5, 3.6, 4.0+4.1
    // https://docs.platformio.org/en/latest/platforms/teensy.html
    #if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)
        delay(3000);
    #endif
    show_free_mem("Memory after setup() starts");

    // Smartmatrix defines the framebuffers itself. Other methods make their own allocation here
    #if !defined(SMARTMATRIX)
        // ESP32 has more memory available for allocation in setup than in global
        // (if this were a global array), so we use malloc here.
        // https://forum.arduino.cc/index.php?topic=647833
        matrixleds = (CRGB *) mallocordie("matrixleds", sizeof(CRGB) * NUMMATRIX, 1);
        // and then fix the until now NULL pointer in the object.
        matrix->newLedsPtr(matrixleds);
        show_free_mem("After matrixleds malloc");
        #ifdef LEDMATRIX
            ledmatrix.SetLEDArray(matrixleds);
        #endif
    #endif


    #if defined(SMARTMATRIX)
        matrix_gamma = 1; // SmartMatrix should be good by default.
        matrixLayer.addLayer(&backgroundLayer);
        // SmartMatrix takes all the RAM it can get its hands on. Get it to leave some
        // free RAM so that other libraries can work too.
        #ifdef ESP32
            if (reservemem) matrixLayer.begin(reservemem); else matrixLayer.begin();
        #else
            matrixLayer.begin();
        #endif
        // This sets the neomatrix and LEDMatrix pointers
        show_callback();
        matrixLayer.setRefreshRate(240);
        backgroundLayer.enableColorCorrection(true);
        Serial.print(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SmartMatrix GFX output, total LEDs: ");
        Serial.println(NUMMATRIX);
        // Quick hello world test
        #ifndef DISABLE_MATRIX_TEST
            Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SmartMatrix Grey Demo");
            backgroundLayer.fillScreen( {0x80, 0x80, 0x80} );
            // backgroundLayer.swapBuffers();
            show_callback();
            delay(1000);
        #endif
        Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SmartMatrix Init Done");

    //============================================================================================
    #elif defined(APA1023030)
        #define COLOR_ORDER BGR
        FastLED.addLeds<APA102,7, 14, COLOR_ORDER, DATA_RATE_MHZ(8)>(matrixleds, NUMMATRIX).setCorrection(TypicalLEDStrip);
        Serial.print("Neomatrix total APA102 LEDs: ");
        Serial.print(NUMMATRIX);
        Serial.println();
    //============================================================================================
    #elif defined(WS1616)
        #define COLOR_ORDER BGR
        FastLED.addLeds<WS2811,18, COLOR_ORDER>(matrixleds, NUMMATRIX).setCorrection(TypicalLEDStrip);
        Serial.print("Neomatrix total WS2811 LEDs: ");
        Serial.print(NUMMATRIX);
        Serial.println();
   //============================================================================================
    #else
        #error "Undefined Matrix"
    #endif
    //============================================================================================
    // Matrix Init End
    //============================================================================================

    show_free_mem("Before matrix->begin");
    matrix->begin();
    //show_free_mem("After matrix->begin");

    Serial.print("Setting Brightness: ");
    Serial.println(matrix_brightness);
    #if defined(SMARTMATRIX)
        matrixLayer.setBrightness(matrix_brightness);
    #else
        matrix->setBrightness(matrix_brightness);
    #endif
    Serial.print("Gamma Correction: ");
    Serial.println(matrix_gamma);
    // Gamma is used by AnimatedGIFs and others, as such:
    // CRGB color = CRGB(matrix->gamma[red], matrix->gamma[green], matrix->gamma[blue]);
    matrix->precal_gamma(matrix_gamma);

// LEDMatrix alignment is tricky, this test helps make sure things are aligned correctly
    #ifndef DISABLE_MATRIX_TEST
        #ifdef LEDMATRIX
            Serial.println("LEDMatrix Test");
            ledmatrix.DrawLine (0, 0, ledmatrix.Width() - 1, ledmatrix.Height() - 1, CRGB(0, 255, 0));
            ledmatrix.DrawPixel(0, 0, CRGB(255, 0, 0));
            ledmatrix.DrawPixel(ledmatrix.Width() - 1, ledmatrix.Height() - 1, CRGB(0, 0, 255));

            ledmatrix.DrawLine (ledmatrix.Width() - 5, 4, 4, ledmatrix.Height() - 5, CRGB(128, 128, 128));
            ledmatrix.DrawPixel(ledmatrix.Width() - 5, 4,  CRGB(255, 64, 64));
            ledmatrix.DrawPixel(4, ledmatrix.Height() - 5, CRGB(64, 64, 255));
            matrix->show();
            delay(1000);
        #endif
    #endif

    Serial.println("matrix_setup done");

    // ESP32 speed tests
    // - WROVER at 24Mhz is 25fps, doesn't seem to work any faster
    // - https://github.com/loboris/ESP32_TFT_lib (DMA) at 24Mhz is only 7fps?
    //
    // ILI9314: 80Mhz: TFT 40fps, NO PSRAM: 32fps, PSRAM show: 12ps  => unstable, no display
    // ILI9314: 40Mhz: TFT 25fps, NO PSRAM: 21fps, PSRAM show: 10fps => unstable, no display
    // ILI9314: 39Mhz: TFT 18fps, NO PSRAM: 16fps, PSRAM show:  9fps => unstable, garbled
    // ILI9314: 30Mhz: TFT 18fps, NO PSRAM: 16fps, PSRAM show:  9fps => still a bit unstabled, garbled
    // ILI9314: 24Mhz: TFT 14fps, NO PSRAM: 12fps, PSRAM show:  8fps => stable
    // ILI9314: 20Mhz: TFT 14fps, NO PSRAM: 12fps, PSRAM show:  8fps
    //
    // ST7735_128b160: 80Mhz: TFT153fps, NO PSRAM:104fps, PSRAM show: 45fps => unstable, no display
    // ST7735_128b160: 60Mhz: TFT 93fps, NO PSRAM: 73fps, PSRAM show: 38fps
    // ST7735_128b160: 60Mhz: TFT 96fps, NO PSRAM: 52fps
    // ST7735_128b160: 40Mhz: TFT 68fps, NO PSRAM: 56fps, PSRAM show: 32fps
    // ST7735_128b160: 20Mhz: TFT 53fps, NO PSRAM: 45fps, PSRAM show: 29fps
    //
    // ST7735_128b128: 60Mhz: TFT117fps, NO PSRAM: 90fps, PSRAM show: 48fps => unstable, garbled
    // ST7735_128b128: 40Mhz: TFT117fps, NO PSRAM: 90fps, PSRAM show: 48fps => unstable, garbled
    // ST7735_128b128: 32Mhz: TFT 84fps, NO PSRAM: 70fps, PSRAM show: 41fps => stable
    // ST7735_128b128: 20Mhz: TFT 66fps, NO PSRAM: 56fps, PSRAM show: 36fps
    //
    // SSD1331: SWSPI: TFT  9fps, NO PSRAM:  9fps, PSRAM show:  8fps => stable
    uint32_t before;
    #ifdef HASTFT
        before = millis();
        for (uint8_t i=0; i<3; i++) {
            tft->fillScreen(0);
            tft->fillScreen(0xFFFF);
        }
        Serial.print("TFT SPI Speed: ");
        Serial.print(tft_spi_speed/1000000);
        Serial.print("Mhz. Resulting fps: ");
        Serial.println((6* 1000/(millis() - before)));
    #endif
    #if defined(BOARD_HAS_PSRAM) && defined(HASTFT)
        before = millis();
        for (uint8_t i=0; i<5; i++) {
            matrix->show(1);
        }
        Serial.print("Matrix->show() PSRAM BYPASS Speed Test fps: ");
        Serial.println((5* 1000/(millis() - before)));
    #endif
    before = millis();
    for (uint8_t i=0; i<5; i++) {
        #if defined(BOARD_HAS_PSRAM) && defined(HASTFT)
            matrix->show(0);
        #else
            matrix->show();
        #endif
    }
    Serial.print("Matrix->show() Speed Test fps: ");
    Serial.println((5* 1000/(millis() - before)));

    // At least on teensy, due to some framework bug it seems, early
    // serial output gets looped back into serial input
    // Hence, flush input.
    while(Serial.available() > 0) { char t = Serial.read(); t = t; }
}
#endif // neomatrix_config_h
// vim:sts=4:sw=4:et
