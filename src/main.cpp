#include <Arduino.h>
#include <avr/sleep.h>
#include <Array.h>

#include "NameTag.h"
#include "Twinkle.h"
#include "Rainbow.h"

using namespace NameTag;

const unsigned int TARGET_DELTA_TIME = 33;
const unsigned int ANIMATION_COUNT = 2;
unsigned long startTime = 0;
void (*animations[ANIMATION_COUNT])(unsigned long) = {updateRainbow, updateTwinkle};
size_t currentAnimation = 0;
static unsigned long animationStart = 0;

void checkButton();

void setup() {
    NameTag::setup();
    
    setBrightness(getBrightness());
}

void loop() {
    delay(TARGET_DELTA_TIME - (millis() - animationStart));

    checkButton();

    animationStart = millis();
}

void checkButton() {
    static bool lastBtnState = false;
    static unsigned long lastBtnChange = 0;
    
    static unsigned long lastBtnPress = 0;

    bool state = !digitalReadFast(PIN_PA6); // invert because LOW indicates pressed button
    if (state != lastBtnState && millis() - lastBtnChange > 100) {
        lastBtnChange = millis();
        lastBtnState = state;

        if (lastBtnState) {
            lastBtnPress = millis();
        } else if (millis() - lastBtnPress < 3000) {
            currentAnimation++;
            if (currentAnimation >= ANIMATION_COUNT) currentAnimation = 0;
            startTime = millis();
        }
    }

    if (lastBtnState && millis() - lastBtnPress > 3000) {
        setBrightness(getBrightness() - 1);
    }
}
