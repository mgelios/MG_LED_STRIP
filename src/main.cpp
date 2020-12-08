#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN    4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

int BUTTON_PIN = 2;
int BUTTON_TIMESLOT_THRESHOLD = 300;
int buttonTimeslotCounter = 0;

int DELAY_SIZE = 50;

int CURRENT_EFFECT = 0;
int NUMBER_OF_EFFECTS = 9;

int counter = 0;
const int NUMBER_OF_LEDS = 112;
short fire_energy[112];
short hsvState[112];
CRGB leds[112];
CRGB fire_colors[8] = {0x000000, 0x3F0000, 0x7F0000, 0xFF0000, 0xFF3F00, 0xFF7F00, 0xFFBF00, 0xFFFF00};

void setup() {
  pinMode(BUTTON_PIN, INPUT);

  delay(1000);
  //FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUMBER_OF_LEDS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUMBER_OF_LEDS).setCorrection(UncorrectedColor);
  FastLED.setBrightness(127);
  memset(fire_energy, 0, sizeof(fire_energy));
  memset(hsvState, 0, sizeof(hsvState));
  short localCounter = 0;
  for (int i = 0; i < NUMBER_OF_LEDS / 2; i++) {
    hsvState[i] = localCounter;
    if (localCounter < 255) {
      localCounter++;
    }
  }

  for (int i = NUMBER_OF_LEDS / 2; i < NUMBER_OF_LEDS; i++) {
    hsvState[i] = localCounter;
    if (localCounter > 0) {
      localCounter--;
    }
  }
}

//-------------------------------------------------------------------
// steps of effects
//-------------------------------------------------------------------

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


void centralHSVLoopStep() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    if (hsvState[i] < 255) {
      hsvState[i] += 1;
    } else {
      hsvState[i] = 0;
    }
  }

  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i].setHSV(hsvState[i], 255, 255);
  }
}

void warmWhiteLoopStep() {
  for(int i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i].setColorCode(CRGB::FairyLightNCC);
  }
}

void pureWhiteLoopStep() {
  for(int i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i].setColorCode(CRGB::White);
  }
}

boolean isButtonPressed() {
  if (buttonTimeslotCounter >= BUTTON_TIMESLOT_THRESHOLD) {
    buttonTimeslotCounter = 0;
    if (digitalRead(BUTTON_PIN) == HIGH) {
      return true;
    }
    return false;
  }
  return false;
}

void loop() {
  while (true) {
    if (isButtonPressed()) {
      CURRENT_EFFECT += 1;
      if (CURRENT_EFFECT > NUMBER_OF_EFFECTS - 1) {
        CURRENT_EFFECT = 0;
      }
    }

    if (counter > 255) {
      counter = 0;
    }

    if (CURRENT_EFFECT == 0) {
      centralHSVLoopStep();
    } else if (CURRENT_EFFECT == 1) {
      colorBlinkLoopStep();
    } else if (CURRENT_EFFECT == 2) {
      globalHSVLoopStep();
    } else if (CURRENT_EFFECT == 3) {
      whiteBlinkLoopStep();
    } else if (CURRENT_EFFECT == 4) {
      smallHSVLoopStep();
    } else if (CURRENT_EFFECT == 5) {
      fireAnimationLoopStep();
    } else if (CURRENT_EFFECT == 6) {
      randomHsvTransitionStep();
    } else if (CURRENT_EFFECT == 7) {
      warmWhiteLoopStep();
    } else if (CURRENT_EFFECT == 8) {
      pureWhiteLoopStep();
    }

    FastLED.show();
    delay(DELAY_SIZE);
    buttonTimeslotCounter += DELAY_SIZE;

    counter++;
  }
}