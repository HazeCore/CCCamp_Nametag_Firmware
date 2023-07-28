// #define MILLIS_USE_TIMERA0 0
// #define MILLIS_USE_TIMERB0 1
// #undef MILLIS_USE_TIMERA0
#include <Arduino.h>
#include <avr/sleep.h>
#include <Array.h>

#include "NameTag.h"
#include "Animation.h"
#include "Twinkle.h"
#include "Rainbow.h"

using namespace NameTag;

const unsigned int TARGET_DELTA_TIME = 33;
const unsigned int ANIMATION_COUNT = 2;
unsigned long startTime = 0;
void (*animations[ANIMATION_COUNT])(unsigned long);
size_t currentAnimation = 0;
static unsigned long animationStart = 0;

void setup() {
    NameTag::setup();

    animations[0] = updateRainbow;
    animations[1] = updateTwinkle;
    
    setBrightness(getBrightness());
}

void loop() {
    delay(TARGET_DELTA_TIME - (millis() - animationStart));
    animationStart = millis();
}
