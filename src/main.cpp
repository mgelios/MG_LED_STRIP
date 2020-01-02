#include <Arduino.h>
#include <FastLED.h>


#define DATA_PIN    4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[112];
int counter = 0;
int NUMBER_OF_LEDS = 112;

void setup() {
  delay(1000);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUMBER_OF_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(127);
}

void colorBlinkLoopStep() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i].fadeToBlackBy(64);
    if (random(10) == 1) {
      leds[i].setHSV(random(256), 255,255);
    }
  }
}

void whiteBlinkLoopStep() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i].fadeToBlackBy(64);
    if (random(16) == 1) {
      leds[i].setRGB(255, 255, 255);
    }
  }
}

void globalHSVLoopStep() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i].setHSV(counter, 255, 255);
  }
}

void smallHSVLoopStep() {
  int localCounter = counter;
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i].setHSV(localCounter, 255, 255);
    localCounter++;
    if (localCounter > 255) {
      localCounter = 0;
    }
  }
}

void loop() {
  while (true) {
    if (counter > 255) {
      counter = 0;
    }

    //colorBlinkLoopStep();
    //globalHSVLoopStep();
    smallHSVLoopStep();

    FastLED.show();
    delay(100);

    counter++;
  }
}