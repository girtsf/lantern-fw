#include <Bounce2.h>
#include <FastLED.h>
#include "Arduino.h"

#define PIN_BUILTIN_LED 13
#define PIN_STRIP 12
#define PIN_BUTTON1 A0
#define PIN_BUTTON2 A1
#define PIN_BUTTON3 A2

#define NUM_SIDES 6
#define NUM_LEDS (NUM_SIDES * 2)
#define NUM_BUTTONS 3

// LED strip.
CRGB leds[NUM_LEDS];

// Buttons.
Bounce buttons[NUM_BUTTONS];

// From https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
const uint8_t GAMMA[] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,
    2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,
    4,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,
    8,   9,   9,   9,   10,  10,  10,  11,  11,  11,  12,  12,  13,  13,  13,
    14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,  20,  21,
    21,  22,  22,  23,  24,  24,  25,  25,  26,  27,  27,  28,  29,  29,  30,
    31,  32,  32,  33,  34,  35,  35,  36,  37,  38,  39,  39,  40,  41,  42,
    43,  44,  45,  46,  47,  48,  49,  50,  50,  51,  52,  54,  55,  56,  57,
    58,  59,  60,  61,  62,  63,  64,  66,  67,  68,  69,  70,  72,  73,  74,
    75,  77,  78,  79,  81,  82,  83,  85,  86,  87,  89,  90,  92,  93,  95,
    96,  98,  99,  101, 102, 104, 105, 107, 109, 110, 112, 114, 115, 117, 119,
    120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142, 144, 146,
    148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175, 177,
    180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
    215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252,
    255};

const uint8_t VALS[256] = {
    0xbf, 0xb5, 0xb6, 0x7f, 0xa8, 0xc5, 0xac, 0x49, 0x52, 0x68, 0x80, 0x78,
    0x9e, 0x5a, 0x51, 0xa9, 0x5a, 0x79, 0x5e, 0x4d, 0x6c, 0x60, 0x4d, 0x7a,
    0x4b, 0xf,  0x3e, 0x4b, 0x32, 0x35, 0x20, 0x24, 0x38, 0x2a, 0x6,  0x1c,
    0x46, 0x4b, 0x55, 0x8d, 0x50, 0x91, 0x4f, 0x7b, 0x79, 0xc7, 0xe7, 0xea,
    0xa8, 0x8f, 0xcb, 0xd1, 0xf3, 0xc5, 0xcf, 0x9e, 0xf1, 0xb7, 0xba, 0xb3,
    0x97, 0x75, 0x6c, 0x5b, 0x61, 0x88, 0x84, 0x90, 0x84, 0x76, 0xb8, 0xaa,
    0x9a, 0x95, 0x99, 0xa7, 0x78, 0xb7, 0xc2, 0x77, 0x83, 0x9e, 0x8e, 0xee,
    0xe7, 0xb1, 0x78, 0x98, 0x81, 0xa6, 0xb2, 0x59, 0x84, 0xba, 0x91, 0x6b,
    0x19, 0x43, 0x8e, 0xa6, 0xd7, 0xb0, 0xd0, 0xa6, 0x76, 0x53, 0x71, 0x30,
    0x36, 0x23, 0x69, 0x96, 0x75, 0x7a, 0x94, 0x82, 0x87, 0xa6, 0xf4, 0xe4,
    0xf6, 0x9a, 0x70, 0x68, 0x58, 0x56, 0x5a, 0x51, 0x32, 0x85, 0x6a, 0x99,
    0x99, 0x81, 0x84, 0x83, 0x9b, 0x8e, 0x95, 0xba, 0xab, 0x83, 0x8e, 0x66,
    0x79, 0x6e, 0x65, 0x94, 0xb0, 0x71, 0xa7, 0xa1, 0x85, 0xac, 0x7c, 0x8b,
    0x9a, 0xa2, 0xc3, 0xb8, 0xac, 0x4f, 0xac, 0x5c, 0x83, 0x82, 0x6d, 0x98,
    0x66, 0x73, 0x81, 0x8c, 0x6a, 0x6e, 0x53, 0x67, 0x89, 0x63, 0x7a, 0x83,
    0x7c, 0x94, 0xd0, 0xbf, 0xab, 0x6d, 0x58, 0x3,  0x0,  0x51, 0x71, 0x52,
    0x25, 0x46, 0x15, 0x36, 0x12, 0x2e, 0x3a, 0x2e, 0x33, 0x7b, 0x6c, 0x6d,
    0x7d, 0x4c, 0x48, 0x5c, 0x79, 0xaa, 0xb9, 0x81, 0xc6, 0xba, 0x7c, 0x65,
    0x66, 0x50, 0x59, 0x70, 0x47, 0x62, 0x7f, 0x34, 0x93, 0xbc, 0x60, 0x7f,
    0x35, 0x89, 0x74, 0x7c, 0xb0, 0xcb, 0xb4, 0xc1, 0xc6, 0xbb, 0xa6, 0x90,
    0xd3, 0xc0, 0xcc, 0xa4, 0xb9, 0xff, 0xd6, 0xc7, 0xbd, 0x86, 0xb1, 0xaa,
    0xbc, 0x9e, 0x9f, 0x8f};

// Which LEDs are on the bottom in each pane.
const bool IS_BOTTOM[NUM_LEDS] = {true,  false, false, true,  true,  false,
                                  false, true,  true,  false, false, true};

void setup() {
  buttons[0].attach(PIN_BUTTON1, INPUT_PULLUP);
  buttons[1].attach(PIN_BUTTON2, INPUT_PULLUP);
  buttons[2].attach(PIN_BUTTON3, INPUT_PULLUP);
  for (Bounce& b : buttons) {
    b.interval(25);
  }

  pinMode(PIN_BUILTIN_LED, OUTPUT);
  FastLED.addLeds<WS2812, PIN_STRIP, GRB>(leds, NUM_LEDS);
}

// Fire mode: redish/yellowish shimmers.
void fire() {
  static uint8_t idx = 0;
  static uint8_t from = VALS[0];
  static uint8_t to = VALS[0];

  static uint32_t next_time = 0;

  uint32_t cur_time = millis();
  if (cur_time > next_time) {
    next_time = millis() + 128;

    from = to;
    to = map8(VALS[idx++], 128, 250);
  }

  uint8_t cur_step = cur_time % 128;
  cur_step *= 2;
  uint8_t val;
  if (to > from) {
    val = map8(cur_step, from, to);
  } else {
    val = map8(255 - cur_step, to, from);
  }
  for (int i = 0; i < NUM_LEDS; ++i) {
    uint8_t hue;
    uint8_t val2;
    if (IS_BOTTOM[i]) {
      hue = random8(15, 20);
      val2 = val - 4 + random(0, 8);
    } else {
      hue = random8(15, 30);
      val2 = val - random(32, 64);
    }
    leds[i].setHSV(hue, 255, GAMMA[val2]);
  }
}

void count() {
  uint8_t lit = (millis() / 512) % NUM_LEDS;
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i].setHSV(0, 255, i == lit ? 255 : 0);
  }
}

void loop() {
  for (Bounce& b : buttons) {
    b.update();
  }

  // if (buttons[0].fell()) {
  //   leds[0].setRGB(255, 0, 0);
  // } else if (buttons[1].fell()) {
  //   leds[0].setRGB(0, 255, 0);
  // } else if (buttons[2].fell()) {
  //   leds[0].setRGB(0, 0, 255);
  // }
  fire();
  // count();

  FastLED.show();
  FastLED.delay(1);  // random8(5, 25));
}
