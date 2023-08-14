#include <EEPROM.h>
#include "Store.h"

const uint8_t PERSONAL_HUE_ADDR = 0x10;
const uint8_t ANIMATION_ADDR = 0x11;
const uint8_t BRIGHTNESS_ADDR = 0x12;

uint8_t storedPersonalHue = 0;
uint8_t storedAnimation = 0;
uint8_t storedBrightness = 0;

void Store::setup() {
    storedPersonalHue = EEPROM.read(PERSONAL_HUE_ADDR);
    storedAnimation = EEPROM.read(ANIMATION_ADDR);
    storedBrightness = EEPROM.read(BRIGHTNESS_ADDR);
}

void Store::setPersonalHue(uint8_t hue) {
    if (hue != storedPersonalHue) {
        storedPersonalHue = hue;
        EEPROM.write(PERSONAL_HUE_ADDR, hue);
    }
}

uint8_t Store::getPersonalHue() {
    return storedPersonalHue;
}

void Store::setAnimation(uint8_t idx) {
    if (idx != storedAnimation) {
        storedAnimation = idx;
        EEPROM.write(ANIMATION_ADDR, idx);
    }
}

uint8_t Store::getAnimation() {
    return storedAnimation;
}

void Store::setBrightness(uint8_t b) {
    if (b != storedBrightness) {
        storedBrightness = b;
        EEPROM.write(BRIGHTNESS_ADDR, b);
    }
}

uint8_t Store::getBrightness() {
    return storedBrightness;
}