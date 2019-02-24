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

void setup() {
  buttons[0].attach(PIN_BUTTON1, INPUT_PULLUP);
  buttons[1].attach(PIN_BUTTON2, INPUT_PULLUP);
  buttons[2].attach(PIN_BUTTON3, INPUT_PULLUP);
  for (Bounce& b: buttons) {
    b.interval(25);
  }

  pinMode(PIN_BUILTIN_LED, OUTPUT);
  FastLED.addLeds<WS2812, PIN_STRIP, GRB>(leds, NUM_LEDS);
}

void loop() {
  for (Bounce& b: buttons) {
    b.update();
  }

  if (buttons[0].fell()) {
    leds[0].setRGB(255, 0, 0);
  } else if (buttons[1].fell()) {
    leds[0].setRGB(0, 255, 0);
  } else if (buttons[2].fell()) {
    leds[0].setRGB(0, 0, 255);
  }

#if 0
  uint16_t x = beat16(60);
  int sector = x / (65536 / NUM_SIDES);
  for (int i = 0; i < NUM_SIDES; ++i) {
    if (i == sector) {
      uint8_t hue = beat8(120);
      leds[i * 2 + 0].setHSV(hue, 255, 255);
      leds[i * 2 + 1].setHSV(hue, 255, 255);
    } else {
      leds[i * 2 + 0].setRGB(0, 0, 0);
      leds[i * 2 + 1].setRGB(0, 0, 0);
    }
  }
#endif
  FastLED.show();
  FastLED.delay(1);
}
