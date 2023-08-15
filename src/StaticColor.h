#pragma once

#include <Arduino.h>
#include "lib8tion.h"
#include "NameTag.h"

void updateStaticColorPanel(unsigned long time, uint8_t brightness, uint8_t hue) {
    using namespace NameTag;

    setPanelColor(gammaHSV(hue*256, brightness+20));
}

void updateStaticColorPanel(unsigned long time, uint8_t brightness) {
    uint16_t hue = Store::getPersonalHue();

    updateStaticColorPanel(time, brightness, hue);
}

void updateStaticColorAll(unsigned long time, uint8_t brightness, uint8_t hue) {
    using namespace NameTag;

    setPanelColor(gammaHSV(hue*256, brightness+20));
    leds.fill(gammaHSV(hue*256, scale8(brightness+10, 200)));
    leds.show();
}

void updateStaticColorAll(unsigned long time, uint8_t brightness) {
    uint16_t hue = Store::getPersonalHue();

    updateStaticColorAll(time, brightness, hue);
}