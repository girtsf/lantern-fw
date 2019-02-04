#include <FastLED.h>
#include "Arduino.h"

#define PIN_BUILTIN_LED 13
#define PIN_STRIP 12

#define NUM_LEDS 2

// LED strip.
CRGB leds[NUM_LEDS];

void setup() {
  pinMode(PIN_BUILTIN_LED, OUTPUT);
  FastLED.addLeds<WS2812, PIN_STRIP, GRB>(leds, NUM_LEDS);
}

void loop() {
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Green;
  FastLED.show();
  digitalWrite(PIN_BUILTIN_LED, HIGH);
  delay(1000);
  FastLED.show();
  digitalWrite(PIN_BUILTIN_LED, LOW);
  delay(1000);
}
