#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN    4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[112];
int counter = 0;
int NUMBER_OF_LEDS = 112;
short fire_energy[112];
CRGB fire_colors[8] = {0x000000, 0x3F0000, 0x7F0000, 0xFF0000, 0xFF3F00, 0xFF7F00, 0xFFBF00, 0xFFFF00};

void setup() {
  delay(1000);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUMBER_OF_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(127);
  memset(fire_energy, 0, sizeof(fire_energy));
}

void colorBlinkLoopStep() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i].fadeToBlackBy(64);
    if (random(16) == 1) {
      leds[i].setHSV(random(256), 255,255);
    }
  }
}

void whiteBlinkLoopStep() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i].fadeToBlackBy(64);
    if (random(32) == 1) {
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

void fireAnimationLoopStep() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    if (fire_energy[i] == 0) {
      fire_energy[i] = random(8);
    } else {
      fire_energy[i] -= 1;
    }

    leds[i] = fire_colors[fire_energy[i]];
  }
}

int currentHSVColor = 0;
int nextHSVColor = 0;

void randomHsvTransitionStep() {
  if (currentHSVColor == nextHSVColor) {
    nextHSVColor = random(256);
  }

  if (currentHSVColor > nextHSVColor) {
    currentHSVColor -= 1;
  } else if (currentHSVColor < nextHSVColor) {
    currentHSVColor += 1;
  }

  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i].setHSV(currentHSVColor, 255, 255);
  }
}

void loop() {
  while (true) {
    if (counter > 255) {
      counter = 0;
    }

    //colorBlinkLoopStep();
    //globalHSVLoopStep();
    //smallHSVLoopStep();
    //whiteBlinkLoopStep();
    //fireAnimationLoopStep();
    randomHsvTransitionStep();

    FastLED.show();
    delay(100);

    counter++;
  }
}